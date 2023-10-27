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

PrefixNumberParser::PrefixNumberParser(char hex, char bin, char oct, char dec)
    : _hex(hex), _bin(bin), _oct(oct), _dec(dec) {}

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

Error PrefixNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    auto p = scan;
    const auto radix = hasPrefix(p);
    if (radix == RADIX_NONE)
        return IntelNumberParser::singleton().parseNumber(scan, val);
    const auto error = val.parseNumber(p, radix);
    if (error == OK)
        scan = p;
    return error;
}

Error IbmNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    auto p = scan;
    const auto radix = hasPrefix(p);
    if (radix == RADIX_NONE)
        return IntelNumberParser::singleton().parseNumber(scan, val);

    if (!p.expect('\'') || !isValidDigit(*p, radix))
        return NOT_AN_EXPECTED;

    const auto error = val.parseNumber(p, radix);
    if (p.expect('\'')) {
        scan = p;
        return error;
    }
    return MISSING_CLOSING_QUOTE;
}

Radix NationalNumberParser::hasPrefix(StrScanner &scan) const {
    if (_hex == 0 && (scan.iexpect('X') || scan.iexpect('H')))
        return RADIX_16;
    if (_oct == 'Q' && scan.iexpect('O'))
        return RADIX_8;
    return PrefixNumberParser::hasPrefix(scan);
}

Error NationalNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    auto p = scan;
    const auto radix = hasPrefix(p);
    if (radix == RADIX_NONE)
        return IntelNumberParser::singleton().parseNumber(scan, val);

    if (!p.expect('\'') || !isValidDigit(*p, radix))
        return NOT_AN_EXPECTED;

    const auto error = val.parseNumber(p, radix);
    if (error == OK) {
        p.expect('\'');  // closing quote is optional
        scan = p;
    }
    return error;
}

Radix CStyleNumberParser::hasPrefix(StrScanner &scan) const {
    if (scan.iexpectText_P(text::common::PSTR_ZERO_X))
        return RADIX_16;
    if (scan.iexpectText_P(text::common::PSTR_ZERO_B))
        return RADIX_2;
    if (*scan == '0' && isoctal(scan[1]))
        return RADIX_8;
    if (isdigit(*scan))
        return RADIX_10;
    return RADIX_NONE;
}

Error CStyleNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    auto p = scan;
    const auto radix = hasPrefix(p);
    if (radix == RADIX_NONE)
        return NOT_AN_EXPECTED;

    const auto error = val.parseNumber(p, radix);
    if (error == OK)
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

Error IntelNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    auto p = scan;
    return parseNumber(scan, val, hasSuffix(p), p);
}

Error IntelNumberParser::parseNumber(
        StrScanner &scan, Value &val, Radix radix, StrScanner &next) const {
    if (radix == RADIX_NONE)
        return CStyleNumberParser::singleton().parseNumber(scan, val);
    const auto error = val.parseNumber(scan, radix);
    if (error == OK)
        scan = next;
    return error;
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

Radix ZilogNumberParser::hasPrefix(StrScanner &scan) const {
    if (scan.iexpectText_P(text::common::PSTR_PERCENT_8))
        return RADIX_8;
    if (scan.iexpectText_P(text::common::PSTR_PERCENT_2))
        return RADIX_2;
    if (scan.expect('%') && isxdigit(*scan))
        return RADIX_16;
    return RADIX_NONE;
}

Error ZilogNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    auto p = scan;
    const auto radix = hasPrefix(p);
    if (radix == RADIX_NONE)
        return IntelNumberParser::singleton().parseNumber(scan, val);

    const auto error = val.parseNumber(p, radix);
    if (error == OK)
        scan = p;
    return error;
}

Error FairchildNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    auto p = scan;
    if (*p == '$' && isxdigit(p[1])) {
        const auto error = val.parseNumber(++p, RADIX_16);
        if (error == OK)
            scan = p;
        return error;
    }
    return _ibm.parseNumber(scan, val);
}

Error RcaNumberParser::parseNumber(StrScanner &scan, Value &val) const {
    auto p = scan;
    if (*p == '#' && isxdigit(p[1])) {
        const auto error = val.parseNumber(++p, RADIX_16);
        if (error == OK)
            scan = p;
        return error;
    }
    return _ibm.parseNumber(scan, val);
}

bool SymbolParser::symbolLetter(char c, bool headOfSymbol) const {
    return isalpha(c) || (!headOfSymbol && isdigit(c)) || c == '_';
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
    auto p = scan;
    if (!letterPrefix(p))
        return NOT_AN_EXPECTED;
    const char delim = letterDelimiter(p);
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

Error MotorolaLetterParser::parseLetter(StrScanner &scan, char &letter) const {
    auto p = scan;
    const char delim = letterDelimiter(p);
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
