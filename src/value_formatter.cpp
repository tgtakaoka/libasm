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

StrBuffer &ValueFormatter::formatDec(StrBuffer &out, uint32_t val, int8_t bits) const {
    val = makePositive(out, val, bits);
    const auto start = out.mark();
    return outDec(out, val).reverse(start);
}

StrBuffer &ValueFormatter::formatHex(
        StrBuffer &out, uint32_t val, int8_t bits, bool upperHex, bool relax) const {
    val = makePositive(out, val, bits);
    const auto start = out.mark();
    if (relax && val <= 32)
        return outDec(out, val).reverse(start);

    const uint8_t width = abs(bits);
    UppercaseBuffer upper(out);
    LowercaseBuffer lower(out);
    StrBuffer *outHex = upperHex ? upper.ptr() : lower.ptr();
    return _hexFormatter.format(*outHex, val, width).over(out);
}

uint32_t ValueFormatter::makePositive(StrBuffer &out, uint32_t val, int8_t bits) const {
    uint8_t bw = bits;
    if (bits < 0) {
        bw = -bits;
        const auto sign = 1UL << (bw - 1);
        if (val & sign) {
            val = ~val + 1;
            out.letter('-');
        }
    }
    const auto mask = (1UL << bw) - 1;
    return val & mask;
}

StrBuffer &ValueFormatter::outDec(StrBuffer &out, uint32_t val) {
    const auto start = out.mark();
    while (val) {
        const uint8_t digit = val % 10;
        out.letter(digit + '0');
        val /= 10;
    }
    if (out.mark() == start)
        out.letter('0');
    return out;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
