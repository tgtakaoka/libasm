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

#include "value_formatter.h"

namespace libasm {

static int8_t constantWidth(uint8_t bitWidth, uint8_t base) {
    if (base == 16) return bitWidth / 4 + (bitWidth % 4 ? 1 : 0);
    if (base == 2)  return bitWidth;
    return bitWidth / 3 + (bitWidth % 3 ? 1 : 0);
}

static char *reverseStr(char *p, char *t) {
    for (char *e = t - 1; p < e; ) {
        char c = *p;
        *p++ = *e;
        *e-- = c;
    }
    return t;
}

char *ValueFormatter::outputNumber(
    char *p, uint32_t val, uint8_t base, int8_t bitWidth) const {
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
    const int8_t textWidth = (bitWidth < 0 || base == 10) ? 0:
        constantWidth(bitWidth, base);
    while (t - p < textWidth)
        *t++ = '0';
    *t = 0;
    return t;
}

static int32_t makeSigned(uint32_t val, int8_t bitWidth) {
    const int8_t bw = (bitWidth < 0) ? -bitWidth : bitWidth;
    if (bw == 8) return static_cast<int8_t>(val);
    if (bw == 16) return static_cast<int16_t>(val);
    if (bw == 24) return static_cast<int32_t>(
        (val & 0x00800000L) ? 0xFF000000L | val : 0x00FFFFFFL & val);
     return static_cast<int32_t>(val);
}

static uint32_t makeUnsigned(uint32_t val, int8_t bitWidth) {
    const int8_t bw = (bitWidth < 0) ? -bitWidth : bitWidth;
    if (bw == 8) return static_cast<uint8_t>(val);
    if (bw == 16) return static_cast<uint16_t>(val);
    if (bw == 24) return static_cast<uint32_t>(val & 0x00FFFFFFL);
    return val;
}

char *ValueFormatter::outputRelaxed(
    char *p, uint32_t val, int8_t radix, int8_t bitWidth) const {
    if (bitWidth >= 0) bitWidth = -bitWidth; // forcibly suppress leading zero.
    if (radix >= 0) {
        auto uval = makeUnsigned(val, bitWidth);
        if (uval <= 32)
            return reverseStr(p, outputNumber(p, uval, 10, bitWidth));
    } else {
        auto sval = makeSigned(val, bitWidth);
        if (sval < 0 && sval >= -32) {
            *p++ = '-';
            return reverseStr(p, outputNumber(p, -sval, 10, bitWidth));
        }
        if (sval >= 0 && sval <= 32)
            return reverseStr(p, outputNumber(p, sval, 10, bitWidth));
    }
    return nullptr;
}

char *positiveValue(char *p, uint32_t &val, int8_t radix, int8_t bitWidth) {
    const auto sval = makeSigned(val, bitWidth);
    if (radix < 0 && sval < 0) {
        *p++ = '-';
        val = makeUnsigned(-sval, bitWidth);
    } else {
        val = makeUnsigned(val, bitWidth);
    }
    return p;
}

char *ValueFormatter::output(
    char *p, uint32_t val, int8_t radix, bool relax, int8_t bitWidth) const {
    char *t;
    if (relax && (t = outputRelaxed(p, val, radix, bitWidth)))
        return t;
    p = positiveValue(p, val, radix, bitWidth);
    const uint8_t base = (radix < 0) ? -radix : radix;
    if (base == 16) {
        *p++ = '0';
        *p++ = 'x';
    } else if (base == 8) {
        *p++ = '0';
    } else if (base == 2) {
        *p++ = '0';
        *p++ = 'b';
    }
    t = outputNumber(p, val, base, bitWidth);
    return reverseStr(p, t);
}

char *MotoValueFormatter::output(
    char *p, uint32_t val, int8_t radix, bool relax, int8_t bitWidth) const {
    char *t;
    if (relax && (t = outputRelaxed(p, val, radix, bitWidth)))
        return t;
    p = positiveValue(p, val, radix, bitWidth);
    const uint8_t base = (radix < 0) ? -radix : radix;
    if (base == 16) *p++ = '$';
    if (base == 8)  *p++ = '@';
    if (base == 2)  *p++ = '%';
    t = ValueFormatter::outputNumber(p, val, base, bitWidth);
    return reverseStr(p, t);
}

char *IntelValueFormatter::output(
    char *p, uint32_t val, int8_t radix, bool relax, int8_t bitWidth) const {
    char *t;
    if (relax && (t = outputRelaxed(p, val, radix, bitWidth)))
        return t;
    p = positiveValue(p, val, radix, bitWidth);
    const uint8_t base = (radix < 0) ? -radix : radix;
    t = ValueFormatter::outputNumber(p, val, base, bitWidth);
    if (base == 16 && t[-1] > '9')
        *t++ = '0';
    t = reverseStr(p, t);
    char suffix = 0;
    if (base == 16) suffix = 'H';
    if (base == 8)  suffix = 'O';
    if (base == 2)  suffix = 'B';
    if (suffix) *t++ = _uppercase ? suffix : (suffix | 0x20);
    *t = 0;
    return t;
}

} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
