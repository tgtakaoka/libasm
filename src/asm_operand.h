/* -*- mode: c++; -*- */
#ifndef __ASM_OPERAND_H__
#define __ASM_OPERAND_H__

#include <ctype.h>

static bool isValidDigit(const char c, const uint8_t radix) {
    if (radix == 16) return isxdigit(c);
    return c >= '0' && c < '0' + radix;
}

static uint8_t toNumber(const char c, const uint8_t radix) {
    if (radix == 16 && !isdigit(c))
        return tolower(c) - 'a' + 10;
    return c - '0';
}

template<typename U>
const char *parseConst(const char *p, U &val, const uint8_t radix) {
    if (!isValidDigit(*p, radix))
        return nullptr;
    U v = 0;
    while (isValidDigit(*p, radix)) {
        v *= radix;
        v += toNumber(*p, radix);
        p++;
    }
    val = v;
    return p;
}

template<typename U, typename S>
const char *parseMotoConst(const char *p, U &val) {
    const char sign = (*p == '+' || *p == '-') ? *p++ : 0;
    if (isdigit(*p)) {
        p = parseConst(p, val, 10);
    } else if (*p == '$') {
        p = parseConst(p + 1, val, 16);
    } else if (*p == '@') {
        p = parseConst(p + 1, val, 8);
    } else if (*p == '%') {
        p = parseConst(p + 1, val, 2);
    } else {
        p = nullptr;
    }
    if (sign == '-') val = -(S)val;
    return p;
}

template<typename U, typename S>
const char *parseIntelConst(const char *scan, U &val) {
    const char sign = (*scan == '+' || *scan == '-') ? *scan++ : 0;
    if (!isdigit(*scan))
        return nullptr;
    const char *p;
    if ((p = parseConst<U>(scan, val, 16)) && tolower(*p) == 'h') {
        p++;
    } else if ((p = parseConst<U>(scan, val, 10)) && !(*p && strchr("OoBb", *p))) {
        ;
    } else if ((p = parseConst<U>(scan, val, 8)) && tolower(*p) == 'o') {
        p++;
    } else if ((p = parseConst<U>(scan, val, 2)) && tolower(*p) == 'b') {
        p++;
    } else {
        p = nullptr;
    }
    if (sign == '-') val = -(S)val;
    return p;
}

#endif
