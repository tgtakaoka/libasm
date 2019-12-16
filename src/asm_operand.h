/* -*- mode: c++; -*- */
#ifndef __ASM_OPERAND_H__
#define __ASM_OPERAND_H__

#include <ctype.h>
#include <string.h>
#include "type_traits.h"

template<typename U>
class AsmOperand {
public:
    const char *eval(const char *expr, U &val) {
        _next = expr;
        _error = false;
        _stack.clear();
        Value<U> v = parseExpr();
        // TODO: Check validity.
        val = v._value;
        return _error ? nullptr : _next;
    }

protected:
    virtual const char *parseConstant(const char *p, U &val) = 0;

    static bool isValidDigit(const char c, const uint8_t radix) {
        if (radix == 16) return isxdigit(c);
        return c >= '0' && c < '0' + radix;
    }

    static uint8_t toNumber(const char c, const uint8_t radix) {
        if (radix == 16 && !isdigit(c))
            return tolower(c) - 'a' + 10;
        return c - '0';
    }

    static const char *parseNumber(
        const char *p, U &val, const uint8_t radix) {
        if (!isValidDigit(*p, radix))
            return nullptr;
        U v = 0;
        while (isValidDigit(*p, radix)) {
            v *= radix;
            v += toNumber(*p, radix);
            p++;
        }
        val = v;
        return p;
    }

private:
    enum Op : char {
        OP_NONE,
        OP_ADD,
        OP_SUB,
        OP_MUL,
        OP_DIV,
        OP_MOD,
        OP_BIT_AND,
        OP_BIT_XOR,
        OP_BIT_OR,
        OP_BIT_SHL,
        OP_BIT_SHR,
    };

    struct Operator {
        Operator(Op op, int precedence)
            : _op(op),
              _precedence(precedence)
        {}
        enum Op _op;
        uint8_t _precedence;
    };

    template<typename T>
    struct Value {
        Value() : _value(0), _valid(false) {}
        Value(T value) : _value(value), _valid(true) {}
        Value(const Value<T> &o) : _value(o._value), _valid(o._valid) {}
        T _value;
        bool _valid;
    };

    template<typename T>
    struct OprAndLval {
        OprAndLval() : _opr(Operator(OP_NONE, 0)), _value() {}
        OprAndLval(const Operator &opr, Value<T> value) : _opr(opr), _value(value) {}
        OprAndLval(const OprAndLval<T> &o) : _opr(o._opr), _value(o._value) {}
        bool isEnd() const { return _opr._op == OP_NONE; }
        int precedence() const { return _opr._precedence; }
        Operator _opr;
        Value<T> _value;
    };

    template<typename E>
    class Stack {
    public:
        void clear() { _top = 0; }
        bool empty() const { return _top <= 0; }
        const E &top() const { return _values[_top - 1]; }
        void push(const E v) { _values[_top++] = v; }
        void pop() { _top--; }
    private:
        int _top;
        E _values[4];           // TODO: check stack overflow
    };

    const char *_next;
    bool _error;
    Stack<OprAndLval<U>> _stack;

    void skipSpaces() {
        while (isspace(*_next))
            _next++;
    }

    Value<U> parseExpr() {
        _stack.push(OprAndLval<U>());
        Value<U> value = readAtom();
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
            _stack.push(OprAndLval<U>(opr, value));
            value = readAtom();
        }
        return Value<U>();
    }

    Value<U> readAtom() {
        skipSpaces();
        if (*_next == '(') {
            _next++;
            Value<U> value(parseExpr());
            skipSpaces();
            if (*_next != ')') {
                // Missing closing ')'.
                _error = true;
                return Value<U>();
            }
            _next++;
            return value;
        }
        U val;
        const char *p = parseConstant(_next, val);
        if (p == nullptr) {
            // Unknown constant.
            _error = true;
            return 0;
        }
        _next = p;
        return Value<U>(val);
    }

    Operator readOperator() {
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

    Value<U> evalExpr(Op op, Value<U> lhs, Value<U> rhs) {
        if (!lhs._valid || !rhs._valid)
            return Value<U>();
        switch (op) {
        case OP_ADD: return lhs._value + rhs._value;
        case OP_SUB: return lhs._value - rhs._value;
        case OP_MUL: return lhs._value * rhs._value;
        case OP_DIV:
            if (rhs._value == 0) return Value<U>();
            return lhs._value / rhs._value;
        case OP_MOD:
            if (rhs._value == 0) return Value<U>();
            return lhs._value % rhs._value;
        case OP_BIT_AND: return lhs._value & rhs._value;
        case OP_BIT_XOR: return lhs._value ^ rhs._value;
        case OP_BIT_OR:  return lhs._value | rhs._value;
        case OP_BIT_SHL: return lhs._value << rhs._value;
        case OP_BIT_SHR: return lhs._value >> rhs._value;
        default: return 0;
        }
    }
};

template<typename U>
class AsmMotoOperand : public AsmOperand<U> {
protected:
    const char *parseConstant(const char *p, U &val) override {
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
            val = -static_cast<typename make_signed<U>::type>(val);
            return p;
        }
        if (isdigit(*p)) {
            p = AsmOperand<U>::parseNumber(p, val, 10);
        } else if (*p == '$') {
            p = AsmOperand<U>::parseNumber(p + 1, val, 16);
        } else if (*p == '@') {
            p = AsmOperand<U>::parseNumber(p + 1, val, 8);
        } else if (*p == '%') {
            p = AsmOperand<U>::parseNumber(p + 1, val, 2);
        } else {
            p = nullptr;
        }
        return p;
    }
};

template<typename U>
class AsmIntelOperand : public AsmOperand<U> {
protected:
    const char *parseConstant(const char *scan, U &val) override {
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
            val = -static_cast<typename make_signed<U>::type>(val);
            return p;
        }
        if (!isdigit(*scan))
            return nullptr;
        if ((p = AsmOperand<U>::parseNumber(scan, val, 16))
            && tolower(*p) == 'h') {
            p++;
        } else if ((p = AsmOperand<U>::parseNumber(scan, val, 10))
                   && !(*p && strchr("OoBb", *p))) {
            ;
        } else if ((p = AsmOperand<U>::parseNumber(scan, val, 8))
                   && tolower(*p) == 'o') {
            p++;
        } else if ((p = AsmOperand<U>::parseNumber(scan, val, 2))
                   && tolower(*p) == 'b') {
            p++;
        } else {
            p = nullptr;
        }
        return p;
    }
};

#endif
