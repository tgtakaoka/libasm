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
    virtual bool commentLine(const StrScanner &scan) const { return endOfLine(scan); }
    virtual bool endOfLine(const StrScanner &scan) const = 0;
};

/**
 * Location counter parser.
 */
struct LocationParser {
    virtual bool locationSymbol(StrScanner &scan) const = 0;
};

/**
 * Symbol Parser
 */
struct SymbolParser {
    virtual bool symbolLetter(char c, bool headOfSymbol = false) const = 0;
};

/**
 * Parse for letter constant.
 */
struct LetterParser {
    /**
     * Parse |scan| as a letter constant.
     * - Returns OK when |scan| is recognized as a valid letter, and
     *   updates |scan| at the end of a letter.
     * - Returns ILLEGAL_CONSTANT when |scan| seems a letter but not
     *   ended as expected. |scan| is updated at the error.
     * - Returns NOT_AN_EXPECTED when |scan| doesn't look like a
     *   letter. |scan| is unchanged.
     */
    virtual Error parseLetter(StrScanner &scan, char &letter) const = 0;

    /**
     * Read a letter constant from |scan| and return it.
     * When |scan| points text which doesn't make sense as a letter,
     * |error| is set as ILLEGAL_CONSTANT.
     */
    virtual char readLetter(StrScanner &scan, ErrorAt &error) const = 0;
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
 * Function call parser.
 */
class ValueParser;
struct FunCallParser {
    /**
     * Parse function call and evaluate the value.
     * Call function |name| with optional parameters |params| and get |val|.
     */
    virtual Error parseFunCall(const StrScanner &name, StrScanner &params, Value &val,
            ErrorAt &error, const ValueParser &parser, const SymbolTable *symtab) const = 0;
};

struct Operator {
    virtual Error eval(Value &val, const Value &rhs) const { return OK; }
    virtual Error eval(Value &val, const Value &lhs, const Value &rhs) const { return OK; }
    bool hasPrecedence(const Operator &o) const { return _precedence <= o._precedence; }

    static const struct Operator OP_NONE;

protected:
    Operator(uint8_t precedence) : _precedence(precedence) {}

private:
    // Operator precedence (smaller value means higher precedence).
    // The same order of C/C++ language.
    const uint8_t _precedence;
};

class ValueParser {
public:
    ValueParser(const NumberParser &number, const CommentParser &comment,
            const SymbolParser &symbol, const LetterParser &letter, const LocationParser &location)
        : _number(number),
          _comment(comment),
          _symbol(symbol),
          _letter(letter),
          _location(location),
          _origin(0),
          _funCall(nullptr) {}

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
     * Parse |scan| text and convert letter constant to |val|.
     * Error should be checked by |getError()|.
     */
    char readLetter(StrScanner &scan, ErrorAt &error) const;

    /**
     * Parse |scan| and read a symbol. Returns StrScanner::EMPTY when error.
     */
    StrScanner readSymbol(StrScanner &scan) const;
    bool symbolLetter(char c, bool head = false) const { return _symbol.symbolLetter(c, head); }

    void setCurrentOrigin(uint32_t origin) { _origin = origin; }
    bool commentLine(const StrScanner &scan) const { return _comment.commentLine(scan); }
    bool endOfLine(const StrScanner &scan) const { return _comment.endOfLine(scan); }
    FunCallParser *setFunCallParser(FunCallParser *parser = nullptr);

private:
    const NumberParser &_number;
    const CommentParser &_comment;
    const SymbolParser &_symbol;
    const LetterParser &_letter;
    const LocationParser &_location;
    uint32_t _origin;
    FunCallParser *_funCall;

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

    Value parseExpr(StrScanner &scan, ErrorAt &error, Stack<const Operator *> &ostack,
            Stack<Value> &vstack, const SymbolTable *symtab) const;
    Value readAtom(StrScanner &scan, ErrorAt &errpr, Stack<const Operator *> &ostack,
            Stack<Value> &vstack, const SymbolTable *symtab) const;
    const Operator *readUnary(StrScanner &scan, ErrorAt &error) const;
    const Operator *readBinary(StrScanner &scan, ErrorAt &error) const;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
