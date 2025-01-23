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

#include "reg_ins8060.h"
#include "reg_base.h"
#include "text_ins8060.h"
#include "value_parser.h"

using namespace libasm::reg;
using namespace libasm::text::ins8060;

namespace libasm {
namespace ins8060 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_E,  REG_E  },
    { TEXT_REG_P0, REG_P0 },
    { TEXT_REG_P1, REG_P1 },
    { TEXT_REG_P2, REG_P2 },
    { TEXT_REG_P3, REG_P3 },
    { TEXT_REG_PC, REG_PC },
};

PROGMEM constexpr NameTable TABLE{ARRAY_RANGE(REG_ENTRIES)};

// clang-format on
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

bool isPointerReg(RegName name) {
    switch (name) {
    case REG_PC:
    case REG_P1:
    case REG_P2:
    case REG_P3:
    case REG_P0:
        return true;
    default:
        return false;
    }
}

uint8_t encodePointerReg(RegName name) {
    if (name == REG_P0)
        return 0;
    return uint8_t(name);
}

RegName decodePointerReg(uint8_t num) {
    return RegName(num & 3);
}

}  // namespace reg
}  // namespace ins8060
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
