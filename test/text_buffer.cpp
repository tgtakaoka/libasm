#include "text_buffer.h"

#include <ctype.h>
#include <string.h>

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
    } else if ((p[0] == '-' || p[0] == '>')
               && p[1] == '$' && isXdigits(s, p + 2) && s - p >= 3) {
        p = s;
        return true;
    } else if ((*p == '-' || *p == '+') && isDigits(s, p + 1)) {
        p = s;
        return true;
    } else if (isDigits(s, p)) {
        p = s;
        return true;
    }
    return false;
}

#define TOKEN_DIGITS(n) (0x80 | (n))
#define IS_DIGITS(t) (((t) & 0x80) == 0x80)
#define DIGITS_LEN(t) ((t) & ~0x80)

void TextBuffer::toTokens() {
    const char *b = _buffer;
    uint8_t *t = _tokens;
    while (*b) {
        const char *tmp = b;
        if (isNumber(tmp)) {
            *t++ = TOKEN_DIGITS(tmp - b);
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
        if (IS_DIGITS(*ap) && IS_DIGITS(*bp)) {
            int ad = DIGITS_LEN(*ap);
            int bd = DIGITS_LEN(*bp);
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
        if (IS_DIGITS(*as) && IS_DIGITS(*bs)) {
            int ad = DIGITS_LEN(*as);
            int bd = DIGITS_LEN(*bs);
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

