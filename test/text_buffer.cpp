/*
 * Copyright 2020 Tadashi G. Takaoka
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

#include "text_buffer.h"

#include <ctype.h>
#include <string.h>

namespace libasm {
namespace test {

static bool isBdigits(const char *&r, const char *p) {
    if (*p != '0' && *p != '1') return false;
    while (*p == '0' || *p == '1')
        p++;
    r = p;
    return true;
}

static bool isXdigits(const char *&r, const char *p) {
    if (!isxdigit(*p)) return false;
    while (isxdigit(*p))
        p++;
    r = p;
    return true;
}

static bool isDigits(const char *&r, const char *p) {
    if (!isdigit(*p)) return false;
    while (isdigit(*p))
        p++;
    r = p;
    return true;
}

static bool isNumber(const char *p, const char *&r) {
    const char *s;
    if (*p == '$' && isXdigits(s, p + 1) && s - p >= 3) {
        r = s;
        return true;
    } else if (*p == '%' && isBdigits(s, p + 1) && s - p >= 9) {
        r = s;
        return true;
    } else if (isXdigits(s, p) && toupper(*s) == 'H') {
        r = s + 1;
        return true;
    } else if (p[0] == '0' && toupper(p[1]) == 'X'
               && isXdigits(s, p + 2) && s - p >= 3) {
        r = s;
        return true;
    } else if (isDigits(s, p)) {
        r = s;
        return true;
    }
    r = p;
    return false;
}

static constexpr uint8_t TOKEN_DIGITS = 0x80;

TokenizedText::TokenizedText(const char *text)
    : _tokens(tokenize(text)),
      _count(0)
{}

std::vector<uint8_t> TokenizedText::tokenize(const char *text) {
    std::vector<uint8_t> t;
    const char *b = text;
    while (*b) {
        const char *tmp;
        if (isNumber(b, tmp)) {
            t.push_back(TOKEN_DIGITS);
            b = tmp;
        } else {
            t.push_back(*b++);
        }
    }
    return t;
}

std::size_t TokenizedText::hash::operator()(
    const TokenizedText &it) const {
    std::size_t seed = it.length();
    for (auto t : it._tokens) {
        seed ^= t + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

bool TokenizedText::eq::operator()(
    const TokenizedText &a, const TokenizedText &b) const {
    if (a.length() != b.length()) return false;
    for (std::size_t i = 0; i < a.length(); i++) {
        if (a._tokens.at(i) != b._tokens.at(i)) return false;
    }
    return true;
}

} // namespace test
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
