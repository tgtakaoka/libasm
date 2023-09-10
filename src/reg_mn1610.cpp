/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "reg_mn1610.h"

#include "reg_base.h"
#include "text_mn1610.h"

using namespace libasm::reg;
using namespace libasm::text::mn1610;

namespace libasm {
namespace mn1610 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_CSBR, REG_CSBR },
    { TEXT_REG_DR0,  REG_DR0  },
    { TEXT_REG_IC,   REG_IC   },
    { TEXT_REG_ICB,  REG_ICB  },
    { TEXT_REG_IISR, REG_IISR },
    { TEXT_REG_NPP,  REG_NPP  },
    { TEXT_REG_OSR0, REG_OSR0 },
    { TEXT_REG_OSR1, REG_OSR1 },
    { TEXT_REG_OSR2, REG_OSR2 },
    { TEXT_REG_OSR3, REG_OSR3 },
    { TEXT_REG_R0,   REG_R0   },
    { TEXT_REG_R1,   REG_R1   },
    { TEXT_REG_R2,   REG_R2   },
    { TEXT_REG_R3,   REG_R3   },
    { TEXT_REG_R4,   REG_R4   },
    { TEXT_REG_SBRB, REG_SBRB },
    { TEXT_REG_SCR,  REG_SCR  },
    { TEXT_REG_SIR,  REG_SIR  },
    { TEXT_REG_SOR,  REG_SOR  },
    { TEXT_REG_SP,   REG_SP   },
    { TEXT_REG_SSBR, REG_SSBR },
    { TEXT_REG_SSR,  REG_SSR  },
    { TEXT_REG_STR,  REG_STR  },
    { TEXT_REG_TCR,  REG_TCR  },
    { TEXT_REG_TIR,  REG_TIR  },
    { TEXT_REG_TSR,  REG_TSR  },
    { TEXT_REG_TSR0, REG_TSR0 },
    { TEXT_REG_TSR1, REG_TSR1 },
    { TEXT_REG_X0,   REG_X0   },
    { TEXT_REG_X1,   REG_X1   },
};

constexpr NameEntry CC_ENTRIES[] PROGMEM = {
    { TEXT_CC_C,   CC_C   },
    { TEXT_CC_CE,  CC_CE  },
    { TEXT_CC_E,   CC_E   },
    { TEXT_CC_ENZ, CC_ENZ },
    { TEXT_CC_EZ,  CC_EZ  },
    { TEXT_CC_LM,  CC_LM  },
    { TEXT_CC_LMZ, CC_LMZ },
    { TEXT_CC_LP,  CC_LP  },
    { TEXT_CC_LPZ, CC_LPZ },
    { TEXT_CC_M,   CC_M   },
    { TEXT_CC_MZ,  CC_MZ  },
    { TEXT_CC_NE,  CC_NE  },
    { TEXT_CC_NZ,  CC_NZ  },
    { TEXT_CC_ONZ, CC_ONZ },
    { TEXT_CC_OZ,  CC_OZ  },
    { TEXT_CC_P,   CC_P   },
    { TEXT_CC_PZ,  CC_PZ  },
    { TEXT_CC_RE,  CC_RE  },
    { TEXT_CC_SE,  CC_SE  },
    { TEXT_CC_SKP, CC_SKP },
    { TEXT_CC_Z,   CC_Z   },
};

PROGMEM constexpr NameTable REG_TABLE{ARRAY_RANGE(REG_ENTRIES)};
PROGMEM constexpr NameTable CC_TABLE{ARRAY_RANGE(CC_ENTRIES)};

// clang-format on
}  // namespace

RegName parseRegName(StrScanner &scan) {
    const auto *entry = REG_TABLE.searchText(scan);
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

bool isGeneric(RegName name) {
    if (name == REG_X0 || name == REG_X1)
        return true;
    const auto n = int8_t(name);
    return n >= int8_t(REG_R0) && n <= int8_t(REG_STR);
}

bool isIndex(RegName name) {
    return name == REG_X0 || name == REG_X1 || name == REG_R3 || name == REG_R4;
}

bool isIndirect(RegName name) {
    if (name == REG_X0 || name == REG_X1)
        return true;
    const auto n = int8_t(name);
    return n >= int8_t(REG_R1) && n <= int8_t(REG_R4);
}

bool isSegmentBase(RegName name) {
    const auto n = int8_t(name);
    return n >= int8_t(REG_CSBR) && n <= int8_t(REG_TSR1);
}

bool isSegment(RegName name) {
    const auto n = int8_t(name);
    return n >= int8_t(REG_CSBR) && n <= int8_t(REG_OSR3);
}

bool isHardware(RegName name) {
    const auto n = int8_t(name);
    return n >= int8_t(REG_TCR) && n <= int8_t(REG_SOR);
}

bool isSpecial(RegName name) {
    const auto n = int8_t(name);
    return n >= int8_t(REG_SBRB) && n <= int8_t(REG_NPP);
}

uint16_t encodeGeneric(RegName name) {
    uint8_t n = int8_t(name);
    if (name == REG_X0 || name == REG_X1)
        n -= 8;
    return n;
}

uint16_t encodeIndex(RegName name) {
    return (name == REG_X0 || name == REG_R3) ? 4 : 5;
}

uint16_t encodeIndirect(RegName name) {
    uint8_t n = int8_t(name);
    if (name == REG_X0 || name == REG_X1)
        n -= 8;
    return n - 1;
}

uint16_t encodeSegment(RegName name) {
    return int8_t(name) - 16;
}

uint16_t encodeHardware(RegName name) {
    if (name == REG_SOR)
        return 5;
    return int8_t(name) - 24;
}

uint16_t encodeSpecial(RegName name) {
    return int8_t(name) - 32;
}

RegName decodeRegNum(uint8_t num) {
    const auto r = num & 7;
    return r == 7 ? REG_UNDEF : RegName(r);
}

RegName decodeIndirect(uint8_t num) {
    return RegName((num & 3) + 1);
}

RegName decodeSegment(uint8_t num) {
    return RegName((num & 7) + 16);
}

RegName decodeHardware(uint8_t num) {
    const auto r = num & 7;
    return r == 7 ? REG_UNDEF : RegName(r + 24);
}

RegName decodeSpecial(uint8_t num) {
    const auto r = num & 7;
    return r >= 3 ? REG_UNDEF : RegName(r + 32);
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    const auto *entry = REG_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

CcName parseCcName(StrScanner &scan) {
    auto p = scan;
    const auto *entry = CC_TABLE.searchText(p);
    const auto name = entry ? CcName(entry->name()) : CC_UNDEF;
    scan = p;
    return name;
}

bool isSkip(CcName name) {
    const auto n = int8_t(name);
    return n >= int8_t(CC_SKP) && n <= int8_t(CC_NE);
}

bool isCop(CcName name) {
    return name == CC_C;
}

bool isEop(CcName name) {
    const auto n = int8_t(name);
    return n >= int8_t(CC_RE);
}

uint16_t encodeSkip(CcName name) {
    return int8_t(name) & 0x0F;
}

uint16_t encodeCop(CcName name) {
    return name == CC_NONE ? 0 : 1;
}

uint16_t encodeEop(CcName name) {
    return name == CC_NONE ? 0 : int8_t(name) - 48;
}

CcName decodeSkip(uint8_t num) {
    return CcName(num & 0xF);
}

CcName decodeCop(uint8_t num) {
    return (num & 1) == 0 ? CC_NONE : CC_C;
}

CcName decodeEop(uint8_t num) {
    const auto eop = num & 3;
    return eop == 0 ? CC_NONE : CcName(eop + 48);
}

StrBuffer &outCcName(StrBuffer &out, CcName name) {
    const auto *entry = CC_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

}  // namespace reg
}  // namespace mn1610
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
