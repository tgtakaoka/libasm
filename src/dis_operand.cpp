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

#include "dis_operand.h"

namespace libasm {

static int8_t constantWidth(uint8_t bitWidth, int8_t base) {
    if (base < 0) return 0;     // zero suppress
    if (base == 16) return bitWidth / 4;
    if (base == 2)  return bitWidth;
    return (bitWidth + 2) / 3;  // base == 8
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
    char *p, uint32_t val, int8_t radix, int8_t bitWidth) const {
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
        const int8_t textWidth = constantWidth(bitWidth, radix);
        while (t - p < textWidth)
            *t++ = '0';
    }
    *t = 0;
    return t;
}

char *DisOperand::outputRelaxed(
    char *p, uint32_t val, int8_t radix, uint8_t bitWidth) const {
    if (radix > 0 && val < static_cast<uint8_t>(radix))
        return reverseStr(p, outputNumber(p, val, 10, bitWidth));
    if (radix < 0) {
        int32_t v = static_cast<int32_t>(val);
        if (v < -radix && v < 10 && v > radix)
            return reverseStr(p, outputNumber(p, v, -10, bitWidth));
    }
    return nullptr;
 }

char *DisMotoOperand::output(
    char *p, uint32_t val, int8_t radix, bool relax, uint8_t bitWidth) const {
    char *t;
    if (relax && (t = outputRelaxed(p, val, radix, bitWidth)))
        return t;
    if (radix == 16) *p++ = '$';
    else if (radix == 8) *p++ = '@';
    else if (radix == 2) *p++ = '%';
    t = DisOperand::outputNumber(p, val, radix, bitWidth);
    return reverseStr(p, t);
}

char *DisIntelOperand::output(
    char *p, uint32_t val, int8_t radix, bool relax, uint8_t bitWidth) const {
    char *t;
    if (relax && (t = outputRelaxed(p, val, radix, bitWidth)))
        return t;
    t = DisOperand::outputNumber(p, val, radix, bitWidth);
    if (radix == 16 && t[-1] > '9')
        *t++ = '0';
    t = reverseStr(p, t);
    if (radix == 16) *t++ = _uppercase ? 'H' : 'h';
    if (radix == 8) *t++ = _uppercase ? 'O' : 'o';
    if (radix == 2) *t++ = _uppercase ? 'B' : 'b';
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
