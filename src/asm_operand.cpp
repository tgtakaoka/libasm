#include "asm_operand.h"
#include "type_traits.h"

#include <ctype.h>

const char *AsmOperand::eval(
    const char *expr, uint32_t &val32, SymbolTable *symtab) {
    _symtab = symtab;
    _next = expr;
    _stack.clear();
    setError(OK);
    Value v = parseExpr();
    // TODO: Check validity.
    val32 = v._value;
    return _next;
}

const char *AsmOperand::eval(
    const char *expr, uint16_t &val16, SymbolTable *symtab) {
    uint32_t val32 = 0;
    eval(expr, val32, symtab);
    if (getError()) return _next;
    if (int32_t(val32) < 0 && int32_t(val32) < -0x8000)
        setError(OVERFLOW_RANGE);
    if (int32_t(val32) >= 0 && val32 >= 0x10000)
        setError(OVERFLOW_RANGE);
    val16 = val32;
    return _next;
}

const char *AsmOperand::eval(
    const char *expr, uint8_t &val8, SymbolTable *symtab) {
    uint32_t val32 = 0;
    eval(expr, val32, symtab);
    if (getError()) return _next;
    if (int32_t(val32) < 0 && int32_t(val32) < -0x80)
        setError(OVERFLOW_RANGE);
    if (int32_t(val32) >= 0 && val32 >= 0x100)
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

const char *AsmOperand::parseNumber(
    const char *p, uint32_t &val, const uint8_t base) {
    if (!isValidDigit(*p, base)) {
        setError(ILLEGAL_CONSTANT);
        return nullptr;
    }
    uint32_t v = 0;
    while (isValidDigit(*p, base)) {
        if (checkOverflow(v, toNumber(*p, base), base)) {
            setError(OVERFLOW_RANGE);
            return nullptr;
        }
        p++;
    }
    setError(OK);
    val = v;
    return p;
}

void AsmOperand::skipSpaces() {
    while (isspace(*_next))
        _next++;
}

AsmOperand::Value AsmOperand::parseExpr() {
    _stack.push(OprAndLval());
    Value value = readAtom();
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
    return readConstant();
}

#include <stdio.h>
AsmOperand::Value AsmOperand::readConstant() {
    uint32_t val;
    const char *p;
    switch (*_next) {
    case '~':
        p = parseConstant(_next + 1, val);
        val = ~val;
        break;
    case '+':
        p = parseConstant(_next + 1, val);
        break;
    case '-':
        p = parseConstant(_next + 1, val);
        if (getError()) break;
        if (val > 0x80000000) {
            setError(OVERFLOW_RANGE);
            p = nullptr;
        } else {
            val = -val;
        }
        break;
    default:
        p = parseConstant(_next, val);
        break;
    }
    if (p == nullptr)
        return Value();
    _next = p;
    return Value(val);
}

AsmOperand::Operator AsmOperand::readOperator() {
    skipSpaces();
    switch (*_next) {
    case '*': _next++; return Operator(OP_MUL, 6);
    case '/': _next++; return Operator(OP_DIV, 6);
    case '%': _next++; return Operator(OP_MOD, 6);
    case '+': _next++; return Operator(OP_ADD, 5);
    case '-': _next++; return Operator(OP_SUB, 5);
    case '<':
        if (*++_next != '<') {
            setError(UNKNOWN_EXPR_OPERATOR);
            break;
        }
        _next++; return Operator(OP_BIT_SHL, 4);
    case '>':
        if (*++_next != '>') {
            setError(UNKNOWN_EXPR_OPERATOR);
            break;
        }
        _next++; return Operator(OP_BIT_SHR, 4);
    case '&': _next++; return Operator(OP_BIT_AND, 3);
    case '^': _next++; return Operator(OP_BIT_XOR, 2);
    case '|': _next++; return Operator(OP_BIT_OR,  1);
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

const char *AsmMotoOperand::parseConstant(const char *p, uint32_t &val) {
    if (isdigit(*p)) {
        p = parseNumber(p, val, 10);
    } else if (*p == '$') {
        p = parseNumber(p + 1, val, 16);
    } else if (*p == '@') {
        p = parseNumber(p + 1, val, 8);
    } else if (*p == '%') {
        p = parseNumber(p + 1, val, 2);
    } else {
        p = nullptr;
        setError(ILLEGAL_CONSTANT);
    }
    return p;
}

bool AsmIntelOperand::isCurrentAddressSymbol(char c) const {
    return c == '$';
}

static const char *scanConstEnd(
    const char *p, const uint8_t base, char suffix = 0) {
    while (isValidDigit(*p, base))
        p++;
    if (suffix == 0 || toupper(*p++) == suffix)
        return p;
    return nullptr;
}

const char *AsmIntelOperand::parseConstant(const char *scan, uint32_t &val) {
    const char *p;
    if ((p = scanConstEnd(scan, 16, 'H'))) {
        parseNumber(scan, val, 16);
        return p;
    }
    if ((p = scanConstEnd(scan, 10))
        && (toupper(*p) != 'O' && toupper(*p) != 'B')) {
        parseNumber(scan, val, 10);
        return p;
    }
    if ((p = scanConstEnd(scan, 8, 'O'))) {
        parseNumber(scan, val, 8);
        return p;
    }
    if ((p = scanConstEnd(scan, 2, 'B'))) {
        parseNumber(scan, val, 2);
        return p;
    }
    setError(ILLEGAL_CONSTANT);
    return nullptr;
}
