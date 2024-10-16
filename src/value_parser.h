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

#ifndef __LIBASM_VALUE_PARSER_H__
#define __LIBASM_VALUE_PARSER_H__

#include <stdint.h>
#include "comment_parser.h"
#include "function_table.h"
#include "letter_parser.h"
#include "number_parser.h"
#include "symbol_parser.h"
#include "symbol_table.h"

namespace libasm {

struct ParserContext final {
    ParserContext(uint32_t loc = 0, const SymbolTable *symtab = nullptr, char delim = 0,
            Radix radix = RADIX_10)
        : currentLocation(loc), symbolTable(symtab), delimitor(delim), defaultRadix(radix) {}
    ParserContext(const ParserContext &) = default;

    uint32_t currentLocation;
    const SymbolTable *symbolTable;
    char delimitor;
    Radix defaultRadix;
};

struct ValueParser {
    struct Plugins {
        virtual const NumberParser &number() const;
        virtual const CommentParser &comment() const;
        virtual const SymbolParser &symbol() const;
        virtual const LetterParser &letter() const;
        virtual const OperatorParser &operators() const;
        virtual const FunctionTable &function() const;

        static const Plugins &defaultPlugins();
        static const Plugins &intel();
        static const Plugins &texas();
        static const Plugins &motorola();
    };

    struct IntelPlugins : Plugins {
        const NumberParser &number() const override;
        const SymbolParser &symbol() const override;
        const OperatorParser &operators() const override;
    };

    struct MotorolaPlugins : Plugins {
        const NumberParser &number() const override;
        const CommentParser &comment() const override;
        const SymbolParser &symbol() const override;
        const LetterParser &letter() const override;
        const OperatorParser &operators() const override;
    };

    struct TexasPlugins : Plugins {
        const NumberParser &number() const override;
        const CommentParser &comment() const override;
        const OperatorParser &operators() const override;
    };

    struct ZilogPlugins : Plugins {
        const NumberParser &number() const override;
        const LetterParser &letter() const override;
        const OperatorParser &operators() const override;
    };

    ValueParser(const Plugins &plugins = Plugins::defaultPlugins())
        : _number(plugins.number()),
          _comment(plugins.comment()),
          _symbol(plugins.symbol()),
          _letter(plugins.letter()),
          _operators(plugins.operators()),
          _function(plugins.function()) {}

    /**
     * Parse |scan| text using |context| and return expression valu|.  Undefined
     * symbol reference in expression will be checked by UNDEFINED_SYMBOL in
     * |error|.
     */
    Value eval(StrScanner &scan, ErrorAt &error, ParserContext &context) const;

    /**
     * Parse |scan| text and return letter constant.
     */
    char readLetter(StrScanner &scan, ErrorAt &error, char delim) const {
        return _letter.readLetter(scan, error, delim);
    }

    /**
     * Parse |scan| and read a |symbol|, then return OK. Otherwise return NOT_AN_EXPECTED.
     */
    Error readSymbol(StrScanner &scan, StrScanner &symbol) const;
    Error readLabel(StrScanner &scan, StrScanner &label) const;
    Error readInstruction(StrScanner &scan, StrScanner &inst) const;
    bool commentLine(StrScanner &scan) const { return _comment.commentLine(scan); }
    bool endOfLine(StrScanner &scan) const { return _comment.endOfLine(scan); }
    bool stringPrefix(StrScanner &scan) const { return _letter.stringPrefix(scan); }
    char stringDelimiter(StrScanner &scan) const { return _letter.stringDelimiter(scan); }

private:
    const NumberParser &_number;
    const CommentParser &_comment;
    const SymbolParser &_symbol;
    const LetterParser &_letter;
    const OperatorParser &_operators;
    const FunctionTable &_function;

    Value _eval(StrScanner &scan, ErrorAt &error, ParserContext &context) const;
    Error parseConstant(StrScanner &scan, Value &val, ParserContext &context) const;
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
