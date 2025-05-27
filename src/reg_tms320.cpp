/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "reg_tms320.h"
#include "reg_base.h"
#include "str_buffer.h"
#include "str_scanner.h"
#include "text_tms320.h"
#include "value_parser.h"

using namespace libasm::reg;
using namespace libasm::text::tms320;

namespace libasm {
namespace tms320 {
namespace reg {

namespace {
// clang-format off
constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_C,    REG_C    },
    { TEXT_REG_CNF,  REG_CNF  },
    { TEXT_REG_HM,   REG_HM   },
    { TEXT_REG_INTM, REG_INTM },
    { TEXT_REG_OVM,  REG_OVM  },
    { TEXT_REG_SXM,  REG_SXM  },
    { TEXT_REG_TC,   REG_TC   },
    { TEXT_REG_XF,   REG_XF   },
};

PROGMEM constexpr NameTable REG_TABLE{ARRAY_RANGE(REG_ENTRIES)};

constexpr NameEntry CC_ENTRIES[] PROGMEM = {
    { TEXT_CC_BIO, CC_BIO },
    { TEXT_CC_C,   CC_C   },
    { TEXT_CC_EQ,  CC_EQ  },
    { TEXT_CC_GEQ, CC_GEQ },
    { TEXT_CC_GT,  CC_GT  },
    { TEXT_CC_LEQ, CC_LEQ },
    { TEXT_CC_LT,  CC_LT  },
    { TEXT_CC_NC,  CC_NC  },
    { TEXT_CC_NEQ, CC_NEQ },
    { TEXT_CC_NOV, CC_NOV },
    { TEXT_CC_NTC, CC_NTC },
    { TEXT_CC_OV,  CC_OV  },
    { TEXT_CC_TC,  CC_TC  },
    { TEXT_CC_UNC, CC_UNC },
};

PROGMEM constexpr NameTable CC_TABLE{ARRAY_RANGE(CC_ENTRIES)};

// clang-format on
}  // namespace

RegName parseRegName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    if (p.iexpectText_P(TEXT_REG_AR)) {
        const auto num = parseRegNumber(p);
        if (num >= 0 && num <= 7) {
            scan = p;
            return RegName(REG_AR0 + num);
        }
        return REG_UNDEF;
    }
    if (p.iexpectText_P(TEXT_REG_PA)) {
        const auto num = parseRegNumber(p);
        if (num >= 0 && num <= 15) {
            scan = p;
            return RegName(REG_PA0 + num);
        }
        return REG_UNDEF;
    }
    const auto *entry = REG_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        scan = p;
        return RegName(entry->name());
    }
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    if (name >= REG_PA0)
        return out.text_P(TEXT_REG_PA).int16(name - REG_PA0);
    return out.text_P(TEXT_REG_AR).int16(name - REG_AR0);
}

uint_fast8_t encodeRegName(RegName name) {
    return int8_t(name) - (name >= REG_PA0 ? REG_PA0 : REG_AR0);
}

bool isControlName(RegName name) {
    return name >= CTL_BASE;
}

uint_fast8_t encodeControlName(RegName name) {
    return name - CTL_BASE;
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

uint16_t encodeCcName(CcName name) {
    static constexpr uint8_t CC[] PROGMEM = {
            0xFF,  // CC_UNC = 0,   Unconditional
            0xF6,  // CC_EQ = 1,    Z=1:    BZ
            0xF5,  // CC_NEQ = 2,   Z=0:    BNZ
            0xF3,  // CC_LT = 3,    ACC<0:  BLZ
            0xF2,  // CC_LEQ = 4,   ACC<=0: BLEZ
            0xF1,  // CC_GT = 5,    ACC>0:  BGZ
            0xF4,  // CC_GEQ = 6,   ACC>=0: BGEZ
            0xF7,  // CC_NOV = 7,   V=0:    BNV
            0xF0,  // CC_OV = 8,    V=1:    BV
            0x5F,  // CC_NC = 9,    C=0:    BNC
            0x5E,  // CC_C = 10,    C=1:    BC
            0xFA,  // CC_BIO = 11,  BIO=0:  BIOZ
            0xF8,  // CC_NTC = 12,  TC=0:   BBZ
            0xF9,  // CC_TC = 13,   TC=1:   BBNZ
    };
    return pgm_read_byte(CC + name) << 8;
}

}  // namespace reg
}  // namespace tms320
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
