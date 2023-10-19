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

#include "reg_z80.h"

#include "reg_base.h"
#include "table_z80.h"
#include "text_z80.h"

using namespace libasm::text::z80;
using namespace libasm::reg;

namespace libasm {
namespace z80 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_A,   REG_A   },
    { TEXT_REG_AF,  REG_AF  },
    { TEXT_REG_AFP, REG_AFP },
    { TEXT_REG_B,   REG_B   },
    { TEXT_REG_BC,  REG_BC  },
    { TEXT_REG_C,   REG_C   },
    { TEXT_REG_D,   REG_D   },
    { TEXT_REG_DE,  REG_DE  },
    { TEXT_REG_E,   REG_E   },
    { TEXT_REG_H,   REG_H   },
    { TEXT_REG_HL,  REG_HL  },
    { TEXT_REG_I,   REG_I   },
    { TEXT_REG_IM,  REG_IM  },
    { TEXT_REG_IX,  REG_IX  },
    { TEXT_REG_IY,  REG_IY  },
    { TEXT_REG_L,   REG_L   },
    { TEXT_REG_R,   REG_R   },
    { TEXT_REG_SP,  REG_SP  },
};

constexpr NameEntry CC_ENTRIES[] PROGMEM = {
    { TEXT_CC_C,  CC_C  },
    { TEXT_CC_M,  CC_M  },
    { TEXT_CC_NC, CC_NC },
    { TEXT_CC_NZ, CC_NZ },
    { TEXT_CC_P,  CC_P  },
    { TEXT_CC_PE, CC_PE },
    { TEXT_CC_PO, CC_PO },
    { TEXT_CC_Z,  CC_Z  },
};

PROGMEM constexpr NameTable REG_TABLE{ARRAY_RANGE(REG_ENTRIES)};
PROGMEM constexpr NameTable CC_TABLE{ARRAY_RANGE(CC_ENTRIES)};

// clang-format on
}  // namespace

RegName parseRegName(StrScanner &scan) {
    const auto *entry = REG_TABLE.searchText(scan);
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    const auto *entry = REG_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

uint8_t encodeDataReg(RegName name) {
    // (HL) is parsed as I_HL, then looked up as M_REG(reg=REG_HL), so
    // we have to map REG_HL to register number 6.
    if (name == REG_HL)
        return 6;
    return int8_t(name) - 8;
}

RegName decodeDataReg(uint8_t num) {
    // REG_HL represents (HL).
    if ((num &= 7) == 6)
        return REG_HL;
    return RegName(num + 8);
}

uint8_t encodePointerReg(RegName name) {
    return int8_t(name);
}

uint8_t encodePointerRegIx(RegName name, RegName ix) {
    return encodePointerReg(name == ix ? REG_HL : name);
}

RegName decodePointerReg(uint8_t num, Config::opcode_t prefix) {
    const auto name = RegName(num & 3);
    if (name == REG_HL) {
        const auto ix = decodeIndexReg(prefix);
        return ix == REG_UNDEF ? name : ix;
    }
    return name;
}

uint8_t encodeStackReg(RegName name) {
    if (name == REG_AF)
        return 3;
    return uint8_t(name);
}

RegName decodeStackReg(uint8_t num) {
    if ((num &= 3) == 3)
        return REG_AF;
    return RegName(num);
}

uint8_t encodeIndirectBase(RegName name) {
    return uint8_t(name);
}

RegName decodeIndirectBase(uint8_t num) {
    return RegName(num & 1);
}

RegName decodeIndexReg(Config::opcode_t prefix) {
    if (prefix == TableZ80::PREFIX_IX)
        return REG_IX;
    if (prefix == TableZ80::PREFIX_IY)
        return REG_IY;
    return REG_UNDEF;
}

CcName parseCcName(StrScanner &scan) {
    const auto *entry = CC_TABLE.searchText(scan);
    return entry ? CcName(entry->name()) : CC_UNDEF;
}

StrBuffer &outCcName(StrBuffer &out, const CcName name) {
    const auto *entry = CC_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

bool isCc4Name(CcName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 4;
}

uint8_t encodeCcName(const CcName name) {
    return uint8_t(name);
}

CcName decodeCcName(uint8_t num) {
    return CcName(num & 7);
}

}  // namespace reg
}  // namespace z80
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
