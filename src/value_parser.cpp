/*
 * Copyright 2020 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "value_parser.h"
#include <ctype.h>
#include <stdlib.h>
#include "config_base.h"
#include "stack.h"

namespace libasm {

const NumberParser &ValueParser::Plugins::number() const {
    return CStyleNumberParser::singleton();
}

const CommentParser &ValueParser::Plugins::comment() const {
    return SemicolonCommentParser::singleton();
}

const SymbolParser &ValueParser::Plugins::symbol() const {
    static const SymbolParser SINGLETON;
    return SINGLETON;
}

const LetterParser &ValueParser::Plugins::letter() const {
    static const LetterParser SINGLETON;
    return SINGLETON;
}

const LocationParser &ValueParser::Plugins::location() const {
    return DollarLocationParser::singleton();
}

const OperatorParser &ValueParser::Plugins::operators() const {
    return CStyleOperatorParser::singleton();
}

const FunctionTable &ValueParser::Plugins::function() const {
    static const FunctionTable SINGLETON;
    return SINGLETON;
}

const ValueParser::Plugins &ValueParser::Plugins::defaultPlugins() {
    static const Plugins PLUGINS;
    return PLUGINS;
}

const ValueParser::Plugins &ValueParser::Plugins::intel() {
    static const IntelPlugins PLUGINS;
    return PLUGINS;
}

const NumberParser &ValueParser::IntelPlugins::number() const {
    return IntelNumberParser::singleton();
}

const OperatorParser &ValueParser::IntelPlugins::operators() const {
    return IntelOperatorParser::singleton();
}

const ValueParser::Plugins &ValueParser::Plugins::motorola() {
    static const MotorolaPlugins PLUGINS;
    return PLUGINS;
}

const NumberParser &ValueParser::MotorolaPlugins::number() const {
    return MotorolaNumberParser::singleton();
}
const CommentParser &ValueParser::MotorolaPlugins::comment() const {
    return StarCommentParser::singleton();
}
const SymbolParser &ValueParser::MotorolaPlugins::symbol() const {
    return Mc68xxSymbolParser::singleton();
}
const LetterParser &ValueParser::MotorolaPlugins::letter() const {
    return MotorolaLetterParser::singleton();
}
const LocationParser &ValueParser::MotorolaPlugins::location() const {
    return StarLocationParser::singleton();
}
const OperatorParser &ValueParser::MotorolaPlugins::operators() const {
    return Mc68xxOperatorParser::singleton();
}

namespace fairchild {
/**
 * Fairchild style numbers are the same as IBM plus '$hh' for hexadecimal.
 */
struct FairchildNumberParser final : NumberParser, Singleton<FairchildNumberParser> {
    Error parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const override {
        auto p = scan;
        if (*p == '$' && isxdigit(p[1])) {
            const auto error = val.read(++p, RADIX_16);
            if (error != NOT_AN_EXPECTED)
                scan = p;
            return error;
        }
        return _ibm.parseNumber(scan, val, defaultRadix);
    }

private:
    IbmNumberParser _ibm{'H', 'B', 'O', 'D'};
};

struct FairchildLetterParser final : LetterParser, Singleton<FairchildLetterParser> {
    /** Fairchild style letter is [cC]'[:print:]', #[:print:], '[:print:]'? */
    Error parseLetter(StrScanner &scan, char &letter) const override {
        auto p = scan;
        if (p.iexpect('C')) {  // C'C'
            const auto error = LetterParser::parseLetter(p, letter);
            if (error == OK)
                scan = p;
            return error;
        }
        if (p.expect('#')) {  // #C
            if ((letter = *p) == 0)
                return NOT_AN_EXPECTED;
            scan = ++p;
            return OK;
        }
        if (p.expect('\'')) {  // 'c' or 'c
            ErrorAt error;
            letter = readLetter(p, error, '\'');
            if (error.isOK()) {
                p.expect('\'');  // closing quote is optional
                scan = p;
                return OK;
            }
            return error.getError();
        }
        return NOT_AN_EXPECTED;
    }

    /** Fairchild style string constant is C'str'. */
    bool stringPrefix(StrScanner &scan) const override {
        scan.iexpect('C');  // optional
        return true;
    }
};

struct FairchildLocationParser final : SimpleLocationParser, Singleton<FairchildLocationParser> {
    FairchildLocationParser() : SimpleLocationParser(text::common::PSTR_DOT_STAR_DOLLAR) {}
};

}  // namespace fairchild

const ValueParser::Plugins &ValueParser::Plugins::fairchild() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override {
            return fairchild::FairchildNumberParser::singleton();
        }
        const CommentParser &comment() const override { return StarCommentParser::singleton(); }
        const LetterParser &letter() const override {
            return fairchild::FairchildLetterParser::singleton();
        }
        const LocationParser &location() const override {
            return fairchild::FairchildLocationParser::singleton();
        }
    } PLUGINS{};
    return PLUGINS;
}

struct OperatorStack : Stack<Operator, 8> {
    OperatorStack() : Stack() {}
    Operator &top() { return _contents[_size - 1]; }
};

Value ValueParser::eval(StrScanner &scan, ErrorAt &error, ParserContext &context) const {
    const auto start = scan.skipSpaces();
    const auto value = _eval(scan, error, context);
    if (scan.str() == start.str())
        error.setErrorIf(start, NOT_AN_EXPECTED);
    return value;
}

Value ValueParser::_eval(StrScanner &scan, ErrorAt &error, ParserContext &context) const {
    ValueStack vstack;
    OperatorStack ostack;
    char end_of_expr = context.delimitor;
    uint8_t in_paren = 0;
    bool maybe_prefix = true;
    bool expect_fn_args = false;
    uint8_t in_fn_args = 0;
    while (true) {
        const auto expr = scan;
        if (endOfLine(scan.skipSpaces()))
            break;

        // expression ends with |delim|
        if (*scan == end_of_expr && !vstack.empty())
            break;

        const auto at = scan;
        const Operator *opr = nullptr;
        if (maybe_prefix) {
            // Prefix operator may consist of alpha numeric text. Check operator before handling
            // symbols.
            opr = _operators.readPrefix(scan, vstack, context);
            if (error.hasError())
                return Value();
        }
        if (maybe_prefix && opr == nullptr) {
            Value val;
            auto err = parseConstant(scan, val, context);
            if (err == OK) {
                if (vstack.full()) {
                    error.setErrorIf(at, TOO_COMPLEX_EXPRESSION);
                    return Value();
                }
                vstack.push(val);
                maybe_prefix = false;
                continue;
            } else if (err != NOT_AN_EXPECTED) {
                error.setErrorIf(at, err);
                return val;
            }

            StrScanner symbol;
            if (readFunctionName(scan, symbol) == OK) {
                auto fn = _function.lookupFunction(symbol);
                if (fn == nullptr && context.symbolTable)
                    fn = context.symbolTable->lookupFunction(symbol);
                if (fn) {
                    const auto open = scan.skipSpaces();
                    if (!_operators.isOpenExpr(scan)) {
                        error.setErrorIf(at, MISSING_FUNC_ARGUMENT);
                        return val;
                    }
                    scan = open;
                    const Operator opr(fn);
                    ostack.push(opr);
                    ostack.top().setAt(at);
                    maybe_prefix = false;
                    expect_fn_args = true;
                    in_fn_args++;
                    continue;
                }
                scan = at;
            }

            if (readSymbol(scan, symbol) == OK) {
                if (vstack.full()) {
                    error.setErrorIf(at, TOO_COMPLEX_EXPRESSION);
                    return Value();
                }
                const auto *value =
                        context.symbolTable ? context.symbolTable->lookupSymbol(symbol) : nullptr;
                if (value) {
                    vstack.push(*value);
                } else {
                    error.setErrorIf(at, UNDEFINED_SYMBOL);
                    vstack.push(Value());
                }
                maybe_prefix = false;
                continue;
            }
        }

        if (opr == nullptr) {
            if (maybe_prefix) {
                opr = _operators.readPrefix(scan, vstack, context);
            } else {
                opr = _operators.readInfix(scan, vstack, context);
            }
            if (error.hasError())
                return Value();
        }
        if (opr) {
            while (!ostack.empty()) {
                const auto top = ostack.top();
                if (top.isNoneAssoc(*opr)) {
                    // This can be warning and continue as left associative.
                    error.setErrorIf(at, OPERATOR_NOT_ASSOCIATIVE);
                    return Value();
                }
                if (!top.isHigher(*opr))
                    break;
                const auto op = ostack.pop();
                const auto err = op.eval(vstack, context);
                if (err) {
                    error.setErrorIf(op, err);
                    return Value();
                }
            }
            if (ostack.full()) {
                error.setErrorIf(at, TOO_COMPLEX_EXPRESSION);
                return Value();
            }
            ostack.push(*opr);
            ostack.top().setAt(at);
            maybe_prefix = true;
            continue;
        }

        if (in_fn_args && scan.expect(',')) {
            // non-zero |in_fn_args| ensures the existence of open parenthesis in |ostack|.
            while (!ostack.top().isOpenParen()) {
                const auto op = ostack.pop();
                const auto err = op.eval(vstack, context);
                if (err) {
                    error.setErrorIf(op, err);
                    return Value();
                }
            }
            maybe_prefix = true;
            continue;
        }

        if (_operators.isOpenExpr(scan)) {
            // expression ends with '('
            if (!maybe_prefix && !expect_fn_args) {
                --scan;
                break;
            }
            if (ostack.full()) {
                error.setErrorIf(at, TOO_COMPLEX_EXPRESSION);
                return Value();
            }
            Operator openParen(vstack.size());
            ostack.push(openParen);
            ostack.top().setAt(at);
            end_of_expr = 0;
            in_paren++;
            maybe_prefix = true;
            expect_fn_args = false;
            continue;
        }

        if (_operators.isCloseExpr(scan)) {
            while (!ostack.empty() && !ostack.top().isOpenParen()) {
                const auto op = ostack.pop();
                const auto err = op.eval(vstack, context);
                if (err) {
                    error.setErrorIf(op, err);
                    return Value();
                }
            }
            // expression ends with ')'
            if (ostack.empty())
                break;

            const auto openParen = ostack.pop();
            if (!ostack.empty() && ostack.top().isFunction()) {
                const auto argc = vstack.size() - openParen.stackPosition();
                const auto fn = ostack.pop();
                const auto err = fn.eval(vstack, context, argc);
                if (err) {
                    error.setErrorIf(fn, err);
                    return Value();
                }
                in_fn_args--;
            }
            if (--in_paren == 0)
                end_of_expr = context.delimitor;
            maybe_prefix = false;
            continue;
        }

        scan = expr;
        break;
    }

    while (!ostack.empty()) {
        const auto op = ostack.pop();
        if (op.isOpenParen()) {
            error.setErrorIf(op, MISSING_CLOSING_PAREN);
            return Value();
        }
        const auto err = op.eval(vstack, context);
        if (err) {
            error.setErrorIf(op, err);
            return Value();
        }
    }
    return vstack.pop();
}

Error ValueParser::parseConstant(StrScanner &scan, Value &val, ParserContext &context) const {
    auto p = scan;

    char letter;
    auto err = _letter.parseLetter(p, letter);
    if (err == OK) {
        val.setUnsigned(static_cast<uint8_t>(letter));
        scan = p;
        return OK;
    }
    if (err != NOT_AN_EXPECTED)
        return err;

    err = _number.parseNumber(p, val, context.defaultRadix);
#if !defined(LIBASM_ASM_NOFLOAT)
    const auto fpnum = (err == OK && (*p == '.' || toupper(*p) == 'E')) && context.delimitor != '.';
    if (fpnum || err == OVERFLOW_RANGE || err == NOT_AN_EXPECTED) {
        char *end;
        Value::float_t f80;
        const auto error = f80.read(scan.str(), &end);
        if (end > scan.str()) {
            val.setFloat(f80);
            scan = end;
            return error == 0 || f80.isSubnormal() ? OK : OVERFLOW_RANGE;
        }
    }
#endif
    if (err != NOT_AN_EXPECTED) {
        scan = p;
        return err;
    }

    if (_location.locationSymbol(p)) {
        val.setUnsigned(context.currentLocation);
        scan = p;
        return OK;
    }

    return NOT_AN_EXPECTED;
}

Error ValueParser::readSymbol(StrScanner &scan, StrScanner &symbol) const {
    symbol = scan;
    if (_symbol.symbolLetter(*symbol++, true)) {
        scan = symbol.takeWhile([this](char c) { return _symbol.symbolLetter(c); });
        --symbol;
        return OK;
    }
    symbol.empty();
    return NOT_AN_EXPECTED;
}

Error ValueParser::readLabel(StrScanner &scan, StrScanner &label) const {
    label = scan;
    if (_symbol.symbolLetter(*label++, true)) {
        scan = label.takeWhile([this](char c) { return _symbol.symbolLetter(c); });
        --label;
        _symbol.labelDelimitor(scan);
        return OK;
    }
    label.empty();
    return NOT_AN_EXPECTED;
}

Error ValueParser::readInstruction(StrScanner &scan, StrScanner &inst) const {
    auto p = scan;
    for (;;) {
        const auto c = *p;
        if (!_symbol.instructionLetter(c))
            break;
        ++p;
        if (_symbol.instructionTerminator(c))
            break;
    }
    if (p.str() != scan.str()) {
        inst = StrScanner{scan.str(), p.str()};
        scan = p;
        return OK;
    }
    inst.empty();
    return NOT_AN_EXPECTED;
}

Error ValueParser::readFunctionName(StrScanner &scan, StrScanner &name) const {
    name = scan;
    if (_symbol.symbolLetter(*name++, true)) {
        scan = name.takeWhile([this](char c) { return _symbol.functionNameLetter(c); });
        --name;
        return OK;
    }
    name.empty();
    return NOT_AN_EXPECTED;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
