/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "reg_mc68hc12.h"
#include "reg_base.h"
#include "text_mc68hc12.h"
#include "value_parser.h"

using namespace libasm::reg;
using namespace libasm::text::mc68hc12;

namespace libasm {
namespace mc68hc12 {
namespace reg {

namespace {
// clang-format off
constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_A,   REG_A   },
    { TEXT_REG_B,   REG_B   },
    { TEXT_REG_CCR, REG_CCR },
    { TEXT_REG_D,   REG_D   },
    { TEXT_REG_PC,  REG_PC  },
    { TEXT_REG_SP,  REG_SP  },
    { TEXT_REG_X,   REG_X   },
    { TEXT_REG_Y,   REG_Y   },
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
    if (entry)
        entry->outText(out);
    return out;
}

RegName decodeRegNum(uint_fast8_t num) {
    const auto name = RegName(num & 7);
    const auto *entry = TABLE.searchName(name);
    return entry ? name : REG_UNDEF;
}

uint_fast8_t regSize(RegName name) {
    if (name == REG_UNDEF)
        return 0;
    return name >= REG_D ? 16 : 8;
}

uint_fast8_t encodeRegNum(RegName name) {
    return name & 7;
}

}  // namespace reg
}  // namespace mc68hc12
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
