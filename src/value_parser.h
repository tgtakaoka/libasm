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

#ifndef __VALUE_PARSER_H__
#define __VALUE_PARSER_H__

#include "error_reporter.h"
#include "symbol_table.h"

namespace libasm {

class Value {
public:
    static Value makeSigned(int32_t value) { return Value(value, SIGNED); }

    static Value makeUnsigned(uint32_t value) { return Value(value, UNSIGNED); }

    Value() : _value(0), _type(UNDEFINED) {}

    bool isUndefined() const { return _type == UNDEFINED; }
    bool isSigned() const { return _type == SIGNED; }
    bool isUnsigned() const { return _type == UNSIGNED; }
    bool overflowUint8() const;
    bool overflowUint16() const;

    void setSigned(int32_t value) {
        _value = value;
        _type = SIGNED;
    }

    void setUnsigned(uint32_t value) {
        _value = value;
        _type = UNSIGNED;
    }

    int32_t getSigned() const { return static_cast<int32_t>(_value); }

    uint32_t getUnsigned() const { return _value; }

    Value &complement() {
        _value = ~_value;
        return *this;
    }

    Value &negate() {
        if (!isUndefined())
            setSigned(-getSigned());
        return *this;
    }

private:
    enum ValueType : uint8_t {
        UNDEFINED = 0,
        SIGNED = 1,
        UNSIGNED = 2,
    };

    uint32_t _value;
    ValueType _type;

    Value(uint32_t value, ValueType type) : _value(value), _type(type) {}
};

class ValueParser : protected ErrorReporter {
public:
    ValueParser(char curSym = '.') : ErrorReporter(), _curSym(curSym) {}

    Error error() const { return getError(); }
    /*
     * Parse |scan| text and convert expression to |val|.
     * Undefined symbol reference in expression should be checked by
     * |val.isUndefined()|. Other error should be checked by |error()|.
     */
    const char *eval(const char *scan, Value &val, SymbolTable *symtab);
    /*
     * Parse |scan| text and convert character constant to |val|.
     * Error should be checked by |error()|.
     */
    const char *readChar(const char *scan, char &val);

    bool isSymbolLetter(char c, bool head = false) const;
    const char *scanSymbol(const char *scan) const;

protected:
    virtual Error readNumber(const char *scan, Value &val);
    Error parseNumber(const char *scan, Value &val, const uint8_t base,
            const char suffix = 0);
    Error scanNumberEnd(const char *scan, const uint8_t base, char suffix = 0);

private:
    const char _curSym;
    const char *_next;

    enum Op : uint8_t {
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
        Operator(Op op, uint8_t precedence)
            : _op(op), _precedence(precedence) {}
        enum Op _op;
        uint8_t _precedence;
    };

    struct OprAndLval {
        OprAndLval() : _opr(OP_NONE, 0), _value() {}
        OprAndLval(const Operator &opr, Value value)
            : _opr(opr), _value(value) {}
        OprAndLval(const OprAndLval &o) : _opr(o._opr), _value(o._value) {}
        bool isEnd() const { return _opr._op == OP_NONE; }
        int precedence() const { return _opr._precedence; }
        Operator _opr;
        Value _value;
    };

    template <typename E>
    class Stack {
    public:
        void clear() { _top = 0; }
        bool empty() const { return _top <= 0; }
        bool full() const { return _top >= capacity; }
        const E &top() const { return _values[_top - 1]; }
        void push(const E v) { _values[_top++] = v; }
        void pop() { _top--; }
        uint8_t _top;

    private:
        static constexpr uint8_t capacity = 8;
        E _values[capacity];
    };

    SymbolTable *_symtab;
    Stack<OprAndLval> _stack;

    Value parseExpr(const char *scan);
    Value readAtom(const char *scan);
    Value readCharacterConstant(const char *scan);
    Operator readOperator(const char *scan);
    Value evalExpr(const Op op, const Value lhs, const Value rhs);
};

class MotoValueParser : public ValueParser {
public:
    MotoValueParser() : ValueParser('*') {}

protected:
    Error readNumber(const char *scan, Value &val) override;
};

class IntelValueParser : public ValueParser {
public:
    IntelValueParser() : ValueParser('$') {}

protected:
    Error readNumber(const char *scan, Value &val) override;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
