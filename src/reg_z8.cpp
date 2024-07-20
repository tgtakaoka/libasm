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

#include "reg_z8.h"

#include "reg_base.h"
#include "text_z8.h"

using namespace libasm::reg;
using namespace libasm::text::z8;

namespace libasm {
namespace z8 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry CC_ENTRIES[] PROGMEM = {
    { TEXT_CC_C,   CC_C   },
    { TEXT_CC_EQ,  CC_EQ  },
    { TEXT_CC_F,   CC_F   },
    { TEXT_CC_GE,  CC_GE  },
    { TEXT_CC_GT,  CC_GT  },
    { TEXT_CC_LE,  CC_LE  },
    { TEXT_CC_LT,  CC_LT  },
    { TEXT_CC_MI,  CC_MI  },
    { TEXT_CC_NC,  CC_NC  },
    { TEXT_CC_NE,  CC_NE  },
    { TEXT_CC_NOV, CC_NOV },
    { TEXT_CC_NZ,  CC_NZ  },
    { TEXT_CC_OV,  CC_OV  },
    { TEXT_CC_PL,  CC_PL  },
    { TEXT_CC_UGE, CC_UGE },
    { TEXT_CC_UGT, CC_UGT },
    { TEXT_CC_ULE, CC_ULE },
    { TEXT_CC_ULT, CC_ULT },
    { TEXT_CC_Z,   CC_Z   },
};

PROGMEM constexpr NameTable CC_TABLE{ARRAY_RANGE(CC_ENTRIES)};

// clang-format on
}  // namespace

bool isWorkRegAlias(bool super8, uint8_t addr) {
    return (addr & 0xF0) == (super8 ? 0xC0 : 0xE0);
}

uint8_t encodeWorkRegAddr(bool super8, RegName name) {
    return encodeRegName(name) | (super8 ? 0xC0 : 0xE0);
}

RegName parseRegName(StrScanner &scan) {
    auto p = scan;
    if (p.iexpectText_P(TEXT_REG_RR)) {
        const auto num = parseRegNumber(p);
        if (num < 0 || num >= 16)
            return REG_UNDEF;
        if (num % 2)
            return REG_ILLEGAL;
        scan = p;
        return RegName(REG_RR0 + num);
    }
    if (p.iexpect('R')) {
        const auto num = parseRegNumber(p);
        if (num < 0 || num >= 16)
            return REG_UNDEF;
        scan = p;
        return RegName(REG_R0 + num);
    }
    return REG_UNDEF;
}

uint8_t encodeRegName(RegName name) {
    return uint8_t(name) & 0xF;
}

RegName decodeRegNum(uint8_t num) {
    return RegName(num & 0xF);
}

RegName decodePairRegNum(uint8_t num) {
    return RegName((num & 0xF) + 16);
}

bool isPairReg(RegName name) {
    return int8_t(name) >= 16;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    return isPairReg(name) ? out.text_P(TEXT_REG_RR).int16(name - REG_RR0)
                           : out.letter('R').int16(name - REG_R0);
}

CcName parseCcName(StrScanner &scan) {
    const auto *entry = CC_TABLE.searchText(scan);
    return entry ? CcName(entry->name()) : CC_UNDEF;
}

StrBuffer &outCcName(StrBuffer &out, CcName name) {
    const auto *entry = CC_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

uint8_t encodeCcName(CcName name) {
    const auto cc = uint8_t(name);
    if (cc < 16)
        return cc;
    return cc - 16;  // Aliases
}

CcName decodeCcNum(uint8_t num) {
    return CcName(num);
}

}  // namespace reg
}  // namespace z8
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
