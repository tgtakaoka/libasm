#include "dis_operand.h"

static uint8_t constantWidth(uint8_t size, uint8_t base) {
    if (base == 16) return size * 2;
    if (base == 2)  return size * 8;
    return (size * 8 + 2) / 3;  // base == 8
}

static char *reverseStr(char *p, char *t) {
    for (char *e = t - 1; p < e; ) {
        char c = *p;
        *p++ = *e;
        *e-- = c;
    }
    return t;
}

char *DisOperand::outputNumber(
    char *p, uint32_t val, int8_t radix, uint8_t size) const {
    bool negative = false;
    if (radix == -10 && static_cast<int32_t>(val) < 0) {
        negative = true;
        val = -static_cast<int32_t>(val);
    }
    const uint8_t base = (radix < 0) ? -radix : radix;
    const char hexBase = _uppercase ? 'A' : 'a';
    char *t = p;
    while (val) {
        const uint8_t digit = static_cast<uint8_t>(val % base);
        if (base == 16 && digit >= 10) {
            *t++ = digit - 10 + hexBase;
        } else {
            *t++ = digit + '0';
        }
        val /= base;
    }
    if (t == p) *t++ = '0';
    if (base == 10) {
        if (negative)
            *t++ = '-';
    } else {
        const uint8_t width = constantWidth(size, radix);
        while (t - p < width)
            *t++ = '0';
    }
    *t = 0;
    return t;
}

char *DisOperand::outputRelaxed(
    char *p, uint32_t val, int8_t radix, uint8_t size) const {
    if (radix > 0 && val < static_cast<uint8_t>(radix) && val < 10)
        return reverseStr(p, outputNumber(p, val, 10, size));
    if (radix < 0) {
        int32_t v = static_cast<int32_t>(val);
        if (v < -radix && v < 10 && v > radix && v > -10)
            return reverseStr(p, outputNumber(p, v, -10, size));
    }
    return nullptr;
 }

char *DisMotoOperand::output(
    char *p, uint32_t val, int8_t radix, bool relax, uint8_t size) const {
    char *t;
    if (relax && (t = outputRelaxed(p, val, radix, size)))
        return t;
    if (radix == 16) *p++ = '$';
    else if (radix == 8) *p++ = '@';
    else if (radix == 2) *p++ = '%';
    t = DisOperand::outputNumber(p, val, radix, size);
    return reverseStr(p, t);
}

char *DisIntelOperand::output(
    char *p, uint32_t val, int8_t radix, bool relax, uint8_t size) const {
    char *t;
    if (relax && (t = outputRelaxed(p, val, radix, size)))
        return t;
    t = DisOperand::outputNumber(p, val, radix, size);
    if (radix == 16 && t[-1] > '9')
        *t++ = '0';
    t = reverseStr(p, t);
    if (radix == 16) *t++ = _uppercase ? 'H' : 'h';
    if (radix == 8) *t++ = _uppercase ? 'O' : 'o';
    if (radix == 2) *t++ = _uppercase ? 'B' : 'b';
    *t = 0;
    return t;
}
