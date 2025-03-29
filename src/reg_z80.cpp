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

#include "reg_z80.h"
#include "reg_base.h"
#include "table_z80.h"
#include "text_z80.h"
#include "value_parser.h"

using namespace libasm::text::z80;
using namespace libasm::text::common;
using namespace libasm::reg;

namespace libasm {
namespace z80 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_A,    REG_A    },
    { TEXT_REG_AF,   REG_AF   },
    { TEXT_REG_B,    REG_B    },
    { TEXT_REG_BC,   REG_BC   },
    { TEXT_REG_C,    REG_C    },
    { TEXT_REG_D,    REG_D    },
    { TEXT_REG_DE,   REG_DE   },
    { TEXT_REG_DEHL, REG_DEHL },
    { TEXT_REG_DSR,  REG_DSR  },
    { TEXT_REG_E,    REG_E    },
    { TEXT_REG_H,    REG_H    },
    { TEXT_REG_HL,   REG_HL   },
    { TEXT_REG_I,    REG_I    },
    { TEXT_REG_IX,   REG_IX   },
    { TEXT_REG_IXH,  REG_IXH  },
    { TEXT_REG_IXL,  REG_IXL  },
    { TEXT_REG_IXU,  REG_IXU  },
    { TEXT_REG_IY,   REG_IY   },
    { TEXT_REG_IYH,  REG_IYH  },
    { TEXT_REG_IYL,  REG_IYL  },
    { TEXT_REG_IYU,  REG_IYU  },
    { TEXT_REG_L,    REG_L    },
    { TEXT_REG_PC,   REG_PC   },
    { TEXT_REG_R,    REG_R    },
    { TEXT_REG_SP,   REG_SP   },
    { TEXT_REG_SR,   REG_SR   },
    { TEXT_REG_USP,  REG_USP  },
    { TEXT_REG_XSR,  REG_XSR  },
    { TEXT_REG_YSR,  REG_YSR  },
};

constexpr NameEntry CC_ENTRIES[] PROGMEM = {
    { TEXT_CC_C,  CC_C  },
    { TEXT_CC_M,  CC_M  },
    { TEXT_CC_NC, CC_NC },
    { TEXT_CC_NS, CC_NS },
    { TEXT_CC_NV, CC_NV },
    { TEXT_CC_NZ, CC_NZ },
    { TEXT_CC_P,  CC_P  },
    { TEXT_CC_PE, CC_PE },
    { TEXT_CC_PO, CC_PO },
    { TEXT_CC_S,  CC_S  },
    { TEXT_CC_V,  CC_V  },
    { TEXT_CC_Z,  CC_Z  },
};

constexpr NameEntry DD_ENTRIES[] PROGMEM = {
    { PSTR_IB, DD_IB },
    { PSTR_IW, DD_IW },
    { PSTR_LW, DD_LW },
    { PSTR_W,  DD_W  },
};

constexpr NameEntry CTL_ENTRIES[] PROGMEM = {
    { PSTR_LCK, CTL_LCK },
    { PSTR_LW,  CTL_LW  },
    { PSTR_XM,  CTL_XM  },
};

PROGMEM constexpr NameTable REG_TABLE{ARRAY_RANGE(REG_ENTRIES)};
PROGMEM constexpr NameTable CC_TABLE{ARRAY_RANGE(CC_ENTRIES)};
PROGMEM constexpr NameTable DD_TABLE{ARRAY_RANGE(DD_ENTRIES)};
PROGMEM constexpr NameTable CTL_TABLE{ARRAY_RANGE(CTL_ENTRIES)};

// clang-format on
}  // namespace

RegName parseRegName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = REG_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        auto name = RegName(entry->name());
        if (name <= REG_A && p.expect('\''))
            name = RegName(name + ALT_BASE);
        scan = p;
        return name;
    }
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    if (name >= ALT_BASE)
        return outRegName(out, RegName(name - ALT_BASE)).letter('\'');
    const auto *entry = REG_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

uint8_t encodeDataReg(RegName name) {
    // (HL) is parsed as I_HL, then looked up as M_REG(reg=REG_HL), so
    // we have to map REG_HL to register number 6.
    if (name == REG_HL)
        return 6;
    return int8_t(name) - REG_B;
}

RegName decodeDataReg(uint8_t num) {
    // REG_HL represents (HL).
    if ((num &= 7) == 6)
        return REG_HL;
    return RegName(num + REG_B);
}

uint8_t encodePointerReg(RegName name) {
    return int8_t(name) - REG_BC;
}

uint8_t encodePointerRegIx(RegName name, RegName ix) {
    return encodePointerReg(name == ix ? REG_HL : name);
}

uint8_t encodeStackReg(RegName name) {
    if (name == REG_AF)
        return 3;
    return uint8_t(name) - REG_BC;
}

RegName decodeStackReg(uint8_t num) {
    if ((num &= 3) == 3)
        return REG_AF;
    return RegName(num + REG_BC);
}

uint8_t encodeIndirectBase(RegName name) {
    return uint8_t(name - REG_BC);
}

RegName decodeIndirectBase(uint8_t num) {
    return RegName((num & 1) + REG_BC);
}

bool isIndexReg(RegName name) {
    return name == REG_IX || name == REG_IY;
}

bool isAlternateReg(RegName name) {
    return name >= ALT_BASE;
}

RegName alt2BaseReg(RegName alt) {
    return RegName(alt - ALT_BASE);
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
    const auto *entry = CC_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

bool isCc4Name(CcName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 4;
}

bool isCcAlias(CcName name) {
    return name >= CC_alias;
}

uint8_t encodeCcName(CcName name) {
    if (isCcAlias(name))
        return encodeCcName(CcName(name - CC_alias));
    return uint8_t(name);
}

CcName decodeCcName(uint8_t num) {
    return CcName(num & 7);
}

DdName parseDdName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = DD_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        scan = p;
        return DdName(entry->name());
    }
    return DD_UNDEF;
}

StrBuffer &outDdName(StrBuffer &out, DdName name) {
    const auto *entry = DD_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

int8_t encodeDdNames(DdName word, DdName imm) {
    const int8_t dd = int8_t(word) | int8_t(imm);
    if (dd >= 0)
        return dd;
    if (imm == DD_IB)
        return 3;
    if (imm == DD_IW)
        return 7;
    return -1;
}

StrBuffer &outDdNames(StrBuffer &out, const DisInsn &insn) {
    auto word = (insn.prefix() & 0x20) ? DD_LW : DD_W;
    const auto im = insn.opCode() & 3;
    if (im == 3)
        return outDdName(out, word == DD_LW ? DD_IW : DD_IB);
    if (im)
        outDdName(out, im == 1 ? DD_IB : DD_IW).letter(',');
    return outDdName(out, word);
}

CtlName parseCtlName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = CTL_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        scan = p;
        return CtlName(entry->name());
    }
    return CTL_UNDEF;
}

uint8_t encodeCtlName(CtlName name) {
    return (uint8_t(name) << 4) | 0xCD;
}

StrBuffer &outCtlName(StrBuffer &out, const DisInsn &insn) {
    const auto name = CtlName((insn.prefix() >> 4) & 3);
    const auto *entry = CTL_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

}  // namespace reg
}  // namespace z80
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
