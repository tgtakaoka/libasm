/* -*- mode: c++; -*- */
#ifndef __ASM_OPERAND_H__
#define __ASM_OPERAND_H__

#include <ctype.h>
#include <string.h>
#include "make_signed.h"

template<typename U>
class AsmOperand {
public:
    virtual const char *parseConstant(const char *p, U &val) = 0;

protected:
    static bool isValidDigit(const char c, const uint8_t radix) {
        if (radix == 16) return isxdigit(c);
        return c >= '0' && c < '0' + radix;
    }

    static uint8_t toNumber(const char c, const uint8_t radix) {
        if (radix == 16 && !isdigit(c))
            return tolower(c) - 'a' + 10;
        return c - '0';
    }

    static const char *parseNumber(
        const char *p, U &val, const uint8_t radix) {
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
};

template<typename U>
class AsmMotoOperand : public AsmOperand<U> {
public:
    const char *parseConstant(const char *p, U &val) override {
        switch (*p) {
        case '~':
            p = parseConstant(p + 1, val);
            val = ~val;
            return p;
        case '+':
            p++;
            break;
        case '-':
            p = parseConstant(p + 1, val);
            val = -static_cast<typename make_signed<U>::type>(val);
            return p;
        }
        if (isdigit(*p)) {
            p = AsmOperand<U>::parseNumber(p, val, 10);
        } else if (*p == '$') {
            p = AsmOperand<U>::parseNumber(p + 1, val, 16);
        } else if (*p == '@') {
            p = AsmOperand<U>::parseNumber(p + 1, val, 8);
        } else if (*p == '%') {
            p = AsmOperand<U>::parseNumber(p + 1, val, 2);
        } else {
            p = nullptr;
        }
        return p;
    }
};

template<typename U>
class AsmIntelOperand : public AsmOperand<U> {
public:
    const char *parseConstant(const char *scan, U &val) override {
        const char *p;
        switch (*scan) {
        case '~':
            p = parseConstant(scan + 1, val);
            val = ~val;
            return p;
        case '+':
            scan++;
            break;
        case '-':
            p = parseConstant(scan + 1, val);
            val = -static_cast<typename make_signed<U>::type>(val);
            return p;
        }
        if (!isdigit(*scan))
            return nullptr;
        if ((p = AsmOperand<U>::parseNumber(scan, val, 16))
            && tolower(*p) == 'h') {
            p++;
        } else if ((p = AsmOperand<U>::parseNumber(scan, val, 10))
                   && !(*p && strchr("OoBb", *p))) {
            ;
        } else if ((p = AsmOperand<U>::parseNumber(scan, val, 8))
                   && tolower(*p) == 'o') {
            p++;
        } else if ((p = AsmOperand<U>::parseNumber(scan, val, 2))
                   && tolower(*p) == 'b') {
            p++;
        } else {
            p = nullptr;
        }
        return p;
    }
};

#endif
