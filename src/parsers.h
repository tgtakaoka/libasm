/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef __PARSERS_H__
#define __PARSERS_H__

#include "error_reporter.h"
#include "operators.h"
#include "str_scanner.h"
#include "symbol_table.h"
#include "value.h"

namespace libasm {

class ValueParser;

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
`* C-style numbers are
 * - Decimal:     "[1-9][0-9]*"
 * - Hexadecimal: "0[xX][0-9A-Fa-f]+"
 * - Octal:       "0[0-7]*"
 * - Binary:      "0[bB][01]+"
 */
struct CStyleNumberParser : NumberParser {
    Error parseNumber(StrScanner &scan, Value &val) const override;
};

/**
 * Motorola style numbers are
 * - Decimal:     "{&}[0-9]+"
 * - Hexadecimal: "$[0-9A-Fa-f]+"
 * - Octal:       "@[0-7]+"
 * - Binary:      "%[01]+"
 */
struct MotorolaNumberParser : CStyleNumberParser {
    Error parseNumber(StrScanner &scan, Value &val) const override;
};

/**
 * Intel style numbers are
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
 * Zilog style numbers are
 * - Decimal:     "[0-9]+"
 * - Hexadecimal: "%[0-9A-Fa-f]+"
 * - Octal:       "%(8)[0-7]+"
 * - Binary:      "%(2)[01]+"
 */
struct ZilogNumberParser : IntelNumberParser {
    Error parseNumber(StrScanner &scan, Value &val) const override;
};

/**
 * IBM style numbers are
 * - Decimal:     "[0-9]+|[dD]'[0-9]+'"
 * - Hexadecimal: "[hH]'[0-9A-Fa-f]+'"
 * - Octal:       "[oO]'[0-7]+'"
 * - Binary:      "[bB]'[01]+'"
 */
struct IbmNumberParser : CStyleNumberParser {
    IbmNumberParser(char hex, bool closingQuote = true)
        : _hex(hex), _bin(0), _oct(0), _dec(0), _closingQuote(closingQuote) {}
    IbmNumberParser(char hex, char bin, bool closingQuote = true)
        : _hex(hex), _bin(bin), _oct(0), _dec('D'), _closingQuote(closingQuote) {}
    IbmNumberParser(char hex, char bin, char oct, bool closingQuote = true)
        : _hex(hex), _bin(bin), _oct(oct), _dec('D'), _closingQuote(closingQuote) {}
    Error parseNumber(StrScanner &scan, Value &val) const override;

private:
    const char _hex;
    const char _bin;
    const char _oct;
    const char _dec;
    const bool _closingQuote;
};

/**
 * National style numbers are
 * - Decimal:     "[0-9]+|[dD]'[0-9]+'?"
 * - Hexadecimal: "[hH]'[0-9A-Fa-f]+'?"
 * - Octal:       "[oO]'[0-7]+'?"
 * - Binary:      "[bB]'[01]+'?"
 */
struct NationalNumberParser : IbmNumberParser {
    NationalNumberParser(char hex) : IbmNumberParser(hex, false) {}
    NationalNumberParser(char hex, char bin) : NationalNumberParser(hex, bin, false) {}
    NationalNumberParser(char hex, char bin, char oct) : IbmNumberParser(hex, bin, oct, false) {}
};

/**
 * Fairchild style numbers are the same as IBM plus '$hh' for hexadecimal.
 */
struct FairchildNumberParser : IbmNumberParser {
    FairchildNumberParser() : IbmNumberParser('H', 'B', 'O') {}
    Error parseNumber(StrScanner &scan, Value &val) const override;
};

/**
 * RCA style numbers are the same as IBM plus '#hh' for hexadecimal.
 */
struct RcaNumberParser : IbmNumberParser {
    RcaNumberParser() : IbmNumberParser('X', 'B') {}
    Error parseNumber(StrScanner &scan, Value &val) const override;

private:
    const IntelNumberParser _intel;
};

/**
 * Signetics style numbers are the same as IBM plus '#hh' for hexadecimal.
 */
struct SigneticsNumberParser : IbmNumberParser {
    SigneticsNumberParser() : IbmNumberParser('H', 'B', 'O') {}
    Error parseNumber(StrScanner &scan, Value &val) const override;

private:
    const IntelNumberParser _intel;
};

/**
 * Texas Instrument style numbers are the same as C-Style and '>hh' as
 * hexadecimal.
 */
struct TexasNumberParser : CStyleNumberParser {
    Error parseNumber(StrScanner &scan, Value &val) const override;

private:
    IntelNumberParser _intel;
};

struct SemicolonCommentParser : CommentParser {
    bool endOfLine(const StrScanner &scan) const { return *scan == ';' || *scan == 0; }
};

struct SharpCommentParser : SemicolonCommentParser {
    bool endOfLine(const StrScanner &scan) const {
        return *scan == '#' || SemicolonCommentParser::endOfLine(scan);
    }
};

struct AsteriskCommentParser : SemicolonCommentParser {
    bool commentLine(const StrScanner &scan) const { return *scan == '*' || endOfLine(scan); }
};

/**
 * Default symbol is '[:alpha:][:alnum:]*'.
 */
struct DefaultSymbolParser : SymbolParser {
    bool symbolLetter(char c, bool headOfSymbol = false) const override;
};

struct CStyleLetterParser : LetterParser {
    /** C-style letter is enclosed by sigle quotes */
    Error parseLetter(StrScanner &scan, char &letter) const override;
    /** C-style letter is: [:print:], \['"?\btnt], \x[0-9A-Fa-f]+, \[0-7]+ */
    char readLetter(StrScanner &scan, ErrorAt &error) const override;
};

struct DefaultLetterParser : LetterParser {
    /** Default style letter is enclosed by single quotes */
    Error parseLetter(StrScanner &scan, char &letter) const override;
    /** Default style letter is: [:print:], '' */
    char readLetter(StrScanner &scan, ErrorAt &error) const override;
};

struct MotorolaLetterParser : DefaultLetterParser {
    MotorolaLetterParser(bool closingQuote = false) : _closingQuote(closingQuote) {}

    /**
     * Motorola style letter is follwed after a single quote and
     * optionally closed with another single quote
     */
    Error parseLetter(StrScanner &scan, char &letter) const override;
    /** Motorola style letter is: [:print] */
    char readLetter(StrScanner &scan, ErrorAt &error) const override;

private:
    const bool _closingQuote;
    Error hasSuffix(StrScanner &scan) const;
};

struct ZilogLetterParser : DefaultLetterParser {
    /**
     * Zilog style letter is follwed after a single quote and
     * hexadecimal escape sequence with '%hh'. Also a single quote is
     * expressed as %Q.
     */
    char readLetter(StrScanner &scan, ErrorAt &error) const override;
};

struct IbmLetterParser : DefaultLetterParser {
    IbmLetterParser(char prefix) : _prefix(prefix) {}

    /** IBM style letter constant is C'c'. */
    Error parseLetter(StrScanner &scan, char &letter) const override;

private:
    const char _prefix;
};

struct FairchildLetterParser : DefaultLetterParser {
    FairchildLetterParser() : DefaultLetterParser(), _motorolaLetter(false) {}

    /** Fairchild style letter is: [cC]'[:print:]', #[:print:], '[:print:]'? */
    Error parseLetter(StrScanner &scan, char &letter) const override;

private:
    const MotorolaLetterParser _motorolaLetter;

    static Error hasPrefix(StrScanner &scan, char &prefix);
    static Error hasSuffix(StrScanner &scan, char prefix);
};

struct AsteriskLocationParser : LocationParser {
    bool locationSymbol(StrScanner &scan) const override { return scan.expect('*'); }
};

struct DollarLocationParser : LocationParser {
    bool locationSymbol(StrScanner &scan) const override { return scan.expect('$'); }
};

struct NationalLocationParser : LocationParser {
    NationalLocationParser(char extra = 0) : _extra(extra) {}
    bool locationSymbol(StrScanner &scan) const override {
        return scan.expect('.') || (_extra && scan.expect(_extra));
    }

private:
    const char _extra;
};

struct FairchildLocationParser : LocationParser {
    bool locationSymbol(StrScanner &scan) const override {
        return scan.expect('*') || scan.expect('$');
    }
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
