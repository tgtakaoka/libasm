/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "reg_h8300.h"

#include "reg_base.h"
#include "text_h8300.h"
#include "value_parser.h"

using namespace libasm::reg;
using namespace libasm::text::h8300;

namespace libasm {
namespace h8300 {
namespace reg {

namespace {
// clang-format off
// Sorted alphabetically for binary search
constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_CCR, REG_CCR },
    { TEXT_REG_R0,  REG_R0  },
    { TEXT_REG_R0H, REG_R0H },
    { TEXT_REG_R0L, REG_R0L },
    { TEXT_REG_R1,  REG_R1  },
    { TEXT_REG_R1H, REG_R1H },
    { TEXT_REG_R1L, REG_R1L },
    { TEXT_REG_R2,  REG_R2  },
    { TEXT_REG_R2H, REG_R2H },
    { TEXT_REG_R2L, REG_R2L },
    { TEXT_REG_R3,  REG_R3  },
    { TEXT_REG_R3H, REG_R3H },
    { TEXT_REG_R3L, REG_R3L },
    { TEXT_REG_R4,  REG_R4  },
    { TEXT_REG_R4H, REG_R4H },
    { TEXT_REG_R4L, REG_R4L },
    { TEXT_REG_R5,  REG_R5  },
    { TEXT_REG_R5H, REG_R5H },
    { TEXT_REG_R5L, REG_R5L },
    { TEXT_REG_R6,  REG_R6  },
    { TEXT_REG_R6H, REG_R6H },
    { TEXT_REG_R6L, REG_R6L },
    { TEXT_REG_R7,  REG_R7  },
    { TEXT_REG_R7H, REG_R7H },
    { TEXT_REG_R7L, REG_R7L },
    { TEXT_REG_SP,  REG_SP  },
};
// clang-format on

PROGMEM constexpr NameTable TABLE{ARRAY_RANGE(REG_ENTRIES)};

}  // namespace

RegName parseRegName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = TABLE.searchText(parser.readRegName(p));
    if (entry) {
        scan = p;
        return RegName(entry->name());
    }
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    const auto *entry = TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

uint_fast8_t encodeReg8(RegName reg) {
    return static_cast<uint_fast8_t>(reg) & 0x0F;
}

uint_fast8_t encodeReg16(RegName reg) {
    return static_cast<uint_fast8_t>(reg) & 0x07;
}

RegName decodeReg8(uint_fast8_t nibble) {
    return RegName(nibble & 0x0F);
}

RegName decodeReg16(uint_fast8_t nibble) {
    return RegName((nibble & 0x07) + REG_R0);
}

bool isReg8(RegName reg) {
    const auto r = static_cast<uint_fast8_t>(reg);
    return r <= REG_R7L;
}

bool isReg16(RegName reg) {
    const auto r = static_cast<uint_fast8_t>(reg);
    return r >= REG_R0 && r <= REG_R7;
}

char sizeSuffix(OprSize size) {
    switch (size) {
    case SZ_BYTE:
        return 'B';
    case SZ_WORD:
        return 'W';
    default:
        return 0;
    }
}

}  // namespace reg
}  // namespace h8300
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
