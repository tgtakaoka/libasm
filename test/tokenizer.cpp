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
    if (isDigits(s, s)) {  // integer part
        bool isFloatNum = false;
        if (*s == '.') {  // optional fractional part ".ddd"
            const char *t = s + 1;
            if (isDigits(t, t)) {
                s = t;
                isFloatNum = true;
            }
        }
        if (toupper(*s) == 'E') {  // optional exponent "E[+/-]ddd" (sign optional)
            const char *t = s + 1;
            if (*t == '+' || *t == '-')
                ++t;
            if (isDigits(t, t)) {
                s = t;
                isFloatNum = true;
            }
        }
        // A plain integer (no '.' and no 'E') is not a float.  The match must
        // also end at a boundary: if an alphanumeric follows, this is a hex/octal
        // literal that merely contains 'E' (Intel "0000E0H", "0E34H"), not a
        // float -- leave it for the number tokenizer.
        if (isFloatNum && !isalnum(static_cast<unsigned char>(*s))) {
            r = s;
            return true;
        }
    }
    if (strncasecmp(s, "nan", 3) == 0 || strncasecmp(s, "inf", 3) == 0) {
        r = s + 3;
        return true;
    }
    r = p;
    return false;
}

// Intel: hexadecimal "<xdigits>H" or octal "<digits>Q" (suffix).  Note pdp11
// emits octal as "<digits>Q" under intel-style, matched here.
bool IntelNumber::match(const char *p, const char *&r) {
    const char *s;
    if (isXdigits(s, p) && (toupper(*s) == 'H' || toupper(*s) == 'Q')) {
        r = s + 1;
        return true;
    }
    return false;
}

// Motorola: hexadecimal "$<xdigits>" (at least two hex digits).
bool MotorolaNumber::match(const char *p, const char *&r) {
    const char *s;
    if (*p == '$' && isXdigits(s, p + 1) && s - p >= 3) {
        r = s;
        return true;
    }
    return false;
}

// National / IBM quote "[XHOBD]'<xdigits>'" (optional closing quote); also the
// C-style "0x<xdigits>" some National-style disassemblers emit.
bool NationalNumber::match(const char *p, const char *&r) {
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

// C-style prefixed number: "0x<xdigits>" hex or "0b<bindigits>" binary.  Bare
// decimal is handled separately by DecimalNumber (matched after the hex policy).
bool CstyleNumber::match(const char *p, const char *&r) {
    const char *s;
    if (p[0] == '0' && toupper(p[1]) == 'X' && isXdigits(s, p + 2)) {
        r = s;
        return true;
    }
    if (p[0] == '0' && toupper(p[1]) == 'B' && isDigits(s, p + 2)) {
        r = s;
        return true;
    }
    return false;
}

const char *FloatTokenizer::tokenize(const char *p, std::string &out) const {
    const char *tmp;
    if (!isFloat(p, tmp))
        return nullptr;
    out.append("n.fE+e");
    return tmp;
}

RegisterTokenizer::RegisterTokenizer(
        const char *prefix, int maxReg, const char *classToken, const char *suffix)
    : _prefix(prefix),
      _plen(std::strlen(prefix)),
      _maxReg(maxReg),
      _class(classToken),
      _suffix(suffix) {}

const char *RegisterTokenizer::tokenize(const char *p, std::string &out) const {
    // The register must start at a word boundary so "R0" matches but the "R0" in
    // a wider identifier ("MR0") does not.
    if (!out.empty() && isalnum(static_cast<unsigned char>(out.back())))
        return nullptr;
    for (std::size_t i = 0; i < _plen; i++) {
        if (toupper(p[i]) != toupper(_prefix[i]))
            return nullptr;
    }
    const char *q = p + _plen;
    if (!isdigit(*q))
        return nullptr;
    int n = 0;
    for (; isdigit(*q); q++)
        n = n * 10 + (*q - '0');
    if (n > _maxReg)
        return nullptr;
    std::string token(_class);
    // An optional byte/half suffix (e.g. "R0H"/"R0L", or lowercase "r0h") is kept
    // in the token, matched case-insensitively and normalized to upper case.
    if (_suffix && *q && std::strchr(_suffix, toupper(*q))) {
        token.push_back(toupper(*q));
        q++;
    }
    // ... and end at a boundary so "R1L"/"R1X" (other families) are not eaten.
    if (isalnum(static_cast<unsigned char>(*q)))
        return nullptr;
    out.append(token);
    return q;
}

const FloatTokenizer FLOAT_TOKENIZER;

}  // namespace gen
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
