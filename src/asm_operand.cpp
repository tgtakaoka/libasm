#include "asm_operand.h"
#include "type_traits.h"

#include <ctype.h>

const char *AsmOperand::eval(
    const char *expr, uint32_t &val32, SymbolTable *symtab) {
    Value v(eval(expr, symtab));
    val32 = v._value;
    return _next;
}

const char *AsmOperand::eval(
    const char *expr, uint16_t &val16, SymbolTable *symtab) {
    Value v(eval(expr, symtab));
    if (getError()) return _next;
    uint32_t val32 = v._value;
    if (int32_t(val32) < -0x8000 || int32_t(val32) >= 0x10000)
        setError(OVERFLOW_RANGE);
    val16 = val32;
    return _next;
}

const char *AsmOperand::eval(
    const char *expr, uint8_t &val8, SymbolTable *symtab) {
    Value v(eval(expr, symtab));
    if (getError()) return _next;
    uint32_t val32 = v._value;
    if (int32_t(val32) < -0x80 || int32_t(val32) >= 0x100)
        setError(OVERFLOW_RANGE);
    val8 = val32;
    return _next;
}

static bool isValidDigit(const char c, const uint8_t base) {
    if (base == 16) return isxdigit(c);
    return c >= '0' && c < '0' + base;
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
    const char *p, uint32_t &val, const uint8_t base, const char suffix) {
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
    val = v;
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
    if (getError() == OK && !v._valid)
        setError(UNDEFINED_SYMBOL);
    return v;
}

AsmOperand::Value AsmOperand::parseExpr() {
    _stack.push(OprAndLval());
    Value value(readAtom());
    if (getError()) return Value();
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
    if (*_next == '(') {
        _next++;
        Value value(parseExpr());
        skipSpaces();
        if (*_next != ')') {
            setError(MISSING_CLOSING_PAREN);
            return Value();
        }
        _next++;
        return value;
    }
    if (*_next == '\'') {
        _next++;
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
    if (*_next == '~') {
        _next++;
        Value value(readAtom());
        value._value = ~value._value;
        return value;
    }
    if (*_next == '-' || *_next == '+') {
        if (_next[1] == '-' || _next[1] == '+') {
            setError(UNKNOWN_EXPR_OPERATOR);
            return Value();
        }
        const char op = *_next++;
        Value value(readAtom());
        if (op == '+') return value;
        if (value._value > 0x80000000)
            setError(OVERFLOW_RANGE);
        value._value = -static_cast<int32_t>(value._value);
        return value;
    }
    if (_symtab && isCurrentAddressSymbol(*_next)) {
        _next++;
        return Value(_symtab->currentAddress());
    }
    if (_symtab && isSymbolLetter(*_next, true)) {
        char symbol[20];
        _next = readSymbol(_next, symbol, symbol + sizeof(symbol) - 1);
        if (_symtab->hasSymbol(symbol))
            return Value(_symtab->lookup(symbol));
        return Value();
    }

    uint32_t val;
    if (readNumber(val))
        return Value();
    return Value(val);
}

AsmOperand::Value AsmOperand::readCharacterConstant() {
    const char *p = _next;
    uint32_t val32;
    if (*p == '\\') {
        p++;
        const char c = *p++;
        if (toupper(c) == 'X') {
            const char *saved_next = _next;
            parseNumber(p, val32, 16);
            p = _next;
            _next = saved_next;
        } else if (isValidDigit(c, 8)) {
            const char *saved_next = _next;
            parseNumber(p - 1, val32, 8);
            p = _next;
            _next = saved_next;
        } else {
            switch (c) {
            case '\'': case '"': case '?': case '\\':
                val32 = c;
                break;
            case 'b': val32 = 0x08; break;
            case 't': val32 = 0x09; break;
            case 'n': val32 = 0x0a; break;
            case 'r': val32 = 0x0d; break;
            default:
                setError(UNKNOWN_ESCAPE_SEQUENCE);
                break;
            }
        }
    } else {
        val32 = *p++;
    }
    if (getError())
        return Value();
    if (val32 >= 0x100)
        return setError(OVERFLOW_RANGE);
    _next = p;
    return Value(static_cast<uint8_t>(val32));
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
    if (isalpha(c) || c == '_' || c == '.') return true;
    return !head && isdigit(c);
}

const char *AsmOperand::readSymbol(
    const char *scan, char *buffer, char *const end) const {
    while (isSymbolLetter(*scan)) {
        if (buffer && buffer < end)
            *buffer++ = *scan;
        scan++;
    }
    if (buffer) *buffer = 0;
    return scan;
}

AsmOperand::Value AsmOperand::evalExpr(
    const Op op, const Value lhs, const Value rhs) {
    if (!lhs._valid || !rhs._valid)
        return Value();
    switch (op) {
    case OP_ADD: return lhs._value + rhs._value;
    case OP_SUB: return lhs._value - rhs._value;
    case OP_MUL: return lhs._value * rhs._value;
    case OP_DIV:
        if (rhs._value == 0) {
            setError(DIVIDE_BY_ZERO);
            return Value();
        }
        return lhs._value / rhs._value;
    case OP_MOD:
        if (rhs._value == 0) {
            setError(DIVIDE_BY_ZERO);
            return Value();
        }
        return lhs._value % rhs._value;
    case OP_BIT_AND: return lhs._value & rhs._value;
    case OP_BIT_XOR: return lhs._value ^ rhs._value;
    case OP_BIT_OR:  return lhs._value | rhs._value;
    case OP_BIT_SHL: return lhs._value << rhs._value;
    case OP_BIT_SHR: return lhs._value >> rhs._value;
    default: return 0;
    }
}

bool AsmMotoOperand::isCurrentAddressSymbol(char c) const {
    return c == '*';
}

Error AsmMotoOperand::readNumber(uint32_t &val) {
    const char *p = _next;
    if (isdigit(*p))
        return parseNumber(p, val, 10);
    if (*p == '$')
        return parseNumber(p + 1, val, 16);
    if (*p == '@')
        return parseNumber(p + 1, val, 8);
    if (*p == '%')
        return parseNumber(p + 1, val, 2);
    return setError(ILLEGAL_CONSTANT);
}

bool AsmIntelOperand::isCurrentAddressSymbol(char c) const {
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

Error AsmIntelOperand::readNumber(uint32_t &val) {
    if (scanNumberEnd(_next, 16, 'H') == OK)
        return parseNumber(_next, val, 16, 'H');
    if (scanNumberEnd(_next, 8, 'O') == OK)
        return parseNumber(_next, val, 8, 'O');
    if (scanNumberEnd(_next, 2, 'B') == OK)
        return parseNumber(_next, val, 2, 'B');
    if (scanNumberEnd(_next, 10) == OK)
        return parseNumber(_next, val, 10);
    return setError(ILLEGAL_CONSTANT);
}
