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
#include "text_common.h"

namespace libasm {

using namespace text::common;

const DecFormatter &ValueFormatter::Plugins::dec() const {
    static const DecFormatter FORMATTER{};
    return FORMATTER;
}

const BinFormatter &ValueFormatter::Plugins::bin() const {
    static const BinFormatter FORMATTER{};
    return FORMATTER;
}

const OctFormatter &ValueFormatter::Plugins::oct() const {
    static const OctFormatter FORMATTER{};
    return FORMATTER;
}

const HexFormatter &ValueFormatter::Plugins::hex() const {
    static const HexFormatter FORMATTER{};
    return FORMATTER;
}

char ValueFormatter::Plugins::locationSymbol() const {
    return '$';
}

const /*PROGMEM*/ char *ValueFormatter::Plugins::lineComment_P() const {
    return PSTR_SEMI;
}

const ValueFormatter::Plugins &ValueFormatter::Plugins::singleton() {
    static const Plugins PLUGINS{};
    return PLUGINS;
}

const ValueFormatter::Plugins &ValueFormatter::Plugins::cstyle() {
    static const struct fianl : Plugins {
        const BinFormatter &bin() const override { return CStyleBinFormatter::singleton(); }
        const OctFormatter &oct() const override { return CStyleOctFormatter::singleton(); }
        const HexFormatter &hex() const override { return CStyleHexFormatter::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

const ValueFormatter::Plugins &ValueFormatter::Plugins::intel() {
    static const struct fianl : Plugins {
        const BinFormatter &bin() const override { return IntelBinFormatter::singleton(); }
        const OctFormatter &oct() const override { return IntelOctFormatter::singleton(); }
        const HexFormatter &hex() const override { return IntelHexFormatter::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

const ValueFormatter::Plugins &ValueFormatter::Plugins::motorola() {
    static const struct fianl : Plugins {
        const BinFormatter &bin() const override { return MotorolaBinFormatter::singleton(); }
        const OctFormatter &oct() const override { return MotorolaOctFormatter::singleton(); }
        const HexFormatter &hex() const override { return MotorolaHexFormatter::singleton(); }
        char locationSymbol() const override { return '*'; }
        const /*PROGMEM*/ char *lineComment_P() const override { return PSTR_STAR; }
    } PLUGINS{};
    return PLUGINS;
}

const ValueFormatter::Plugins &ValueFormatter::Plugins::fairchild() {
    static const struct fianl : Plugins {
        const BinFormatter &bin() const override { return FairchildBinFormatter::singleton(); }
        const OctFormatter &oct() const override { return FairchildOctFormatter::singleton(); }
        const HexFormatter &hex() const override { return FairchildHexFormatter::singleton(); }
        const /*PROGMEM*/ char *lineComment_P() const override { return PSTR_STAR; }
    } PLUGINS{};
    return PLUGINS;
}

const ValueFormatter::Plugins &ValueFormatter::Plugins::national() {
    static const struct fianl : Plugins {
        const BinFormatter &bin() const override { return NationalBinFormatter::singleton(); }
        const OctFormatter &oct() const override { return NationalOctFormatter::singleton(); }
        const HexFormatter &hex() const override { return NationalHexFormatter::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

const ValueFormatter::Plugins &ValueFormatter::Plugins::texas() {
    static const struct fianl : Plugins {
        const BinFormatter &bin() const override { return TexasBinFormatter::singleton(); }
        const HexFormatter &hex() const override { return TexasHexFormatter::singleton(); }
        const /*PROGMEM*/ char *lineComment_P() const override { return PSTR_STAR; }
    } PLUGINS{};
    return PLUGINS;
}

const ValueFormatter::Plugins &ValueFormatter::Plugins::zilog() {
    static const struct fianl : Plugins {
        const BinFormatter &bin() const override { return ZilogBinFormatter::singleton(); }
        const OctFormatter &oct() const override { return ZilogOctFormatter::singleton(); }
        const HexFormatter &hex() const override { return ZilogHexFormatter::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

StrBuffer &ValueFormatter::formatDec(StrBuffer &out, uint32_t val, int8_t bits) const {
    return _dec.format(out, makePositive(out, val, bits));
}

StrBuffer &ValueFormatter::formatBin(StrBuffer &out, uint32_t val, int8_t bits) const {
    return _bin.format(out, makePositive(out, val, bits), abs(bits));
}

StrBuffer &ValueFormatter::formatOct(StrBuffer &out, uint32_t val, int8_t bits) const {
    return _oct.format(out, makePositive(out, val, bits), abs(bits));
}

StrBuffer &ValueFormatter::formatHex(StrBuffer &out, uint32_t val, int8_t bits) const {
    return _hex.format(out, makePositive(out, val, bits), abs(bits));
}

bool ValueFormatter::absolute(uint32_t &val, int8_t bits) {
    auto negative = false;
    uint8_t bw = bits;
    if (bits < 0) {
        bw = -bits;
        const auto sign = 1UL << (bw - 1);
        if (val & sign) {
            negative = true;
            val = ~val + 1;
        }
    }
    const auto mask = (1UL << bw) - 1;
    val &= mask;
    return negative;
}

uint32_t ValueFormatter::makePositive(StrBuffer &out, uint32_t val, int8_t bits) {
    auto abs = val;
    if (absolute(abs, bits))
        out.letter('-');
    return abs;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
