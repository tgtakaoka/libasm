#include "asm_operand.h"
#include "symbol_table.h"
#include "type_traits.h"

#include <ctype.h>
#include <string.h>

#include <stdio.h>

const char *AsmOperand::eval(const char *expr, uint32_t &val32) {
    _next = expr;
    _error = false;
    _stack.clear();
    Value v = parseExpr();
    // TODO: Check validity.
    val32 = v._value;
    return _error ? nullptr : _next;
}

const char *AsmOperand::eval(const char *expr, uint16_t &val16) {
    uint32_t val32 = 0;
    const char *p = eval(expr, val32);
    if (int32_t(val32) < 0 && int32_t(val32) < -32768) {
        printf("@@@@@ expr='%s' overflow 16bit 0x%x\n", expr, val32);
        return nullptr;     // overflow
    }
    if (int32_t(val32) >= 0 && val32 >= 0x10000) {
        printf("@@@@@ expr='%s' overflow 16bit 0x%x\n", expr, val32);
        return nullptr;     // overflow
    }
    val16 = val32;
    return p;
}

const char *AsmOperand::eval(const char *expr, uint8_t &val8) {
    uint32_t val32 = 0;
    const char *p = eval(expr, val32);
    if (int32_t(val32) < 0 && int32_t(val32) < -128) {
        printf("@@@@@ expr='%s' overflow 8bit 0x%x\n", expr, val32);
        return nullptr;     // overflow
    }
    if (int32_t(val32) >= 0 && val32 >= 0x100) {
        printf("@@@@@ expr='%s' overflow 8bit 0x%x\n", expr, val32);
        return nullptr;     // overflow
    }
    val8 = val32;
    return p;
}

bool AsmOperand::isValidDigit(const char c, const uint8_t radix) {
    if (radix == 16) return isxdigit(c);
    return c >= '0' && c < '0' + radix;
}

uint8_t AsmOperand::toNumber(const char c, const uint8_t radix) {
    if (radix == 16 && !isdigit(c))
        return toupper(c) - 'A' + 10;
    return c - '0';
}

const char *AsmOperand::parseNumber(
    const char *p, uint32_t &val, const uint8_t radix) {
    if (!isValidDigit(*p, radix))
        return nullptr;
    uint32_t v = 0;
    while (isValidDigit(*p, radix)) {
        v *= radix;
        v += toNumber(*p, radix);
        p++;
    }
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
            // Missing closing ')'.
            _error = true;
            return Value();
        }
        _next++;
        return value;
    }
    if (_symtab && isSymbolLetter(*_next, true)) {
        char symbol[20];
        readSymbol(symbol, symbol + sizeof(symbol) - 1);
        if (_symtab->hasSymbol(symbol))
            return Value(_symtab->lookup(symbol));
        return Value();
    }
    uint32_t val;
    const char *p = parseConstant(_next, val);
    if (p == nullptr) {
        // Unknown constant.
        _error = true;
        return 0;
    }
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
        if (*++_next != '<') // Unknown operator.
            break;
        _next++; return Operator(OP_BIT_SHL, 4);
    case '>':
        if (*++_next != '>') // Unknown operator.
            break;
        _next++; return Operator(OP_BIT_SHR, 4);
    case '&': _next++; return Operator(OP_BIT_AND, 3);
    case '^': _next++; return Operator(OP_BIT_XOR, 2);
    case '|': _next++; return Operator(OP_BIT_OR,  1);
    default: break;
    }
    return Operator(OP_NONE, 0);
}

void AsmOperand::readSymbol(char *buffer, char *const end) {
    while (isSymbolLetter(*_next)) {
        if (buffer < end)
            *buffer++ = *_next;
        _next++;
    }
    *buffer = 0;
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
        if (rhs._value == 0) return Value();
        return lhs._value / rhs._value;
    case OP_MOD:
        if (rhs._value == 0) return Value();
        return lhs._value % rhs._value;
    case OP_BIT_AND: return lhs._value & rhs._value;
    case OP_BIT_XOR: return lhs._value ^ rhs._value;
    case OP_BIT_OR:  return lhs._value | rhs._value;
    case OP_BIT_SHL: return lhs._value << rhs._value;
    case OP_BIT_SHR: return lhs._value >> rhs._value;
    default: return 0;
    }
}

bool AsmMotoOperand::isSymbolLetter(char c, bool head) const {
    if (isalpha(c) || c == '_' || c == '.') return true;
    return !head && isdigit(c);
}

const char *AsmMotoOperand::parseConstant(const char *p, uint32_t &val) const {
    switch (*p) {
    case '~':
        p = parseConstant(p + 1, val);
        val = ~val;
        return p;
    case '+':
        p++;
        break;
    case '-':
        p = parseConstant(p + 1, val);
        val = -static_cast<int32_t>(val);
        return p;
    }
    if (isdigit(*p)) {
        p = AsmOperand::parseNumber(p, val, 10);
    } else if (*p == '$') {
        p = AsmOperand::parseNumber(p + 1, val, 16);
    } else if (*p == '@') {
        p = AsmOperand::parseNumber(p + 1, val, 8);
    } else if (*p == '%') {
        p = AsmOperand::parseNumber(p + 1, val, 2);
    } else {
        p = nullptr;
    }
    return p;
}

bool AsmIntelOperand::isSymbolLetter(char c, bool head) const {
    if (isalpha(c) || c == '_' || c == '.') return true;
    return !head && isdigit(c);
}

const char *AsmIntelOperand::parseConstant(
    const char *scan, uint32_t &val) const {
    const char *p;
    switch (*scan) {
    case '~':
        p = parseConstant(scan + 1, val);
        val = ~val;
        return p;
    case '+':
        scan++;
        break;
    case '-':
        p = parseConstant(scan + 1, val);
        val = -static_cast<int32_t>(val);
        return p;
    }
    if (!isdigit(*scan))
        return nullptr;
    if ((p = AsmOperand::parseNumber(scan, val, 16))
        && toupper(*p) == 'H') {
        p++;
    } else if ((p = AsmOperand::parseNumber(scan, val, 10))
               && !(*p && strchr("OoBb", *p))) {
        ;
    } else if ((p = AsmOperand::parseNumber(scan, val, 8))
               && toupper(*p) == 'O') {
        p++;
    } else if ((p = AsmOperand::parseNumber(scan, val, 2))
               && toupper(*p) == 'B') {
        p++;
    } else {
        p = nullptr;
    }
    return p;
}
