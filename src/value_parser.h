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
#include "str_scanner.h"
#include "symbol_table.h"

namespace libasm {

class Value {
public:
    static bool overflowRel8(int16_t s16);
    static bool overflowRel8(int32_t s32);
    static bool overflowRel16(int32_t s32);
    static bool overflowUint8(uint16_t u16);
    static bool overflowUint8(uint32_t u32);
    static bool overflowUint16(uint32_t u32);
    static Value makeSigned(int32_t value) { return Value(value, SIGNED); }
    static Value makeUnsigned(uint32_t value) { return Value(value, UNSIGNED); }

    Value() : _value(0), _type(UNDEFINED) {}

    bool isUndefined() const { return _type == UNDEFINED; }
    bool isSigned() const { return _type == SIGNED; }
    bool isUnsigned() const { return _type == UNSIGNED; }
    bool overflowUint8() const { return overflowUint8(_value); }
    bool overflowUint16() const { return overflowUint16(_value); }

    Value &setSign(bool sign) {
        _type = sign ? SIGNED : UNSIGNED;
        return *this;
    }

    Value &setValue(uint32_t value) { return setSign(static_cast<int32_t>(_value = value) >= 0); }

    int32_t getSigned() const { return static_cast<int32_t>(_value); }

    uint32_t getUnsigned() const { return _value; }

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

class ValueParser : public ErrorReporter {
public:
    ValueParser(char curSym = '.')
        : ErrorReporter(), _origin(0), _curSym(curSym), _funcParser(nullptr) {}

    /*
     * Parse |scan| text and return expression |value|.  Undefined
     * symbol reference in expression should be checked by
     * |value.isUndefined()|. Other error should be checked by
     * |getError()|.
     */
    Value eval(StrScanner &scan, const SymbolTable *symtab);

    /*
     * Parse |scan| text and convert character constant to |val|.
     * Error should be checked by |getError()|.
     */
    char readChar(StrScanner &scan);

    void setCurrentOrigin(uint32_t origin) { _origin = origin; }
    static bool isSymbolLetter(char c, bool head = false);
    StrScanner readSymbol(StrScanner &scan) const;

    struct FuncParser : public ErrorReporter {
        virtual Error parseFunc(ValueParser &parser, const StrScanner &name, StrScanner &scan,
                Value &val, const SymbolTable *symtab) = 0;
    };
    void setFuncParser(FuncParser *parser = nullptr) { _funcParser = parser; }

protected:
    virtual Error readNumber(StrScanner &scan, Value &val);
    Error parseNumber(StrScanner &scan, Value &val, const uint8_t base, const char suffix = 0);
    Error scanNumberEnd(const StrScanner &scan, const uint8_t base, char suffix = 0);

private:
    uint32_t _origin;
    const char _curSym;
    FuncParser *_funcParser;

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
        Operator(Op op, uint8_t precedence) : _op(op), _precedence(precedence) {}
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

    template <typename E>
    struct Stack {
        Stack() : _size(0) {}
        bool empty() const { return _size == 0; }
        bool full() const { return _size >= capacity; }
        const E &top() const { return _values[_size - 1]; }
        void push(const E v) { _values[_size++] = v; }
        void pop() { _size--; }

    private:
        static constexpr uint8_t capacity = 8;
        uint8_t _size;
        E _values[capacity];
    };

    Value parseExpr(StrScanner &scan, Stack<OprAndLval> &stack, const SymbolTable *symtab);
    Error parseFunction(const uint16_t funid, StrScanner &scan, Value &val);
    Value readAtom(StrScanner &scan, Stack<OprAndLval> &stack, const SymbolTable *symtab);
    Value readCharacterConstant(StrScanner &scan);
    Operator readOperator(StrScanner &scan);
    Value evalExpr(const Op op, const Value lhs, const Value rhs);
};

class MotorolaValueParser : public ValueParser {
public:
    MotorolaValueParser() : ValueParser('*') {}

protected:
    Error readNumber(StrScanner &scan, Value &val) override;
};

class IntelValueParser : public ValueParser {
public:
    IntelValueParser() : ValueParser('$') {}

protected:
    Error readNumber(StrScanner &scan, Value &val) override;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
