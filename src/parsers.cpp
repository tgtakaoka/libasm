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

#include "parsers.h"

#include <ctype.h>

#include "text_common.h"

namespace libasm {

namespace {

bool isoctal(char c) {
    return c >= '0' && c < '8';
}

bool isbinary(char c) {
    return c == '0' || c == '1';
}

uint8_t tonumbert(char c) {
    return isdigit(c) ? c - '0' : toupper(c) - 'A' + 10;
}

bool isValidDigit(const char c, const Radix radix) {
    if (radix == RADIX_16)
        return isxdigit(c);
    return c >= '0' && c < '0' + uint8_t(radix);
}

}  // namespace

Error CStyleNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    auto p = scan;
    auto radix = RADIX_NONE;
    if (p.iexpectText_P(text::common::PSTR_ZERO_X)) {
        radix = RADIX_16;
    } else if (p.iexpectText_P(text::common::PSTR_ZERO_B)) {
        radix = RADIX_2;
    } else if (p.expect('0')) {
        if (!isalnum(*p) || isoctal(*p)) {
            radix = RADIX_8;
            p = scan;
        } else {
            return ILLEGAL_CONSTANT;
        }
    } else if (isdigit(*p)) {
        radix = RADIX_10;
    }

    if (radix == RADIX_NONE)
        return NOT_AN_EXPECTED;

    const auto err = val.parseNumber(p, radix);
    if (err == OK)
        scan = p;
    return err;
}

Error MotorolaNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    auto p = scan;
    auto radix = RADIX_NONE;
    if (p.expect('$')) {
        radix = RADIX_16;
    } else if (p.expect('@')) {
        radix = RADIX_8;
    } else if (p.expect('%')) {
        radix = RADIX_2;
    } else if (p.expect('&')) {
        radix = RADIX_10;
    } else {
        return CStyleNumberParser::singleton().parseNumber(scan, val);
    }

    const auto err = val.parseNumber(p, radix);
    if (err == OK)
        scan = p;
    return err;
}

Error IntelNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    auto p = scan;
    auto radix = RADIX_NONE;
    if (scanNumberEnd(p, RADIX_16, 'H') == OK) {
        radix = RADIX_16;
    } else if (scanNumberEnd(p, RADIX_8, 'O') == OK) {
        radix = RADIX_8;
    } else if (scanNumberEnd(p, RADIX_8, 'Q') == OK) {
        radix = RADIX_8;
    } else if (scanNumberEnd(p, RADIX_2, 'B') == OK) {
        radix = RADIX_2;
    } else if (scanNumberEnd(p, RADIX_10, 'D') == OK) {
        radix = RADIX_10;
    } else {
        return CStyleNumberParser::singleton().parseNumber(scan, val);
    }

    const auto err = val.parseNumber(scan, radix);
    if (err == OK)
        scan = p;
    return err;
}

Error IntelNumberParser::scanNumberEnd(StrScanner &scan, Radix radix, char suffix) {
    auto p = scan;
    if (suffix == 'B') {
        // Check whether Intel binary or C-style binary, '010b' vs '0b10'
        if (p.expect('0') && p.iexpect('B') && isbinary(*p))
            return NOT_AN_EXPECTED;
        p = scan;
    }
    if (isValidDigit(*p, radix)) {
        if (radix == RADIX_16 && !isValidDigit(*p, RADIX_10))
            return NOT_AN_EXPECTED;
        p.trimStart([radix](char c) { return isValidDigit(c, radix); });
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

Error ZilogNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    auto p = scan;
    auto radix = RADIX_NONE;
    if (p.iexpectText_P(text::common::PSTR_PERCENT_8)) {
        radix = RADIX_8;
    } else if (p.iexpectText_P(text::common::PSTR_PERCENT_2)) {
        radix = RADIX_2;
    } else if (p.expect('%') && isxdigit(*p)) {
        radix = RADIX_16;
    } else {
        return IntelNumberParser::singleton().parseNumber(scan, val);
    }

    const auto err = val.parseNumber(p, radix);
    if (err == OK)
        scan = p;
    return err;
}

Error IbmNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    auto p = scan;
    auto radix = RADIX_NONE;
    if (_hex && p.iexpect(_hex)) {
        radix = RADIX_16;
    } else if (_hex == 0 && (p.iexpect('X') || p.iexpect('H'))) {
        radix = RADIX_16;
    } else if (_bin && p.iexpect(_bin)) {
        radix = RADIX_2;
    } else if (_oct && (p.iexpect(_oct) || p.iexpect('O'))) {
        radix = RADIX_8;
    } else if (_dec && p.iexpect(_dec)) {
        radix = RADIX_10;
    } else {
        return CStyleNumberParser::singleton().parseNumber(scan, val);
    }

    if (!p.expect('\'') || !isValidDigit(*p, radix))
        return NOT_AN_EXPECTED;

    const auto err = val.parseNumber(p, radix);
    if (p.expect('\'')) {
        scan = p;
        return err;
    }
    scan = p;
    // Closing quote is optional
    return _closingQuote ? MISSING_CLOSING_QUOTE : err;
}

Error FairchildNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    if (*scan == '$' && isxdigit(scan[1])) {
        auto p = scan;
        const auto err = val.parseNumber(++p, RADIX_16);
        if (err == OK)
            scan = p;
        return err;
    }
    return _ibm.parseNumber(scan, val);
}

Error RcaNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    if (*scan == '#' && isxdigit(scan[1])) {
        auto p = scan;
        const auto err = val.parseNumber(++p, RADIX_16);
        if (err == OK)
            scan = p;
        return err;
    }
    const auto err = IntelNumberParser::singleton().parseNumber(scan, val);
    if (err == OK)
        return OK;
    return _ibm.parseNumber(scan, val);
}

Error SigneticsNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    const auto err = IntelNumberParser::singleton().parseNumber(scan, val);
    if (err == OK)
        return OK;
    return _ibm.parseNumber(scan, val);
}

Error TexasNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    if (*scan == '>' && isxdigit(scan[1])) {
        auto p = scan;
        const auto err = val.parseNumber(++p, RADIX_16);
        if (err == OK)
            scan = p;
        return err;
    }
    const auto err = IntelNumberParser::singleton().parseNumber(scan, val);
    if (err == OK)
        return OK;
    return CStyleNumberParser::singleton().parseNumber(scan, val);
}

bool SymbolParser::symbolLetter(char c, bool headOfSymbol) const {
    return isalpha(c) || (!headOfSymbol && isdigit(c));
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

Error LetterParser::parseLetter(StrScanner &scan, char &letter) const {
    if (scan.expect('\'')) {
        ErrorAt error;
        letter = readLetter(scan, error);
        if (error.isOK())
            return scan.expect('\'') ? OK : error.setErrorIf(scan, MISSING_CLOSING_QUOTE);
        return error.getError();
    }
    return NOT_AN_EXPECTED;
}

char LetterParser::readLetter(StrScanner &scan, ErrorAt &error, char delim) {
    const auto c = *scan;
    if (c == 0) {
        error.setErrorIf(ILLEGAL_CONSTANT);
        return c;
    }
    if (c == delim) {
        if (scan[1] == delim) {
            scan += 2;  // successive delimiters
        } else {
            return error.setErrorIf(
                    scan, delim == '"' ? MISSING_CLOSING_DQUOTE : MISSING_CLOSING_QUOTE);
        }
    } else {
        scan += 1;
    }
    return c;
}

char CStyleLetterParser::readLetter(StrScanner &scan, ErrorAt &error, char delim) {
    auto p = scan;
    auto c = *p++;
    if (c == '\\') {
        // Backslash escape sequence
        if (p.iexpect('x')) {
            // hexadecimal digits.
            uint32_t n = 0;
            while (isxdigit(*p)) {
                n *= 16;
                n += tonumbert(*p++);
            }
            scan = p;
            if (ConfigBase::overflowUint8(n))
                error.setErrorIf(OVERFLOW_RANGE);
            return n;
        }
        if (isoctal(*p)) {
            // 1~3 octal digits.
            uint16_t n = 0;
            for (auto i = 0; isoctal(*p) && i < 3; i++) {
                n *= 8;
                n += tonumbert(*p++);
            }
            scan = p;
            if (ConfigBase::overflowUint8(n))
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

char ZilogLetterParser::readLetter(StrScanner &scan, ErrorAt &error) const {
    auto p = scan;
    auto c = *p++;
    if (c == '%') {
        // Percent escape sequence
        if (isxdigit(p[0]) && isxdigit(p[1])) {
            c = tonumbert(p[0]) * 16 + tonumbert(p[1]);
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
    } else if (c == '\'') {
        error.setErrorIf(MISSING_CLOSING_QUOTE);
    }
    scan = p;
    return c;
}

Error MotorolaLetterParser::parseLetter(StrScanner &scan, char &letter) const {
    if (!scan.expect('\''))
        return NOT_AN_EXPECTED;
    ErrorAt error;
    letter = readLetter(scan, error);
    if (error.isOK())
        scan.expect('\'');
    return error.getError();
}

char MotorolaLetterParser::readLetter(StrScanner &scan, ErrorAt &error) const {
    if (*scan == 0)
        error.setErrorIf(ILLEGAL_CONSTANT);
    return *scan++;
}

char MotorolaLetterParser::readLetterInString(StrScanner &scan, ErrorAt &error) const {
    return LetterParser::readLetter(scan, error, '\'');
}

Error IbmLetterParser::parseLetter(StrScanner &scan, char &letter) const {
    auto p = scan;
    if (p.iexpect(_prefix) && p.expect('\'')) {
        ErrorAt error;
        letter = readLetter(p, error);
        if (p.expect('\'')) {
            scan = p;
            return error.getError() ? error.getError() : OK;
        }
        scan = p;
        return MISSING_CLOSING_QUOTE;
    }
    return DefaultLetterParser::singleton().parseLetter(scan, letter);
}

Error FairchildLetterParser::hasPrefix(StrScanner &scan, char &prefix) {
    auto p = scan;
    // C'x'
    if (p.iexpect('c')) {
        if (*p != '\'')
            return NOT_AN_EXPECTED;
        ++p;
        prefix = 'C';
    } else if (p.expect('#')) {
        // #c
        prefix = '#';
    } else if (p.expect('\'')) {
        // 'c' or 'c
        prefix = '\'';
    } else {
        return NOT_AN_EXPECTED;
    }
    scan = p;
    return OK;
}

Error FairchildLetterParser::parseLetter(StrScanner &scan, char &letter) const {
    char prefix;
    if (hasPrefix(scan, prefix) != OK)
        return NOT_AN_EXPECTED;
    ErrorAt error;
    if (prefix == '#') {
        if (*scan == 0)
            return error.setErrorIf(ILLEGAL_CONSTANT);
        letter = *scan++;
    } else {
        letter = readLetter(scan, error);
    }
    return error.getError() ? error.getError() : hasSuffix(scan, prefix);
}

Error FairchildLetterParser::hasSuffix(StrScanner &scan, char prefix) {
    if (prefix == 'C') {
        // closing quote is necessary for C'x'.
        return scan.expect('\'') ? OK : MISSING_CLOSING_QUOTE;
    }
    if (prefix == '#') {
        // no closing quite for #c
        return OK;
    }
    // closing quote is optional for 'x'
    scan.expect('\'');
    return OK;
}

bool AsteriskLocationParser::locationSymbol(StrScanner &scan) const {
    return scan.expect('*') && !isalnum(*scan);
}

bool DollarLocationParser::locationSymbol(StrScanner &scan) const {
    return scan.expect('$') && !isalnum(*scan);
}

bool SimpleLocationParser::locationSymbol(StrScanner &scan) const {
    auto p = scan;
    const auto c = *p++;
    if (c && strchr_P(_letters_P, c) && !isalnum(*p)) {
        scan = p;
        return true;
    }
    return false;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
