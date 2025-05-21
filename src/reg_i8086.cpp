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

#include "reg_i8086.h"
#include "reg_base.h"
#include "text_i8086.h"
#include "value_parser.h"

using namespace libasm::reg;
using namespace libasm::text::i8086;

namespace libasm {
namespace i8086 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry REG_ENTRIES[] PROGMEM = {
    { TEXT_REG_AH,  REG_AH  },
    { TEXT_REG_AL,  REG_AL  },
    { TEXT_REG_AX,  REG_AX  },
    { TEXT_REG_BH,  REG_BH  },
    { TEXT_REG_BL,  REG_BL  },
    { TEXT_REG_BP,  REG_BP  },
    { TEXT_REG_BX,  REG_BX  },
    { TEXT_REG_CH,  REG_CH  },
    { TEXT_REG_CL,  REG_CL  },
    { TEXT_REG_CS,  REG_CS  },
    { TEXT_REG_CX,  REG_CX  },
    { TEXT_REG_DH,  REG_DH  },
    { TEXT_REG_DI,  REG_DI  },
    { TEXT_REG_DL,  REG_DL  },
    { TEXT_REG_DS,  REG_DS  },
    { TEXT_REG_DX,  REG_DX  },
    { TEXT_REG_EAX, REG_EAX },
    { TEXT_REG_EBP, REG_EBP },
    { TEXT_REG_EBX, REG_EBX },
    { TEXT_REG_ECX, REG_ECX },
    { TEXT_REG_EDI, REG_EDI },
    { TEXT_REG_EDX, REG_EDX },
    { TEXT_REG_ES,  REG_ES  },
    { TEXT_REG_ESI, REG_ESI },
    { TEXT_REG_ESP, REG_ESP },
    { TEXT_REG_FS,  REG_FS  },
    { TEXT_REG_GS,  REG_GS  },
    { TEXT_REG_SI,  REG_SI  },
    { TEXT_REG_SP,  REG_SP  },
    { TEXT_REG_SS,  REG_SS  },
    { TEXT_REG_ST,  REG_ST  },
};

PROGMEM constexpr NameTable REG_TABLE{ARRAY_RANGE(REG_ENTRIES)};

constexpr NameEntry PRE_ENTRIES[] PROGMEM = {
    { TEXT_PRE_ADDR16, PRE_ADDR16 },
    { TEXT_PRE_ADDR32, PRE_ADDR32 },
    { TEXT_PRE_BYTE,   PRE_BYTE   },
    { TEXT_PRE_DWORD,  PRE_DWORD  },
    { TEXT_PRE_FAR,    PRE_FAR    },
    { TEXT_PRE_PTR,    PRE_PTR    },
    { TEXT_PRE_QWORD,  PRE_QWORD  },
    { TEXT_PRE_TBYTE,  PRE_TBYTE  },
    { TEXT_PRE_WORD,   PRE_WORD   },
};

PROGMEM constexpr NameTable PREFIX_TABLE{ARRAY_RANGE(PRE_ENTRIES)};

// clang-format on
}  // namespace

RegName parseRegName(StrScanner &scan, const CpuSpec &cpuSpec, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = REG_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        const auto reg = RegName(entry->name());
        if (isGeneralReg(reg, cpuSpec) || isSegmentReg(reg, cpuSpec) ||
                (cpuSpec.fpu != FPU_NONE && reg == REG_ST)) {
            scan = p;
            return reg;
        }
    } else if (cpuSpec.cpu == I80386) {
        auto base = REG_UNDEF;
        if (p.iexpectText_P(TEXT_REG_CR)) {
            base = REG_CR_BASE;
        } else if (p.iexpectText_P(TEXT_REG_DR)) {
            base = REG_DR_BASE;
        } else if (p.iexpectText_P(TEXT_REG_TR)) {
            base = REG_TR_BASE;
        }
        const auto num = parseRegNumber(p);
        if (num >= 0 && num < 8) {
            if (base == REG_CR_BASE && num < 4)
                return RegName(base + num);
            if (base == REG_DR_BASE && (num < 4 || num >= 6))
                return RegName(base + num);
            if (base == REG_TR_BASE && num >= 6)
                return RegName(base + num);
        }
    }
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    const auto *entry = REG_TABLE.searchName(name);
    if (entry)
        return entry->outText(out);
    if (name >= REG_TR_BASE)
        return out.text_P(TEXT_REG_TR).int16(name - REG_TR_BASE);
    if (name >= REG_DR_BASE)
        return out.text_P(TEXT_REG_DR).int16(name - REG_DR_BASE);
    if (name >= REG_CR_BASE)
        return out.text_P(TEXT_REG_CR).int16(name - REG_CR_BASE);
    return out;
}

RegName decodeByteReg(uint8_t num) {
    return RegName((num & 7) + REG_AL);
}

RegName decodeWordReg(uint8_t num) {
    return RegName((num & 7) + REG_AX);
}

RegName decodeDwordReg(uint8_t num) {
    return RegName((num & 7) + REG_EAX);
}

RegName decodeSegReg(uint8_t num, const CpuSpec &cpuSpec) {
    const auto seg = num & 7;
    const auto no = cpuSpec.hasExtraSeg() ? 6 : 4;
    return seg < no ? RegName(seg + REG_ES) : REG_UNDEF;
}

RegName decodeCtlReg(uint8_t num, const CpuSpec &cpuSpec) {
    if (cpuSpec.cpu == I80386) {
        num &= 7;
        if (num < 4)
            return RegName(REG_CR_BASE + num);
    }
    return REG_UNDEF;
}

RegName decodeDbgReg(uint8_t num, const CpuSpec &cpuSpec) {
    if (cpuSpec.cpu == I80386) {
        num &= 7;
        if (num < 4 || num >= 6)
            return RegName(REG_DR_BASE + num);
    }
    return REG_UNDEF;
}

RegName decodeTstReg(uint8_t num, const CpuSpec &cpuSpec) {
    if (cpuSpec.cpu == I80386) {
        num &= 7;
        if (num >= 6)
            return RegName(REG_TR_BASE + num);
    }
    return REG_UNDEF;
}

bool isGeneralReg(RegName name, const CpuSpec &cpuSpec) {
    if (cpuSpec.cpu < I80386)
        return name >= REG_AX && name < REG_EAX;
    return name >= REG_AX && name < REG_ES;
}

bool isSegmentReg(RegName name, const CpuSpec &cpuSpec) {
    if (cpuSpec.cpu < I80386)
        return name >= REG_ES && name < REG_FS;
    return name >= REG_ES && name < REG_ST;
}

OprSize generalRegSize(RegName name) {
    if (name < REG_AL)
        return SZ_WORD;
    return name < REG_EAX ? SZ_BYTE : SZ_DWORD;
}

uint_fast8_t encodeRegNum(RegName name) {
    return name & 7;
}

PrefixName parsePrefixName(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto *entry = PREFIX_TABLE.searchText(parser.readRegName(p));
    if (entry) {
        scan = p;
        return PrefixName(entry->name());
    }
    return PRE_UNDEF;
}

StrBuffer &outPrefixName(StrBuffer &out, PrefixName name) {
    const auto *entry = PREFIX_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

PrefixName decodePrefixName(OprSize size) {
    return size == SZ_NONE ? PRE_UNDEF : PrefixName(PRE_PTR + size);
}

OprSize pointerSize(PrefixName name) {
    return (name >= PRE_BYTE) ? OprSize(name - PRE_PTR) : SZ_NONE;
}

}  // namespace reg
}  // namespace i8086
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
