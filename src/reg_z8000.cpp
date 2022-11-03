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

#include "table_z8000.h"

#include <ctype.h>

namespace libasm {
namespace z8000 {

RegName RegZ8000::parseRegName(StrScanner &scan) {
    StrScanner p(scan);
    if (p.iexpect('R')) {
        RegName base = REG_UNDEF;
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

StrBuffer &RegZ8000::outRegName(StrBuffer &out, RegName name) const {
    if (isCtlReg(name))
        return outCtlName(out, name);

    int8_t num = int8_t(name);
    if (num < 0)
        return out;
    const /*PROGMEM*/ char *prefix_P;
    if (isWordReg(name)) {
        num -= int8_t(REG_R0);
        prefix_P = PSTR("R");
    } else if (isLongReg(name)) {
        num -= int8_t(REG_RR0);
        prefix_P = PSTR("RR");
    } else if (isQuadReg(name)) {
        num -= int8_t(REG_RQ0);
        prefix_P = PSTR("RQ");
    } else if (num < int8_t(REG_RH0) + 8) {
        num -= int8_t(REG_RH0);
        prefix_P = PSTR("RH");
    } else {
        num -= int8_t(REG_RL0);
        prefix_P = PSTR("RL");
    }
    return outRegNumber(out.text_P(prefix_P, isUppercase()), num);
}

uint8_t RegZ8000::encodeGeneralRegName(RegName name) {
    return uint8_t(name) & 0xF;
}

RegName RegZ8000::decodeRegNum(uint8_t num, OprSize size) {
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
        return TableZ8000::TABLE.segmentedModel() ? decodeLongReg(num) : decodeWordReg(num);
    default:
        return REG_UNDEF;
    }
}

RegName RegZ8000::decodeWordReg(uint8_t num) {
    num &= 0x0f;
    return RegName(num);
}

RegName RegZ8000::decodeByteReg(uint8_t num) {
    num &= 0x0f;
    return RegName(num + 16);
}

RegName RegZ8000::decodeLongReg(uint8_t num) {
    num &= 0x0f;
    return num % 2 == 0 ? RegName(num + 32) : REG_ILLEGAL;
}

RegName RegZ8000::decodeQuadReg(uint8_t num) {
    num &= 0x0f;
    return num % 4 == 0 ? RegName(num + 48) : REG_ILLEGAL;
}

bool RegZ8000::isWordReg(RegName name) {
    const int8_t r = static_cast<int8_t>(name);
    return r >= 0 && r < 16;
}

bool RegZ8000::isByteReg(RegName name) {
    const int8_t r = static_cast<int8_t>(name);
    return r >= 16 && r < 16 + 16;
}

bool RegZ8000::isLongReg(RegName name) {
    const int8_t r = static_cast<int8_t>(name);
    return r >= 32 && r < 32 + 16;
}

bool RegZ8000::isQuadReg(RegName name) {
    const int8_t r = static_cast<int8_t>(name);
    return r >= 48 && r < 48 + 16;
}

bool RegZ8000::isCtlReg(RegName name) {
    return static_cast<int8_t>(name) >= 64;
}

// clang-format off
static const char TEXT_REG_FLAGS[]   PROGMEM = "FLAGS";
static const char TEXT_REG_FCW[]     PROGMEM = "FCW";
static const char TEXT_REG_REFRESH[] PROGMEM = "REFRESH";
static const char TEXT_REG_PSAPSEG[] PROGMEM = "PSAPSEG";
static const char TEXT_REG_PSAPOFF[] PROGMEM = "PSAPOFF";
static const char TEXT_REG_PSAP[]    PROGMEM = "PSAP";
static const char TEXT_REG_NSPSEG[]  PROGMEM = "NSPSEG";
static const char TEXT_REG_NSPOFF[]  PROGMEM = "NSPOFF";
static const char TEXT_REG_NSP[]     PROGMEM = "NSP";
// clang-format on
static const RegBase::NameEntry CTL_TABLE[] PROGMEM = {
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

RegName RegZ8000::parseCtlReg(StrScanner &scan) {
    const NameEntry *entry = searchText(scan, ARRAY_RANGE(CTL_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &RegZ8000::outCtlName(StrBuffer &out, RegName name) const {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(CTL_TABLE));
    if (entry)
        out.text_P(entry->text_P(), isUppercase());
    return out;
}

RegName RegZ8000::decodeCtlReg(uint8_t num) {
    num &= 7;
    const NameEntry *entry = searchName(num + 64, ARRAY_RANGE(CTL_TABLE));
    RegName name = entry ? RegName(entry->name()) : REG_ILLEGAL;
    if (!TableZ8000::TABLE.segmentedModel() && isSegCtlReg(name)) {
        name = RegName(entry->name() + 8);
        return isNonSegCtlReg(name) ? name : REG_ILLEGAL;
    }
    return name;
}

int8_t RegZ8000::encodeCtlReg(RegName name) {
    const int8_t num = (static_cast<int8_t>(name) - 64) & 7;
    if (TableZ8000::TABLE.segmentedModel())
        return isNonSegCtlReg(name) ? -1 : num;
    return isSegCtlReg(name) ? -1 : num;
}

static const char TEXT_INTR_VI[] PROGMEM = "VI";
static const char TEXT_INTR_NVI[] PROGMEM = "NVI";
static const RegBase::NameEntry INTR_TABLE[] PROGMEM = {
        NAME_ENTRY(INTR_NVI),
        NAME_ENTRY(INTR_VI),
};

IntrName RegZ8000::parseIntrName(StrScanner &scan) {
    const NameEntry *entry = searchText(scan, ARRAY_RANGE(INTR_TABLE));
    return entry ? IntrName(entry->name()) : INTR_UNDEF;
}

StrBuffer &RegZ8000::outIntrNames(StrBuffer &out, uint8_t intrs) const {
    char c = 0;
    if ((intrs & int8_t(INTR_VI)) == 0) {
        out.text_P(TEXT_INTR_VI, isUppercase());
        c = ',';
    }
    if ((intrs & int8_t(INTR_NVI)) == 0) {
        if (c)
            out.letter(c);
        out.text_P(TEXT_INTR_NVI, isUppercase());
    }
    return out;
}

uint8_t RegZ8000::encodeIntrName(IntrName name) {
    return uint8_t(name);
}

// clang-format off
static const char TEXT_CC_F[]   PROGMEM = "F";
static const char TEXT_CC_LT[]  PROGMEM = "LT";
static const char TEXT_CC_LE[]  PROGMEM = "LE";
static const char TEXT_CC_ULE[] PROGMEM = "ULE";
static const char TEXT_CC_OV[]  PROGMEM = "OV";
static const char TEXT_CC_MI[]  PROGMEM = "MI";
static const char TEXT_CC_Z[]   PROGMEM = "Z";
static const char TEXT_CC_C[]   PROGMEM = "C";
static const char TEXT_CC_T[]   PROGMEM = "";
static const char TEXT_CC_GE[]  PROGMEM = "GE";
static const char TEXT_CC_GT[]  PROGMEM = "GT";
static const char TEXT_CC_UGT[] PROGMEM = "UGT";
static const char TEXT_CC_NOV[] PROGMEM = "NOV";
static const char TEXT_CC_PL[]  PROGMEM = "PL";
static const char TEXT_CC_NZ[]  PROGMEM = "NZ";
static const char TEXT_CC_NC[]  PROGMEM = "NC";
static const char TEXT_CC_EQ[]  PROGMEM = "EQ";
static const char TEXT_CC_ULT[] PROGMEM = "ULT";
static const char TEXT_CC_NE[]  PROGMEM = "NE";
static const char TEXT_CC_UGE[] PROGMEM = "UGE";
// clang-format on
static const RegBase::NameEntry CC_TABLE[] PROGMEM = {
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

CcName RegZ8000::parseCcName(StrScanner &scan) {
    const NameEntry *entry = searchText(scan, ARRAY_RANGE(CC_TABLE));
    const CcName name = entry ? CcName(entry->name()) : CC_UNDEF;
    return name == CC_T ? CC_UNDEF : name;
}

uint8_t RegZ8000::encodeCcName(CcName name) {
    return uint8_t(name) & 0xF;
}

CcName RegZ8000::decodeCcNum(uint8_t num) {
    return CcName(num & 0xF);
}

StrBuffer &RegZ8000::outCcName(StrBuffer &out, CcName name) const {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(CC_TABLE));
    if (entry)
        out.text_P(entry->text_P(), isUppercase());
    return out;
}

static const char TEXT_FLAG_C[] PROGMEM = "C";
static const char TEXT_FLAG_Z[] PROGMEM = "Z";
static const char TEXT_FLAG_S[] PROGMEM = "S";
static const char TEXT_FLAG_P[] PROGMEM = "P";
static const char TEXT_FLAG_V[] PROGMEM = "V";
static const RegBase::NameEntry FLAG_TABLE[] PROGMEM = {
        NAME_ENTRY(FLAG_C),
        NAME_ENTRY(FLAG_Z),
        NAME_ENTRY(FLAG_S),
        NAME_ENTRY(FLAG_P),
        NAME_ENTRY(FLAG_V),
};

FlagName RegZ8000::parseFlagName(StrScanner &scan) {
    const NameEntry *entry = searchText(scan, ARRAY_RANGE(FLAG_TABLE));
    return entry ? FlagName(entry->name()) : FLAG_UNDEF;
}

StrBuffer &RegZ8000::outFlagNames(StrBuffer &out, uint8_t flags) const {
    char sep = 0;
    for (uint8_t bit = 0x8; bit; bit >>= 1) {
        if (flags & bit) {
            if (sep)
                out.letter(sep);
            sep = ',';
            const NameEntry *entry = searchName(bit, ARRAY_RANGE(FLAG_TABLE));
            out.text_P(entry->text_P(), isUppercase());
        }
    }
    return out;
}

uint8_t RegZ8000::encodeFlagName(FlagName name) {
    return uint8_t(name) & 0xF;
}

bool RegZ8000::checkOverlap(RegName dst, RegName src, RegName cnt) {
    const uint8_t dnum = encodeGeneralRegName(dst);
    const uint8_t ds = isByteReg(dst) && dnum >= 8 ? dnum - 8 : dnum;
    const uint8_t de = isLongReg(dst) ? ds + 1 : ds;
    const uint8_t ss = encodeGeneralRegName(src);
    const uint8_t se = isLongReg(src) ? ss + 1 : ss;
    if (ds == ss || ds == se || de == ss || de == se)
        return true;
    if (cnt == REG_UNDEF)
        return false;
    const uint8_t c = encodeGeneralRegName(cnt);
    return ds == c || de == c || ss == c || se == c;
}

}  // namespace z8000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
