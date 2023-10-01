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

namespace libasm {

class ValueParser {
public:
    struct Plugins {
        virtual const NumberParser &number() const { return CStyleNumberParser::singleton(); }
        virtual const CommentParser &comment() const { return SemicolonCommentParser::singleton(); }
        virtual const SymbolParser &symbol() const { return SymbolParser::singleton(); }
        virtual const LetterParser &letter() const { return DefaultLetterParser::singleton(); }
        virtual const LocationParser &location() const { return DollarLocationParser::singleton(); }
        virtual const OperatorParser &operators() const {
            return CStyleOperatorParser::singleton();
        }
        virtual const FunctionTable &function() const { return FunctionTable::singleton(); }
    };

    struct Locator {
        virtual uint32_t currentLocation() const = 0;
    };

    ValueParser(const Plugins &plugins, const Locator &locator)
        : _number(plugins.number()),
          _comment(plugins.comment()),
          _symbol(plugins.symbol()),
          _letter(plugins.letter()),
          _location(plugins.location()),
          _operators(plugins.operators()),
          _function(plugins.function()),
          _locator(locator) {}

    /**
     * Parse |scan| text and return expression |value|.  Undefined symbol reference in expression
     * will be checked by UNDEFINED_SYMBOL in |error|.
     */
    Value eval(StrScanner &scan, ErrorAt &error, const SymbolTable *symtab, char delim = 0) const;

    /**
     * Parse |scan| text and return letter constant.
     */
    char readLetter(StrScanner &scan, ErrorAt &error) const {
        return _letter.readLetter(scan, error);
    }
    /**
     * Parse |scan| text and return a letter in a string.
     */
    char readLetterInString(StrScanner &scan, ErrorAt &error) const {
        return _letter.readLetterInString(scan, error);
    }

    /**
     * Parse |scan| and read a |symbol|, then return OK. Otherwise return NOT_AN_EXPECTED.
     */
    Error readSymbol(StrScanner &scan, StrScanner &symbol) const;
    Error readLabel(StrScanner &scan, StrScanner &label) const;
    Error readInstruction(StrScanner &scan, StrScanner &inst) const;
    bool locationSymbol(StrScanner &scan) const { return _location.locationSymbol(scan); }
    bool commentLine(const StrScanner &scan) const { return _comment.commentLine(scan); }
    bool endOfLine(const StrScanner &scan) const { return _comment.endOfLine(scan); }
    char stringPrefix() const { return _letter.stringPrefix(); }
    char stringDelimiter() const { return _letter.stringDelimiter(); }

private:
    const NumberParser &_number;
    const CommentParser &_comment;
    const SymbolParser &_symbol;
    const LetterParser &_letter;
    const LocationParser &_location;
    const OperatorParser &_operators;
    const FunctionTable &_function;
    const Locator &_locator;

    Error parseConstant(StrScanner &scan, Value &val) const;
    Error readFunctionName(StrScanner &scan, StrScanner &name) const;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
