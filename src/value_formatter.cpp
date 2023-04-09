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

#include "config_base.h"

#include <stdlib.h>

namespace libasm {

const /*PROGMEM*/ char HexFormatter::ZERO_X[] PROGMEM = "0x";
const /*PROGMEM*/ char HexFormatter::DOLLAR[] PROGMEM = "$";
const /*PROGMEM*/ char HexFormatter::X_DASH[] PROGMEM = "x'";
const /*PROGMEM*/ char HexFormatter::H_DASH[] PROGMEM = "h'";

const HexFormatter ValueFormatter::_DefaultHex;
const CStyleHexFormatter ValueFormatter::_CStyleHex;

StrBuffer &ValueFormatter::formatDec(StrBuffer &out, uint32_t val, int8_t bits) const {
    val = makePositive(out, val, bits);
    const auto start = out.mark();
    return outDec(out, val).reverse(start);
}

StrBuffer &ValueFormatter::formatHex(StrBuffer &out, uint32_t val, int8_t bits, bool relax) const {
    val = makePositive(out, val, bits);
    const auto start = out.mark();

    if (relax && val <= 32)
        return outDec(out, val).reverse(start);

    const HexFormatter *formatter = _cstyle ? &_CStyleHex : &_hexFormatter;

    const uint8_t width = abs(bits);
    if (_upperHex) {
        UppercaseBuffer upper(out);
        return formatter->format(upper, val, width).over(out);
    }
    return formatter->format(out, val, width);
}

uint32_t ValueFormatter::makePositive(StrBuffer &out, uint32_t val, int8_t bits) const {
    uint8_t bw = bits;
    if (bits < 0) {
        bw = -bits;
        const auto sign = ConfigBase::shiftLeftOne(bw - 1);
        if (val & sign) {
            val = ~val + 1;
            out.letter('-');
        }
    }
    const auto mask = ConfigBase::shiftLeftOne(bw) - 1;
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

StrBuffer &HexFormatter::outHex(StrBuffer &out, uint32_t val, uint8_t width) {
    const auto start = out.mark();
    while (val) {
        const uint8_t digit = val & 0xF;
        if (digit < 10)
            out.letter(digit + '0');
        else
            out.letter(digit - 10 + 'a');
        val >>= 4;
    }
    // zero filling
    const int8_t digit = width / 4 + (width % 4 ? 1 : 0);
    while (out.mark() - start < digit && out.isOK())
        out.letter('0');
    return out;
}

StrBuffer &HexFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    const auto start = out.mark();
    return outHex(out, val, width).reverse(start);
}

StrBuffer &CStyleHexFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    out.rtext_P(HexFormatter::ZERO_X);  // raw text
    return HexFormatter::format(out, val, width);
}

StrBuffer &PrefixHexFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    out.text_P(_prefix_P);
    return HexFormatter::format(out, val, width);
}

StrBuffer &SuffixHexFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    const auto start = out.mark();
    const auto top = outHex(out, val, width).mark();
    if (top[-1] > '9')
        out.letter('0');  // prefixed with '0'
    return out.reverse(start).letter(_suffix);
}

StrBuffer &SurroundHexFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    out.text_P(_prefix_P);
    return HexFormatter::format(out, val, width).letter(_suffix);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
