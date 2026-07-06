/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "tokenizer.h"

#include <cctype>
#include <cstring>

namespace libasm {
namespace gen {

bool isXdigits(const char *&r, const char *p) {
    if (!isxdigit(*p++))
        return false;
    while (isxdigit(*p))
        p++;
    r = p;
    return true;
}

bool isDigits(const char *&r, const char *p) {
    if (!isdigit(*p++))
        return false;
    while (isdigit(*p))
        p++;
    r = p;
    return true;
}

bool isFloat(const char *p, const char *&r) {
    const char *s = p;
    if (*s == '-')
        ++s;
    if (isDigits(s, s) && *s == '.' && isDigits(s, s + 1)) {
        const char *e = s + 1;
        if (toupper(*s) == 'E' && (*e == '-' || *e == '+') && isDigits(e, e + 1)) {
            r = e;
            return true;
        }
        r = s;
        return true;
    }
    if (strncasecmp(s, "nan", 3) == 0 || strncasecmp(s, "inf", 3) == 0) {
        r = s + 3;
        return true;
    }
    r = p;
    return false;
}

namespace {

// Intel: hexadecimal "<xdigits>H" or octal "<digits>Q" (suffix).  Note pdp11
// emits octal as "<digits>Q" under intel-style, matched here.
bool isIntelNumber(const char *p, const char *&r) {
    const char *s;
    if (isXdigits(s, p) && (toupper(*s) == 'H' || toupper(*s) == 'Q')) {
        r = s + 1;
        return true;
    }
    return false;
}

// Motorola: hexadecimal "$<xdigits>" (at least two hex digits).
bool isMotorolaNumber(const char *p, const char *&r) {
    const char *s;
    if (*p == '$' && isXdigits(s, p + 1) && s - p >= 3) {
        r = s;
        return true;
    }
    return false;
}

// National / C-style: "0x<xdigits>" or an IBM/National quote form
// "[XHOBD]'<xdigits>'" (optional closing quote).
bool isNationalNumber(const char *p, const char *&r) {
    const char *s;
    if (p[0] == '0' && toupper(p[1]) == 'X' && isXdigits(s, p + 2)) {
        r = s;
        return true;
    }
    const auto radix = toupper(p[0]);
    if ((radix == 'X' || radix == 'H' || radix == 'O' || radix == 'B' || radix == 'D') &&
            p[1] == '\'' && isXdigits(s, p + 2)) {
        r = (*s == '\'') ? s + 1 : s;
        return true;
    }
    return false;
}

// Plain decimal "<digits>" (also matches bare octal -- any radix collapses to
// 'n', so the digit run is all the tokenizer needs).
bool isDecimalNumber(const char *p, const char *&r) {
    return isDigits(r, p);
}

}  // namespace

bool isNumber(const char *p, const char *&r) {
    return isNationalNumber(p, r) || isIntelNumber(p, r) || isMotorolaNumber(p, r) ||
           isDecimalNumber(p, r);
}

const char *FloatTokenizer::tokenize(const char *p, std::string &out) const {
    const char *tmp;
    if (!isFloat(p, tmp))
        return nullptr;
    out.append("n.fE+e");
    return tmp;
}

const char *IntelNumberTokenizer::tokenize(const char *p, std::string &out) const {
    const char *tmp;
    if (isIntelNumber(p, tmp) || (*p == '-' && isIntelNumber(p + 1, tmp))) {
        out.push_back('n');
        return tmp;
    }
    return nullptr;
}

const char *MotorolaNumberTokenizer::tokenize(const char *p, std::string &out) const {
    const char *tmp;
    if (isMotorolaNumber(p, tmp) || (*p == '-' && isMotorolaNumber(p + 1, tmp))) {
        out.push_back('n');
        return tmp;
    }
    return nullptr;
}

const char *NationalNumberTokenizer::tokenize(const char *p, std::string &out) const {
    const char *tmp;
    if (isNationalNumber(p, tmp) || (*p == '-' && isNationalNumber(p + 1, tmp))) {
        out.push_back('n');
        return tmp;
    }
    return nullptr;
}

const char *DecimalNumberTokenizer::tokenize(const char *p, std::string &out) const {
    const char *tmp;
    if (isDecimalNumber(p, tmp) || (*p == '-' && isDecimalNumber(p + 1, tmp))) {
        out.push_back('n');
        return tmp;
    }
    return nullptr;
}

const char *RelativeTokenizer::tokenize(const char *p, std::string &out) const {
    if (*p != _loc)
        return nullptr;
    const auto n = *(p + 1);
    const char *tmp;
    if ((n == '+' || n == '-') && isNumber(p + 2, tmp)) {
        out.push_back(_loc);
        out.push_back('+');
        out.push_back('r');
        return tmp;
    }
    // Standalone location symbol (no +/-n offset): the symbol is the whole
    // target only when it is not the start of a wider token -- i.e. the next
    // char is neither alphanumeric nor a '+'/'-' offset sign (those are handled
    // above; a bare sign with no number is not a relative target).
    if (!isalnum(static_cast<unsigned char>(n)) && n != '+' && n != '-') {
        out.push_back(_loc);
        out.push_back('+');
        out.push_back('r');
        return p + 1;
    }
    return nullptr;
}

const FloatTokenizer FLOAT_TOKENIZER;
const IntelNumberTokenizer INTEL_NUMBER_TOKENIZER;
const MotorolaNumberTokenizer MOTOROLA_NUMBER_TOKENIZER;
const NationalNumberTokenizer NATIONAL_NUMBER_TOKENIZER;
const DecimalNumberTokenizer DECIMAL_NUMBER_TOKENIZER;

TokenizerList standardTokenizers(char originSymbol) {
    // One persistent RelativeTokenizer per origin char the disassemblers emit
    // ('$' base default, '*' Motorola, '.' some, '!' CP1600).
    static const RelativeTokenizer relDollar('$');
    static const RelativeTokenizer relStar('*');
    static const RelativeTokenizer relDot('.');
    static const RelativeTokenizer relBang('!');
    const Tokenizer *rel;
    switch (originSymbol) {
    case '*':
        rel = &relStar;
        break;
    case '.':
        rel = &relDot;
        break;
    case '!':
        rel = &relBang;
        break;
    default:  // '$' and any other inherit the base default symbol
        rel = &relDollar;
        break;
    }
    // Number families before the shared decimal matcher (so a suffixed/prefixed
    // hex like "0302H" or "0x1F" is taken whole, not split by decimal).
    return TokenizerList{&FLOAT_TOKENIZER, &NATIONAL_NUMBER_TOKENIZER, &INTEL_NUMBER_TOKENIZER,
            &MOTOROLA_NUMBER_TOKENIZER, &DECIMAL_NUMBER_TOKENIZER, rel};
}

}  // namespace gen
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
