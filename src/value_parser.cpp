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
#include "config_base.h"
#include "stack.h"

namespace libasm {

namespace {
bool isValidDigit(char c, Radix radix) {
    if (radix == RADIX_16)
        return isxdigit(c);
    return c >= '0' && c < '0' + uint8_t(radix);
}

uint8_t toNumber(char c, Radix radix) {
    if (radix == RADIX_16 && c >= 'A')
        return (c & ~0x20) - 'A' + 10;
    return c - '0';
}

template <typename VAL_T, VAL_T VAL_MAX>
Error strToNumber(StrScanner &scan, Radix radix, VAL_T &value) {
    auto p = scan;
    if (!isValidDigit(*p, radix))
        return NOT_AN_EXPECTED;
    const VAL_T limit = VAL_MAX / uint8_t(radix);
    const uint8_t limit_digit = VAL_MAX % uint8_t(radix);
    Error error = OK;
    VAL_T v = 0;
    while (isValidDigit(*p, radix)) {
        const auto n = toNumber(*p, radix);
        if (v > limit || (v == limit && n > limit_digit))
            error = OVERFLOW_RANGE;
        v *= uint8_t(radix);
        v += n;
        ++p;
    }
    scan = p;
    value = v;
    return error;
}
}  // namespace

Error Value::parseNumber(StrScanner &scan, Radix radix, uint64_t &value) {
    return strToNumber<uint64_t, UINT64_MAX>(scan, radix, value);
}

Error Value::parseNumber(StrScanner &scan, Radix radix, uint32_t &value) {
    return strToNumber<uint32_t, UINT32_MAX>(scan, radix, value);
}

const NumberParser &ValueParser::Plugins::number() const {
    return CStyleNumberParser::singleton();
}

const CommentParser &ValueParser::Plugins::comment() const {
    return SemicolonCommentParser::singleton();
}

const SymbolParser &ValueParser::Plugins::symbol() const {
    return SymbolParser::singleton();
}

const LetterParser &ValueParser::Plugins::letter() const {
    return DefaultLetterParser::singleton();
}

const LocationParser &ValueParser::Plugins::location() const {
    return DollarLocationParser::singleton();
}

const OperatorParser &ValueParser::Plugins::operators() const {
    return CStyleOperatorParser::singleton();
}

const FunctionTable &ValueParser::Plugins::function() const {
    return FunctionTable::singleton();
}

struct OperatorStack : Stack<Operator, 8> {
    OperatorStack() : Stack() {}
    Operator &top() { return _contents[_size - 1]; }
};

Value ValueParser::eval(
        StrScanner &scan, ErrorAt &error, const SymbolTable *symtab, char delim) const {
    const auto start = scan.skipSpaces();
    const auto value = _eval(scan, error, symtab, delim);
    if (scan.str() == start.str())
        error.setErrorIf(start, NOT_AN_EXPECTED);
    return value;
}

Value ValueParser::_eval(
        StrScanner &scan, ErrorAt &error, const SymbolTable *symtab, char delim) const {
    ValueStack vstack;
    OperatorStack ostack;
    char end_of_expr = delim;
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
            opr = _operators.readOperator(scan, error, Operator::PREFIX);
            if (error.hasError())
                return Value();
        }
        if (maybe_prefix && opr == nullptr) {
            Value val;
            auto err = parseConstant(scan, val);
            if (err == OK) {
                if (vstack.full()) {
                    error.setError(at, TOO_COMPLEX_EXPRESSION);
                    return Value();
                }
                vstack.push(val);
                maybe_prefix = false;
                continue;
            } else if (err != NOT_AN_EXPECTED) {
                error.setError(scan, err);
                return val;
            }

            StrScanner symbol;
            if (readFunctionName(scan, symbol) == OK) {
                auto fn = _function.lookupFunction(symbol);
                if (fn == nullptr && symtab)
                    fn = reinterpret_cast<const Functor *>(symtab->lookupFunction(symbol));
                if (fn) {
                    if (*scan.skipSpaces() != '(') {
                        error.setError(at, MISSING_FUNC_ARGUMENT);
                        return val;
                    }
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
                    error.setError(at, TOO_COMPLEX_EXPRESSION);
                    return Value();
                }
                if (symtab && symtab->hasSymbol(symbol)) {
                    vstack.pushSigned(symtab->lookupSymbol(symbol));
                } else {
                    error.setErrorIf(at, UNDEFINED_SYMBOL);
                    vstack.push(Value());
                }
                maybe_prefix = false;
                continue;
            }
        }

        if (opr == nullptr) {
            const auto oprType = maybe_prefix ? Operator::PREFIX : Operator::INFIX;
            opr = _operators.readOperator(scan, error, oprType);
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
                const auto err = op.eval(vstack);
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
                const auto err = op.eval(vstack);
                if (err) {
                    error.setErrorIf(op, err);
                    return Value();
                }
            }
            maybe_prefix = true;
            continue;
        }

        if (scan.expect('(')) {
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

        if (scan.expect(')')) {
            while (!ostack.empty() && !ostack.top().isOpenParen()) {
                const auto op = ostack.pop();
                const auto err = op.eval(vstack);
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
                const auto err = fn.eval(vstack, argc);
                if (err) {
                    error.setErrorIf(fn, err);
                    return Value();
                }
                in_fn_args--;
            }
            if (--in_paren == 0)
                end_of_expr = delim;
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
        const auto err = op.eval(vstack);
        if (err) {
            error.setErrorIf(op, err);
            return Value();
        }
    }
    return vstack.pop();
}

Error ValueParser::parseConstant(StrScanner &scan, Value &val) const {
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

    err = _number.parseNumber(p, val);
    if (err == OK) {
        scan = p;
        return OK;
    }
    if (err != NOT_AN_EXPECTED)
        return err;

    if (_location.locationSymbol(p)) {
        val.setUnsigned(_locator.currentLocation());
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
    symbol = StrScanner::EMPTY;
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
    label = StrScanner::EMPTY;
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
    inst = StrScanner::EMPTY;
    return NOT_AN_EXPECTED;
}

Error ValueParser::readFunctionName(StrScanner &scan, StrScanner &name) const {
    name = scan;
    if (_symbol.symbolLetter(*name++, true)) {
        scan = name.takeWhile([this](char c) { return _symbol.functionNameLetter(c); });
        --name;
        return OK;
    }
    name = StrScanner::EMPTY;
    return NOT_AN_EXPECTED;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
