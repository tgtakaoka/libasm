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

const DecFormatter &ValueFormatter::Plugins::dec() const {
    return DecFormatter::singleton();
}

const BinFormatter &ValueFormatter::Plugins::bin() const {
    return BinFormatter::singleton();
}

const OctFormatter &ValueFormatter::Plugins::oct() const {
    return OctFormatter::singleton();
}

const HexFormatter &ValueFormatter::Plugins::hex() const {
    return HexFormatter::singleton();
}

char ValueFormatter::Plugins::locationSymbol() const {
    return '.';
}

const /*PROGMEM*/ char *ValueFormatter::Plugins::lineComment_P() const {
    return PSTR(";");
}

const ValueFormatter::Plugins &ValueFormatter::Plugins::cstyle() {
    static const struct fianl : Plugins {
        const BinFormatter &bin() const override { return _bin; }
        const OctFormatter &oct() const override { return _oct; }
        const HexFormatter &hex() const override { return _hex; }
        const CStyleBinFormatter _bin{};
        const CStyleOctFormatter _oct{};
        const CStyleHexFormatter _hex{};
    } PLUGINS{};
    return PLUGINS;
}

const ValueFormatter::Plugins &ValueFormatter::Plugins::fairchild() {
    static const struct fianl : Plugins {
        const BinFormatter &bin() const override { return _bin; }
        const OctFormatter &oct() const override { return _oct; }
        const HexFormatter &hex() const override { return _hex; }
        char locationSymbol() const override { return '*'; }
        const SurroundBinFormatter _bin{BinFormatter::B_DASH, '\''};
        const SurroundOctFormatter _oct{OctFormatter::O_DASH, '\''};
        const SurroundHexFormatter _hex{HexFormatter::H_DASH, '\''};
    } PLUGINS{};
    return PLUGINS;
}

const ValueFormatter::Plugins &ValueFormatter::Plugins::intel() {
    static const struct fianl : Plugins {
        const BinFormatter &bin() const override { return _bin; }
        const OctFormatter &oct() const override { return _oct; }
        const HexFormatter &hex() const override { return _hex; }
        char locationSymbol() const override { return '$'; }
        const SuffixBinFormatter _bin{'b'};
        const SuffixOctFormatter _oct{'q'};
        const SuffixHexFormatter _hex{'h'};
    } PLUGINS{};
    return PLUGINS;
}

const ValueFormatter::Plugins &ValueFormatter::Plugins::motorola() {
    static const struct fianl : Plugins {
        const BinFormatter &bin() const override { return _bin; }
        const OctFormatter &oct() const override { return _oct; }
        const HexFormatter &hex() const override { return _hex; }
        char locationSymbol() const override { return '*'; }
        const /*PROGMEM*/ char *lineComment_P() const override { return PSTR("*"); }
        const PrefixBinFormatter _bin{BinFormatter::PERCENT};
        const PrefixOctFormatter _oct{OctFormatter::ATMARK};
        const PrefixHexFormatter _hex{HexFormatter::DOLLAR};
    } PLUGINS{};
    return PLUGINS;
}

const ValueFormatter::Plugins &ValueFormatter::Plugins::national() {
    static const struct fianl : Plugins {
        const BinFormatter &bin() const override { return _bin; }
        const OctFormatter &oct() const override { return _oct; }
        const HexFormatter &hex() const override { return _hex; }
        char locationSymbol() const override { return '$'; }
        const PrefixBinFormatter _bin{BinFormatter::B_DASH};
        const PrefixOctFormatter _oct{OctFormatter::O_DASH};
        const PrefixHexFormatter _hex{HexFormatter::X_DASH};
    } PLUGINS{};
    return PLUGINS;
}

const ValueFormatter::Plugins &ValueFormatter::Plugins::texas() {
    static const struct fianl : Plugins {
        const HexFormatter &hex() const override { return _hex; }
        char locationSymbol() const override { return '$'; }
        const /*PROGMEM*/ char *lineComment_P() const override { return PSTR("*"); }
        const PrefixHexFormatter _hex{HexFormatter::LESS};
    } PLUGINS{};
    return PLUGINS;
}

const ValueFormatter::Plugins &ValueFormatter::Plugins::zilog() {
    static const struct fianl : Plugins {
        const BinFormatter &bin() const override { return _bin; }
        const OctFormatter &oct() const override { return _oct; }
        const HexFormatter &hex() const override { return _hex; }
        char locationSymbol() const override { return '$'; }
        const PrefixBinFormatter _bin{BinFormatter::PERCENT2};
        const PrefixOctFormatter _oct{OctFormatter::PERCENT8};
        const PrefixHexFormatter _hex{HexFormatter::PERCENT};
    } PLUGINS{};
    return PLUGINS;
}

StrBuffer &ValueFormatter::formatDec(StrBuffer &out, uint32_t val, int8_t bits) const {
    val = makePositive(out, val, bits);
    return _dec.format(out, val);
}

StrBuffer &ValueFormatter::formatBin(StrBuffer &out, uint32_t val, int8_t bits) const {
    val = makePositive(out, val, bits);
    return _bin.format(out, val, abs(bits));
}

StrBuffer &ValueFormatter::formatOct(StrBuffer &out, uint32_t val, int8_t bits) const {
    val = makePositive(out, val, bits);
    return _oct.format(out, val, abs(bits));
}

StrBuffer &ValueFormatter::formatHex(
        StrBuffer &out, uint32_t val, int8_t bits, bool upperHex, bool relax) const {
    val = makePositive(out, val, bits);
    if (relax && val <= 32)
        return _dec.format(out, val);
    UppercaseBuffer upper(out);
    LowercaseBuffer lower(out);
    StrBuffer *outHex = upperHex ? upper.ptr() : lower.ptr();
    return _hex.format(*outHex, val, abs(bits)).over(out);
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

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
