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

#include "reg_h16.h"

#include "reg_base.h"
#include "text_h16.h"
#include "value_parser.h"

using namespace libasm::reg;
using namespace libasm::text::h16;

namespace libasm {
namespace h16 {
namespace reg {

namespace {
// clang-format off

// Register names are parsed/emitted as a prefix (R / CR / PR) plus a 0..15
// number rather than enumerated; no NameEntry table.

// Parse 0..15.  Returns -1 on failure (no digit, or value > 15).
int8_t parseRegNum(StrScanner &p) {
    if (!isdigit(*p)) return -1;
    int n = *p++ - '0';
    if (isdigit(*p)) {
        n = n * 10 + (*p++ - '0');
    }
    if (n > 15) return -1;
    return int8_t(n);
}


// Control register entries (sorted by text).  NameEntry::name is int8_t; CR
// codes >= 0x80 wrap to negative when stored, but uint8_t() cast on read
// recovers them.
constexpr NameEntry CR_ENTRIES[] PROGMEM = {
    { TEXT_REG_BMR,  int8_t(CR_BMR)  },
    { TEXT_REG_BSP,  int8_t(CR_BSP)  },
    { TEXT_REG_CBNR, int8_t(CR_CBNR) },
    { TEXT_REG_CCR,  int8_t(CR_CCR)  },
    { TEXT_REG_EBR,  int8_t(CR_EBR)  },
    { TEXT_REG_GBNR, int8_t(CR_GBNR) },
    { TEXT_REG_IBR,  int8_t(CR_IBR)  },
    { TEXT_REG_RBR,  int8_t(CR_RBR)  },
    { TEXT_REG_SR,   int8_t(CR_SR)   },
    { TEXT_REG_USP,  int8_t(CR_USP)  },
    { TEXT_REG_VBNR, int8_t(CR_VBNR) },
};

PROGMEM constexpr NameTable CR_TABLE{ARRAY_RANGE(CR_ENTRIES)};

// Condition code entries (sorted by text).  HS and LO are at the 0x10
// offset so they parse to distinct enum values but encode to the same
// 4-bit code as CC and CS respectively.
constexpr NameEntry CC_ENTRIES[] PROGMEM = {
    { TEXT_CC_CC,  CC_CC  },
    { TEXT_CC_CS,  CC_CS  },
    { TEXT_CC_EQ,  CC_EQ  },
    { TEXT_CC_F,   CC_F   },
    { TEXT_CC_GE,  CC_GE  },
    { TEXT_CC_GT,  CC_GT  },
    { TEXT_CC_HI,  CC_HI  },
    { TEXT_CC_HS,  CC_HS  },  // 0x14 == CC + 0x10
    { TEXT_CC_LE,  CC_LE  },
    { TEXT_CC_LO,  CC_LO  },  // 0x15 == CS + 0x10
    { TEXT_CC_LS,  CC_LS  },
    { TEXT_CC_LT,  CC_LT  },
    { TEXT_CC_MI,  CC_MI  },
    { TEXT_CC_NE,  CC_NE  },
    { TEXT_CC_PL,  CC_PL  },
    { TEXT_CC_T,   CC_T   },
    { TEXT_CC_VC,  CC_VC  },
    { TEXT_CC_VS,  CC_VS  },
};

PROGMEM constexpr NameTable CC_TABLE{ARRAY_RANGE(CC_ENTRIES)};

// clang-format on
}  // namespace

// Parse Rn, CRn, or PRn (n = 0..15).  Reads an alphanumeric identifier and
// splits it into a 1- or 2-character prefix and a 1- or 2-digit number.
RegName parseRegName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    auto token = parser.readRegName(p);
    if (!token.size()) return REG_UNDEF;

    int8_t base = -1;
    if (token.iexpect('C')) {
        if (!token.iexpect('R')) return REG_UNDEF;
        base = REG_CR0;
    } else if (token.iexpect('P')) {
        if (!token.iexpect('R')) return REG_UNDEF;
        base = REG_PR0;
    } else if (token.iexpect('R')) {
        base = REG_R0;
    } else {
        return REG_UNDEF;
    }
    const auto num = parseRegNum(token);
    if (num < 0 || token.size()) return REG_UNDEF;
    scan = p;
    return RegName(base + num);
}

CrName parseCrName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = CR_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        scan = p;
        return CrName(uint8_t(entry->name()));
    }
    return CR_UNDEF;
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

void outRegName(StrBuffer &out, RegName reg) {
    if (reg == REG_UNDEF) return;
    if (isCurBankReg(reg)) {
        out.letter('C').letter('R');
    } else if (isPrevBankReg(reg)) {
        out.letter('P').letter('R');
    } else {
        out.letter('R');
    }
    const auto n = regNum(reg);
    if (n >= 10) out.letter('1');
    out.letter('0' + (n % 10));
}

void outCrName(StrBuffer &out, CrName cr) {
    const auto *entry = CR_TABLE.searchName(int8_t(uint8_t(cr)));
    if (entry)
        out.text_P(entry->text_P());
}

void outCcName(StrBuffer &out, CcName cc) {
    const auto *entry = CC_TABLE.searchName(int8_t(cc));
    if (entry)
        out.text_P(entry->text_P());
}

// CrName values are the CR-code bytes, so crCode is a direct cast.
uint8_t crCode(CrName cr) {
    return uint8_t(cr);
}

// Validate that |code| is one of the documented CR codes.  Reuses the CR
// NameTable's searchName: CR_TABLE entries store the code as |name|, so the
// search succeeds iff |code| is a known CR.
CrName crFromCode(uint8_t code) {
    if (CR_TABLE.searchName(int8_t(code)) == nullptr)
        return CR_UNDEF;
    return CrName(code);
}

}  // namespace reg
}  // namespace h16
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
