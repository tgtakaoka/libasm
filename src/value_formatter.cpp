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

static char *reverseStr(char *p, char *t) {
    for (char *e = t - 1; p < e;) {
        char c = *p;
        *p++ = *e;
        *e-- = c;
    }
    return t;
}

char *ValueFormatter::outHex(char *out, uint32_t val, int8_t bits) const {
    const char hexBase = (_uppercase ? 'A' : 'a') - 10;
    char *t = out;
    while (val) {
        const uint8_t digit = val & 0xF;
        if (digit < 10)
            *t++ = digit + '0';
        else
            *t++ = digit + hexBase;
        val >>= 4;
    }
    const uint8_t bw = (bits < 0) ? -bits : bits;
    const int8_t width = bw / 4 + (bw % 4 ? 1 : 0);
    while (t - out < width)
        *t++ = '0';
    *t = 0;
    return t;
}

char *ValueFormatter::outDec(char *out, uint32_t val) const {
    char *t = out;
    while (val) {
        const uint8_t digit = val % 10;
        *t++ = digit + '0';
        val /= 10;
    }
    if (t == out)
        *t++ = '0';
    *t = 0;
    return t;
}

uint32_t ValueFormatter::makePositive(
        char *out, uint32_t val, int8_t bits) const {
    *out = 0;
    uint8_t bw = bits;
    if (bits < 0) {
        bw = -bits;
        if (val & (1 << (bw - 1))) {
            val = ~val + 1;
            *out = '-';
        }
    }
    if (bw >= 32)
        return val;
    return val & ~(0xFFFFFFFF << bw);
}

char *ValueFormatter::formatHex(
        char *out, uint32_t val, int8_t bits, bool relax) const {
    val = makePositive(out, val, bits);
    if (*out)
        out++;
    if (relax && val <= 32)
        return reverseStr(out, outDec(out, val));
    if (_cstyle)
        return ValueFormatter::formatPositiveHex(out, val, bits);
    return formatPositiveHex(out, val, bits);
}

char *ValueFormatter::formatDec(char *out, uint32_t val, int8_t bits) const {
    val = makePositive(out, val, bits);
    if (*out)
        out++;
    return reverseStr(out, outDec(out, val));
}

char *ValueFormatter::formatPositiveHex(
        char *out, uint32_t val, int8_t bits) const {
    *out++ = '0';
    *out++ = 'x';
    return reverseStr(out, outHex(out, val, bits));
}

char *MotoValueFormatter::formatPositiveHex(
        char *out, uint32_t val, int8_t bits) const {
    *out++ = '$';
    return reverseStr(out, outHex(out, val, bits));
}

char *IntelValueFormatter::formatPositiveHex(
        char *out, uint32_t val, int8_t bits) const {
    char *t = outHex(out, val, bits);
    if (t[-1] > '9')
        *t++ = '0';
    t = reverseStr(out, t);
    *t++ = _uppercase ? 'H' : 'h';
    *t = 0;
    return t;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
