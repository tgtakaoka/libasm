/*
 * Copyright 2024 Tadashi G. Takaoka
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

#ifndef __LIBASM_SYMBOL_PARSER_H__
#define __LIBASM_SYMBOL_PARSER_H__

#include <ctype.h>
#include "str_scanner.h"
#include "text_common.h"

namespace libasm {

/**
 * Symbol Parser
 */
struct SymbolParser {
    /** Default symbol is '[:alpha:][:alnum:]*' */
    virtual bool symbolLetter(char c, bool headOfSymbol = false) const {
        return isalpha(c) || (!headOfSymbol && isdigit(c)) || c == '_';
    }

    /** Default location symbol is '$' */
    virtual bool locationSymbol(StrScanner &scan) const { return locationSymbol(scan, '$'); }

    /** Label may have delimitor letter */
    virtual bool labelDelimitor(StrScanner &scan) const { return scan.expect(':'); }

    /** Instruction and directive letter at end */
    virtual bool instructionLetter(char c) const { return symbolLetter(c); }

    /** Instruction terminates when this returns true */
    virtual bool instructionTerminator(char) const { return false; }

    /** Function name is same as symbol */
    virtual bool functionNameLetter(char c) const { return symbolLetter(c); }

protected:
    bool locationSymbol(StrScanner &scan, char location) const {
        auto p = scan;
        if (p.expect(location) && !symbolLetter(*p)) {
            scan = p;
            return true;
        }
        return false;
    }
};

/**
 * Simple symbol is '([:alpha:]|[extra])([:alnum:]|[extra])*'.
 */
struct SimpleSymbolParser : SymbolParser {
    SimpleSymbolParser(const /*PROGMEM*/ char *extra_P) : _extra_P(extra_P) {}
    bool symbolLetter(char c, bool headOfSymbol = false) const override {
        return SymbolParser::symbolLetter(c, headOfSymbol) || (c && strchr_P(_extra_P, c));
    }

private:
    const /*PROGMEMD*/ char *const _extra_P;
};

/**
 * Prefix symbol is '([:alpha:]|[prefix])([:alnum:]|[extra])*'.
 */
struct PrefixSymbolParser : SymbolParser {
    PrefixSymbolParser(const /*PROGMEM*/ char *prefix_P, const /*PROGMEM*/ char *extra_P = nullptr)
        : _prefix_P(prefix_P), _extra_P(extra_P) {}
    bool symbolLetter(char c, bool headOfSymbol = false) const override {
        if (SymbolParser::symbolLetter(c, headOfSymbol))
            return true;
        if (headOfSymbol && _prefix_P)
            return c && strchr_P(_prefix_P, c);
        if (_extra_P)
            return c && strchr_P(_extra_P, c);
        return false;
    }

private:
    const /*PROGMEMD*/ char *const _prefix_P;
    const /*PROGMEMD*/ char *const _extra_P;
};

struct UnderQuestionSymbolParser final : PrefixSymbolParser, Singleton<UnderQuestionSymbolParser> {
    UnderQuestionSymbolParser() : PrefixSymbolParser(nullptr, text::common::PSTR_UNDER_QUESTION) {}
};

struct Mc68xxSymbolParser final : PrefixSymbolParser, Singleton<Mc68xxSymbolParser> {
    Mc68xxSymbolParser()
        : PrefixSymbolParser(text::common::PSTR_DOT, text::common::PSTR_UNDER_DOT_DOLLAR) {}
    bool locationSymbol(StrScanner &scan) const override {
        return SymbolParser::locationSymbol(scan, '*');
    }
};

struct Ins80xxSymbolParser final : PrefixSymbolParser, Singleton<Ins80xxSymbolParser> {
    Ins80xxSymbolParser() : PrefixSymbolParser(text::common::PSTR_DOLLAR) {}
    bool locationSymbol(StrScanner &scan) const override {
        return SymbolParser::locationSymbol(scan, '.') || SymbolParser::locationSymbol(scan, '$');
    }
    bool instructionLetter(char c) const override {
        return PrefixSymbolParser::instructionLetter(c) || c == '=' || c == '.';
    }
    bool instructionTerminator(char c) const override { return c == '='; }
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
