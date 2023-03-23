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
#include "parsers.h"

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

char ValueParser::readLetter(StrScanner &scan, ErrorAt &error) const {
    return _letter.readLetter(scan, error);
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
        auto err = _number.parseNumber(p, val);
        if (err == OK)
            continue;
        char letter;
        err = _letter.parseLetter(p, letter);
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
        }
        return value;
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

    char letter;
    auto err = _letter.parseLetter(p, letter);
    if (err == OK) {
        scan = p;
        return Value::makeUnsigned(letter);
    } else if (err != NOT_AN_EXPECTED) {
        error.setError(p, err);
        return Value();
    }

    Value val;
    err = _number.parseNumber(p, val);
    if (err != NOT_AN_EXPECTED) {
        if (err)
            error.setErrorIf(scan, err);
        scan = p;
        return val;
    }

    if (_location.locationSymbol(p)) {
        scan = p;
        return Value::makeUnsigned(_origin);
    }

    const auto symbol = readSymbol(p);
    if (symbol.size()) {
        if (*p.skipSpaces() == '(' && _funCall) {
            auto params = p;
            params.expect('(');
            const auto err = _funCall->parseFunCall(symbol, params, val, error, *this, symtab);
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

FunCallParser *ValueParser::setFunCallParser(FunCallParser *parser) {
    auto prev = _funCall;
    _funCall = parser;
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

StrScanner ValueParser::readSymbol(StrScanner &scan) const {
    auto p = scan;
    if (!_symbol.symbolLetter(*p++, true))
        return StrScanner::EMPTY;
    p.trimStart([this](char c) { return this->_symbol.symbolLetter(c); });
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

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
