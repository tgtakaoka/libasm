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

#include "reg_i8051.h"

#include "reg_base.h"
#include "text_i8051.h"

using namespace libasm::reg;
using namespace libasm::text::i8051;

namespace libasm {
namespace i8051 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_A,    REG_A    },
    { TEXT_REG_AB,   REG_AB   },
    { TEXT_REG_C,    REG_C    },
    { TEXT_REG_DPTR, REG_DPTR },
    { TEXT_REG_PC,   REG_PC   },
    { TEXT_REG_R0,   REG_R0   },
    { TEXT_REG_R1,   REG_R1   },
    { TEXT_REG_R2,   REG_R2   },
    { TEXT_REG_R3,   REG_R3   },
    { TEXT_REG_R4,   REG_R4   },
    { TEXT_REG_R5,   REG_R5   },
    { TEXT_REG_R6,   REG_R6   },
    { TEXT_REG_R7,   REG_R7   },
};

PROGMEM constexpr NameTable TABLE{ARRAY_RANGE(REG_ENTRIES)};

// clang-format on
}  // namespace

RegName parseRegName(StrScanner &scan) {
    const auto *entry = TABLE.searchText(scan);
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    const auto *entry = TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

bool isRReg(RegName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 8;
}

uint8_t encodeRRegName(RegName name) {
    return uint8_t(name);
}

RegName decodeRRegNum(const uint8_t num) {
    return RegName(num);
}

}  // namespace reg
}  // namespace i8051
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
