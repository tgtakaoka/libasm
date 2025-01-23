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

#include "reg_mc6809.h"
#include "reg_base.h"
#include "text_mc6809.h"
#include "value_parser.h"

using namespace libasm::reg;
using namespace libasm::text::mc6809;

namespace libasm {
namespace mc6809 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_0,   REG_0   },
    { TEXT_REG_A,   REG_A   },
    { TEXT_REG_B,   REG_B   },
    { TEXT_REG_CC,  REG_CC  },
    { TEXT_REG_D,   REG_D   },
    { TEXT_REG_DP,  REG_DP  },
    { TEXT_REG_E,   REG_E   },
    { TEXT_REG_F,   REG_F   },
    { TEXT_REG_PC,  REG_PC  },
    { TEXT_REG_PCR, REG_PCR },
    { TEXT_REG_S,   REG_S   },
    { TEXT_REG_U,   REG_U   },
    { TEXT_REG_V,   REG_V   },
    { TEXT_REG_W,   REG_W   },
    { TEXT_REG_X,   REG_X   },
    { TEXT_REG_Y,   REG_Y   },
    { TEXT_REG_Z,   REG_Z   },
};

PROGMEM constexpr NameTable TABLE{ARRAY_RANGE(REG_ENTRIES)};

constexpr RegName SYSTEM_STACK_REGS[8] PROGMEM = {
    REG_CC, REG_A, REG_B, REG_DP, REG_X, REG_Y, REG_U, REG_PC,
};

constexpr RegName USER_STACK_REGS[8] PROGMEM = {
    REG_CC, REG_A, REG_B, REG_DP, REG_X, REG_Y, REG_S, REG_PC,
};

// clang-format on
}  // namespace

RegName parseRegName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    // exclude bit position '.n' at end of register name
    const auto *entry = TABLE.searchText(parser.readRegName(p, true));
    if (entry) {
        scan = p;
        return RegName(entry->name());
    }
    if (scan.iexpectWord_P(PSTR("SP")))
        return REG_S;
    if (scan.iexpectWord_P(TEXT_REG_0))
        return REG_0;
    return REG_UNDEF;
}

OprSize regSize(RegName name) {
    const auto num = int8_t(name);
    if (num < 0)
        return SZ_NONE;  // REG_UNDEF
    if (num < 8)
        return SZ_WORD;  // REG_D..REG_V
    if (num < 12)
        return SZ_BYTE;  // REG_A..REG_DP
    if (num >= 14)
        return SZ_BYTE;  // REG_E..REG_F
    return SZ_NONE;      // REG_Z..REG_0
}

StrBuffer &outRegName(StrBuffer &out, RegName name, bool fullName) {
    const auto *entry = TABLE.searchName(name);
    if (entry) {
        entry->outText(out);
        if (name == REG_S && fullName)
            out.letter('P');
    }
    return out;
}

RegName decodeDataReg(CpuType cpuType, uint8_t num) {
    num &= 0xF;
    const auto name = RegName(num);
    if (cpuType == MC6809) {
        if (name == REG_W || name == REG_V || num >= 12)
            return REG_UNDEF;
    }
    return (name == REG_0) ? REG_Z : name;
}

bool isDataReg(CpuType cpuType, RegName name) {
    if (name == REG_UNDEF)
        return false;
    switch (name) {
    case REG_W:
    case REG_V:
    case REG_E:
    case REG_F:
    case REG_Z:
    case REG_0:
        return cpuType == HD6309;
    case REG_PCR:
        return false;
    default:
        return true;
    }
}

uint8_t encodeDataReg(RegName name) {
    if (name == REG_0)
        name = REG_Z;
    return uint8_t(name);
}

RegName decodeBaseReg(uint8_t num) {
    return RegName((num & 3) + 1);
}

bool isBaseReg(RegName name) {
    switch (name) {
    case REG_X:
    case REG_Y:
    case REG_U:
    case REG_S:
    case REG_W:
        return true;
    default:
        return false;
    }
}

bool isIndexedBase(RegName name) {
    switch (name) {
    case REG_X:
    case REG_Y:
    case REG_U:
    case REG_S:
    case REG_PC:
    case REG_PCR:
    case REG_W:
        return true;
    default:
        return false;
    }
}

uint8_t encodeBaseReg(RegName name) {
    // REG_W is handled separately in TableMc6809::searchPostSpec().
    return uint8_t(name) - 1;
}

uint8_t encodeStackReg(RegName name, bool userStack) {
    if (name == REG_D)
        return 0x06;
    auto *reg = userStack ? ARRAY_BEGIN(USER_STACK_REGS) : ARRAY_BEGIN(SYSTEM_STACK_REGS);
    auto *end = userStack ? ARRAY_END(USER_STACK_REGS) : ARRAY_END(SYSTEM_STACK_REGS);
    uint8_t bit = 0x01;
    while (reg < end) {
        if (pgm_read_byte(reg) == name)
            break;
        reg++;
        bit <<= 1;
    }
    return bit;
}

RegName decodeStackReg(uint8_t bitPos, bool userStack) {
    auto *reg = userStack ? ARRAY_BEGIN(USER_STACK_REGS) : ARRAY_BEGIN(SYSTEM_STACK_REGS);
    return RegName(pgm_read_byte(reg + bitPos));
}

RegName decodeBitOpReg(uint8_t num) {
    switch (num) {
    case 0:
        return REG_CC;
    case 1:
        return REG_A;
    case 2:
        return REG_B;
    default:
        return REG_UNDEF;
    }
}

bool isBitOpReg(RegName name) {
    const auto num = uint8_t(name);
    return num >= 8 && num < 11;
}

uint8_t encodeBitOpReg(RegName name) {
    if (name == REG_CC)
        return 0;
    return uint8_t(name) - uint8_t(REG_A) + 1;
}

bool isTfmBaseReg(RegName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 5;
}

uint8_t encodeTfmBaseReg(RegName name) {
    return uint8_t(name);
}

int8_t encodeTfmMode(char src, char dst) {
    if (src && src == dst)
        return src == '-' ? 1 : 0;
    if (src == '+' && dst == 0)
        return 2;
    if (src == 0 && dst == '+')
        return 3;
    return -1;
}

char tfmSrcModeChar(uint8_t mode) {
    if (mode == 0 || mode == 2)
        return '+';
    return mode == 1 ? '-' : 0;
}

char tfmDstModeChar(uint8_t mode) {
    if (mode == 0 || mode == 3)
        return '+';
    return mode == 1 ? '-' : 0;
}

}  // namespace reg
}  // namespace mc6809
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
