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

#include <stdlib.h>

namespace libasm {

StrBuffer &ValueFormatter::outHex(StrBuffer &out, uint32_t val, int8_t bits) const {
    char *start = out.mark();
    while (val) {
        const uint8_t digit = val & 0xF;
        if (digit < 10)
            out.letter(digit + '0');
        else
            out.letter(digit - 10 + 'A', _uppercase);
        val >>= 4;
    }
    const uint8_t bw = abs(bits);
    const int8_t width = bw / 4 + (bw % 4 ? 1 : 0);
    while (out.mark() - start < width && out.isOK())
        out.letter('0');
    return out;
}

StrBuffer &ValueFormatter::outDec(StrBuffer &out, uint32_t val) const {
    char *start = out.mark();
    while (val) {
        const uint8_t digit = val % 10;
        out.letter(digit + '0');
        val /= 10;
    }
    if (out.mark() == start)
        out.letter('0');
    return out;
}

uint32_t ValueFormatter::makePositive(StrBuffer &out, uint32_t val, int8_t bits) const {
    uint8_t bw = bits;
    if (bits < 0) {
        bw = -bits;
        if (val & (1UL << (bw - 1))) {
            val = ~val + 1;
            out.letter('-');
        }
    }
    return val & ((1UL << bw) - 1);
}

StrBuffer &ValueFormatter::formatHex(StrBuffer &out, uint32_t val, int8_t bits, bool relax) const {
    val = makePositive(out, val, bits);
    char *start = out.mark();
    if (relax && val <= 32)
        return outDec(out, val).reverse(start);
    return formatPositiveHex(out, val, bits);
}

StrBuffer &ValueFormatter::formatDec(StrBuffer &out, uint32_t val, int8_t bits) const {
    val = makePositive(out, val, bits);
    char *start = out.mark();
    return outDec(out, val).reverse(start);
}

StrBuffer &ValueFormatter::formatPositiveHex(StrBuffer &out, uint32_t val, int8_t bits) const {
    if (_cstyle)
        out.letter('0').letter('x');
    char *start = out.mark();
    return outHex(out, val, bits).reverse(start);
}

StrBuffer &MotorolaValueFormatter::formatPositiveHex(StrBuffer &out, uint32_t val, int8_t bits) const {
    if (_cstyle) {
        out.letter('0').letter('x');
    } else {
        out.letter('$');
    }
    char *start = out.mark();
    return outHex(out, val, bits).reverse(start);
}

StrBuffer &IntelValueFormatter::formatPositiveHex(StrBuffer &out, uint32_t val, int8_t bits) const {
    if (_cstyle)
        out.letter('0').letter('x');
    char *start = out.mark();
    char *top = outHex(out, val, bits).mark();
    if (_cstyle)
        return out.reverse(start);
    if (top[-1] > '9')
        out.letter('0');
    return out.reverse(start).letter('H', _uppercase);
}

StrBuffer &NationalValueFormatter::formatPositiveHex(StrBuffer &out, uint32_t val, int8_t bits) const {
    if (_cstyle) {
        out.letter('0').letter('x');
    } else {
        out.letter('X', _uppercase).letter('\'');
    }
    char *start = out.mark();
    outHex(out, val, bits).reverse(start);
    if (!_cstyle && _suffix)
        out.letter('\'');
    return out;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
