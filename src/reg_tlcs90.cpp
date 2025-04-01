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

#include "reg_tlcs90.h"
#include "reg_base.h"
#include "text_tlcs90.h"
#include "value_parser.h"

using namespace libasm::reg;
using namespace libasm::text::tlcs90;

namespace libasm {
namespace tlcs90 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_A,   REG_A   },
    { TEXT_REG_AF,  REG_AF  },
    { TEXT_REG_B,   REG_B   },
    { TEXT_REG_BC,  REG_BC  },
    { TEXT_REG_C,   REG_C   },
    { TEXT_REG_D,   REG_D   },
    { TEXT_REG_DE,  REG_DE  },
    { TEXT_REG_E,   REG_E   },
    { TEXT_REG_H,   REG_H   },
    { TEXT_REG_HL,  REG_HL  },
    { TEXT_REG_IX,  REG_IX  },
    { TEXT_REG_IY,  REG_IY  },
    { TEXT_REG_L,   REG_L   },
    { TEXT_REG_SP,  REG_SP  },
};

constexpr NameEntry CC_ENTRIES[] PROGMEM = {
    { TEXT_CC_C,   CC_C   },
    { TEXT_CC_EQ,  CC_EQ  },
    { TEXT_CC_F,   CC_F   },
    { TEXT_CC_GE,  CC_GE  },
    { TEXT_CC_GT,  CC_GT  },
    { TEXT_CC_LE,  CC_LE  },
    { TEXT_CC_LT,  CC_LT  },
    { TEXT_CC_M,   CC_M   },
    { TEXT_CC_MI,  CC_MI  },
    { TEXT_CC_NC,  CC_NC  },
    { TEXT_CC_NE,  CC_NE  },
    { TEXT_CC_NOV, CC_NOV },
    { TEXT_CC_NZ,  CC_NZ  },
    { TEXT_CC_OV,  CC_OV  },
    { TEXT_CC_P,   CC_P   },
    { TEXT_CC_PE,  CC_PE  },
    { TEXT_CC_PL,  CC_PL  },
    { TEXT_CC_PO,  CC_PO  },
    { TEXT_CC_UGE, CC_UGE },
    { TEXT_CC_UGT, CC_UGT },
    { TEXT_CC_ULE, CC_ULE },
    { TEXT_CC_ULT, CC_ULT },
    { TEXT_CC_Z,   CC_Z   },
};

PROGMEM constexpr NameTable REG_TABLE{ARRAY_RANGE(REG_ENTRIES)};
PROGMEM constexpr NameTable CC_TABLE{ARRAY_RANGE(CC_ENTRIES)};

// clang-format on
}  // namespace

RegName parseRegName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = REG_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        auto name = RegName(entry->name());
        if (name == REG_AF && p.expect('\''))
            name = REG_AFP;
        scan = p;
        return name;
    }
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    if (name == REG_AFP)
        return outRegName(out, REG_AF).letter('\'');
    const auto *entry = REG_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

bool isReg16(RegName name) {
    return uint8_t(name) <= REG_SP;
}

bool isRegIndex(RegName name) {
    return isReg16(name) && name >= REG_IX;
}

uint8_t encodeReg8(RegName name) {
    return int8_t(name) - REG_B;
}

RegName decodeReg8(uint8_t num) {
    num &= 7;
    if (num == 7)
        return REG_UNDEF;
    return RegName(num + REG_B);
}

uint8_t encodeReg16(RegName name) {
    return int8_t(name);
}

RegName decodeReg16(uint8_t num) {
    num &= 7;
    if (num == 3 || num == 7)
        return REG_UNDEF;
    return RegName(num);
}

uint8_t encodeStackReg(RegName name) {
    if (name == REG_AF)
        return 6;
    return uint8_t(name);
}

RegName decodeStackReg(uint8_t num) {
    num &= 7;
    if (num == 3 || num == 7)
        return REG_UNDEF;
    if (num == 6)
        return REG_AF;
    return RegName(num);
}

uint8_t encodeIndexReg(RegName name) {
    return int8_t(name) - REG_IX;
}

RegName decodeIndexReg(uint8_t num) {
    num &= 3;
    if (num == 3)
        return REG_UNDEF;
    return RegName(num + REG_IX);
}

CcName parseCcName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = CC_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        scan = p;
        return CcName(entry->name());
    }
    return CC_UNDEF;
}

StrBuffer &outCcName(StrBuffer &out, const CcName name) {
    const auto *entry = CC_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

uint8_t encodeCcName(const CcName name) {
    const auto cc = uint8_t(name);
    return cc >= ALIAS_BASE ? cc - ALIAS_BASE : cc;
}

CcName decodeCcName(uint8_t num) {
    return CcName(num & 0xF);
}

}  // namespace reg
}  // namespace tlcs90
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
