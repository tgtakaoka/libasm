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
#include "value.h"

#include <stdint.h>

namespace libasm {

enum Radix : uint8_t {
    RADIX_2 = 2,
    RADIX_8 = 8,
    RADIX_10 = 10,
    RADIX_16 = 16,
};

class NumberParser : public ErrorAt {
public:
    virtual bool numberPrefix(const StrScanner &scan) const;
    virtual Error readNumber(StrScanner &scan, Value &val);

    Error parseNumber(StrScanner &scan, Value &val, const Radix radix);

protected:
    Error scanNumberEnd(const StrScanner &scan, const Radix radix, char suffix = 0) const;
    Error expectNumberSuffix(StrScanner &scan, char suffux = 0) const;
};

class MotorolaNumberParser : public NumberParser {
public:
    bool numberPrefix(const StrScanner &scan) const override;
    Error readNumber(StrScanner &scan, Value &val) override;
};

class IntelNumberParser : public NumberParser {
public:
    bool numberPrefix(const StrScanner &scan) const override;
    Error readNumber(StrScanner &scan, Value &val) override;
};

class NationalNumberParser : public NumberParser {
public:
    bool numberPrefix(const StrScanner &scan) const override;
    Error readNumber(StrScanner &scan, Value &val) override;
};

class FairchildNumberParser : public NumberParser {
public:
    bool numberPrefix(const StrScanner &scan) const override;
    Error readNumber(StrScanner &scan, Value &val) override;

private:
    MotorolaNumberParser _motorola;
    NationalNumberParser _national;
};

class ValueParser : public ErrorAt {
public:
    ValueParser(char locSym = '.', NumberParser &numberParser = NUMBER_PARSER)
        : ErrorAt(),
          _numberParser(numberParser),
          _locSym(locSym),
          _origin(0),
          _funcParser(nullptr),
          _commentChar(0) {}

    /*
     * Parse |scan| text and return expression |value|.  Undefined
     * symbol reference in expression should be checked by
     * |value.isUndefined()|. Other error should be checked by
     * |getError()|.
     */
    Value eval(StrScanner &scan, const SymbolTable *symtab);

    /*
     * Scan |scan| text, and find |delim| letter.  Return
     * StrScanner::EMPTY if not found.
     */
    StrScanner scanExpr(const StrScanner &scan, char delim);

    /*
     * Parse |scan| text and convert character constant to |val|.
     * Error should be checked by |getError()|.
     */
    char readChar(StrScanner &scan);

    void setCurrentOrigin(uint32_t origin) { _origin = origin; }
    virtual bool symbolLetter(char c, bool head = false) const;
    StrScanner readSymbol(StrScanner &scan) const;
    void setCommentChar(char c) { _commentChar = c; }
    bool endOfLine(char c) const { return c == 0 || c == ';' || c == _commentChar; }

    struct FuncParser : public ErrorAt {
        virtual Error parseFunc(ValueParser &parser, const StrScanner &name, StrScanner &scan,
                Value &val, const SymbolTable *symtab) = 0;
    };
    FuncParser *setFuncParser(FuncParser *parser = nullptr);

protected:
    virtual bool locationSymbol(StrScanner &scan) const;
    virtual bool charPrefix(StrScanner &scan, char &prefix) const;
    virtual bool charSuffix(StrScanner &scan, char prefix) const;

private:
    NumberParser &_numberParser;
    const char _locSym;
    uint32_t _origin;
    FuncParser *_funcParser;
    char _commentChar;

    static NumberParser NUMBER_PARSER;

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
    MotorolaValueParser(bool closingQuote = false)
        : ValueParser('*', NUMBER_PARSER), _closingQuote(closingQuote) {}

protected:
    bool charSuffix(StrScanner &scan, char prefix) const override;

private:
    bool _closingQuote;

    static MotorolaNumberParser NUMBER_PARSER;
};

class IntelValueParser : public ValueParser {
public:
    IntelValueParser(char locSym = '$') : ValueParser(locSym, NUMBER_PARSER) {}

private:
    static IntelNumberParser NUMBER_PARSER;
};

class NationalValueParser : public ValueParser {
public:
    NationalValueParser(char locSym = '.', NumberParser &numberParser = NUMBER_PARSER)
        : ValueParser(locSym, numberParser) {}

protected:
    bool symbolLetter(char c, bool head = false) const override;

private:
    static NationalNumberParser NUMBER_PARSER;
};

class FairchildValueParser : public NationalValueParser {
public:
    FairchildValueParser() : NationalValueParser('*', NUMBER_PARSER) {}

protected:
    bool locationSymbol(StrScanner &scan) const override;
    bool symbolLetter(char c, bool head = false) const override;
    bool charPrefix(StrScanner &scan, char &prefix) const override;
    bool charSuffix(StrScanner &scan, char prefix) const override;

private:
    static FairchildNumberParser NUMBER_PARSER;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
