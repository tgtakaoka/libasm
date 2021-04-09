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

bool Value::overflowUint8() const {
    return getSigned() < -128 || (getSigned() >= 0 && getUnsigned() >= 0x100);
}

bool Value::overflowUint16() const {
    return getSigned() < -32768L ||
           (getSigned() >= 0 && getUnsigned() >= 0x10000L);
}

const char *ValueParser::readChar(const char *expr, char &val) {
    setOK();
    const char *p = expr;
    char c = *p++;
    if (c != '\\') {
        val = c;
        return p;
    }
    uint8_t base = 0;
    c = *p++;
    if (c == 'x' || c == 'X') {
        base = 16;
    } else if (c >= '0' && c < '8') {
        --p;
        base = 8;
    } else {
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
            setError(UNKNOWN_ESCAPE_SEQUENCE);
            return expr;
        }
        val = c;
        return p;
    }
    Value value;
    parseNumber(p, value, base);
    if (getError())
        return expr;
    if (value.overflowUint8()) {
        setError(OVERFLOW_RANGE);
        return expr;
    }
    val = value.getUnsigned();
    return _next;
}

static bool isValidDigit(const char c, const uint8_t base) {
    if (base == 16)
        return isxdigit(c);
    return c >= '0' && c < '0' + base;
}

static uint8_t toNumber(const char c, const uint8_t base) {
    if (base == 16 && c >= 'A')
        return (c & ~0x20) - 'A' + 10;
    return c - '0';
}

Error ValueParser::parseNumber(
        const char *p, Value &val, const uint8_t base, const char suffix) {
    if (!isValidDigit(*p, base))
        return setError(ILLEGAL_CONSTANT);
    uint32_t v = 0;
    while (isValidDigit(*p, base)) {
        const uint32_t prev = v;
        v *= base;
        v += toNumber(*p, base);
        if (v < prev)
            return setError(OVERFLOW_RANGE);
        p++;
    }
    if (suffix && toupper(*p++) != suffix)
        return setError(ILLEGAL_CONSTANT);
    if (isSymbolLetter(*p))
        return setError(ILLEGAL_CONSTANT);
    _next = p;
    if (v & 0x80000000) {
        val.setUnsigned(v);
    } else {
        val.setSigned(v);
    }
    return setOK();
}

const char *ValueParser::skipSpaces(const char *p) const {
    while (*p == ' ' || *p == '\t')
        p++;
    return (_end == nullptr || p < _end) ? p : nullptr;
}

const char *ValueParser::eval(
        const char *expr, const char *end, Value &value, SymbolTable *symtab) {
    _symtab = symtab;
    _stack.clear();
    setOK();
    _end = end;
    value = parseExpr(expr);
    return _next;
}

Value ValueParser::parseExpr(const char *scan) {
    Value value(readAtom(scan));
    if (getError())
        return Value();
    if (_stack.full()) {
        setError(TOO_COMPLEX_EXPRESSION);
        return Value();
    }
    _stack.push(OprAndLval());
    while (!_stack.empty()) {
        Operator opr(readOperator(_next));
        while (opr._precedence <= _stack.top().precedence()) {
            if (_stack.top().isEnd()) {
                _stack.pop();
                return value;
            }
            value = evalExpr(_stack.top()._opr._op, _stack.top()._value, value);
            _stack.pop();
        }
        _stack.push(OprAndLval(opr, value));
        value = readAtom(_next);
        if (getError())
            return Value();
    }
    return Value();
}

Value ValueParser::readAtom(const char *scan) {
    const char *p = skipSpaces(scan);
    if (p == nullptr) {
        setError(ILLEGAL_CONSTANT);
        return Value();
    }
    const char c = *p++;

    if (c == '(') {
        Value value(parseExpr(p));
        if (getError() == OK) {
            const char expected = ')';
            p = skipSpaces(_next);
            if (p == nullptr || *p != expected) {
                setError(MISSING_CLOSING_PAREN);
                return Value();
            }
            _next = p + 1;
            return value;
        }
        return value;
    }
    if (c == '\'') {
        Value value(readCharacterConstant(p));
        p = _next;
        if (getError() == OK) {
            if (*p == '\'') {
                _next = p + 1;
            } else {
                setError(MISSING_CLOSING_QUOTE);
            }
        }
        return value;
    }
    if (c == '~') {
        Value value(readAtom(p));
        if (getError() == OK)
            return value.complement();
        return value;
    }
    if (c == '-' || c == '+') {
        if (*p == '-' || *p == '+') {
            setError(UNKNOWN_EXPR_OPERATOR);
            return Value();
        }
        if (c == '+')
            return readAtom(p);
        Value value(readAtom(p));
        if (getError() == OK) {
            if (value.isUnsigned() && value.getUnsigned() > 0x80000000)
                setError(OVERFLOW_RANGE);
            return value.negate();
        }
        return value;
    }
    if (_symtab && (c == _curSym || c == '.')) {
        _next = p;
        return Value::makeUnsigned(_symtab->currentOrigin());
    }

    p--;
    if (_symtab && isSymbolLetter(c, true)) {
        const char *symbol = p;
        const char *end = scanSymbol(symbol);
        _next = end;
        if (_symtab->hasSymbol(symbol, end)) {
            const uint32_t v = _symtab->lookupSymbol(symbol, end);
            if (v & 0x80000000) {
                return Value::makeSigned(v);
            } else {
                return Value::makeUnsigned(v);
            }
        }
        return Value();
    }

    Value val;
    if (readNumber(p, val))
        return Value();
    return val;
}

Value ValueParser::readCharacterConstant(const char *scan) {
    char c = 0;
    _next = readChar(scan, c);
    if (getError())
        return Value();
    return Value::makeSigned(c);
}

// Operator precedence (larger value means higher precedence).
// The same order of C/C++ language.
ValueParser::Operator ValueParser::readOperator(const char *scan) {
    const char *p = skipSpaces(scan);
    if (p == nullptr)
        return Operator(OP_NONE, 0);
    const char c = *p++;
    _next = p;
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
        if (*p++ == '<') {
            _next = p;
            return Operator(OP_BIT_SHL, 11);
        }
        setError(UNKNOWN_EXPR_OPERATOR);
        break;
    case '>':
        if (*p++ == '>') {
            _next = p;
            return Operator(OP_BIT_SHR, 11);
        }
        setError(UNKNOWN_EXPR_OPERATOR);
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
    _next = scan;
    return Operator(OP_NONE, 0);
}

bool ValueParser::isSymbolLetter(char c, bool head) const {
    if (isalpha(c) || c == '_')
        return true;
    if (head && c == '.')
        return true;
    return !head && isdigit(c);
}

const char *ValueParser::scanSymbol(const char *scan) const {
    while (isSymbolLetter(*scan))
        scan++;
    return scan;
}

static uint32_t shift_left(uint32_t value, uint8_t count) {
    for (unsigned i = 0; i <= 32 && i < count; i++)
        value <<= 1;
    return value;
}

static uint32_t shift_right(uint32_t value, uint8_t count, bool sign) {
    for (unsigned i = 0; i <= 32 && i < count; i++) {
        value >>= 1;
        if (sign)
            value |= 0x80000000;
    }
    return value;
}

Value ValueParser::evalExpr(const Op op, const Value lhs, const Value rhs) {
    if (lhs.isUndefined() || rhs.isUndefined())
        return Value();
    switch (op) {
    case OP_ADD:
        if (lhs.isSigned() && rhs.isSigned())
            return Value::makeSigned(lhs.getSigned() + rhs.getSigned());
        return Value::makeUnsigned(lhs.getUnsigned() + rhs.getUnsigned());
    case OP_SUB:
        if (lhs.isSigned() && rhs.isSigned())
            return Value::makeSigned(lhs.getSigned() - rhs.getSigned());
        return Value::makeUnsigned(lhs.getUnsigned() - rhs.getUnsigned());
    case OP_MUL:
        if (lhs.isSigned() && rhs.isSigned())
            return Value::makeSigned(lhs.getSigned() * rhs.getSigned());
        return Value::makeUnsigned(lhs.getUnsigned() * rhs.getUnsigned());
    case OP_DIV:
        if (rhs.getUnsigned() == 0) {
            setError(DIVIDE_BY_ZERO);
            return Value();
        }
        if (lhs.isSigned() && rhs.isSigned())
            return Value::makeSigned(lhs.getSigned() / rhs.getSigned());
        return Value::makeUnsigned(lhs.getUnsigned() / rhs.getUnsigned());
    case OP_MOD:
        if (rhs.getUnsigned() == 0) {
            setError(DIVIDE_BY_ZERO);
            return Value();
        }
        if (lhs.isSigned() && rhs.isSigned())
            return Value::makeSigned(lhs.getSigned() % rhs.getSigned());
        return Value::makeUnsigned(lhs.getUnsigned() % rhs.getUnsigned());
    case OP_BIT_AND:
        if (lhs.isSigned() && rhs.isSigned())
            return Value::makeSigned(lhs.getSigned() & rhs.getSigned());
        return Value::makeUnsigned(lhs.getUnsigned() & rhs.getUnsigned());
    case OP_BIT_XOR:
        if (lhs.isSigned() && rhs.isSigned())
            return Value::makeSigned(lhs.getSigned() ^ rhs.getSigned());
        return Value::makeUnsigned(lhs.getUnsigned() ^ rhs.getUnsigned());
    case OP_BIT_OR:
        if (lhs.isSigned() && rhs.isSigned())
            return Value::makeSigned(lhs.getSigned() | rhs.getSigned());
        return Value::makeUnsigned(lhs.getUnsigned() | rhs.getUnsigned());
    case OP_BIT_SHL:
        if (lhs.isSigned())
            return Value::makeSigned(
                    shift_left(lhs.getSigned(), rhs.getUnsigned()));
        return Value::makeUnsigned(
                shift_left(lhs.getUnsigned(), rhs.getUnsigned()));
    case OP_BIT_SHR:
        if (lhs.isSigned())
            return Value::makeSigned(shift_right(lhs.getSigned(),
                    rhs.getUnsigned(), lhs.getSigned() & 0x80000000));
        return Value::makeUnsigned(
                shift_right(lhs.getUnsigned(), rhs.getUnsigned(), false));
    default:
        return Value();
    }
}

Error ValueParser::scanNumberEnd(
        const char *scan, const uint8_t base, char suffix) {
    if (suffix == 'B') {
        // Check whether intel binary or C-style binary
        const char *p = scan;
        if (*p++ == '0' && toupper(*p++) == 'B' && (*p == '0' || *p == '1'))
            return ILLEGAL_CONSTANT;  // expect intel but found C-style
    }
    while (isValidDigit(*scan, base))
        scan++;
    if (suffix && toupper(*scan++) != suffix)
        return ILLEGAL_CONSTANT;
    return OK;
}

Error ValueParser::readNumber(const char *scan, Value &val) {
    if (*scan == '0') {
        scan++;
        const char c = toupper(*scan);
        if (c >= '0' && c < '8' && scanNumberEnd(scan, 8) == OK)
            return parseNumber(scan, val, 8);
        scan++;
        if (c == 'X' && scanNumberEnd(scan, 16) == OK)
            return parseNumber(scan, val, 16);
        if (c == 'B' && scanNumberEnd(scan, 2) == OK)
            return parseNumber(scan, val, 2);
        scan -= 2;
    }
    if (*scan >= '0' && *scan <= '9' && scanNumberEnd(scan, 10) == OK)
        return parseNumber(scan, val, 10);
    return setError(ILLEGAL_CONSTANT);
}

Error MotoValueParser::readNumber(const char *scan, Value &val) {
    if (ValueParser::readNumber(scan, val) != ILLEGAL_CONSTANT)
        return getError();
    const char c = *scan++;
    if (c == '$' && scanNumberEnd(scan, 16) == OK)
        return parseNumber(scan, val, 16);
    if (c == '@' && scanNumberEnd(scan, 8) == OK)
        return parseNumber(scan, val, 8);
    if (c == '%' && scanNumberEnd(scan, 2) == OK)
        return parseNumber(scan, val, 2);
    return setError(ILLEGAL_CONSTANT);
}

Error IntelValueParser::readNumber(const char *scan, Value &val) {
    if (scanNumberEnd(scan, 16, 'H') == OK)
        return parseNumber(scan, val, 16, 'H');
    if (scanNumberEnd(scan, 8, 'O') == OK)
        return parseNumber(scan, val, 8, 'O');
    if (scanNumberEnd(scan, 2, 'B') == OK)
        return parseNumber(scan, val, 2, 'B');
    return ValueParser::readNumber(scan, val);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
