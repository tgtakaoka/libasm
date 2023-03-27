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

#include "parsers.h"

#include <stdint.h>

namespace libasm {

class ValueParser {
public:
    ValueParser(const NumberParser &number, const CommentParser &comment,
            const SymbolParser &symbol, const LetterParser &letter, const LocationParser &location)
        : _number(number),
          _comment(comment),
          _symbol(symbol),
          _letter(letter),
          _location(location),
          _operator(_cstyleOperators),
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
    const OperatorParser &_operator;
    const CStyleOperatorParser _cstyleOperators;
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
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
