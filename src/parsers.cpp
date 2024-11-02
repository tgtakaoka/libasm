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

#include <ctype.h>
#include "comment_parser.h"
#include "config_base.h"
#include "function_table.h"
#include "letter_parser.h"
#include "number_parser.h"
#include "symbol_parser.h"
#include "text_common.h"

namespace libasm {

using namespace text::common;

namespace {

bool is_octal(char c) {
    return c >= '0' && c < '8';
}

bool is_binary(char c) {
    return c == '0' || c == '1';
}

uint8_t to_digit(char c) {
    return isdigit(c) ? c - '0' : toupper(c) - 'A' + 10;
}

bool is_digit(const char c, const Radix radix) {
    if (radix == RADIX_16)
        return isxdigit(c);
    return c >= '0' && c < '0' + uint8_t(radix);
}

}  // namespace

Error NumberParser::scanNumberEnd(StrScanner &scan, Radix radix, char suffix) {
    auto p = scan;
    if (radix == RADIX_2 && suffix == 'B') {
        // Check whether Intel binary or C-style binary, '010b' vs '0b10'
        if (p.expect('0') && p.iexpect('B') && is_binary(*p))
            return NOT_AN_EXPECTED;
        p = scan;
    }
    if (is_digit(*p, radix)) {
        if (radix == RADIX_16 && !is_digit(*p, RADIX_10))
            return NOT_AN_EXPECTED;
        p.trimStart([radix](char c) { return is_digit(c, radix); });
        if (suffix == 0) {
            if (!isalnum(*p)) {
                scan = p;
                return OK;
            }
        } else if (p.iexpect(suffix)) {
            scan = p;
            return OK;
        }
    }
    return NOT_AN_EXPECTED;
}

Radix CStyleNumberParser::hasPrefix(StrScanner &scan, Radix defaultRadix) const {
    if (scan.iexpectText_P(PSTR_ZERO_X))
        return RADIX_16;
    if (scan.iexpectText_P(PSTR_ZERO_B))
        return RADIX_2;
    if (*scan == '0' && is_octal(scan[1]))
        return RADIX_8;
    if (is_digit(*scan, defaultRadix))
        return defaultRadix;
    return RADIX_NONE;
}

Error CStyleNumberParser::parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const {
    auto p = scan;
    const auto radix = hasPrefix(p, defaultRadix);
    if (radix == RADIX_NONE)
        return NOT_AN_EXPECTED;

    const auto error = val.read(p, radix);
    if (error != NOT_AN_EXPECTED)
        scan = p;
    return error;
}

Radix IntelNumberParser::hasSuffix(StrScanner &scan) {
    if (scanNumberEnd(scan, RADIX_16, 'H') == OK)
        return RADIX_16;
    if (scanNumberEnd(scan, RADIX_8, 'O') == OK || scanNumberEnd(scan, RADIX_8, 'Q') == OK)
        return RADIX_8;
    if (scanNumberEnd(scan, RADIX_2, 'B') == OK)
        return RADIX_2;
    if (scanNumberEnd(scan, RADIX_10, 'D') == OK)
        return RADIX_10;
    return RADIX_NONE;
}

Error IntelNumberParser::parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const {
    auto end = scan;
    const auto radix = hasSuffix(end);
    if (radix == RADIX_NONE)
        return CStyleNumberParser::singleton().parseNumber(scan, val, defaultRadix);
    return parseNumber(scan, val, radix, end);
}

Error IntelNumberParser::parseNumber(
        StrScanner &scan, Value &val, Radix radix, StrScanner &end) const {
    const auto error = val.read(scan, radix);
    if (error != NOT_AN_EXPECTED)
        scan = end;
    return error;
}

Radix PrefixNumberParser::hasPrefix(StrScanner &scan) const {
    if (_hex && scan.iexpect(_hex))
        return RADIX_16;
    if (_bin && scan.iexpect(_bin))
        return RADIX_2;
    if (_oct && scan.iexpect(_oct))
        return RADIX_8;
    if (_dec && scan.iexpect(_dec))
        return RADIX_10;
    return RADIX_NONE;
}

Error PrefixNumberParser::parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const {
    auto p = scan;
    const auto radix = hasPrefix(p);
    if (radix == RADIX_NONE)
        return IntelNumberParser::singleton().parseNumber(scan, val, defaultRadix);
    const auto error = val.read(p, radix);
    if (error != NOT_AN_EXPECTED)
        scan = p;
    return error;
}

Radix ZilogNumberParser::hasPrefix(StrScanner &scan) const {
    if (scan.iexpectText_P(PSTR_PERCENT_8))
        return RADIX_8;
    if (scan.iexpectText_P(PSTR_PERCENT_2))
        return RADIX_2;
    if (scan.expect('%') && isxdigit(*scan))
        return RADIX_16;
    return RADIX_NONE;
}

Error ZilogNumberParser::parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const {
    auto p = scan;
    const auto radix = hasPrefix(p);
    if (radix == RADIX_NONE)
        return IntelNumberParser::singleton().parseNumber(scan, val, defaultRadix);

    const auto error = val.read(p, radix);
    if (error != NOT_AN_EXPECTED)
        scan = p;
    return error;
}

Error IbmNumberParser::parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const {
    auto p = scan;
    const auto radix = hasPrefix(p);
    if (radix == RADIX_NONE)
        return IntelNumberParser::singleton().parseNumber(scan, val, defaultRadix);

    if (!p.expect('\'') || !is_digit(*p, radix))
        return NOT_AN_EXPECTED;

    const auto error = val.read(p, radix);
    if (p.expect('\'')) {
        scan = p;
        return error;
    }
    return MISSING_CLOSING_QUOTE;
}

Error RcaNumberParser::parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const {
    auto p = scan;
    if (*p == '#' && isxdigit(p[1])) {
        const auto error = val.read(++p, RADIX_16);
        if (error == OK)
            scan = p;
        return error;
    }
    return IbmNumberParser::parseNumber(scan, val, defaultRadix);
}

Radix NationalNumberParser::hasPrefix(StrScanner &scan) const {
    if (_hex == 0 && (scan.iexpect('X') || scan.iexpect('H')))
        return RADIX_16;
    if (_oct == 'Q' && scan.iexpect('O'))
        return RADIX_8;
    return PrefixNumberParser::hasPrefix(scan);
}

Error NationalNumberParser::parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const {
    auto p = scan;
    const auto radix = hasPrefix(p);
    if (radix == RADIX_NONE)
        return IntelNumberParser::singleton().parseNumber(scan, val, defaultRadix);

    if (!p.expect('\'') || !is_digit(*p, radix))
        return NOT_AN_EXPECTED;

    const auto error = val.read(p, radix);
    p.expect('\'');  // closing quote is optional
    if (error != NOT_AN_EXPECTED)
        scan = p;
    return error;
}

Error FairchildNumberParser::parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const {
    auto p = scan;
    if (*p == '$' && isxdigit(p[1])) {
        const auto error = val.read(++p, RADIX_16);
        if (error != NOT_AN_EXPECTED)
            scan = p;
        return error;
    }
    return IbmNumberParser::parseNumber(scan, val, defaultRadix);
}

Error Pdp11NumberParser::parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const {
    if (scan[0] == '"' && scan[1] && scan[2]) {
        const auto value = (static_cast<uint16_t>(scan[2]) << 8) | scan[1];
        val.setUnsigned(value);
        scan += 3;
        return OK;
    }
    auto end = scan;
    // '[0-9]+.' should be decimal
    if (scanNumberEnd(end, RADIX_10, '.') == OK && !isdigit(*end))
        return IntelNumberParser::singleton().parseNumber(scan, val, RADIX_10, end);
    return IntelNumberParser::singleton().parseNumber(scan, val, defaultRadix);
}

bool SymbolParser::symbolLetter(char c, bool headOfSymbol) const {
    return isalpha(c) || (!headOfSymbol && isdigit(c)) || c == '_';
}

bool SymbolParser::locationSymbol(StrScanner &scan, char location) const {
    auto p = scan;
    if (p.expect(location) && !symbolLetter(*p)) {
        scan = p;
        return true;
    }
    return false;
}

bool SimpleSymbolParser::symbolLetter(char c, bool headOfSymbol) const {
    return SymbolParser::symbolLetter(c, headOfSymbol) || (c && strchr_P(_extra_P, c));
}

bool PrefixSymbolParser::symbolLetter(char c, bool headOfSymbol) const {
    if (SymbolParser::symbolLetter(c, headOfSymbol))
        return true;
    if (headOfSymbol && _prefix_P)
        return c && strchr_P(_prefix_P, c);
    if (_extra_P)
        return c && strchr_P(_extra_P, c);
    return false;
}

bool RcaSymbolParser::functionNameLetter(char c) const {
    return symbolLetter(c) || c == '.';
}

bool RcaSymbolParser::instructionLetter(char c) const {
    return SymbolParser::instructionLetter(c) || c == '=';
}

bool FairchildSymbolParser::locationSymbol(StrScanner &scan) const {
    return SymbolParser::locationSymbol(scan, '*') || SymbolParser::locationSymbol(scan, '$') ||
           SymbolParser::locationSymbol(scan, '.');
}

bool MostekSymbolParser::instructionLetter(char c) const {
    return SymbolParser::instructionLetter(c) || c == '.' || c == '=' || c == '*' || c == ':';
}

bool Ins80xxSymbolParser::locationSymbol(StrScanner &scan) const {
    return SymbolParser::locationSymbol(scan, '.') || SymbolParser::locationSymbol(scan, '$');
}

bool Ins80xxSymbolParser::instructionLetter(char c) const {
    return PrefixSymbolParser::instructionLetter(c) || c == '=' || c == '.';
}

bool Mc68000SymbolParser::instructionLetter(char c) const {
    return PrefixSymbolParser::instructionLetter(c) || c == '.';
}

bool Ns32000SymbolParser::locationSymbol(StrScanner &scan) const {
    return SymbolParser::locationSymbol(scan, '.') || SymbolParser::locationSymbol(scan, '*');
}

bool Ns32000SymbolParser::instructionLetter(char c) const {
    return SimpleSymbolParser::instructionLetter(c) || c == '.';
}

bool Pdp11SymbolParser::locationSymbol(StrScanner &scan) const {
    return SymbolParser::locationSymbol(scan, '.') || SymbolParser::locationSymbol(scan, '*');
}

bool Pdp11SymbolParser::instructionLetter(char c) const {
    return PrefixSymbolParser::instructionLetter(c) || c == '=' || c == '.';
}

bool Pdp8SymbolParser::locationSymbol(StrScanner &scan) const {
    return SymbolParser::locationSymbol(scan, '.') || SymbolParser::locationSymbol(scan, '$');
}

bool Pdp8SymbolParser::instructionLetter(char c) const {
    return SymbolParser::symbolLetter(c) || c == '*' || c == '=';
}

bool Pdp8SymbolParser::instructionTerminator(char c) const {
    return c == '*' || c == '=';
}

bool Tms32010SymbolParser::instructionLetter(char c) const {
    return SimpleSymbolParser::instructionLetter(c) || c == '.';
}

Error LetterParser::parseLetter(StrScanner &scan, char &letter) const {
    auto p = scan;
    if (!letterPrefix(p))
        return NOT_AN_EXPECTED;
    const auto delim = letterDelimiter(p);
    if (delim) {
        ErrorAt error;
        letter = readLetter(p, error, delim);
        if (error.isOK()) {
            if (p.expect(delim)) {
                scan = p;
                return OK;
            }
            return delim == '"' ? MISSING_CLOSING_DQUOTE : MISSING_CLOSING_QUOTE;
        }
        return error.getError();
    }
    return NOT_AN_EXPECTED;
}

char LetterParser::readLetter(StrScanner &scan, ErrorAt &error, char delim) const {
    auto p = scan;
    if (p.expect(delim)) {
        if (p.expect(delim)) {
            scan = p;
            return delim;  // Successive delimitor=
        }
        error.setErrorIf(scan, delim == '"' ? MISSING_CLOSING_DQUOTE : MISSING_CLOSING_QUOTE);
        return 0;
    }
    const auto c = *p++;
    if (c == 0) {
        error.setErrorIf(ILLEGAL_CONSTANT);
    } else {
        scan = p;
    }
    return c;
}

char CStyleLetterParser::readLetter(StrScanner &scan, ErrorAt &error, char delim) const {
    auto p = scan;
    auto c = *p++;
    if (c == '\\') {
        // Backslash escape sequence
        if (p.iexpect('x')) {
            // hexadecimal digits.
            uint32_t n = 0;
            while (isxdigit(*p)) {
                n *= 16;
                n += to_digit(*p++);
            }
            scan = p;
            if (n > UINT8_MAX)
                error.setErrorIf(OVERFLOW_RANGE);
            return n;
        }
        if (is_octal(*p)) {
            // 1~3 octal digits.
            uint32_t n = 0;
            for (auto i = 0; is_octal(*p) && i < 3; i++) {
                n *= 8;
                n += to_digit(*p++);
            }
            scan = p;
            if (n > UINT8_MAX)
                error.setErrorIf(scan, OVERFLOW_RANGE);
            return n;
        }
        c = *p++;
        switch (c) {
        case '\'':
        case '"':
        case '?':
        case '\\':
            break;
        case 'b':
            c = 0x08;
            break;
        case 't':
            c = 0x09;
            break;
        case 'n':
            c = 0x0a;
            break;
        case 'r':
            c = 0x0d;
            break;
        default:
            error.setErrorIf(scan, UNKNOWN_ESCAPE_SEQUENCE);
        }
        scan = p;
        return c;
    }
    scan = p;
    if (c == delim)
        error.setErrorIf(ILLEGAL_CONSTANT);
    return c;
}

Error MotorolaLetterParser::parseLetter(StrScanner &scan, char &letter) const {
    auto p = scan;
    const auto delim = letterDelimiter(p);
    if (delim) {
        if ((letter = *p++) == 0)
            return ILLEGAL_CONSTANT;
        p.expect(delim);  // closing delimiter is optional
        scan = p;
        return OK;
    }
    return NOT_AN_EXPECTED;
}

char ZilogLetterParser::readLetter(StrScanner &scan, ErrorAt &error, char delim) const {
    auto p = scan;
    auto c = *p++;
    if (c == '%') {
        // Percent escape sequence
        if (isxdigit(p[0]) && isxdigit(p[1])) {
            c = to_digit(p[0]) * 16 + to_digit(p[1]);
            p += 2;
        } else {
            c = *p++;
            switch (toupper(c)) {
            case 'L':
                c = 0x0A;
                break;
            case 'T':
                c = 0x09;
                break;
            case 'R':
                c = 0x0D;
                break;
            case 'P':
                c = 0x0C;
                break;
            case '%':
                break;
            case 'Q':
                c = '\'';
                break;
            default:
                error.setErrorIf(scan, UNKNOWN_ESCAPE_SEQUENCE);
            }
        }
    } else if (c == delim) {
        error.setErrorIf(scan, ILLEGAL_CONSTANT);
    }
    scan = p;
    return c;
}

Error FairchildLetterParser::parseLetter(StrScanner &scan, char &letter) const {
    auto p = scan;
    if (p.iexpect('C')) {  // C'C'
        const auto error = LetterParser::parseLetter(p, letter);
        if (error == OK)
            scan = p;
        return error;
    }
    if (p.expect('#')) {  // #C
        if ((letter = *p) == 0)
            return NOT_AN_EXPECTED;
        scan = ++p;
        return OK;
    }
    if (p.expect('\'')) {  // 'c' or 'c
        ErrorAt error;
        letter = readLetter(p, error, '\'');
        if (error.isOK()) {
            p.expect('\'');  // closing quote is optional
            scan = p;
            return OK;
        }
        return error.getError();
    }
    return NOT_AN_EXPECTED;
}

bool FairchildLetterParser::stringPrefix(StrScanner &scan) const {
    scan.iexpect('C');  // optional
    return true;
}

char MostekLetterParser::stringDelimiter(StrScanner &scan) const {
    const auto c = scan.expect('\'');
    return c ? c : scan.expect('"');
}

char Ns32000LetterParser::readLetter(StrScanner &scan, ErrorAt &error, char delim) const {
    auto c = *scan++;
    if (c == '\\' && *scan) {
        c = *scan++;
    } else if (c == delim) {
        error.setErrorIf(ILLEGAL_CONSTANT);
    }
    return c;
}

Error Pdp8LetterParser::parseLetter(StrScanner &scan, char &letter) const {
    if (*scan == '\'')
        return LetterParser::parseLetter(scan, letter);
    auto p = scan;
    if (p.expect('"')) {
        ErrorAt error;
        letter = readLetter(p, error, 0);
        if (error.isOK()) {
            scan = p;
            return OK;
        }
        return ILLEGAL_CONSTANT;
    }
    return NOT_AN_EXPECTED;
}

char Pdp8LetterParser::readLetter(StrScanner &scan, ErrorAt &error, char) const {
    const auto c = *scan;
    if (c == 0 || (c >= 0x60 && c < 0x7F)) {
        error.setErrorIf(ILLEGAL_CONSTANT);  // no lowercase letter
        return 0;
    }
    ++scan;
    return c + 0x80;  // convert to DEC 8-Bit form.
}

bool SemicolonCommentParser::endOfLine(StrScanner &scan) const {
    return *scan.skipSpaces() == 0 || *scan == ';';
}

bool SharpCommentParser::endOfLine(StrScanner &scan) const {
    return SemicolonCommentParser::singleton().endOfLine(scan) || *scan == '#';
}

bool StarCommentParser::commentLine(StrScanner &scan) const {
    return *scan == '*' || *scan == ';';
}

bool StarCommentParser::endOfLine(StrScanner &scan) const {
    const auto end = (*scan == 0 || *scan == ' ');
    scan.skipSpaces();
    return end || SemicolonCommentParser::singleton().endOfLine(scan);
}

bool RcaCommentParser::commentLine(StrScanner &scan) const {
    return scan.iexpectText_P(PSTR("..")) || endOfLine(scan);
}

bool Pdp8CommentParser::endOfLine(StrScanner &scan) const {
    return *scan == 0 || *scan == '/' || *scan == ';';
};

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
