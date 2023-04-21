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
    StrScanner readSymbol(StrScanner &scan) const;
    virtual bool symbolLetter(char c, bool headOfSymbol = false) const = 0;

    static const /*PROGMEM*/ char NONE[];                 // ""
    static const /*PROGMEM*/ char DOLLAR[];               // "$"
    static const /*PROGMEM*/ char DOT[];                  // "."
    static const /*PROGMEM*/ char QUESTION[];             // "?"
    static const /*PROGMEM*/ char ATMARK_QUESTION[];      // "@?";
    static const /*PROGMEM*/ char DOLLAR_DOT[];           // "$."
    static const /*PROGMEM*/ char DOLLAR_QUESTION[];      // "$?"
    static const /*PROGMEM*/ char DOLLAR_DOT_QUESTION[];  // "$.?"
};

/**
 * Parsing function call.
 */
struct FunctionParser : Singleton<FunctionParser> {
    /**
     * Parsing |scan| and returns a Functor pointer for a function. |scan|  should point an
     * opening parenthesis  for arguments  list.
     *
     * - Returns nullptr if no function call and |scan| should be unchanged.
     *
     * - Returns the no-operation varargs function which returns undefined value when |symtab| is
     *   nullptr and found '(' after function name.
     */
    virtual const Functor *parseFunction(StrScanner &scan, ErrorAt &error,
            const SymbolParser &symParser, const SymbolTable *symtab) const;

protected:
    virtual StrScanner readFunctionName(StrScanner &scan, const SymbolParser &symParser) const;
};

/**
 * Parse for letter constant.
 */
struct LetterParser {
    /**
     * Parse |scan| as a letter constant.  Default style letter is enclosed by single quotes.
     *
     * - Returns OK when |scan| is recognized as a valid letter, and updates |scan| at the end of a
     *   letter.
     * - Returns ILLEGAL_CONSTANT when |scan| seems a letter but not ended as expected. |scan| is
     *   updated at the error.
     * - Returns NOT_AN_EXPECTED when |scan| doesn't look like a letter. |scan| is unchanged.
     */
    virtual Error parseLetter(StrScanner &scan, char &letter) const;

    /**
     * Read a letter constant from |scan| and return it.  Default style letter is: [:print:], ''
     *
     * When |scan| points text which doesn't make sense as a letter, |error| is set as
     * ILLEGAL_CONSTANT.
     */
    virtual char readLetter(StrScanner &scan, ErrorAt &error) const;
};

/**
 * Parse text |scan| as a number.
 *
 * - Returns OK when |scan| is recognized as a valid number, and updates |scan| at the end of a
 *   number.
 * - Returns ILLEGAL_CONSTANT when |scan| seems a number but not ended as expected. |scan| is
 *   updated at the error.
 * - Returns NOT_AN_EXPECTED when |scan| doesn't look like a number. |scan| is unchanged.
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
struct CStyleNumberParser final : NumberParser, Singleton<CStyleNumberParser> {
    Error parseNumber(StrScanner &scan, Value &val) const override;
};

/**
 * Motorola style numbers are
 * - Decimal:     "{&}[0-9]+"
 * - Hexadecimal: "$[0-9A-Fa-f]+"
 * - Octal:       "@[0-7]+"
 * - Binary:      "%[01]+"
 */
struct MotorolaNumberParser final : NumberParser, Singleton<MotorolaNumberParser> {
    Error parseNumber(StrScanner &scan, Value &val) const override;
};

/**
 * Intel style numbers are
 * - Decimal:     "[0-9]+"
 * - Hexadecimal: "([0-9]|0[A-Fa-f])[0-9A-Fa-f]*[hH]"
 * - Octal:       "[0-7]+[oOqQ]"
 * - Binary:      "[01]+[bB]"
 */
struct IntelNumberParser final : NumberParser, Singleton<IntelNumberParser> {
    Error parseNumber(StrScanner &scan, Value &val) const override;

private:
    /**
     * Search |scan| as a |radix| based number with optional |suffix|.
     *
     * - Returns OK when |scan| looks a valid number, and |scan| is updated to the end of a number
     *   including |suffix|.
     * - Returns NOT_AN_EXPECTED when |scan| doesn't look like a number, or lacks |suffix|.
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
struct ZilogNumberParser final : NumberParser, Singleton<ZilogNumberParser> {
    Error parseNumber(StrScanner &scan, Value &val) const override;
};

/**
 * IBM style numbers are
 * - Decimal:     "[0-9]+|[dD]'[0-9]+'"
 * - Hexadecimal: "[hH]'[0-9A-Fa-f]+'"
 * - Octal:       "[oO]'[0-7]+'"
 * - Binary:      "[bB]'[01]+'"
 */
struct IbmNumberParser final : NumberParser, Singleton<IbmNumberParser> {
    IbmNumberParser(char hex = 'H', bool closingQuote = true)
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
struct NationalNumberParser final : NumberParser, Singleton<NationalNumberParser> {
    NationalNumberParser(char hex = 'X') : _ibm(hex, false) {}
    NationalNumberParser(char hex, char bin) : _ibm(hex, bin, false) {}
    NationalNumberParser(char hex, char bin, char oct) : _ibm(hex, bin, oct, false) {}
    Error parseNumber(StrScanner &scan, Value &val) const override {
        return _ibm.parseNumber(scan, val);
    }

private:
    const IbmNumberParser _ibm;
};

/**
 * Fairchild style numbers are the same as IBM plus '$hh' for hexadecimal.
 */
struct FairchildNumberParser final : NumberParser, Singleton<FairchildNumberParser> {
    FairchildNumberParser() : _ibm('H', 'B', 'O') {}
    Error parseNumber(StrScanner &scan, Value &val) const override;

private:
    const IbmNumberParser _ibm;
};

/**
 * RCA style numbers are the same as IBM plus '#hh' for hexadecimal.
 */
struct RcaNumberParser final : NumberParser, Singleton<RcaNumberParser> {
    RcaNumberParser() : _ibm('X', 'B'), _intel() {}
    Error parseNumber(StrScanner &scan, Value &val) const override;

private:
    const IbmNumberParser _ibm;
    const IntelNumberParser _intel;
};

/**
 * Signetics style numbers are the same as IBM plus '#hh' for hexadecimal.
 */
struct SigneticsNumberParser final : NumberParser, Singleton<SigneticsNumberParser> {
    SigneticsNumberParser() : _ibm('H', 'B', 'O'), _intel() {}
    Error parseNumber(StrScanner &scan, Value &val) const override;

private:
    const IbmNumberParser _ibm;
    const IntelNumberParser _intel;
};

/**
 * Texas Instrument style numbers are the same as C-Style and '>hh' as hexadecimal.
 */
struct TexasNumberParser final : NumberParser, Singleton<TexasNumberParser> {
    TexasNumberParser() : _intel() {}
    Error parseNumber(StrScanner &scan, Value &val) const override;

private:
    const IntelNumberParser _intel;
};

struct SemicolonCommentParser final : CommentParser, Singleton<SemicolonCommentParser> {
    bool endOfLine(const StrScanner &scan) const override { return *scan == ';' || *scan == 0; }
};

struct RcaCommentParser final : CommentParser, Singleton<RcaCommentParser> {
    bool commentLine(const StrScanner &scan) const override {
        return (scan[0] == '.' && scan[1] == '.') || endOfLine(scan);
    }
    bool endOfLine(const StrScanner &scan) const override { return *scan == ';' || *scan == 0; }
};

struct SharpCommentParser final : CommentParser, Singleton<SharpCommentParser> {
    bool endOfLine(const StrScanner &scan) const {
        return *scan == '#' || _semicolon.endOfLine(scan);
    }

private:
    const SemicolonCommentParser _semicolon;
};

struct AsteriskCommentParser final : CommentParser, Singleton<AsteriskCommentParser> {
    bool commentLine(const StrScanner &scan) const { return *scan == '*' || endOfLine(scan); }
    bool endOfLine(const StrScanner &scan) const { return *scan == ';' || *scan == 0; }
};

/**
 * Default symbol is '[:alpha:][:alnum:]*'.
 */
struct DefaultSymbolParser final : SymbolParser, Singleton<DefaultSymbolParser> {
    bool symbolLetter(char c, bool headOfSymbol = false) const override;
};

/**
 * Simple symbol is '([:alpha:]|[prefix])([:alnum:]|[extra])*'.
 */
struct SimpleSymbolParser final : SymbolParser, Singleton<SimpleSymbolParser> {
    SimpleSymbolParser(const /*PROGMEM*/ char *extra_P) : _prefix_P(extra_P), _extra_P(extra_P) {}
    SimpleSymbolParser(const /*PROGMEM*/ char *prefix_P, const /*PROGMEM*/ char *extra_P)
        : _prefix_P(prefix_P), _extra_P(extra_P) {}
    bool symbolLetter(char c, bool headOfSymbol = false) const override;

private:
    const /*PROGMEMD*/ char *const _prefix_P;
    const /*PROGMEMD*/ char *const _extra_P;
};

struct CStyleLetterParser final : LetterParser, Singleton<CStyleLetterParser> {
    /** C-style letter is: [:print:], \['"?\btnt], \x[0-9A-Fa-f]+, \[0-7]+ */
    char readLetter(StrScanner &scan, ErrorAt &error) const override;
};

struct DefaultLetterParser final : LetterParser, Singleton<DefaultLetterParser> {};

struct MotorolaLetterParser final : LetterParser, Singleton<MotorolaLetterParser> {
    MotorolaLetterParser(bool closingQuote = false) : _closingQuote(closingQuote) {}

    /**
     * Motorola style letter is followed after a single quote and optionally closed with another
     * single quote
     */
    Error parseLetter(StrScanner &scan, char &letter) const override;
    char readLetter(StrScanner &scan, ErrorAt &error) const override;

private:
    const bool _closingQuote;
    Error hasSuffix(StrScanner &scan) const;
};

struct ZilogLetterParser final : LetterParser, Singleton<ZilogLetterParser> {
    /**
     * Zilog style letter is followed after a single quote and hexadecimal escape sequence with
     * '%hh'. Also a single quote is expressed as %Q.
     */
    char readLetter(StrScanner &scan, ErrorAt &error) const override;
};

struct IbmLetterParser final : LetterParser, Singleton<IbmLetterParser> {
    IbmLetterParser(char prefix = 'C') : _prefix(prefix) {}

    /** IBM style letter constant is C'c'. */
    Error parseLetter(StrScanner &scan, char &letter) const override;

private:
    const char _prefix;
};

struct FairchildLetterParser final : LetterParser, Singleton<FairchildLetterParser> {
    FairchildLetterParser() {}

    /** Fairchild style letter is: [cC]'[:print:]', #[:print:], '[:print:]'? */
    Error parseLetter(StrScanner &scan, char &letter) const override;

private:
    static Error hasPrefix(StrScanner &scan, char &prefix);
    static Error hasSuffix(StrScanner &scan, char prefix);
};

struct AsteriskLocationParser final : LocationParser, Singleton<AsteriskLocationParser> {
    bool locationSymbol(StrScanner &scan) const override;
};

struct DollarLocationParser final : LocationParser, Singleton<DollarLocationParser> {
    bool locationSymbol(StrScanner &scan) const override;
};

struct NationalLocationParser final : LocationParser, Singleton<NationalLocationParser> {
    NationalLocationParser(char extra = '$') : _extra(extra) {}
    bool locationSymbol(StrScanner &scan) const override;

private:
    const char _extra;
};

struct FairchildLocationParser final : LocationParser, Singleton<FairchildLocationParser> {
    bool locationSymbol(StrScanner &scan) const override;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
