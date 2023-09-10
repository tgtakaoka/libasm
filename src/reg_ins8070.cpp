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

#include "reg_ins8070.h"

#include "reg_base.h"
#include "text_ins8070.h"

using namespace libasm::reg;
using namespace libasm::text::ins8070;

namespace libasm {
namespace ins8070 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_A,  REG_A  },
    { TEXT_REG_E,  REG_E  },
    { TEXT_REG_EA, REG_EA },
    { TEXT_REG_P2, REG_P2 },
    { TEXT_REG_P3, REG_P3 },
    { TEXT_REG_PC, REG_PC },
    { TEXT_REG_S,  REG_S  },
    { TEXT_REG_SP, REG_SP },
    { TEXT_REG_T,  REG_T  },
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

bool isPointerReg(RegName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 4;
}

uint8_t encodePointerReg(RegName name) {
    return uint8_t(name);
}

RegName decodePointerReg(uint8_t num) {
    return RegName(num & 3);
}

}  // namespace reg
}  // namespace ins8070
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
