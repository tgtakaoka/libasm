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

#include "reg_h8500.h"

#include "reg_base.h"
#include "text_h8500.h"
#include "value_parser.h"

using namespace libasm::reg;
using namespace libasm::text::h8500;

namespace libasm {
namespace h8500 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_FP,  REG_FP  },
    { TEXT_REG_R0,  REG_R0  },
    { TEXT_REG_R1,  REG_R1  },
    { TEXT_REG_R2,  REG_R2  },
    { TEXT_REG_R3,  REG_R3  },
    { TEXT_REG_R4,  REG_R4  },
    { TEXT_REG_R5,  REG_R5  },
    { TEXT_REG_R6,  REG_R6  },
    { TEXT_REG_R7,  REG_R7  },
    { TEXT_REG_SP,  REG_SP  },
};

PROGMEM constexpr NameTable REG_TABLE{ARRAY_RANGE(REG_ENTRIES)};

constexpr NameEntry CR_ENTRIES[] PROGMEM = {
    { TEXT_REG_BR,  CR_BR   },
    { TEXT_REG_CCR, CR_CCR  },
    { TEXT_REG_CP,  -1      },  // CP not a standard CR but may appear
    { TEXT_REG_DP,  CR_DP   },
    { TEXT_REG_EP,  CR_EP   },
    { TEXT_REG_SR,  CR_SR   },
    { TEXT_REG_TP,  CR_TP   },
};

PROGMEM constexpr NameTable CR_TABLE{ARRAY_RANGE(CR_ENTRIES)};

constexpr NameEntry CC_ENTRIES[] PROGMEM = {
    { TEXT_BCC,    CC_CC  },
    { TEXT_BCS,    CC_CS  },
    { TEXT_BEQ,    CC_EQ  },
    { TEXT_BF,     CC_F   },
    { TEXT_BGE,    CC_GE  },
    { TEXT_BGT,    CC_GT  },
    { TEXT_BHI,    CC_HI  },
    { TEXT_BHS,    CC_CC  },
    { TEXT_BLE,    CC_LE  },
    { TEXT_BLO,    CC_CS  },
    { TEXT_BLS,    CC_LS  },
    { TEXT_BLT,    CC_LT  },
    { TEXT_BMI,    CC_MI  },
    { TEXT_BNE,    CC_NE  },
    { TEXT_BPL,    CC_PL  },
    { TEXT_BRA,    CC_T   },
    { TEXT_BRN,    CC_F   },
    { TEXT_BT,     CC_T   },
    { TEXT_BVC,    CC_VC  },
    { TEXT_BVS,    CC_VS  },
};

PROGMEM constexpr NameTable CC_TABLE{ARRAY_RANGE(CC_ENTRIES)};

// clang-format on
}  // namespace

RegName parseRegName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = REG_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        scan = p;
        return RegName(entry->name());
    }
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    const auto *entry = REG_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

CrName parseCrName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = CR_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        const auto code = entry->name();
        if (code >= 0) {
            scan = p;
            return CrName(code);
        }
    }
    return CR_UNDEF;
}

StrBuffer &outCrName(StrBuffer &out, CrName name) {
    const auto *entry = CR_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
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

StrBuffer &outCcName(StrBuffer &out, CcName name) {
    static const char *const CC_NAMES[] PROGMEM = {
        TEXT_BRA, TEXT_BRN, TEXT_BHI, TEXT_BLS,
        TEXT_BCC, TEXT_BCS, TEXT_BNE, TEXT_BEQ,
        TEXT_BVC, TEXT_BVS, TEXT_BPL, TEXT_BMI,
        TEXT_BGE, TEXT_BLT, TEXT_BGT, TEXT_BLE,
    };
    if (name >= 0 && name <= CC_LE) {
        const auto *n = reinterpret_cast<const char *>(pgm_read_ptr(&CC_NAMES[name]));
        out.text_P(n);
    }
    return out;
}

uint8_t encodeReg(RegName reg) {
    return static_cast<uint8_t>(reg) & 0x07;
}

RegName decodeReg(uint8_t bits3) {
    return RegName(bits3 & 0x07);
}

uint8_t encodeCr(CrName cr) {
    return static_cast<uint8_t>(cr) & 0x07;
}

CrName decodeCr(uint8_t ccc) {
    switch (ccc & 0x07) {
    case 0: return CR_SR;
    case 1: return CR_CCR;
    case 3: return CR_BR;
    case 4: return CR_EP;
    case 5: return CR_DP;
    case 7: return CR_TP;
    default: return CR_UNDEF;
    }
}

bool isBytesCr(CrName cr) {
    return cr != CR_SR;
}

}  // namespace reg
}  // namespace h8500
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
