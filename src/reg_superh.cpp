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

#include "reg_superh.h"

#include "reg_base.h"
#include "text_superh.h"
#include "value_parser.h"

using namespace libasm::reg;
using namespace libasm::text::superh;

namespace libasm {
namespace superh {
namespace reg {

namespace {

// clang-format off
// Sorted alphabetically by text for binary search
constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_GBR,  REG_GBR  },
    { TEXT_REG_MACH, REG_MACH },
    { TEXT_REG_MACL, REG_MACL },
    { TEXT_REG_PC,   REG_PC   },
    { TEXT_REG_PR,   REG_PR   },
    { TEXT_REG_R0,   REG_R0   },
    { TEXT_REG_R1,   REG_R1   },
    { TEXT_REG_R10,  REG_R10  },
    { TEXT_REG_R11,  REG_R11  },
    { TEXT_REG_R12,  REG_R12  },
    { TEXT_REG_R13,  REG_R13  },
    { TEXT_REG_R14,  REG_R14  },
    { TEXT_REG_R15,  REG_R15  },
    { TEXT_REG_R2,   REG_R2   },
    { TEXT_REG_R3,   REG_R3   },
    { TEXT_REG_R4,   REG_R4   },
    { TEXT_REG_R5,   REG_R5   },
    { TEXT_REG_R6,   REG_R6   },
    { TEXT_REG_R7,   REG_R7   },
    { TEXT_REG_R8,   REG_R8   },
    { TEXT_REG_R9,   REG_R9   },
    { TEXT_REG_SR,   REG_SR   },
    { TEXT_REG_VBR,  REG_VBR  },
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

RegName decodeRn(uint16_t word) {
    return RegName((word >> 8) & 0xF);
}

RegName decodeRm(uint16_t word) {
    return RegName((word >> 4) & 0xF);
}

uint8_t encodeRegNum(RegName r) {
    return static_cast<uint8_t>(r) & 0x0F;
}

}  // namespace reg
}  // namespace superh
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
