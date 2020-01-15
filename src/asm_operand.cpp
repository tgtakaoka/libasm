#include "asm_operand.h"
#include "type_traits.h"

#include <ctype.h>

const char *AsmOperand::eval(
    const char *expr, uint32_t &val32, SymbolTable *symtab) {
    Value v(eval(expr, symtab));
    if (getError()) return _next;
    val32 = v.getUnsigned();
    return _next;
}

const char *AsmOperand::eval(
    const char *expr, uint16_t &val16, SymbolTable *symtab) {
    Value v(eval(expr, symtab));
    if (getError()) return _next;
    if (v.isSigned() && v.getSigned() >= -32768 && v.getSigned() < 65536L) {
        val16 = v.getSigned();
    } else if (v.isUnsigned() && v.getUnsigned() < 0x10000L) {
        val16 = v.getUnsigned();
    } else {
        setError(OVERFLOW_RANGE);
    }
    return _next;
}

const char *AsmOperand::eval(
    const char *expr, uint8_t &val8, SymbolTable *symtab) {
    Value v(eval(expr, symtab));
    if (getError()) return _next;
    if (v.isSigned() && v.getSigned() >= -128 && v.getSigned() < 256) {
        val8 = v.getSigned();
    } else if (v.isUnsigned() && v.getUnsigned() < 0x100) {
        val8 = v.getUnsigned();
    } else {
        setError(OVERFLOW_RANGE);
    }
    return _next;
}

static bool isValidDigit(const char c, const uint8_t base) {
    if (base == 16) return isxdigit(c);
    return c >= '0' && c < '0' + base;
}

const char *AsmOperand::readChar(const char *scan, char &c) {
    setError(OK);
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

Error AsmOperand::parseNumber(
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
    return setError(OK);
}

void AsmOperand::skipSpaces() {
    while (isspace(*_next))
        _next++;
}

AsmOperand::Value AsmOperand::eval(const char *expr, SymbolTable *symtab) {
    _symtab = symtab;
    _next = expr;
    _stack.clear();
    setError(OK);
    Value v(parseExpr());
    if (getError() == OK && v.isUndefined())
        setError(UNDEFINED_SYMBOL);
    return v;
}

AsmOperand::Value AsmOperand::parseExpr() {
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

AsmOperand::Value AsmOperand::readAtom() {
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
            if (value.isSigned()) value.setSigned(~value.getSigned());
            else value.setUnsigned(~value.getUnsigned());
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
            value.setSigned(-value.getSigned());
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
        if (_symtab->hasSymbol(symbol, end)) {
            _next = end;
            const uint32_t v = _symtab->lookup(symbol, end);
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

AsmOperand::Value AsmOperand::readCharacterConstant() {
    char c;
    _next = readChar(_next, c);
    if (getError()) return Value();
    return Value::makeSigned(c);
}

// Operator precedence (larger value means higher precedence).
// The same order of C/C++ language.
AsmOperand::Operator AsmOperand::readOperator() {
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

bool AsmOperand::isSymbolLetter(char c, bool head) const {
    if (isalpha(c) || c == '_') return true;
    if (head && c == '.') return true;
    return !head && isdigit(c);
}

const char *AsmOperand::scanSymbol(const char *scan) const {
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

AsmOperand::Value AsmOperand::evalExpr(
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

bool AsmOperand::isCurrentOriginSymbol(char c) const {
    return c == '*';
}

Error AsmOperand::readNumber(Value &val) {
    const char *p = _next;
    if (*p == '0') {
        const char c = toupper(p[1]);
        if (c == 'X')
            return parseNumber(p + 2, val, 16);
        if (isValidDigit(c, 8))
            return parseNumber(p + 1, val, 8);
        if (c == 'B')
            return parseNumber(p + 2, val, 2);
    }
    if (isdigit(*p))
        return parseNumber(p, val, 10);
    return setError(ILLEGAL_CONSTANT);
}

bool AsmMotoOperand::isCurrentOriginSymbol(char c) const {
    return c == '*';
}

Error AsmMotoOperand::readNumber(Value &val) {
    const char *p = _next;
    if (isdigit(*p))
        return parseNumber(p, val, 10);
    if (*p == '$')
        return parseNumber(p + 1, val, 16);
    if (*p == '@')
        return parseNumber(p + 1, val, 8);
    if (*p == '%')
        return parseNumber(p + 1, val, 2);
    return AsmOperand::readNumber(val);
}

bool AsmIntelOperand::isCurrentOriginSymbol(char c) const {
    return c == '$';
}

Error AsmIntelOperand::scanNumberEnd(
    const char *p, const uint8_t base, char suffix) {
    while (isValidDigit(*p, base))
        p++;
    if (suffix && toupper(*p++) != suffix)
        return ILLEGAL_CONSTANT;
    return OK;
}

Error AsmIntelOperand::readNumber(Value &val) {
    if (scanNumberEnd(_next, 16, 'H') == OK)
        return parseNumber(_next, val, 16, 'H');
    if (scanNumberEnd(_next, 8, 'O') == OK)
        return parseNumber(_next, val, 8, 'O');
    if (scanNumberEnd(_next, 2, 'B') == OK)
        return parseNumber(_next, val, 2, 'B');
    if (scanNumberEnd(_next, 10) == OK)
        return parseNumber(_next, val, 10);
    return AsmOperand::readNumber(val);
}
