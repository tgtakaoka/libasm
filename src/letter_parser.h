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

#ifndef __LIBASM_LETTER_PARSER_H__
#define __LIBASM_LETTER_PARSER_H__

#include "error_reporter.h"
#include "str_scanner.h"

namespace libasm {

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
    virtual bool letterPrefix(StrScanner &) const { return true; }

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
    virtual bool stringPrefix(StrScanner &) const { return true; }

    /**
     * Returns a delimiter which encloses a string and cosume it. Returns zero if no delimiter
     * exists.
     */
    virtual char stringDelimiter(StrScanner &scan) const { return scan.expect('\''); }
};

struct CStyleLetterParser final : LetterParser, Singleton<CStyleLetterParser> {
    /** C-style letter is 'c' where c is [:print:], \['"?\btnt], \x[0-9A-Fa-f]+, \[0-7]+ */
    char readLetter(StrScanner &scan, ErrorAt &error, char delim) const override;
    /** C-style string is "str". */
    char stringDelimiter(StrScanner &scan) const override { return scan.expect('"'); }
};

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

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
