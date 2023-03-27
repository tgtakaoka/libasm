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
    setUnsigned(v);
    return error;
}

Value ValueParser::eval(StrScanner &expr, ErrorAt &error, const SymbolTable *symtab) const {
    Stack<const Operator *> ostack;
    Stack<Value> vstack;
    ostack.push(&Operator::OP_NONE);
    return parseExpr(expr, error, ostack, vstack, symtab);
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

Value ValueParser::parseExpr(StrScanner &scan, ErrorAt &error, Stack<const Operator *> &ostack,
        Stack<Value> &vstack, const SymbolTable *symtab) const {
    Value value;
    while (true) {
        value = readAtom(scan, error, ostack, vstack, symtab);
        if (error.hasError())
            break;
        const auto *opr = endOfLine(scan.skipSpaces()) ? &Operator::OP_NONE
                                                       : _operator.readBinary(scan, error);
        if (error.hasError())
            break;
        while (ostack.top()->hasHigherPriority(*opr)) {
            if (ostack.top() == &Operator::OP_NONE) {
                ostack.pop();
                return value;
            }
            const auto err = ostack.top()->eval(value, vstack.top(), value);
            if (err)
                error.setErrorIf(err);
            ostack.pop();
            vstack.pop();
        }
        if (ostack.full() || vstack.full()) {
            error.setError(scan, TOO_COMPLEX_EXPRESSION);
            break;
        }
        ostack.push(opr);
        vstack.push(value);
    }
    return value;
}

Value ValueParser::readAtom(StrScanner &scan, ErrorAt &error, Stack<const Operator *> &ostack,
        Stack<Value> &vstack, const SymbolTable *symtab) const {
    auto p = scan;
    if (endOfLine(p.skipSpaces())) {
        error.setError(scan, ILLEGAL_CONSTANT);
        return Value();
    }

    Value val;
    if (p.expect('(')) {
        if (ostack.full()) {
            error.setError(p, TOO_COMPLEX_EXPRESSION);
        } else {
            ostack.push(&Operator::OP_NONE);
            val = parseExpr(p, error, ostack, vstack, symtab);
            if (!p.skipSpaces().expect(')'))
                error.setErrorIf(p, MISSING_CLOSING_PAREN);
            scan = p;
        }
        return val;
    }

    const auto *opr = _operator.readUnary(p, error);
    if (error.hasError())
        return val;
    if (opr != &Operator::OP_NONE) {
        const auto atom = readAtom(p, error, ostack, vstack, symtab);
        scan = p;
        const auto err = opr->eval(val, atom);
        if (err)
            error.setErrorIf(err);
        return val;
    }

    char letter;
    auto err = _letter.parseLetter(p, letter);
    if (err == OK) {
        scan = p;
        return val.setUnsigned(letter);
    } else if (err != NOT_AN_EXPECTED) {
        error.setError(p, err);
        return val;
    }

    err = _number.parseNumber(p, val);
    if (err != NOT_AN_EXPECTED) {
        if (err)
            error.setErrorIf(scan, err);
        scan = p;
        return val;
    }

    if (_location.locationSymbol(p)) {
        scan = p;
        return val.setUnsigned(_origin);
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
            return val.setSigned(v);
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

StrScanner ValueParser::readSymbol(StrScanner &scan) const {
    auto p = scan;
    if (!_symbol.symbolLetter(*p++, true))
        return StrScanner::EMPTY;
    p.trimStart([this](char c) { return this->_symbol.symbolLetter(c); });
    auto symbol = StrScanner(scan.str(), p.str());
    scan = p;
    return symbol;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
