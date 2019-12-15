/* -*- mode: c++; -*- */
#ifndef _DIS_OPERAND_H__
#define _DIS_OPERAND_H__

#include <stdint.h>

template<typename U>
class DisOperand {
public:
    virtual char *outputConstant(char *p, U val, uint8_t radix) = 0;

protected:
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

    char *outputNumber(char *p, U val, const uint8_t radix) {
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
};

template<typename U>
class DisMotoOperand : public DisOperand<U> {
public:
    char *outputConstant(char *p, U val, uint8_t radix) override {
        if (val < radix && val < 10)
            return DisOperand<U>::outputNumber(p, val, 10);
        if (radix == 16) *p++ = '$';
        else if (radix == 8) *p++ = '@';
        else if (radix == 2) *p++ = '%';
        char *t = DisOperand<U>::outputNumber(p, val, radix);
        return DisOperand<U>::reverseStr(p, t);
    }
};

template<typename U>
class DisIntelOperand : public DisOperand<U> {
public:
    char *outputConstant(char *p, U val, uint8_t radix) override {
        if (val < radix && val < 10)
            return DisOperand<U>::outputNumber(p, val, 10);
        char *t = DisOperand<U>::outputNumber(p, val, radix);
        if (radix == 16 && t[-1] > '9')
            *t++ = '0';
        t = DisOperand<U>::reverseStr(p, t);
        if (radix == 16) *t++ = 'H';
        if (radix == 8) *t++ = 'O';
        if (radix == 2) *t++ = 'B';
        *t = 0;
        return t;
    }
};

#endif
