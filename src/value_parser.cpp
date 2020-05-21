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
    return getSigned() < -32768L || (getSigned() >= 0 && getUnsigned() >= 0x10000L);
}

const char *ValueParser::eval(
    const char *expr, uint32_t &val32, SymbolTable *symtab) {
    Value val;
    const char *next = eval(expr, val, symtab);
    val32 = val.getUnsigned();
    return next;
}

const char *ValueParser::eval(
    const char *expr, uint16_t &val16, SymbolTable *symtab) {
    Value val;
    const char *next = eval(expr, val, symtab);
    if (val.overflowUint16()) {
        setError(OVERFLOW_RANGE);
    } else {
        val16 = val.getUnsigned();
    }
    return next;
}

const char *ValueParser::eval(
    const char *expr, uint8_t &val8, SymbolTable *symtab) {
    Value val;
    const char *next = eval(expr, val, symtab);
    if (val.overflowUint8()) {
        setError(OVERFLOW_RANGE);
    } else {
        val8 = val.getUnsigned();
    }
    return next;
}

static bool isValidDigit(const char c, const uint8_t base) {
    if (base == 16) return isxdigit(c);
    return c >= '0' && c < '0' + base;
}

const char *ValueParser::readChar(const char *scan, char &c) {
    setOK();
    if (*scan == '\\') {
        uint8_t base = 0;
        if (toupper(scan[1]) == 'X') base = 16;
        if (isValidDigit(scan[1], 8)) base = 8;
        if (base) {
            Value val;
            parseNumber(base == 16 ? scan + 2 : scan + 1, val, base);
            if (getError()) return scan;
            if (val.getSigned() >= 0x100 || val.getSigned() < -0x80) {
                setError(OVERFLOW_RANGE);
                return scan;
            }
            c = val.getUnsigned();
            return _next;
        }
        switch (scan[1]) {
        case '\'': case '"': case '?': case '\\':
            c = scan[1];
            break;
        case 'b': c = 0x08; break;
        case 't': c = 0x09; break;
        case 'n': c = 0x0a; break;
        case 'r': c = 0x0d; break;
        default:
            setError(UNKNOWN_ESCAPE_SEQUENCE);
            return scan;
        }
        return scan + 2;
    }
    c = *scan;
    return scan + 1;
}

static uint8_t toNumber(const char c, const uint8_t base) {
    if (base == 16 && !isdigit(c))
        return toupper(c) - 'A' + 10;
    return c - '0';
}

static bool checkOverflow(
    uint32_t &val, const uint8_t digit, const uint8_t base) {
    if (base == 16) {
        if (val >= 0x10000000) return true;
    } else if (base == 8) {
        if (val >= 0x20000000) return true;
    } else if (base == 2) {
        if (val >= 0x80000000) return true;
    } else {
        if (val >= 429496730)  return true;
        if (val >= 429496729 && digit >= 6) return true;
    }
    val *= base;
    val += digit;
    return false;
}

Error ValueParser::parseNumber(
    const char *p, Value &val, const uint8_t base, const char suffix) {
    if (!isValidDigit(*p, base))
        return setError(ILLEGAL_CONSTANT);
    uint32_t v = 0;
    while (isValidDigit(*p, base)) {
        if (checkOverflow(v, toNumber(*p, base), base))
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

void ValueParser::skipSpaces() {
    while (isspace(*_next))
        _next++;
}

const char *ValueParser::eval(
    const char *expr, Value &value, SymbolTable *symtab) {
    _symtab = symtab;
    _next = expr;
    _stack.clear();
    setOK();
    value = parseExpr();
    if (getError() == OK && value.isUndefined())
        setError(UNDEFINED_SYMBOL);
    return _next;
}

Value ValueParser::parseExpr() {
    Value value(readAtom());
    if (getError()) return Value();
    if (_stack.full()) {
        setError(TOO_COMPLEX_EXPRESSION);
        return Value();
    }
    _stack.push(OprAndLval());
    while (!_stack.empty()) {
        Operator opr(readOperator());
        while (opr._precedence <= _stack.top().precedence()) {
            if (_stack.top().isEnd()) {
                _stack.pop();
                return value;
            }
            value = evalExpr(
                _stack.top()._opr._op, _stack.top()._value, value);
            _stack.pop();
        }
        _stack.push(OprAndLval(opr, value));
        value = readAtom();
        if (getError()) return Value();
    }
    return Value();
}

Value ValueParser::readAtom() {
    skipSpaces();
    const char c = *_next++;

    if (c == '(' || c == '[') {
        Value value(parseExpr());
        if (getError() == OK) {
            skipSpaces();
            const char expected = (c == '(') ? ')' : ']';
            if (*_next != expected) {
                setError(MISSING_CLOSING_PAREN);
                return Value();
            }
            _next++;
        }
        return value;
    }
    if (c == '\'') {
        Value value(readCharacterConstant());
        if (getError() == OK) {
            if (*_next == '\'') {
                _next++;
            } else {
                setError(MISSING_CLOSING_QUOTE);
            }
        }
        return value;
    }
    if (c == '~') {
        Value value(readAtom());
        if (getError() == OK) {
            if (value.isSigned())   value.setSigned(~value.getSigned());
            if (value.isUnsigned()) value.setUnsigned(~value.getUnsigned());
        }
        return value;
    }
    if (c == '-' || c == '+') {
        if (*_next == '-' || *_next == '+') {
            setError(UNKNOWN_EXPR_OPERATOR);
            return Value();
        }
        const char op = c;
        Value value(readAtom());
        if (getError() == OK) {
            if (op == '+') return value;
            if (value.isUnsigned() && value.getUnsigned() > 0x80000000)
                setError(OVERFLOW_RANGE);
            if (!value.isUndefined()) value.setSigned(-value.getSigned());
        }
        return value;
    }
    if (_symtab && isCurrentOriginSymbol(c)) {
        return Value::makeUnsigned(_symtab->currentOrigin());
    }

    _next--;
    if (_symtab && isSymbolLetter(c, true)) {
        const char *symbol = _next;
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
    if (readNumber(val))
        return Value();
    return val;
}

Value ValueParser::readCharacterConstant() {
    char c;
    _next = readChar(_next, c);
    if (getError()) return Value();
    return Value::makeSigned(c);
}

// Operator precedence (larger value means higher precedence).
// The same order of C/C++ language.
ValueParser::Operator ValueParser::readOperator() {
    skipSpaces();
    switch (*_next) {
    case '*': _next++; return Operator(OP_MUL, 13);
    case '/': _next++; return Operator(OP_DIV, 13);
    case '%': _next++; return Operator(OP_MOD, 13);
    case '+': _next++; return Operator(OP_ADD, 12);
    case '-': _next++; return Operator(OP_SUB, 12);
    case '<':
        if (_next[1] == '<') {
            _next += 2; return Operator(OP_BIT_SHL, 11);
        }
        setError(UNKNOWN_EXPR_OPERATOR);
        break;
    case '>':
        if (_next[1] == '>') {
            _next += 2; return Operator(OP_BIT_SHR, 11);
        }
        setError(UNKNOWN_EXPR_OPERATOR);
        break;
    case '&': _next++; return Operator(OP_BIT_AND, 8);
    case '^': _next++; return Operator(OP_BIT_XOR, 7);
    case '|': _next++; return Operator(OP_BIT_OR,  6);
    default: break;
    }
    return Operator(OP_NONE, 0);
}

bool ValueParser::isSymbolLetter(char c, bool head) const {
    if (isalpha(c) || c == '_') return true;
    if (head && c == '.') return true;
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
        if (sign) value |= 0x80000000;
    }
    return value;
}

Value ValueParser::evalExpr(
    const Op op, const Value lhs, const Value rhs) {
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
            return Value::makeSigned(
                shift_right(lhs.getSigned(), rhs.getUnsigned(),
                            lhs.getSigned() & 0x80000000));
        return Value::makeUnsigned(
            shift_right(lhs.getUnsigned(), rhs.getUnsigned(), false));
    default:
        return Value();
    }
}

bool ValueParser::isCurrentOriginSymbol(char c) const {
    return c == '*' || c == '$';
}

Error ValueParser::readNumber(Value &val) {
    const char *p = _next;
    if (*p == '0') {
        const char c = toupper(p[1]);
        if (c == 'X' && scanNumberEnd(p + 2, 16) == OK)
            return parseNumber(p + 2, val, 16);
        if (isValidDigit(c, 8) && scanNumberEnd(p + 1, 8) == OK)
            return parseNumber(p + 1, val, 8);
        if (c == 'B' && scanNumberEnd(p + 2, 2) == OK)
            return parseNumber(p + 2, val, 2);
    }
    if (isdigit(*p) && scanNumberEnd(p, 10) == OK)
        return parseNumber(p, val, 10);
    return setError(ILLEGAL_CONSTANT);
}

Error ValueParser::scanNumberEnd(
    const char *p, const uint8_t base, char suffix) {
    while (isValidDigit(*p, base))
        p++;
    if (suffix && toupper(*p++) != suffix)
        return ILLEGAL_CONSTANT;
    return OK;
}

bool MotoValueParser::isCurrentOriginSymbol(char c) const {
    return c == '*';
}

Error MotoValueParser::readNumber(Value &val) {
    const char *p = _next;
    if (*p == '$' && scanNumberEnd(p + 1, 16) == OK)
        return parseNumber(p + 1, val, 16);
    if (*p == '@' && scanNumberEnd(p + 1, 8) == OK)
        return parseNumber(p + 1, val, 8);
    if (*p == '%' && scanNumberEnd(p + 1, 2) == OK)
        return parseNumber(p + 1, val, 2);
    if (scanNumberEnd(_next, 10) == OK)
        return parseNumber(p, val, 10);
    return ValueParser::readNumber(val);
}

bool IntelValueParser::isCurrentOriginSymbol(char c) const {
    return c == '$';
}

Error IntelValueParser::readNumber(Value &val) {
    if (scanNumberEnd(_next, 16, 'H') == OK)
        return parseNumber(_next, val, 16, 'H');
    if (scanNumberEnd(_next, 8, 'O') == OK)
        return parseNumber(_next, val, 8, 'O');
    if (scanNumberEnd(_next, 2, 'B') == OK)
        return parseNumber(_next, val, 2, 'B');
    if (scanNumberEnd(_next, 10) == OK)
        return parseNumber(_next, val, 10);
    return ValueParser::readNumber(val);
}

} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
