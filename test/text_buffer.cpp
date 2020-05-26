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

static bool isNumber(const char *&p) {
    const char *s;
    if (*p == '$' && isXdigits(s, p + 1) && s - p >= 3) {
        p = s;
        return true;
    } else if (*p == '%' && isBdigits(s, p + 1) && s - p >= 9) {
        p = s;
        return true;
    } else if (isXdigits(s, p) && toupper(*s) == 'H') {
        p = s + 1;
        return true;
    } else if (p[0] == '>' && isXdigits(s, p + 1) && toupper(*s) == 'H') {
        p = s + 1;
        return true;
    } else if ((p[0] == '-' || p[0] == '>')
               && p[1] == '$' && isXdigits(s, p + 2) && s - p >= 3) {
        p = s;
        return true;
    } else if ((*p == '-' || *p == '+') && isDigits(s, p + 1)) {
        p = s;
        return true;
    } else if (p[0] == '0' && toupper(p[1]) == 'X'
               && isXdigits(s, p + 2) && s - p >= 3) {
        p = s;
        return true;
    } else if (isDigits(s, p)) {
        p = s;
        return true;
    }
    return false;
}

static constexpr uint8_t TOKEN_DIGITS = 0x80;
static uint8_t digitsToken(uint8_t length) {
    return TOKEN_DIGITS | length;
}
static bool isDigitsToken(uint8_t token) {
    return (token & TOKEN_DIGITS) == TOKEN_DIGITS;
}
static uint8_t digitsLength(uint8_t token) {
    return token & ~TOKEN_DIGITS;
}

void TextBuffer::toTokens() {
    const char *b = _buffer;
    uint8_t *t = _tokens;
    while (*b) {
        const char *tmp = b;
        if (isNumber(tmp)) {
            *t++ = digitsToken(tmp - b);
            b = tmp;
        } else {
            *t++ = *b++;
        }
    }
    *t = 0;
    _len = b - _buffer;
    _tokenLen = t - _tokens;
    _prefixLen = 0;
    _digitsInPrefix = 0;
    _suffixLen = 0;
    _digitsInSuffix = 0;
}

void TextBuffer::analyze(TextBuffer &a, TextBuffer &b) {
    a.toTokens();
    b.toTokens();
    const uint8_t *ap = a._tokens;
    const uint8_t *bp = b._tokens;
    while (*ap && *bp) {
        if (isDigitsToken(*ap) && isDigitsToken(*bp)) {
            int ad = digitsLength(*ap);
            int bd = digitsLength(*bp);
            a._prefixLen += ad;
            a._digitsInPrefix += ad;
            b._prefixLen += bd;
            b._digitsInPrefix += bd;
        } else if (*ap == *bp) {
            a._prefixLen++;
            b._prefixLen++;
        } else {
            break;
        }
        ap++;
        bp++;
    }
    const uint8_t *as = a._tokens + a._tokenLen;
    const uint8_t *bs = b._tokens + b._tokenLen;
    while (as > ap && bs > bp) {
        as--;
        bs--;
        if (isDigitsToken(*as) && isDigitsToken(*bs)) {
            int ad = digitsLength(*as);
            int bd = digitsLength(*bs);
            a._suffixLen += ad;
            a._digitsInSuffix += ad;
            b._suffixLen += bd;
            b._digitsInSuffix += bd;
        } else if (*as == *bs) {
            a._suffixLen++;
            b._suffixLen++;
        } else {
            break;
        }
    }
}

} // namespace test
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
