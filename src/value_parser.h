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

#include <stdint.h>

#include "parsers.h"
#include "stack.h"

namespace libasm {

class ValueParser {
public:
    struct Locator {
        virtual uint32_t currentLocation() const = 0;
    };

    ValueParser(const NumberParser &number, const CommentParser &comment,
            const SymbolParser &symbol, const LetterParser &letter, const LocationParser &location,
            const Locator &locator, const OperatorParser *operators = nullptr,
            const FunctionParser *function = nullptr)
        : _number(number),
          _comment(comment),
          _symbol(symbol),
          _letter(letter),
          _location(location),
          _locator(locator),
          _operator(operators ? *operators : CSTYLE_OPERATORS),
          _function(function ? *function : DEFAULT_FUNCTION) {}

    /**
     * Parse |scan| text and return expression |value|.  Undefined
     * symbol reference in expression should be checked by
     * |value.isUndefined()|. Other error should be checked by
     * |getError()|.
     */
    Value eval(StrScanner &scan, ErrorAt &error, const SymbolTable *symtab, char delim = 0) const;

    /**
     * Parse |scan| text and convert letter constant to |val|.
     * Error should be checked by |getError()|.
     */
    char readLetter(StrScanner &scan, ErrorAt &error) const;

    /**
     * Parse |scan| and read a symbol. Returns StrScanner::EMPTY when error.
     */
    StrScanner readSymbol(StrScanner &scan) const { return _symbol.readSymbol(scan); }
    bool symbolLetter(char c, bool head = false) const { return _symbol.symbolLetter(c, head); }

    bool commentLine(const StrScanner &scan) const { return _comment.commentLine(scan); }
    bool endOfLine(const StrScanner &scan) const { return _comment.endOfLine(scan); }

private:
    const NumberParser &_number;
    const CommentParser &_comment;
    const SymbolParser &_symbol;
    const LetterParser &_letter;
    const LocationParser &_location;
    const Locator &_locator;
    const OperatorParser &_operator;
    const FunctionParser &_function;

    static const CStyleOperatorParser CSTYLE_OPERATORS;
    static const FunctionParser DEFAULT_FUNCTION;

    struct OperatorStack : Stack<Operator, 8> {
        OperatorStack() : Stack() {}
        Operator &top() { return _contents[_size - 1]; }
    };

    Error parseConstant(StrScanner &scan, Value &val) const;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
