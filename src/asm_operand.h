/* -*- mode: c++; -*- */
#ifndef __ASM_OPERAND_H__
#define __ASM_OPERAND_H__

#include "symbol_table.h"
#include "type_traits.h"

class AsmOperand {
public:
    const char *eval(const char *expr, uint32_t &val32);
    const char *eval(const char *expr, uint16_t &val16);
    const char *eval(const char *expr, uint8_t &val8);

protected:
    AsmOperand(SymbolTable *symtab)
        : _symtab(symtab) {}

    virtual const char *parseConstant(const char *p, uint32_t &val) const = 0;
    virtual bool isSymbolLetter(char c, bool head = false) const = 0;

    static bool isValidDigit(const char c, const uint8_t radix);
    static uint8_t toNumber(const char c, const uint8_t radix);
    static const char *parseNumber(
        const char *p, uint32_t &val, const uint8_t radix);

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

    struct Value {
        Value() : _value(0), _valid(false) {}
        Value(uint32_t value) : _value(value), _valid(true) {}
        Value(const Value &o) : _value(o._value), _valid(o._valid) {}
        uint32_t _value;
        bool _valid;
    };

    struct OprAndLval {
        OprAndLval() : _opr(Operator(OP_NONE, 0)), _value() {}
        OprAndLval(const Operator &opr, Value value) : _opr(opr), _value(value) {}
        OprAndLval(const OprAndLval &o) : _opr(o._opr), _value(o._value) {}
        bool isEnd() const { return _opr._op == OP_NONE; }
        int precedence() const { return _opr._precedence; }
        Operator _opr;
        Value _value;
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

    const SymbolTable *_symtab;
    const char *_next;
    bool _error;
    Stack<OprAndLval> _stack;

    void skipSpaces();
    Value parseExpr();
    Value readAtom();
    Operator readOperator();
    void readSymbol(char *buffer, char *const end);
    Value evalExpr(const Op op, const Value lhs, const Value rhs);
};

class AsmMotoOperand : public AsmOperand {
public:
    AsmMotoOperand(SymbolTable *symtab)
        : AsmOperand(symtab) {}

protected:
    bool isSymbolLetter(char c, bool head) const override;
    const char *parseConstant(const char *p, uint32_t &val) const override;
};

class AsmIntelOperand : public AsmOperand {
public:
    AsmIntelOperand(SymbolTable *symtab)
        : AsmOperand(symtab) {}

protected:
    bool isSymbolLetter(char c, bool head) const override;
    const char *parseConstant(const char *scan, uint32_t &val) const override;
};

#endif
