/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "reg_f3850.h"
#include "reg_base.h"
#include "text_f3850.h"
#include "value_parser.h"

using namespace libasm::text::f3850;
using namespace libasm::reg;

namespace libasm {
namespace f3850 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_A,  REG_A  },
    { TEXT_REG_D,  REG_D  },
    { TEXT_REG_DC, REG_DC },
    { TEXT_REG_H,  REG_H  },
    { TEXT_REG_HL, REG_HL },
    { TEXT_REG_HU, REG_HU },
    { TEXT_REG_I,  REG_I  },
    { TEXT_REG_IS, REG_IS },
    { TEXT_REG_J,  REG_J  },
    { TEXT_REG_K,  REG_K  },
    { TEXT_REG_KL, REG_KL },
    { TEXT_REG_KU, REG_KU },
    { TEXT_REG_P,  REG_P  },
    { TEXT_REG_P0, REG_P0 },
    { TEXT_REG_Q,  REG_Q  },
    { TEXT_REG_QL, REG_QL },
    { TEXT_REG_QU, REG_QU },
    { TEXT_REG_S,  REG_S  },
    { TEXT_REG_W,  REG_W  },
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

RegName decodeRegName(uint8_t opc) {
    const auto regno = opc & 0xF;
    constexpr auto regJ = int8_t(REG_J) - int8_t(REG_alias);
    constexpr auto regD = int8_t(REG_D) - int8_t(REG_alias);
    if (regno >= regJ && regno <= regD)
        return RegName(regno + int8_t(REG_alias));
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    const auto *entry = TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

}  // namespace reg
}  // namespace f3850
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
