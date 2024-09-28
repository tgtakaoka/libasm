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

#include "reg_pdp11.h"

#include "reg_base.h"
#include "text_pdp11.h"

using namespace libasm::reg;
using namespace libasm::text::pdp11;

namespace libasm {
namespace pdp11 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_AC0,   REG_AC0 },
    { TEXT_REG_AC1,   REG_AC1 },
    { TEXT_REG_AC2,   REG_AC2 },
    { TEXT_REG_AC3,   REG_AC3 },
    { TEXT_REG_AC4,   REG_AC4 },
    { TEXT_REG_AC5,   REG_AC5 },
    { TEXT_REG_PC,    REG_PC  },
    { TEXT_REG_R0,    REG_R0  },
    { TEXT_REG_R1,    REG_R1  },
    { TEXT_REG_R2,    REG_R2  },
    { TEXT_REG_R3,    REG_R3  },
    { TEXT_REG_R4,    REG_R4  },
    { TEXT_REG_R5,    REG_R5  },
    { TEXT_REG_R6,    REG_SP  },
    { TEXT_REG_R7,    REG_PC  },
    { TEXT_REG_SP,    REG_SP  },
};

PROGMEM constexpr NameTable TABLE{ARRAY_RANGE(REG_ENTRIES)};

// clang-format on
}  // namespace

RegName parseRegName(StrScanner &scan) {
    const auto *entry = TABLE.searchText(scan);
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    if (name == REG_SP) {
        return out.text_P(TEXT_REG_SP);
    } else if (name == REG_PC) {
        return out.text_P(TEXT_REG_PC);
    } else {
        const auto *entry = TABLE.searchName(name);
        return entry ? entry->outText(out) : out;
    }
}

bool isGeneralReg(RegName name) {
    const auto num = int8_t(name);
    return num >= REG_R0 && num <= REG_PC;
}

bool isAc03Reg(RegName name) {
    const auto num = int8_t(name);
    return num >= REG_AC0 && num <= REG_AC3;
}

bool isFloatReg(RegName name) {
    const auto num = int8_t(name);
    return num >= REG_AC0;
}

Config::opcode_t encodeRegNo(RegName name) {
    // Rn and ACn have the same register number.
    const auto num = int8_t(name);
    return num & 7;
}

RegName decodeGeneralReg(uint8_t regno) {
    return RegName(regno & 7);
}

RegName decodeFloatReg(uint8_t regno) {
    regno &= 7;
    return regno < 6 ? RegName(regno + REG_AC0) : REG_UNDEF;
}

}  // namespace reg
}  // namespace pdp11
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
