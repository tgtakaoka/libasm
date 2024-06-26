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

#ifndef __LIBASM_PARSERS_H__
#define __LIBASM_PARSERS_H__

#include "error_reporter.h"
#include "operators.h"
#include "str_scanner.h"
#include "symbol_table.h"
#include "value.h"

namespace libasm {

/**
 * Comment parser
 */
struct CommentParser {
    virtual bool commentLine(StrScanner &scan) const { return endOfLine(scan); }
    virtual bool endOfLine(StrScanner &scan) const = 0;
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
struct SymbolParser : Singleton<SymbolParser> {
    /** Default symbol is '[:alpha:][:alnum:]*' */
    virtual bool symbolLetter(char c, bool headOfSymbol = false) const;

    /** Label may have delimitor letter */
    virtual bool labelDelimitor(StrScanner &scan) const { return scan.expect(':'); }

    /** Instruction and directive letter at end */
    virtual bool instructionLetter(char c) const { return symbolLetter(c); }

    /** Instruction terminates when this returns true */
    virtual bool instructionTerminator(char c) const {
        UNUSED(c);
        return false;
    }

    /** Function name is same as symbol */
    virtual bool functionNameLetter(char c) const { return symbolLetter(c); }
};

/**
 * Pre-defined function table.
 */
struct FunctionTable : Singleton<FunctionTable> {
    /**
     * Look up |name| and returns a Functor pointer for a function, otherwise return nullptr.
     */
    virtual const Functor *lookupFunction(const StrScanner &name) const {
        UNUSED(name);
        return nullptr;
    }
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
     * Returns true and consumes prefix for a letter surrounded by letterdelimiters. Also returns
     * true if no prefix is required.
     */
    virtual bool letterPrefix(StrScanner &scan) const {
        UNUSED(scan);
        return true;
    }

    /**
     * Returns a delimiters for a letter and cosume it. Returns zero if no delimiter exists.
     */
    virtual char letterDelimiter(StrScanner &scan) const { return scan.expect('\''); }

    /**
     * Read a letter constant from |scan| and return it.  Default style letter is: [:print:], ''
     *
     * When |scan| points text which doesn't make sense as a letter, |error| is set as
     * ILLEGAL_CONSTANT.
     */
    virtual char readLetter(StrScanner &scan, ErrorAt &error, char delim) const;

    /**
     * Returns true and consumes prefix for a string surrounded by string delimiters. Also returns
     * true if no prefix is required.
     */
    virtual bool stringPrefix(StrScanner &scan) const {
        UNUSED(scan);
        return true;
    }

    /**
     * Returns a delimiter which encloses a string and cosume it. Returns zero if no delimiter
     * exists.
     */
    virtual char stringDelimiter(StrScanner &scan) const { return scan.expect('\''); }
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
 * Intel style numbers are
 * - Decimal:     "[0-9]+"
 * - Hexadecimal: "([0-9]|0[A-Fa-f])[0-9A-Fa-f]*[hH]"
 * - Octal:       "[0-7]+[oOqQ]"
 * - Binary:      "[01]+[bB]"
 */
struct IntelNumberParser final : NumberParser, Singleton<IntelNumberParser> {
    Error parseNumber(StrScanner &scan, Value &val) const override;
    Error parseNumber(StrScanner &scan, Value &val, Radix radix, StrScanner &next) const;

    static Radix hasSuffix(StrScanner &scan);

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
`* C-style numbers are
 * - Decimal:     "[1-9][0-9]*"
 * - Hexadecimal: "0[xX][0-9A-Fa-f]+"
 * - Octal:       "0[0-7]*"
 * - Binary:      "0[bB][01]+"
 */
struct CStyleNumberParser final : NumberParser, Singleton<CStyleNumberParser> {
    Error parseNumber(StrScanner &scan, Value &val) const override;

private:
    Radix hasPrefix(StrScanner &scan) const;
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

private:
    Radix hasPrefix(StrScanner &scan) const;
};

/**
 * Base for character prefixed number parser.
 */
struct PrefixNumberParser : NumberParser {
    PrefixNumberParser(char hex, char bin, char oct, char dec);

    Error parseNumber(StrScanner &scan, Value &val) const override;

protected:
    const char _hex;
    const char _bin;
    const char _oct;
    const char _dec;

    virtual Radix hasPrefix(StrScanner &scan) const;
};

/**
 * Motorola style numbers are
 * - Decimal:     "{&}[0-9]+"
 * - Hexadecimal: "$[0-9A-Fa-f]+"
 * - Octal:       "@[0-7]+"
 * - Binary:      "%[01]+"
 */
struct MotorolaNumberParser final : PrefixNumberParser, Singleton<MotorolaNumberParser> {
    MotorolaNumberParser() : PrefixNumberParser('$', '%', '@', '&') {}
};

/**
 * IBM style numbers are
 * - Decimal:     "[0-9]+|[dD]'[0-9]+'"
 * - Hexadecimal: "[hH]'[0-9A-Fa-f]+'"
 * - Octal:       "[oO]'[0-7]+'"
 * - Binary:      "[bB]'[01]+'"
 */
struct IbmNumberParser : PrefixNumberParser {
    IbmNumberParser(char hex, char bin, char oct, char dec)
        : PrefixNumberParser(hex, bin, oct, dec) {}
    Error parseNumber(StrScanner &scan, Value &val) const override;
};

/**
 * National style numbers are
 * - Decimal:     "[0-9]+|[dD]'[0-9]+'?"
 * - Hexadecimal: "[hH]'[0-9A-Fa-f]+'?"
 * - Octal:       "[oO]'[0-7]+'?"
 * - Binary:      "[bB]'[01]+'?"
 */
struct NationalNumberParser final : IbmNumberParser {
    NationalNumberParser(char hex, char bin, char oct, char dec)
        : IbmNumberParser(hex, bin, oct, dec) {}
    Error parseNumber(StrScanner &scan, Value &val) const override;

private:
    Radix hasPrefix(StrScanner &scan) const override;
};

/**
 * Fairchild style numbers are the same as IBM plus '$hh' for hexadecimal.
 */
struct FairchildNumberParser final : NumberParser, Singleton<FairchildNumberParser> {
    Error parseNumber(StrScanner &scan, Value &val) const override;

private:
    IbmNumberParser _ibm{'H', 'B', 'O', 'D'};
};

/**
 * Signetics style numbers are the same as IBM except H'hh' for hexadecimal.
 */
struct SigneticsNumberParser final : IbmNumberParser, Singleton<SigneticsNumberParser> {
    SigneticsNumberParser() : IbmNumberParser('H', 'B', 'O', 'D') {}
};

/**
 * Texas Instrument style numbers is:
 * - '>hh' as hexadecimal.
 * - '?bb' as binary.
 */
struct TexasNumberParser final : PrefixNumberParser, Singleton<TexasNumberParser> {
    TexasNumberParser() : PrefixNumberParser('>', '?', 0, 0) {}
};

struct SemicolonCommentParser final : CommentParser, Singleton<SemicolonCommentParser> {
    bool endOfLine(StrScanner &scan) const override {
        return *scan.skipSpaces() == 0 || *scan == ';';
    }
};

struct SharpCommentParser final : CommentParser, Singleton<SharpCommentParser> {
    bool endOfLine(StrScanner &scan) const override {
        return SemicolonCommentParser::singleton().endOfLine(scan) || *scan == '#';
    }
};

struct AsteriskCommentParser final : CommentParser, Singleton<AsteriskCommentParser> {
    bool commentLine(StrScanner &scan) const override { return *scan == '*' || *scan == ';'; }
    bool endOfLine(StrScanner &scan) const override {
        const auto end = (*scan == 0 || *scan == ' ');
        scan.skipSpaces();
        return end || SemicolonCommentParser::singleton().endOfLine(scan);
    }
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

struct CStyleLetterParser final : LetterParser, Singleton<CStyleLetterParser> {
    /** C-style letter is 'c' where c is [:print:], \['"?\btnt], \x[0-9A-Fa-f]+, \[0-7]+ */
    char readLetter(StrScanner &scan, ErrorAt &error, char delim) const override;
    /** C-style string is "str". */
    char stringDelimiter(StrScanner &scan) const override { return scan.expect('"'); }
};

struct DefaultLetterParser final : LetterParser, Singleton<DefaultLetterParser> {};

struct MotorolaLetterParser final : LetterParser, Singleton<MotorolaLetterParser> {
    /**
     * Motorola style letter is followed after a single quote and optionally closed with another
     * single quote
     */
    Error parseLetter(StrScanner &scan, char &letter) const override;
};

struct ZilogLetterParser final : LetterParser, Singleton<ZilogLetterParser> {
    /**
     * Zilog style letter is followed after a single quote and hexadecimal escape sequence with
     * '%hh'. Also a single quote is expressed as %Q.
     */
    char readLetter(StrScanner &scan, ErrorAt &error, char delim) const override;
};

struct IbmLetterParser final : LetterParser, Singleton<IbmLetterParser> {
    /** IBM style letter constant is C'c'. */
    bool letterPrefix(StrScanner &scan) const override {
        scan.iexpect('C');  // optional
        return true;
    }
    /** IBM style string constant is C'str'. */
    bool stringPrefix(StrScanner &scan) const override {
        scan.iexpect('C');  // optional
        return true;
    }
};

struct FairchildLetterParser final : LetterParser, Singleton<FairchildLetterParser> {
    /** Fairchild style letter is [cC]'[:print:]', #[:print:], '[:print:]'? */
    Error parseLetter(StrScanner &scan, char &letter) const override;
    /** Fairchild style string constant is C'str'. */
    bool stringPrefix(StrScanner &scan) const override {
        scan.iexpect('C');  // optional
        return true;
    }
};

struct AsteriskLocationParser final : LocationParser, Singleton<AsteriskLocationParser> {
    bool locationSymbol(StrScanner &scan) const override;
};

struct DollarLocationParser final : LocationParser, Singleton<DollarLocationParser> {
    bool locationSymbol(StrScanner &scan) const override;
};

struct SimpleLocationParser final : LocationParser {
    SimpleLocationParser(const /*PROGMEM*/ char *letters) : _letters_P(letters) {}
    bool locationSymbol(StrScanner &scan) const override;

private:
    const /*PROGMEM*/ char *_letters_P;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
