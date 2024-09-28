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
    virtual bool symbolLetter(char c, bool headOfSymbol = false) const;

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
    bool locationSymbol(StrScanner &scan, char location) const;
};

/**
 * Simple symbol is '([:alpha:]|[extra])([:alnum:]|[extra])*'.
 */
struct SimpleSymbolParser : SymbolParser {
    SimpleSymbolParser(const /*PROGMEM*/ char *extra_P) : _extra_P(extra_P) {}

    bool symbolLetter(char c, bool headOfSymbol = false) const override;

private:
    const /*PROGMEMD*/ char *const _extra_P;
};

/**
 * Prefix symbol is '([:alpha:]|[prefix])([:alnum:]|[extra])*'.
 */
struct PrefixSymbolParser : SymbolParser {
    PrefixSymbolParser(const /*PROGMEM*/ char *prefix_P, const /*PROGMEM*/ char *extra_P = nullptr)
        : _prefix_P(prefix_P), _extra_P(extra_P) {}

    bool symbolLetter(char c, bool headOfSymbol = false) const override;

private:
    const /*PROGMEMD*/ char *const _prefix_P;
    const /*PROGMEMD*/ char *const _extra_P;
};

struct IntelSymbolParser final : PrefixSymbolParser, Singleton<IntelSymbolParser> {
    IntelSymbolParser() : PrefixSymbolParser(nullptr, text::common::PSTR_UNDER_QUESTION) {}
};

struct MotorolaSymbolParser final : PrefixSymbolParser, Singleton<MotorolaSymbolParser> {
    MotorolaSymbolParser()
        : PrefixSymbolParser(text::common::PSTR_DOT, text::common::PSTR_UNDER_DOT_DOLLAR) {}
    bool locationSymbol(StrScanner &scan) const override {
        return SymbolParser::locationSymbol(scan, '*');
    }
};

struct RcaSymbolParser final : SymbolParser, Singleton<RcaSymbolParser> {
    /** Pre-defined function name may have '.' */
    bool functionNameLetter(char c) const override;
    /** Symbol is definde by "=". */
    bool instructionLetter(char c) const override;
    bool instructionTerminator(char c) const override { return c == '='; }
};

struct FairchildSymbolParser final : SymbolParser, Singleton<FairchildSymbolParser> {
    /** Accepts location symbol '*', '$', '.' */
    bool locationSymbol(StrScanner &scan) const override;
};

struct PanasonicSymbolParser final : SymbolParser, Singleton<PanasonicSymbolParser> {
    bool locationSymbol(StrScanner &scan) const override {
        return SymbolParser::locationSymbol(scan, '*');
    }
};

struct MostekSymbolParser final : SimpleSymbolParser, Singleton<MostekSymbolParser> {
    MostekSymbolParser() : SimpleSymbolParser(text::common::PSTR_UNDER) {}
    bool locationSymbol(StrScanner &scan) const override {
        return SymbolParser::locationSymbol(scan, '*');
    }
    bool instructionLetter(char c) const override;
    bool instructionTerminator(char c) const override { return c == '='; }
};

struct Ins80xxSymbolParser final : PrefixSymbolParser, Singleton<Ins80xxSymbolParser> {
    Ins80xxSymbolParser() : PrefixSymbolParser(text::common::PSTR_DOLLAR) {}
    bool locationSymbol(StrScanner &scan) const override;
    bool instructionLetter(char c) const override;
    bool instructionTerminator(char c) const override { return c == '='; }
};

struct Mc68000SymbolParser final : PrefixSymbolParser, Singleton<Mc68000SymbolParser> {
    Mc68000SymbolParser()
        : PrefixSymbolParser(text::common::PSTR_DOT, text::common::PSTR_UNDER_DOT_DOLLAR) {}
    bool locationSymbol(StrScanner &scan) const override {
        return SymbolParser::locationSymbol(scan, '*');
    }
    bool instructionLetter(char c) const override;
};

struct Ns32000SymbolParser final : SimpleSymbolParser, Singleton<Ns32000SymbolParser> {
    Ns32000SymbolParser() : SimpleSymbolParser(text::common::PSTR_UNDER_DOT) {}
    bool locationSymbol(StrScanner &scan) const override;
    bool instructionLetter(char c) const override;
};

struct Pdp11SymbolParser final : PrefixSymbolParser, Singleton<Pdp11SymbolParser> {
    Pdp11SymbolParser() : PrefixSymbolParser(text::common::PSTR_UNDER_DOT_DOLLAR) {}
    bool locationSymbol(StrScanner &scan) const override;
    bool instructionLetter(char c) const override;
    bool instructionTerminator(char c) const override { return c == '='; }
};

struct Pdp8SymbolParser final : SymbolParser, Singleton<Pdp8SymbolParser> {
    bool locationSymbol(StrScanner &scan) const override;
    bool labelDelimitor(StrScanner &scan) const override { return scan.expect(','); }
    bool instructionLetter(char c) const override;
    bool instructionTerminator(char c) const override;
};

struct Tms32010SymbolParser final : SimpleSymbolParser, Singleton<Tms32010SymbolParser> {
    Tms32010SymbolParser() : SimpleSymbolParser(text::common::PSTR_UNDER_DOLLAR) {}
    bool instructionLetter(char c) const override;
};

struct Z8SymbolParser final : SimpleSymbolParser, Singleton<Z8SymbolParser> {
    Z8SymbolParser() : SimpleSymbolParser(text::common::PSTR_UNDER_DOT_DOLLAR_QUESTION) {}
};

struct Z8000SymbolParser final : PrefixSymbolParser, Singleton<Z8000SymbolParser> {
    Z8000SymbolParser() : PrefixSymbolParser(nullptr, text::common::PSTR_UNDER) {}
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
