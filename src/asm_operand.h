/* -*- mode: c++; -*- */
#ifndef __ASM_OPERAND_H__
#define __ASM_OPERAND_H__

#include "symbol_table.h"
#include "error_reporter.h"

class AsmOperand : public ErrorReporter {
public:
    const char *eval(const char *expr, uint32_t &val32, SymbolTable *symtab);
    const char *eval(const char *expr, uint16_t &val16, SymbolTable *symtab);
    const char *eval(const char *expr, uint8_t &val8, SymbolTable *symtab);
    virtual bool isSymbolLetter(char c, bool head = false) const;
    const char *readSymbol(
        const char *scan, char *buffer, char *const end) const;
    const char *readChar(const char *scan, char &c);

protected:
    class Value {
    public:
        static Value makeSigned(int32_t value) {
            return Value(value, SIGNED);
        }
        static Value makeUnsigned(uint32_t value) {
            return Value(value, UNSIGNED);
        }
        Value() : _value(0), _type(UNDEF) {}
        void setSigned(int32_t value) {
            _value = value;
            _type = SIGNED;
        }
        void setUnsigned(uint32_t value) {
            _value = value;
            _type = UNSIGNED;
        }
        bool isUndefined() const { return _type == UNDEF; }
        bool isSigned() const { return _type == SIGNED; }
        bool isUnsigned() const { return _type == UNSIGNED; }
        int32_t getSigned() const { return static_cast<int32_t>(_value); }
        uint32_t getUnsigned() const { return _value; }
    private:
        uint32_t _value;
        enum ValueType : uint8_t {
            UNDEF,
            SIGNED,
            UNSIGNED,
        } _type;
        Value(uint32_t value, ValueType type) : _value(value), _type(type) {}
    };

    const char *_next;

    virtual bool isCurrentAddressSymbol(char c) const;
    virtual Error readNumber(Value &val);
    Error parseNumber(
        const char *p, Value &val, const uint8_t base,
        const char suffix = 0);

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

    struct OprAndLval {
        OprAndLval() : _opr(OP_NONE, 0), _value() {}
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

    SymbolTable *_symtab;
    Stack<OprAndLval> _stack;

    void skipSpaces();
    Value eval(const char *expr, SymbolTable *symtab);
    Value parseExpr();
    Value readAtom();
    Value readCharacterConstant();
    Operator readOperator();
    Value evalExpr(const Op op, const Value lhs, const Value rhs);
};

class AsmMotoOperand : public AsmOperand {
protected:
    bool isCurrentAddressSymbol(char c) const override;
    Error readNumber(Value &val) override;
};

class AsmIntelOperand : public AsmOperand {
protected:
    bool isCurrentAddressSymbol(char c) const override;
    Error readNumber(Value &val) override;
private:
    Error scanNumberEnd(
        const char *scan, const uint8_t base, char suffix = 0);
};

#endif
