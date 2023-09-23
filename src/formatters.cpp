/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "formatters.h"

namespace libasm {

const /*PROGMEM*/ char BinFormatter::ZERO_B[] PROGMEM = "0b";
const /*PROGMEM*/ char BinFormatter::PERCENT[] PROGMEM = "%";
const /*PROGMEM*/ char BinFormatter::PERCENT2[] PROGMEM = "%(2)";
const /*PROGMEM*/ char BinFormatter::B_DASH[] PROGMEM = "b'";

const /*PROGMEM*/ char OctFormatter::ATMARK[] PROGMEM = "@";
const /*PROGMEM*/ char OctFormatter::PERCENT8[] PROGMEM = "%(8)";
const /*PROGMEM*/ char OctFormatter::O_DASH[] PROGMEM = "o'";

const /*PROGMEM*/ char HexFormatter::ZERO_X[] PROGMEM = "0x";
const /*PROGMEM*/ char HexFormatter::DOLLAR[] PROGMEM = "$";
const /*PROGMEM*/ char HexFormatter::PERCENT[] PROGMEM = "%";
const /*PROGMEM*/ char HexFormatter::LESS[] PROGMEM = ">";
const /*PROGMEM*/ char HexFormatter::X_DASH[] PROGMEM = "x'";
const /*PROGMEM*/ char HexFormatter::H_DASH[] PROGMEM = "h'";

StrBuffer &DecFormatter::format(StrBuffer &out, uint32_t val) const {
    const auto start = out.mark();
    return outDec(out, val).reverse(start);
}

StrBuffer &DecFormatter::outDec(StrBuffer &out, uint32_t val) {
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

StrBuffer &BinFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    const auto start = out.mark();
    return outBin(out, val, width).reverse(start);
}


StrBuffer &BinFormatter::outBin(StrBuffer &out, uint32_t val, uint8_t width) {
    const auto start = out.mark();
    while (val) {
        const uint8_t digit = val & 1;
        out.letter(digit + '0');
        val >>= 1;
    }
    // zero filling
    while (out.mark() - start < width && out.isOK())
        out.letter('0');
    return out;
}

StrBuffer &PrefixBinFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    return BinFormatter::format(out.text_P(_prefix_P), val, width);
}

StrBuffer &SuffixBinFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    return BinFormatter::format(out, val, width).letter(_suffix);
}

StrBuffer &SurroundBinFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    out.text_P(_prefix_P);
    return BinFormatter::format(out, val, width).letter(_suffix);
}

StrBuffer &CStyleBinFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    return BinFormatter::format(out.rtext_P(ZERO_B), val, width);
}

StrBuffer &OctFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    const auto start = out.mark();
    return outOct(out, val, width).reverse(start);
}

StrBuffer &OctFormatter::outOct(StrBuffer &out, uint32_t val, uint8_t width) {
    const auto start = out.mark();
    while (val) {
        const uint8_t digit = val & 7;
        out.letter(digit + '0');
        val >>= 3;
    }
    // zero filling
    const int8_t digit = width / 3 + (width % 3 ? 1 : 0);
    while (out.mark() - start < digit && out.isOK())
        out.letter('0');
    return out;
}

StrBuffer &PrefixOctFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    return OctFormatter::format(out.text_P(_prefix_P), val, width);
}

StrBuffer &SuffixOctFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    return OctFormatter::format(out, val, width).letter(_suffix);
}

StrBuffer &SurroundOctFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    out.text_P(_prefix_P);
    return OctFormatter::format(out, val, width).letter(_suffix);
}

StrBuffer &CStyleOctFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    return OctFormatter::format(out.letter('0'), val, width);
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

StrBuffer &PrefixHexFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    return HexFormatter::format(out.text_P(_prefix_P), val, width);
}

StrBuffer &SuffixHexFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    const auto start = out.mark();
    const auto top = outHex(out, val, width).mark();
    if (top[-1] > '9')
        out.letter('0');  // prefixed with '0'
    return out.reverse(start).letter(_suffix);
}

StrBuffer &SurroundHexFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    return HexFormatter::format(out.text_P(_prefix_P), val, width).letter(_suffix);
}

StrBuffer &CStyleHexFormatter::format(StrBuffer &out, uint32_t val, uint8_t width) const {
    return HexFormatter::format(out.rtext_P(ZERO_X), val, width);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
