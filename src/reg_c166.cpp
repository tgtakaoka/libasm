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

#include "reg_c166.h"
#include "reg_base.h"
#include "text_c166.h"
#include "value_parser.h"

using namespace libasm::reg;
using namespace libasm::text::c166;

namespace libasm {
namespace c166 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry CC_NAMES[] = {
    { TEXT_CC_C,   CC_C   },
    { TEXT_CC_EQ,  CC_EQ  },
    { TEXT_CC_N,   CC_N   },
    { TEXT_CC_NC,  CC_NC  },
    { TEXT_CC_NE,  CC_NE  },
    { TEXT_CC_NET, CC_NET },
    { TEXT_CC_NN,  CC_NN  },
    { TEXT_CC_NUSR0, CC_NUSR0 },
    { TEXT_CC_NUSR1, CC_NUSR1 },
    { TEXT_CC_NV,  CC_NV  },
    { TEXT_CC_NZ,  CC_NZ  },
    { TEXT_CC_SGE, CC_SGE },
    { TEXT_CC_SGT, CC_SGT },
    { TEXT_CC_SLE, CC_SLE },
    { TEXT_CC_SLT, CC_SLT },
    { TEXT_CC_UC,  CC_UC  },
    { TEXT_CC_UGE, CC_UGE },
    { TEXT_CC_UGT, CC_UGT },
    { TEXT_CC_ULE, CC_ULE },
    { TEXT_CC_ULT, CC_ULT },
    { TEXT_CC_USR0, CC_USR0 },
    { TEXT_CC_USR1, CC_USR1 },
    { TEXT_CC_V,   CC_V   },
    { TEXT_CC_Z,   CC_Z   },
};

constexpr NameTable CC_TABLE{ARRAY_RANGE(CC_NAMES)};

constexpr NameEntry MAC_NAMES[] = {
    { TEXT_REG_IDX0, REG_IDX0 },
    { TEXT_REG_IDX1, REG_IDX1 },
    { TEXT_REG_MAH,  REG_MAH  },
    { TEXT_REG_MAL,  REG_MAL  },
    { TEXT_REG_MAS,  REG_MAS  },
    { TEXT_REG_MCW,  REG_MCW  },
    { TEXT_REG_MRW,  REG_MRW  },
    { TEXT_REG_MSW,  REG_MSW  },
    { TEXT_REG_QR0,  REG_QR0  },
    { TEXT_REG_QR1,  REG_QR1  },
    { TEXT_REG_QX0,  REG_QX0  },
    { TEXT_REG_QX1,  REG_QX1  },
};

constexpr NameTable MAC_TABLE{ARRAY_RANGE(MAC_NAMES)};

// The SFRs of the CPU core, by short register number.
constexpr NameEntry SFR_NAMES[] = {
    { TEXT_REG_CP,    int8_t(0x08) },
    { TEXT_REG_CSP,   int8_t(0x04) },
    { TEXT_REG_DPP0,  int8_t(0x00) },
    { TEXT_REG_DPP1,  int8_t(0x01) },
    { TEXT_REG_DPP2,  int8_t(0x02) },
    { TEXT_REG_DPP3,  int8_t(0x03) },
    { TEXT_REG_MDC,   int8_t(0x87) },
    { TEXT_REG_MDH,   int8_t(0x06) },
    { TEXT_REG_MDL,   int8_t(0x07) },
    { TEXT_REG_ONES,  int8_t(0x8F) },
    { TEXT_REG_PSW,   int8_t(0x88) },
    { TEXT_REG_SP,    int8_t(0x09) },
    { TEXT_REG_STKOV, int8_t(0x0A) },
    { TEXT_REG_STKUN, int8_t(0x0B) },
    { TEXT_REG_TFR,   int8_t(0xD6) },
    { TEXT_REG_ZEROS, int8_t(0x8E) },
};

constexpr NameTable SFR_TABLE{ARRAY_RANGE(SFR_NAMES)};

// The SFRs the C166S V2 core adds.
constexpr NameEntry XC16X_SFR_NAMES[] = {
    { TEXT_REG_CPUCON1, int8_t(0x0C) },
    { TEXT_REG_CPUCON2, int8_t(0x0D) },
    { TEXT_REG_SPSEG,   int8_t(0x86) },
    { TEXT_REG_VECSEG,  int8_t(0x89) },
};

constexpr NameTable XC16X_SFR_TABLE{ARRAY_RANGE(XC16X_SFR_NAMES)};

// clang-format on

// Parse a register number which spans the rest of |name|.
int16_t parseNumber(StrScanner &name) {
    const auto num = parseRegNumber(name);
    return name.size() == 0 ? num : -1;
}

}  // namespace

RegName parseRegName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    auto name = parser.readRegName(p, true);
    if (name.iexpect('R')) {
        auto byte = name;
        if (byte.iexpect('L') || byte.iexpect('H')) {
            const auto high = toupper(name[0]) == 'H';
            const auto num = parseNumber(byte);
            if (num >= 0 && num < 8) {
                scan = p;
                return RegName(REG_RL0 + num * 2 + (high ? 1 : 0));
            }
            return REG_UNDEF;
        }
        const auto num = parseNumber(name);
        if (num >= 0 && num < 16) {
            scan = p;
            return RegName(REG_R0 + num);
        }
    }
    return REG_UNDEF;
}

RegName parseMacRegName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto name = parser.readRegName(p);
    const auto *entry = MAC_TABLE.searchText(name);
    if (entry) {
        scan = p;
        return RegName(entry->name());
    }
    return REG_UNDEF;
}

CcName parseCcName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    auto name = parser.readRegName(p);
    name.iexpectText_P(TEXT_CC_);
    const auto *entry = CC_TABLE.searchText(name);
    if (entry) {
        scan = p;
        return CcName(entry->name());
    }
    return CC_UNDEF;
}

bool isWordReg(RegName name) {
    return name >= REG_R0 && name <= REG_R15;
}

bool isByteReg(RegName name) {
    return name >= REG_RL0 && name <= REG_RH7;
}

uint8_t encodeRegNum(RegName name) {
    return uint8_t(name) & 0xF;
}

uint16_t parseSfrName(StrScanner &scan, const ValueParser &parser, CpuType cpuType) {
    auto p = scan;
    const auto name = parser.readRegName(p);
    auto entry = SFR_TABLE.searchText(name);
    if (entry == nullptr && cpuType == XC16X)
        entry = XC16X_SFR_TABLE.searchText(name);
    if (entry == nullptr)
        return 0;
    scan = p;
    const auto num = uint8_t(entry->name());
    return num < 0x80 ? 0xFE00 + 2 * num : 0xFF00 + 2 * (num - 0x80);
}

bool isCoReg(RegName name) {
    return name >= REG_MSW && name <= REG_MRW;
}

uint8_t encodeCoReg(RegName name) {
    return name - REG_MSW;
}

RegName decodeCoReg(uint8_t code) {
    const auto name = RegName(REG_MSW + code);
    return MAC_TABLE.searchName(name) ? name : REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    const auto num = int8_t(name);
    if (num >= REG_IDX0) {
        const auto *entry = MAC_TABLE.searchName(num);
        return entry ? out.text_P(entry->text_P()) : out;
    }
    if (num < 16)
        return out.letter('R').int16(num);
    out.letter('R').letter((num & 1) ? 'H' : 'L');
    return out.int16((num - 16) / 2);
}

StrBuffer &outCcName(StrBuffer &out, CcName name, bool prefix) {
    const auto *entry = CC_TABLE.searchName(name);
    if (entry) {
        if (prefix)
            out.text_P(TEXT_CC_);
        out.text_P(entry->text_P());
    }
    return out;
}

RegName decodeWordReg(uint8_t num) {
    return RegName(num & 0xF);
}

RegName decodeByteReg(uint8_t num) {
    return RegName((num & 0xF) + 16);
}

const char *sfrName_P(uint16_t addr, CpuType cpuType) {
    if (addr < 0xFE00 || (addr & 1) != 0)
        return nullptr;
    const auto num = int8_t((addr - 0xFE00) / 2);
    auto entry = SFR_TABLE.searchName(num);
    if (entry == nullptr && cpuType == XC16X)
        entry = XC16X_SFR_TABLE.searchName(num);
    return entry ? entry->text_P() : nullptr;
}

uint8_t encodeCcNum(CcName name) {
    return uint8_t(name) & 0xF;
}

CcName decodeCcNum(uint8_t num) {
    return CcName(num & 0xF);
}

bool isXccOnly(CcName name) {
    return name >= CC_NUSR0;
}

uint8_t encodeXcc(CcName name) {
    return isXccOnly(name) ? name - 32 : encodeCcNum(name) << 1;
}

CcName decodeXcc(uint8_t code) {
    if ((code & 1) == 0)
        return decodeCcNum(code >> 1);
    return code < 8 ? CcName(32 + code) : CC_UNDEF;
}

}  // namespace reg
}  // namespace c166
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
