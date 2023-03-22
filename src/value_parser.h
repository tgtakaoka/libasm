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

/**
 * Comment parser
 */
struct CommentParser {
    virtual bool commentLine(const StrScanner &scan) const = 0;
    virtual bool endOfLine(const StrScanner &scan) const = 0;
};

struct DefaultCommentParser : CommentParser {
    bool commentLine(const StrScanner &scan) const override;
    bool endOfLine(const StrScanner &scan) const override;
};

struct MotorolaCommentParser : DefaultCommentParser {
    MotorolaCommentParser() : DefaultCommentParser() {}
    bool commentLine(const StrScanner &scan) const override;
};

/**
 * Parse text |scan| as a number.
 *
 * - Returns OK when |scan| is recognized as a valid number, and
 *   updates |scan| at the end of a number.
 * - Returns ILLEGAL_CONSTANT when |scan| seems a number but not
 *   ended as expected. |scan| is updated at the error.
 * - Returns NOT_AN_EXPECTED when |scan| doesn't look like a
 *   number. |scan| is unchanged.
 */
struct NumberParser {
    virtual Error parseNumber(StrScanner &scan, Value &val) const = 0;
};

/**
`* C-style number are,
 * - Decimal:     "[1-9][0-9]*"
 * - Hexadecimal: "0[xX][0-9A-Fa-f]+"
 * - Octal:       "0[0-7]*"
 * - Binary:      "0[bB][01]+"
 */
struct CStyleNumberParser : NumberParser {
    Error parseNumber(StrScanner &scan, Value &val) const override;
};

/**
 * Motorola style number are,
 * - Decimal:     "{&}[0-9]+"
 * - Hexadecimal: "$[0-9A-Fa-f]+"
 * - Octal:       "@[0-7]+"
 * - Binary:      "%[01]+"
 */
struct MotorolaNumberParser : CStyleNumberParser {
    Error parseNumber(StrScanner &scan, Value &val) const override;
};

/**
 * Intel style number are,
 * - Decimal:     "[0-9]+"
 * - Hexadecimal: "([0-9]|0[A-Fa-f])[0-9A-Fa-f]*[hH]"
 * - Octal:       "[0-7]+[oOqQ]"
 * - Binary:      "[01]+[bB]"
 */
struct IntelNumberParser : CStyleNumberParser {
    Error parseNumber(StrScanner &scan, Value &val) const override;

private:
    /**
     * Search |scan| as a |radix| based number with optional |suffix|.
     * - Returns OK when |scan| looks a valid number, and |scan| is
     *   updated to the end of a number including |suffix|.
     * - Returns NOT_AN_EXPECTED when |scan| doesn't look like a
     *   number, or lacks |suffix|.
     */
    static Error scanNumberEnd(StrScanner &scan, Radix radix, char suffix = 0);
};

/**
 * National style number are,
 * - Decimal:     "[0-9]+|[dD]'[0-9]+'?"
 * - Hexadecimal: "[hHxX]'[0-9A-Fa-f]+'?"
 * - Octal:       "[oOqQ]'[0-7]+'?"
 * - Binary:      "[bB]'[01]+'?"
 */
struct NationalNumberParser : CStyleNumberParser {
    Error parseNumber(StrScanner &scan, Value &val) const override;
};

/**
 * Fairchild style number are National or Motorola style number.
 */
struct FairchildNumberParser : CStyleNumberParser {
    FairchildNumberParser() : CStyleNumberParser(), _national(), _motorola() {}
    Error parseNumber(StrScanner &scan, Value &val) const override;

private:
    const NationalNumberParser _national;
    const MotorolaNumberParser _motorola;
};

class ValueParser {
public:
    ValueParser(char locSym = '.')
        : _numberParser(_cStyleNumber),
          _commentParser(_defaultComment),
          _locSym(locSym),
          _origin(0),
          _funcParser(nullptr) {}

    /**
     * Parse |scan| text and return expression |value|.  Undefined
     * symbol reference in expression should be checked by
     * |value.isUndefined()|. Other error should be checked by
     * |getError()|.
     */
    Value eval(StrScanner &scan, ErrorAt &error, const SymbolTable *symtab) const;

    /**
     * Scan |scan| text, and find |delim| letter.  Return
     * StrScanner::EMPTY if not found.
     */
    StrScanner scanExpr(const StrScanner &scan, ErrorAt &error, char delim) const;

    /**
     * Parse |scan| text and convert character constant to |val|.
     * Error should be checked by |getError()|.
     */
    char readChar(StrScanner &scan, ErrorAt &error) const;

    void setCurrentOrigin(uint32_t origin) { _origin = origin; }
    virtual bool symbolLetter(char c, bool head = false) const;
    StrScanner readSymbol(StrScanner &scan) const;
    bool commentLine(const StrScanner &scan) const { return _commentParser.commentLine(scan); }
    bool endOfLine(const StrScanner &scan) const { return _commentParser.endOfLine(scan); }

    struct FuncParser {
        virtual Error parseFunc(const ValueParser &parser, const StrScanner &name, StrScanner &scan,
                Value &val, ErrorAt &error, const SymbolTable *symtab) const = 0;
    };
    FuncParser *setFuncParser(FuncParser *parser = nullptr);

protected:
    ValueParser(const NumberParser &numberParser, const CommentParser &commentParser, char locSym)
        : _numberParser(numberParser),
          _commentParser(commentParser),
          _locSym(locSym),
          _origin(0),
          _funcParser(nullptr) {}

    const DefaultCommentParser _defaultComment;

    virtual bool locationSymbol(StrScanner &scan) const;
    virtual bool charPrefix(StrScanner &scan, char &prefix) const;
    virtual bool charSuffix(StrScanner &scan, char prefix) const;

private:
    const NumberParser &_numberParser;
    const CommentParser &_commentParser;
    const char _locSym;
    uint32_t _origin;
    FuncParser *_funcParser;

    const CStyleNumberParser _cStyleNumber;

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

    Value parseExpr(StrScanner &scan, ErrorAt &error, Stack<OprAndLval> &stack,
            const SymbolTable *symtab) const;
    Value readAtom(StrScanner &scan, ErrorAt &errpr, Stack<OprAndLval> &stack,
            const SymbolTable *symtab) const;
    Value readCharacterConstant(StrScanner &scan, ErrorAt &error) const;
    Operator readOperator(StrScanner &scan, ErrorAt &error) const;
    Value evalExpr(const Op op, const Value lhs, const Value rhs, ErrorAt &error) const;
};

class MotorolaValueParser : public ValueParser {
public:
    MotorolaValueParser(bool closingQuote = false)
        : ValueParser(_motorolaNumber, _motorolaComment, '*'), _closingQuote(closingQuote) {}

protected:
    bool charSuffix(StrScanner &scan, char prefix) const override;

private:
    const bool _closingQuote;
    const MotorolaNumberParser _motorolaNumber;
    const MotorolaCommentParser _motorolaComment;
};

class IntelValueParser : public ValueParser {
public:
    IntelValueParser(char locSym = '$') : ValueParser(_intelNumber, _defaultComment, locSym) {}

private:
    const IntelNumberParser _intelNumber;
};

class NationalValueParser : public ValueParser {
public:
    NationalValueParser(char locSym = '.')
        : ValueParser(_nationalNumber, _defaultComment, locSym) {}

protected:
    NationalValueParser(
            const NumberParser &numberParser, const CommentParser &commentParser, char locSym = '.')
        : ValueParser(numberParser, commentParser, locSym) {}

    bool symbolLetter(char c, bool head = false) const override;

private:
    const NationalNumberParser _nationalNumber;
};

class FairchildValueParser : public NationalValueParser {
public:
    FairchildValueParser() : NationalValueParser(_fairchildNumber, _motorolaComment, '*') {}

protected:
    bool locationSymbol(StrScanner &scan) const override;
    bool symbolLetter(char c, bool head = false) const override;
    bool charPrefix(StrScanner &scan, char &prefix) const override;
    bool charSuffix(StrScanner &scan, char prefix) const override;

private:
    const FairchildNumberParser _fairchildNumber;
    const MotorolaCommentParser _motorolaComment;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
