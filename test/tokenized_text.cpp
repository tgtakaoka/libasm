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

#include "tokenized_text.h"

#include <cctype>
#include <cstring>

namespace libasm {
namespace gen {

static inline bool isXdigits(const char *&r, const char *p) {
    if (!isxdigit(*p++))
        return false;
    while (isxdigit(*p))
        p++;
    r = p;
    return true;
}

static inline bool isDigits(const char *&r, const char *p) {
    if (!isdigit(*p++))
        return false;
    while (isdigit(*p))
        p++;
    r = p;
    return true;
}

static bool isNumber(const char *p, const char *&r) {
    const char *s;
    if (p[0] == '0' && toupper(p[1]) == 'X' && isXdigits(s, p + 2)) {
        r = s;
        return true;
    }
    if (isXdigits(s, p) && toupper(*s) == 'H') {
        r = s + 1;
        return true;
    }
    if (isDigits(s, p)) {
        r = s;
        return true;
    }
    if (*p == '$' && isXdigits(s, p + 1) && s - p >= 3) {
        r = s;
        return true;
    }
    r = p;
    return false;
}

static bool isNs32kSize(char c) {
    c = toupper(c);
    return c == 'B' || c == 'W' || c == 'D' || c == 'Q';
}

TokenizedText::TokenizedText(const char *text) : _tokens(tokenize(text)), _count(0) {}

std::string TokenizedText::tokenize(const char *text) {
    std::string t;
    const char *b = text;
    while (*b) {
        const char *tmp;
        if (isNumber(b, tmp)) {
            t.push_back('n');
            b = tmp;
        } else if (b[0] == '-' && isNumber(b + 1, tmp) && (*tmp == '(' || *tmp == ')')) {
            // reduce displacement variants of NS32000; -n(...) and n(...), (-n) and (n)
            t.push_back('n');
            b = tmp;
        } else if (b[0] == '+' && b[1] == '(' && b[2] == '-' && isNumber(b + 3, tmp) &&
                   *tmp == ')') {
            // reduce displacement variants of NS32000; +(-n) and +n
            t.push_back('+');
            t.push_back('n');
            b = tmp + 1;
        } else if (b[0] == ':' && isNs32kSize(b[1]) && b[2] == ']') {
            // reduce index size variants of NS32000; [Rn:B], [Rn:W], [Rn:D] and [Rn:Q]
            t.push_back(':');
            t.push_back('s');
            t.push_back(']');
            b += 3;
        } else if (*b == '/') {
            // reduce MOVEM variants of MC68000; Rn/Rn and Rn-Rn
            ++b;
            t.push_back('-');
        } else {
            t.push_back(*b++);
        }
    }
    return t;
}

std::size_t TokenizedText::hash() const {
    std::size_t seed = _tokens.size();
    for (auto t : _tokens) {
        seed ^= t + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

}  // namespace gen
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
