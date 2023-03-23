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

namespace libasm {

bool Value::overflowRel8(int16_t s16) {
    return s16 < -128 || s16 >= 128;
}

bool Value::overflowRel8(int32_t s32) {
    return s32 < -128 || s32 >= 128;
}

bool Value::overflowRel16(int32_t s32) {
    return s32 < -32768L || s32 >= 32768L;
}

bool Value::overflowUint8(uint16_t u16) {
    const auto s16 = static_cast<int16_t>(u16);
    return s16 < -128 || (s16 >= 0 && u16 >= 256);
}

bool Value::overflowUint8(uint32_t u32) {
    const auto s32 = static_cast<int32_t>(u32);
    return s32 < -128 || (s32 >= 0 && u32 >= 0x100);
}

bool Value::overflowUint16(uint32_t u32) {
    const auto s32 = static_cast<int32_t>(u32);
    return s32 < -32768L || (s32 >= 0 && u32 >= 0x10000L);
}

bool Value::overflowUint(uint32_t u32, uint8_t bitw) {
    const auto s32 = static_cast<int32_t>(u32);
    if (bitw >= 32)
        return false;
    const uint32_t max = 1UL << bitw;
    const int32_t sign = max >> 1;
    return s32 < -sign || (s32 >= 0 && u32 >= max);
}

static bool isoctal(char c) {
    return c >= '0' && c < '8';
}

char ValueParser::readLetter(StrScanner &scan, ErrorAt &error) const {
    return _letterParser.readLetter(scan, error);
}

Error CStyleLetterParser::parseLetter(StrScanner &scan, char &letter) const {
    if (!scan.expect('\''))
        return NOT_AN_EXPECTED;
    ErrorAt error;
    letter = readLetter(scan, error);
    return error.getError() ?: (scan.expect('\'') ? OK : MISSING_CLOSING_QUOTE);
}

char CStyleLetterParser::readLetter(StrScanner &scan, ErrorAt &error) const {
    auto p = scan;
    if (p.expect('\\')) {
        // Backslash escape sequence
        Radix radix = RADIX_NONE;
        if (p.iexpect('x')) {
            radix = RADIX_16;
        } else if (isoctal(*p)) {
            radix = RADIX_8;
        } else {
            auto c = *p++;
            switch (c) {
            case '\'':
            case '"':
            case '?':
            case '\\':
                break;
            case 'b':
                c = 0x08;
                break;
            case 't':
                c = 0x09;
                break;
            case 'n':
                c = 0x0a;
                break;
            case 'r':
                c = 0x0d;
                break;
            default:
                error.setError(UNKNOWN_ESCAPE_SEQUENCE);
            }
            scan = p;
            return c;
        }
        Value value;
        const auto err = value.parseNumber(p, radix);
        scan = p;
        if (err != OK)
            error.setError(err);
        if (value.overflowUint8())
            error.setError(OVERFLOW_RANGE);
        return value.getUnsigned();
    }

    const auto c = *p++;
    scan = p;
    return c;
}

static bool isValidDigit(const char c, const Radix radix) {
    if (radix == RADIX_16)
        return isxdigit(c);
    return c >= '0' && c < '0' + uint8_t(radix);
}

static uint8_t toNumber(const char c, const Radix radix) {
    if (radix == RADIX_16 && c >= 'A')
        return (c & ~0x20) - 'A' + 10;
    return c - '0';
}

Error Value::parseNumber(StrScanner &scan, Radix radix) {
    auto p = scan;
    if (!isValidDigit(*p, radix))
        return ILLEGAL_CONSTANT;
    const uint32_t limit = UINT32_MAX / uint8_t(radix);
    const uint8_t limit_digit = UINT32_MAX % uint8_t(radix);
    Error error = OK;
    uint32_t v = 0;
    while (isValidDigit(*p, radix)) {
        const auto n = toNumber(*p, radix);
        if (v > limit || (v == limit && n > limit_digit))
            error = OVERFLOW_RANGE;
        v *= uint8_t(radix);
        v += n;
        ++p;
    }
    scan = p;
    setValue(v);
    return error;
}

Value ValueParser::eval(StrScanner &expr, ErrorAt &error, const SymbolTable *symtab) const {
    Stack<OprAndLval> stack;
    return parseExpr(expr, error, stack, symtab);
}

StrScanner ValueParser::scanExpr(const StrScanner &scan, ErrorAt &error, char delim) const {
    auto p = scan;
    while (!endOfLine(p)) {
        Value val;
        auto err = _numberParser.parseNumber(p, val);
        if (err != NOT_AN_EXPECTED)
            continue;
        char letter;
        err = _letterParser.parseLetter(p, letter);
        if (err == OK)
            continue;
        if (*p == delim)
            return StrScanner(scan.str(), p.str());
        if (*p == '(' || *p == '[') {
            const auto close = (*p++ == '(') ? ')' : ']';
            const auto atom = scanExpr(p, error, close);
            if (atom.size() == 0)
                break;
            p += atom.size() + 1;
        } else {
            ++p;
        }
    }
    return StrScanner(p.str(), p.str());
}

Value ValueParser::parseExpr(StrScanner &scan, ErrorAt &error, Stack<OprAndLval> &stack,
        const SymbolTable *symtab) const {
    if (stack.full()) {
        error.setError(scan, TOO_COMPLEX_EXPRESSION);
        return Value();
    }
    stack.push(OprAndLval());
    auto value = readAtom(scan, error, stack, symtab);
    while (!error.hasError() && !stack.empty()) {
        const auto opr = readOperator(scan, error);
        if (error.hasError())
            return Value();
        while (opr._precedence <= stack.top().precedence()) {
            if (stack.top().isEnd()) {
                stack.pop();
                return value;
            }
            value = evalExpr(stack.top()._opr._op, stack.top()._value, value, error);
            stack.pop();
        }
        if (stack.full()) {
            error.setError(scan, TOO_COMPLEX_EXPRESSION);
            return Value();
        }
        stack.push(OprAndLval(opr, value));
        value = readAtom(scan, error, stack, symtab);
    }
    return Value();
}

Value ValueParser::readAtom(StrScanner &scan, ErrorAt &error, Stack<OprAndLval> &stack,
        const SymbolTable *symtab) const {
    auto p = scan;
    if (endOfLine(p.skipSpaces())) {
        error.setError(scan, ILLEGAL_CONSTANT);
        return Value();
    }
    if (p.expect('(')) {
        const auto value = parseExpr(p, error, stack, symtab);
        if (!error.hasError()) {
            if (!p.skipSpaces().expect(')')) {
                error.setError(p, MISSING_CLOSING_PAREN);
                return Value();
            }
            scan = p;
            return value;
        }
        return value;
    }
    char letter;
    auto err = _letterParser.parseLetter(p, letter);
    if (err == OK) {
        scan = p;
        return Value::makeUnsigned(letter);
    } else if (err != NOT_AN_EXPECTED) {
        error.setError(p, err);
        return Value();
    }
    if (p.expect('~')) {
        auto value = readAtom(p, error, stack, symtab);
        scan = p;
        if (!value.isUndefined())
            return value.setValue(~value.getUnsigned());
        return value;
    }
    if (p.expect('-')) {
        if (*p == '-' || *p == '+') {
            error.setError(scan, UNKNOWN_EXPR_OPERATOR);
            return Value();
        }
        auto value = readAtom(p, error, stack, symtab);
        if (!value.isUndefined()) {
            if (value.isUnsigned() && value.getUnsigned() > 0x80000000)
                error.setError(scan, OVERFLOW_RANGE);
            value.setValue(-value.getSigned()).setSign(true);
        }
        scan = p;
        return value;
    }
    if (p.expect('+')) {
        if (*p == '+' || *p == '-') {
            error.setError(scan, UNKNOWN_EXPR_OPERATOR);
            return Value();
        }
        const auto value = readAtom(p, error, stack, symtab);
        scan = p;
        return value;
    }

    Value val;
    err = _numberParser.parseNumber(p, val);
    if (err != NOT_AN_EXPECTED) {
        if (err)
            error.setErrorIf(scan, err);
        scan = p;
        return val;
    }

    if (_locationParser.locationSymbol(p)) {
        scan = p;
        return Value::makeUnsigned(_origin);
    }

    if (symbolLetter(*p, true)) {
        const auto symbol = readSymbol(p);
        if (*p.skipSpaces() == '(' && _funcParser) {
            auto params = p;
            params.expect('(');
            const auto err = _funcParser->parseFunc(*this, symbol, params, val, error, symtab);
            if (err == OK) {
                if (params.expect(')')) {
                    scan = params;
                } else {
                    error.setError(p, MISSING_CLOSING_PAREN);
                }
                return val;
            } else if (err == UNKNOWN_FUNCTION) {
                error.setOK();
            } else {
                return val;
            }
        }
        scan = p;
        if (symtab && symtab->hasSymbol(symbol)) {
            const uint32_t v = symtab->lookupSymbol(symbol);
            return val.setValue(v);
        }
        error.setErrorIf(symbol, UNDEFINED_SYMBOL);
        return val;
    }

    error.setError(ILLEGAL_CONSTANT);
    return Value();
}

ValueParser::FuncParser *ValueParser::setFuncParser(FuncParser *parser) {
    auto prev = _funcParser;
    _funcParser = parser;
    return prev;
}

// Operator precedence (larger value means higher precedence).
// The same order of C/C++ language.
ValueParser::Operator ValueParser::readOperator(StrScanner &scan, ErrorAt &error) const {
    if (endOfLine(scan.skipSpaces()))
        return Operator(OP_NONE, 0);
    const char c = *scan++;
    switch (c) {
    case '*':
        return Operator(OP_MUL, 13);
    case '/':
        return Operator(OP_DIV, 13);
    case '%':
        return Operator(OP_MOD, 13);
    case '+':
        return Operator(OP_ADD, 12);
    case '-':
        return Operator(OP_SUB, 12);
    case '<':
        if (scan.expect('<'))
            return Operator(OP_BIT_SHL, 11);
        error.setError(scan, UNKNOWN_EXPR_OPERATOR);
        break;
    case '>':
        if (scan.expect('>'))
            return Operator(OP_BIT_SHR, 11);
        error.setError(scan, UNKNOWN_EXPR_OPERATOR);
        break;
    case '&':
        return Operator(OP_BIT_AND, 8);
    case '^':
        return Operator(OP_BIT_XOR, 7);
    case '|':
        return Operator(OP_BIT_OR, 6);
    default:
        break;
    }
    --scan;
    return Operator(OP_NONE, 0);
}

bool ValueParser::symbolLetter(char c, bool head) const {
    if (isalpha(c) || c == '_')
        return true;
    if (head && c == '.')
        return true;
    return !head && isdigit(c);
}

StrScanner ValueParser::readSymbol(StrScanner &scan) const {
    auto p = scan;
    p.trimStart([this](char c) { return this->symbolLetter(c); });
    auto symbol = StrScanner(scan.str(), p.str());
    scan = p;
    return symbol;
}

static uint32_t shift_left(uint32_t value, uint8_t count) {
    for (unsigned i = 0; i <= 32 && i < count; i++)
        value <<= 1;
    return value;
}

static uint32_t shift_right(uint32_t value, uint8_t count) {
    for (unsigned i = 0; i <= 32 && i < count; i++)
        value >>= 1;
    return value;
}

static int32_t shift_right_negative(int32_t value, uint8_t count) {
    for (unsigned i = 0; i <= 32 && i < count; i++) {
        value >>= 1;
        value |= 0x80000000;
    }
    return value;
}

Value ValueParser::evalExpr(const Op op, const Value lhs, const Value rhs, ErrorAt &error) const {
    if (lhs.isUndefined() || rhs.isUndefined())
        return Value();

    const auto bsigned = lhs.isSigned() && rhs.isSigned();

    switch (op) {
    case OP_ADD:
        return Value::makeUnsigned(lhs.getUnsigned() + rhs.getUnsigned()).setSign(bsigned);
    case OP_SUB:
        return Value::makeUnsigned(lhs.getUnsigned() - rhs.getUnsigned()).setSign(bsigned);
    case OP_MUL:
        return bsigned ? Value::makeSigned(lhs.getSigned() * rhs.getSigned())
                       : Value::makeUnsigned(lhs.getUnsigned() * rhs.getUnsigned());
    case OP_DIV:
        if (rhs.getUnsigned() == 0) {
            error.setError(DIVIDE_BY_ZERO);
            return Value();
        }
        return bsigned ? Value::makeSigned(lhs.getSigned() / rhs.getSigned())
                       : Value::makeUnsigned(lhs.getUnsigned() / rhs.getUnsigned());
    case OP_MOD:
        if (rhs.getUnsigned() == 0) {
            error.setError(DIVIDE_BY_ZERO);
            return Value();
        }
        return bsigned ? Value::makeSigned(lhs.getSigned() % rhs.getSigned())
                       : Value::makeUnsigned(lhs.getUnsigned() % rhs.getUnsigned());
    case OP_BIT_AND:
        return Value::makeUnsigned(lhs.getUnsigned() & rhs.getUnsigned());
    case OP_BIT_XOR:
        return Value::makeUnsigned(lhs.getUnsigned() ^ rhs.getUnsigned());
    case OP_BIT_OR:
        return Value::makeUnsigned(lhs.getUnsigned() | rhs.getUnsigned());
    case OP_BIT_SHL:
        return Value::makeUnsigned(shift_left(lhs.getUnsigned(), rhs.getUnsigned()));
    case OP_BIT_SHR:
        if (lhs.isSigned() && lhs.getSigned() < 0)
            return Value::makeSigned(shift_right_negative(lhs.getSigned(), rhs.getUnsigned()));
        return Value::makeUnsigned(shift_right(lhs.getUnsigned(), rhs.getUnsigned()));
    default:
        return Value();
    }
}

Error IntelNumberParser::scanNumberEnd(StrScanner &scan, Radix radix, char suffix) {
    auto p = scan;
    if (suffix == 'B') {
        // Check whether Intel binary or C-style binary, '010b' vs '0b10'
        if (p.expect('0') && p.iexpect('B') && isValidDigit(*p, RADIX_2))
            return NOT_AN_EXPECTED;
        p = scan;
    }
    if (isValidDigit(*p, radix)) {
        p.trimStart([radix](char c) { return isValidDigit(c, radix); });
        if (suffix == 0) {
            if (!isalnum(*p)) {
                scan = p;
                return OK;
            }
        } else if (p.iexpect(suffix)) {
            scan = p;
            return OK;
        }
    }
    return NOT_AN_EXPECTED;
}

Error CStyleNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    auto p = scan;
    auto radix = RADIX_NONE;
    if (p.expect('0')) {
        if (!isalnum(*p) || isValidDigit(*p, RADIX_8)) {
            radix = RADIX_8;
            p = scan;
        } else if (p.iexpect('X')) {
            radix = RADIX_16;
        } else if (p.iexpect('B')) {
            radix = RADIX_2;
        } else {
            scan = p;
            return ILLEGAL_CONSTANT;
        }
    } else if (isValidDigit(*p, RADIX_10)) {
        radix = RADIX_10;
    }

    if (radix == RADIX_NONE)
        return NOT_AN_EXPECTED;

    const auto error = val.parseNumber(p, radix);
    scan = p;
    return isalnum(*p) ? ILLEGAL_CONSTANT : error;
}

Error DefaultLetterParser::parseLetter(StrScanner &scan, char &letter) const {
    if (!scan.expect('\''))
        return NOT_AN_EXPECTED;
    ErrorAt error;
    letter = readLetter(scan, error);
    return error.getError() ?: (scan.expect('\'') ? OK : MISSING_CLOSING_QUOTE);
}

char DefaultLetterParser::readLetter(StrScanner &scan, ErrorAt &error) const {
    const auto c = *scan;
    if (c == 0) {
        error.setError(ILLEGAL_CONSTANT);
    } else if (c == '\'') {
        if (scan[1] == '\'') {
            scan += 2;  // successive single quoite
        } else {
            error.setError(MISSING_CLOSING_QUOTE);
        }
    } else {
        scan += 1;
    }
    return c;
}

Error MotorolaLetterParser::parseLetter(StrScanner &scan, char &letter) const {
    if (!scan.expect('\''))
        return NOT_AN_EXPECTED;
    ErrorAt error;
    letter = readLetter(scan, error);
    return error.getError() ?: hasSuffix(scan);
}

char MotorolaLetterParser::readLetter(StrScanner &scan, ErrorAt &error) const {
    if (_closingQuote)
        return DefaultLetterParser::readLetter(scan, error);
    const auto c = *scan;
    if (c == 0) {
        error.setError(ILLEGAL_CONSTANT);
    } else {
        scan += 1;
    }
    return c;
}

Error MotorolaLetterParser::hasSuffix(StrScanner &scan) const {
    return (scan.expect('\'') || !_closingQuote) ? OK : MISSING_CLOSING_QUOTE;
}

Error MotorolaNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    auto p = scan;
    auto radix = RADIX_NONE;
    if (p.expect('$')) {
        radix = RADIX_16;
    } else if (p.expect('@')) {
        radix = RADIX_8;
    } else if (p.expect('%')) {
        radix = RADIX_2;
    } else if (p.expect('&')) {
        radix = RADIX_10;
    } else {
        return CStyleNumberParser::parseNumber(scan, val);
    }

    if (radix == RADIX_NONE || !isValidDigit(*p, radix))
        return NOT_AN_EXPECTED;

    const auto error = val.parseNumber(p, radix);
    scan = p;
    return isalnum(*p) ? ILLEGAL_CONSTANT : error;
}

Error IntelNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    StrScanner p(scan);
    Radix radix = RADIX_NONE;
    if (scanNumberEnd(p, RADIX_16, 'H') == OK) {
        radix = RADIX_16;
    } else if (scanNumberEnd(p, RADIX_8, 'O') == OK) {
        radix = RADIX_8;
    } else if (scanNumberEnd(p, RADIX_8, 'Q') == OK) {
        radix = RADIX_8;
    } else if (scanNumberEnd(p, RADIX_2, 'B') == OK) {
        radix = RADIX_2;
    } else if (scanNumberEnd(p, RADIX_10, 'D') == OK) {
        radix = RADIX_10;
    } else {
        return CStyleNumberParser::parseNumber(scan, val);
    }

    if (radix == RADIX_NONE)
        return NOT_AN_EXPECTED;

    const auto error = val.parseNumber(scan, radix);
    scan = p;
    return error;
}

bool NationalValueParser::symbolLetter(char c, bool head) const {
    if (head && c == '$')
        return true;
    return ValueParser::symbolLetter(c, head);
}

Error NationalNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    auto p = scan;
    auto radix = RADIX_NONE;
    if (p.iexpect('X') || p.iexpect('H')) {
        radix = RADIX_16;
    } else if (p.iexpect('O') || p.iexpect('Q')) {
        radix = RADIX_8;
    } else if (p.iexpect('B')) {
        radix = RADIX_2;
    } else if (p.iexpect('D')) {
        radix = RADIX_10;
    } else {
        return CStyleNumberParser::parseNumber(scan, val);
    }

    if (radix == RADIX_NONE || !p.expect('\'') || !isValidDigit(*p, radix))
        return NOT_AN_EXPECTED;

    const auto error = val.parseNumber(p, radix);
    if (p.expect('\'')) {
        scan = p;
        return error;
    }
    // Closing quote is optional
    scan = p;
    return isalnum(*p) ? ILLEGAL_CONSTANT : error;
}

Error FairchildLetterParser::hasPrefix(StrScanner &scan, char &prefix) {
    auto p = scan;
    // C'x'
    if (p.iexpect('c')) {
        if (*p != '\'')
            return NOT_AN_EXPECTED;
        ++p;
        prefix = 'C';
    } else if (p.expect('#')) {
        // #c
        prefix = '#';
    } else if (p.expect('\'')) {
        // 'c' or 'c
        prefix = '\'';
    } else {
        return NOT_AN_EXPECTED;
    }
    scan = p;
    return OK;
}

Error FairchildLetterParser::parseLetter(StrScanner &scan, char &letter) const {
    char prefix;
    if (hasPrefix(scan, prefix) != OK)
        return NOT_AN_EXPECTED;
    ErrorAt error;
    if (prefix == 'C') {
        letter = readLetter(scan, error);
    } else {
        letter = _motorolaLetter.readLetter(scan, error);
    }
    return error.getError() ?: hasSuffix(scan, prefix);
}

Error FairchildLetterParser::hasSuffix(StrScanner &scan, char prefix) {
    if (prefix == 'C') {
        // closing quote is necessary for C'x'.
        return scan.expect('\'') ? OK : MISSING_CLOSING_QUOTE;
    }
    if (prefix == '#') {
        // no closing quite for #c
        return OK;
    }
    // closing quote is optional for 'x'
    scan.expect('\'');
    return OK;
}

bool FairchildValueParser::symbolLetter(char c, bool head) const {
    return ValueParser::symbolLetter(c, head);
}

Error FairchildNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    auto error = _national.parseNumber(scan, val);
    if (error == NOT_AN_EXPECTED) {
        error = _motorola.parseNumber(scan, val);
        if (error == NOT_AN_EXPECTED)
            return CStyleNumberParser::parseNumber(scan, val);
    }
    return error;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
