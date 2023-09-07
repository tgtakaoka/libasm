
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

#include "text_z8000.h"

using namespace libasm::text::z8000;
using namespace libasm::reg;

namespace libasm {
namespace z8000 {
namespace reg {

RegName parseRegName(StrScanner &scan) {
    auto p = scan;
    if (p.iexpect('R')) {
        auto base = REG_UNDEF;
        int8_t num = -1;
        if (p.iexpect('H')) {
            base = REG_RH0;
            num = parseRegNumber(p, 8);
        } else if (p.iexpect('L')) {
            base = REG_RL0;
            num = parseRegNumber(p, 8);
        } else if (p.iexpect('R')) {
            base = REG_RR0;
            num = parseRegNumber(p, 16);
            if (num % 2)
                num = -1;
        } else if (p.iexpect('Q')) {
            base = REG_RQ0;
            num = parseRegNumber(p, 16);
            if (num % 4)
                num = -1;
        } else if (isdigit(*p)) {
            base = REG_R0;
            num = parseRegNumber(p, 16);
        }
        if (base != REG_UNDEF) {
            if (num < 0)
                return REG_ILLEGAL;
            scan = p;
            return RegName(num + int8_t(base));
        }
    }
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    if (isCtlReg(name))
        return outCtlName(out, name);

    auto num = int8_t(name);
    if (num < 0)
        return out;
    out.letter('R');
    if (isWordReg(name))
        return out.uint8(num - int8_t(REG_R0));
    if (isLongReg(name))
        return out.letter('R').uint8(num - int8_t(REG_RR0));
    if (isQuadReg(name))
        return out.letter('Q').uint8(num - int8_t(REG_RQ0));
    if (num < int8_t(REG_RH0) + 8)
        return out.letter('H').uint8(num - int8_t(REG_RH0));
    return out.letter('L').uint8(num - int8_t(REG_RL0));
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

static constexpr NameEntry CTL_TABLE[] PROGMEM = {
        NAME_ENTRY(REG_FLAGS),
        NAME_ENTRY(REG_FCW),
        NAME_ENTRY(REG_REFRESH),
        NAME_ENTRY(REG_PSAPSEG),
        NAME_ENTRY(REG_PSAPOFF),
        NAME_ENTRY(REG_PSAP),
        NAME_ENTRY(REG_NSPSEG),
        NAME_ENTRY(REG_NSPOFF),
        NAME_ENTRY(REG_NSP),
};

static bool isSegCtlReg(RegName name) {
    return name == REG_PSAPSEG || name == REG_PSAPOFF || name == REG_NSPSEG || name == REG_NSPOFF;
}

static bool isNonSegCtlReg(RegName name) {
    return name == REG_PSAP || name == REG_NSP;
}

RegName parseCtlReg(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(CTL_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &outCtlName(StrBuffer &out, RegName name) {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(CTL_TABLE));
    if (entry)
        out.text_P(entry->text_P());
    return out;
}

RegName decodeCtlReg(bool segmentedModel, uint8_t num) {
    num &= 7;
    const auto *entry = searchName(num + 64, ARRAY_RANGE(CTL_TABLE));
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

static constexpr char TEXT_INTR_VI[] PROGMEM = "VI";
static constexpr char TEXT_INTR_NVI[] PROGMEM = "NVI";
static constexpr NameEntry INTR_TABLE[] PROGMEM = {
        NAME_ENTRY(INTR_NVI),
        NAME_ENTRY(INTR_VI),
};

IntrName parseIntrName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(INTR_TABLE));
    return entry ? IntrName(entry->name()) : INTR_UNDEF;
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

static constexpr NameEntry CC_TABLE[] PROGMEM = {
        NAME_ENTRY(CC_F),
        NAME_ENTRY(CC_LT),
        NAME_ENTRY(CC_LE),
        NAME_ENTRY(CC_ULE),
        NAME_ENTRY(CC_OV),
        NAME_ENTRY(CC_MI),
        NAME_ENTRY(CC_Z),
        NAME_ENTRY(CC_C),
        NAME_ENTRY(CC_GE),
        NAME_ENTRY(CC_GT),
        NAME_ENTRY(CC_UGT),
        NAME_ENTRY(CC_NOV),
        NAME_ENTRY(CC_PL),
        NAME_ENTRY(CC_NZ),
        NAME_ENTRY(CC_NC),
        // Aliases
        NAME_ENTRY(CC_EQ),
        NAME_ENTRY(CC_ULT),
        NAME_ENTRY(CC_NE),
        NAME_ENTRY(CC_UGE),
        // Empty text.
        NAME_ENTRY(CC_T),
};

CcName parseCcName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(CC_TABLE));
    const auto name = entry ? CcName(entry->name()) : CC_UNDEF;
    return name == CC_T ? CC_UNDEF : name;
}

uint8_t encodeCcName(CcName name) {
    return uint8_t(name) & 0xF;
}

CcName decodeCcNum(uint8_t num) {
    return CcName(num & 0xF);
}

StrBuffer &outCcName(StrBuffer &out, CcName name) {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(CC_TABLE));
    if (entry)
        out.text_P(entry->text_P());
    return out;
}

static constexpr NameEntry FLAG_TABLE[] PROGMEM = {
        NAME_ENTRY(FLAG_C),
        NAME_ENTRY(FLAG_Z),
        NAME_ENTRY(FLAG_S),
        NAME_ENTRY(FLAG_P),
        NAME_ENTRY(FLAG_V),
};

FlagName parseFlagName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(FLAG_TABLE));
    return entry ? FlagName(entry->name()) : FLAG_UNDEF;
}

StrBuffer &outFlagNames(StrBuffer &out, uint8_t flags) {
    char sep = 0;
    for (uint8_t bit = 0x8; bit; bit >>= 1) {
        if (flags & bit) {
            if (sep)
                out.letter(sep);
            sep = ',';
            const auto *entry = searchName(bit, ARRAY_RANGE(FLAG_TABLE));
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
