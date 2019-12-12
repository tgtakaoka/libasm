/* -*- mode: c++; -*- */
#ifndef _DIS_OPERAND_H__
#define _DIS_OPERAND_H__

#include <stdint.h>

static char toDigitChar(uint8_t val, uint8_t radix) {
    if (radix == 16 && val >= 10)
        return val - 10 + 'A';
    return val + '0';
}

static char *reverseStr(char *p, char *t) {
    for (char *e = t - 1; p < e; ) {
        char c = *p;
        *p++ = *e;
        *e-- = c;
    }
    return t;
}

template<typename U>
char *outputConst(char *p, U val, const uint8_t radix) {
    char *t = p;
    while (val) {
        *t++ = toDigitChar(uint8_t(val % radix), radix);
        val /= radix;
    }
    if (t == p) *t++ = '0';
    if (radix != 10) {
        constexpr size_t bytes = sizeof(val);
        const uint8_t width = (radix == 16) ? bytes * 2
            : (radix == 2 ? bytes * 8 : (bytes * 8 + 2) / 3);
        while (t - p < width)
            *t++ = '0';
    }
    *t = 0;
    return t;
}

template<typename U>
char *outMotoConst(char *p, U val, const uint16_t radix) {
    if (val < radix && val < 10)
        return outputConst(p, val, 10);
    if (radix == 16) *p++ = '$';
    else if (radix == 8) *p++ = '@';
    else if (radix == 2) *p++ = '%';
    return reverseStr(p, outputConst(p, val, radix));
}

template<typename U>
char *outIntelConst(char *p, U val, const uint16_t radix) {
    if (val < radix && val < 10)
        return outputConst(p, val, 10);
    char *t = outputConst(p, val, radix);
    if (radix == 16 && t[-1] > '9')
        *t++ = '0';
    t = reverseStr(p, t);
    if (radix == 16) *t++ = 'H';
    if (radix == 8) *t++ = 'O';
    if (radix == 2) *t++ = 'B';
    *t = 0;
    return t;
}

#endif
