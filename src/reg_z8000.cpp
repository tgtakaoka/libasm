
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

#include "reg_z8000.h"

#include "reg_base.h"
#include "text_z8000.h"

using namespace libasm::text::z8000;
using namespace libasm::reg;

namespace libasm {
namespace z8000 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry CTL_ENTRIES[] PROGMEM = {
    { TEXT_REG_FCW,     REG_FCW     },
    { TEXT_REG_FLAGS,   REG_FLAGS   },
    { TEXT_REG_NSP,     REG_NSP     },
    { TEXT_REG_NSPOFF,  REG_NSPOFF  },
    { TEXT_REG_NSPSEG,  REG_NSPSEG  },
    { TEXT_REG_PSAP,    REG_PSAP    },
    { TEXT_REG_PSAPOFF, REG_PSAPOFF },
    { TEXT_REG_PSAPSEG, REG_PSAPSEG },
    { TEXT_REG_REFRESH, REG_REFRESH },
};

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

constexpr NameEntry FLAG_ENTRIES[] PROGMEM = {
    { TEXT_FLAG_C, FLAG_C },
    { TEXT_FLAG_P, FLAG_P },
    { TEXT_FLAG_S, FLAG_S },
    { TEXT_FLAG_V, FLAG_V },
    { TEXT_FLAG_Z, FLAG_Z },
};

PROGMEM constexpr NameTable CTL_TABLE{ARRAY_RANGE(CTL_ENTRIES)};
PROGMEM constexpr NameTable CC_TABLE{ARRAY_RANGE(CC_ENTRIES)};
PROGMEM constexpr NameTable FLAG_TABLE{ARRAY_RANGE(FLAG_ENTRIES)};

// clang-format on
}  // namespace

RegName parseRegName(StrScanner &scan) {
    auto p = scan;
    if (p.iexpect('R')) {
        const auto num = parseRegNumber(p);
        if (num >= 0 && num < 16) {
            scan = p;
            return RegName(REG_R0 + num);
        }
    }
    p = scan;
    if (p.iexpectText_P(TEXT_REG_RR)) {
        const auto num = parseRegNumber(p);
        if (num < 0 || num >= 16)
            return REG_UNDEF;
        if (num % 2)
            return REG_ILLEGAL;
        scan = p;
        return RegName(REG_RR0 + num);
    }
    if (p.iexpectText_P(TEXT_REG_RL)) {
        const auto num = parseRegNumber(p);
        if (num < 0 || num >= 8)
            return REG_UNDEF;
        scan = p;
        return RegName(REG_RL0 + num);
    }
    if (p.iexpectText_P(TEXT_REG_RH)) {
        const auto num = parseRegNumber(p);
        if (num < 0 || num >= 8)
            return REG_UNDEF;
        scan = p;
        return RegName(REG_RH0 + num);
    }
    if (p.iexpectText_P(TEXT_REG_RQ)) {
        const auto num = parseRegNumber(p);
        if (num < 0 || num >= 16)
            return REG_UNDEF;
        if (num % 4)
            return REG_ILLEGAL;
        scan = p;
        return RegName(REG_RQ0 + num);
    }
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    if (isWordReg(name))
        return out.letter('R').uint8(name - REG_R0);
    if (isLongReg(name))
        return out.text_P(TEXT_REG_RR).uint8(name - REG_RR0);
    if (isByteReg(name))
        return name < REG_RL0 ?
            out.text_P(TEXT_REG_RH).uint8(name - REG_RH0) :
            out.text_P(TEXT_REG_RL).uint8(name- REG_RL0);
    if (isQuadReg(name))
        return out.text_P(TEXT_REG_RQ).uint8(name - REG_RQ0);
    return outCtlName(out, name);
}

uint8_t encodeGeneralRegName(RegName name) {
    return uint8_t(name) & 0xF;
}

RegName decodeRegNum(bool segmentedModel, uint8_t num, OprSize size) {
    switch (size) {
    case SZ_BYTE:
        return decodeByteReg(num);
    case SZ_WORD:
        return decodeWordReg(num);
    case SZ_LONG:
        return decodeLongReg(num);
    case SZ_QUAD:
        return decodeQuadReg(num);
    case SZ_ADDR:
        return segmentedModel ? decodeLongReg(num) : decodeWordReg(num);
    default:
        return REG_UNDEF;
    }
}

RegName decodeWordReg(uint8_t num) {
    num &= 0x0f;
    return RegName(num);
}

RegName decodeByteReg(uint8_t num) {
    num &= 0x0f;
    return RegName(num + 16);
}

RegName decodeLongReg(uint8_t num) {
    num &= 0x0f;
    return num % 2 == 0 ? RegName(num + 32) : REG_ILLEGAL;
}

RegName decodeQuadReg(uint8_t num) {
    num &= 0x0f;
    return num % 4 == 0 ? RegName(num + 48) : REG_ILLEGAL;
}

bool isWordReg(RegName name) {
    const auto r = static_cast<int8_t>(name);
    return r >= 0 && r < 16;
}

bool isByteReg(RegName name) {
    const auto r = static_cast<int8_t>(name);
    return r >= 16 && r < 16 + 16;
}

bool isLongReg(RegName name) {
    const auto r = static_cast<int8_t>(name);
    return r >= 32 && r < 32 + 16;
}

bool isQuadReg(RegName name) {
    const auto r = static_cast<int8_t>(name);
    return r >= 48 && r < 48 + 16;
}

bool isCtlReg(RegName name) {
    return static_cast<int8_t>(name) >= 64;
}

bool isSegCtlReg(RegName name) {
    return name == REG_PSAPSEG || name == REG_PSAPOFF || name == REG_NSPSEG || name == REG_NSPOFF;
}

bool isNonSegCtlReg(RegName name) {
    return name == REG_PSAP || name == REG_NSP;
}

RegName parseCtlReg(StrScanner &scan) {
    const auto *entry = CTL_TABLE.searchText(scan);
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &outCtlName(StrBuffer &out, RegName name) {
    const auto *entry = CTL_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

RegName decodeCtlReg(bool segmentedModel, uint8_t num) {
    num &= 7;
    const auto *entry = CTL_TABLE.searchName(num + 64);
    auto name = entry ? RegName(entry->name()) : REG_ILLEGAL;
    if (!segmentedModel && isSegCtlReg(name)) {
        name = RegName(entry->name() + 8);
        return isNonSegCtlReg(name) ? name : REG_ILLEGAL;
    }
    return name;
}

int8_t encodeCtlReg(bool segmentedModel, RegName name) {
    const int8_t num = (static_cast<int8_t>(name) - 64) & 7;
    if (segmentedModel)
        return isNonSegCtlReg(name) ? -1 : num;
    return isSegCtlReg(name) ? -1 : num;
}

IntrName parseIntrName(StrScanner &scan) {
    if (scan.iexpectWord_P(TEXT_INTR_VI))
        return INTR_VI;
    if (scan.iexpectWord_P(TEXT_INTR_NVI))
        return INTR_NVI;
    return INTR_UNDEF;
}

StrBuffer &outIntrNames(StrBuffer &out, uint8_t intrs) {
    char c = 0;
    if ((intrs & int8_t(INTR_VI)) == 0) {
        out.text_P(TEXT_INTR_VI);
        c = ',';
    }
    if ((intrs & int8_t(INTR_NVI)) == 0) {
        if (c)
            out.letter(c);
        out.text_P(TEXT_INTR_NVI);
    }
    return out;
}

uint8_t encodeIntrName(IntrName name) {
    return uint8_t(name);
}

CcName parseCcName(StrScanner &scan) {
    const auto *entry = CC_TABLE.searchText(scan);
    return entry ? CcName(entry->name()) : CC_UNDEF;
}

uint8_t encodeCcName(CcName name) {
    return uint8_t(name) & 0xF;
}

CcName decodeCcNum(uint8_t num) {
    return CcName(num & 0xF);
}

StrBuffer &outCcName(StrBuffer &out, CcName name) {
    const auto *entry = CC_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

FlagName parseFlagName(StrScanner &scan) {
    const auto *entry = FLAG_TABLE.searchText(scan);
    return entry ? FlagName(entry->name()) : FLAG_UNDEF;
}

StrBuffer &outFlagNames(StrBuffer &out, uint8_t flags) {
    char sep = 0;
    for (int8_t bit = 0x8; bit; bit >>= 1) {
        if (flags & bit) {
            if (sep)
                out.letter(sep);
            sep = ',';
            const auto *entry = FLAG_TABLE.searchName(bit);
            out.text_P(entry->text_P());
        }
    }
    return out;
}

uint8_t encodeFlagName(FlagName name) {
    return uint8_t(name) & 0xF;
}

bool checkOverlap(RegName dst, RegName src, RegName cnt) {
    const auto dnum = encodeGeneralRegName(dst);
    const auto ds = isByteReg(dst) && dnum >= 8 ? dnum - 8 : dnum;
    const auto de = isLongReg(dst) ? ds + 1 : ds;
    const auto ss = encodeGeneralRegName(src);
    const auto se = isLongReg(src) ? ss + 1 : ss;
    if (ds == ss || ds == se || de == ss || de == se)
        return true;
    if (cnt == REG_UNDEF)
        return false;
    const auto c = encodeGeneralRegName(cnt);
    return ds == c || de == c || ss == c || se == c;
}

}  // namespace reg
}  // namespace z8000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
