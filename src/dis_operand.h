/* -*- mode: c++; -*- */
#ifndef _DIS_OPERAND_H__
#define _DIS_OPERAND_H__

#include <stdio.h>

#include <stdint.h>
#include "type_traits.h"

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

    uint8_t constantWidth(U val, const uint8_t radix) const {
        if (radix == 16) return sizeof(val) * 2;
        if (radix == 2)  return sizeof(val) * 8;
        return (sizeof(val) * 8 + 2) / 3; // radix == 8
    }

    char *outputNumber(char *p, U val, const uint8_t radix) {
        typename make_unsigned<U>::type v = val;
        bool negative = false;
        if (radix == 10 && is_signed<U>::value && val < 0) {
            negative = true;
            v = static_cast<typename make_unsigned<U>::type>(-val);
        }
        char *t = p;
        while (v) {
            *t++ = toDigitChar(uint8_t(v % radix), radix);
            v /= radix;
        }
        if (t == p) *t++ = '0';
        if (radix == 10) {
            if (negative)
                *t++ = '-';
        } else {
            const uint8_t width = constantWidth(v, radix);
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
        if (is_unsigned<U>::value && val < radix && val < 10)
            return DisOperand<U>::outputNumber(p, val, 10);
        if (is_signed<U>::value) {
            auto v = static_cast<typename make_signed<U>::type>(val);
            if (v < radix && v < 10 && v > -(int8_t)radix && v > -10) {
                return DisOperand<U>::reverseStr(
                    p,  DisOperand<U>::outputNumber(p, v, 10));
            }
        }
        if (radix == 16) *p++ = '$';
        else if (radix == 8) *p++ = '@';
        else if (radix == 2) *p++ = '%';
        char *t = DisOperand<U>::outputNumber(
            p, static_cast<typename make_unsigned<U>::type>(val), radix);
        return DisOperand<U>::reverseStr(p, t);
    }
};

template<typename U>
class DisIntelOperand : public DisOperand<U> {
public:
    char *outputConstant(char *p, U val, uint8_t radix) override {
        if (is_unsigned<U>::value && val < radix && val < 10)
            return DisOperand<U>::outputNumber(p, val, 10);
        if (is_signed<U>::value) {
            auto v = static_cast<typename make_signed<U>::type>(val);
            if (v < radix && v < 10 && v > -(int8_t)radix && v > -10)
                return DisOperand<U>::reverseStr(
                    p,  DisOperand<U>::outputNumber(p, v, 10));
        }
        char *t = DisOperand<U>::outputNumber(
            p, static_cast<typename make_unsigned<U>::type>(val), radix);
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
