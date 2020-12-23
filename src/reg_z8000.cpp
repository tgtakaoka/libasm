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

#include <ctype.h>

#include "config_z8000.h"
#include "table_z8000.h"

namespace libasm {
namespace z8000 {

static int8_t parseRegNum(const char *line) {
    if (isdigit(*line) && !RegBase::isidchar(line[1]))
        return *line - '0';
    if (*line++ == '1' && *line >= '0' && *line < '6' &&
            !RegBase::isidchar(line[1]))
        return *line - '0' + 10;
    return -1;
}

RegName RegZ8000::parseRegName(const char *line) {
    if (toupper(*line++) == 'R') {
        const char type = toupper(*line);
        if (type == 'H' || type == 'L' || type == 'R' || type == 'Q')
            line++;
        int8_t num = parseRegNum(line);
        if (num < 0)
            return REG_UNDEF;
        if (type == 'H')
            return num < 8 ? RegName(num + 16) : REG_ILLEGAL;
        if (type == 'L')
            return num < 8 ? RegName(num + 24) : REG_ILLEGAL;
        if (type == 'R')
            return (num % 2) == 0 ? RegName(num + 32) : REG_ILLEGAL;
        if (type == 'Q')
            return (num % 4) == 0 ? RegName(num + 48) : REG_ILLEGAL;
        return RegName(num);
    }
    return REG_UNDEF;
}

uint8_t RegZ8000::regNameLen(RegName name) {
    if (isCtlReg(name))
        return ctlRegLen(name);
    const int8_t num = int8_t(name);
    if (num < 0)
        return 0;
    if (isByteReg(name))
        return 3;
    const uint8_t len = isWordReg(name) ? 1 : 2;
    return len + ((num & 0xF) < 10 ? 1 : 2);
}

char *RegZ8000::outRegName(char *out, RegName name) const {
    int8_t num = int8_t(name);
    if (num < 0)
        return out;
    if (isCtlReg(name))
        return outCtlName(out, name);

    out = outChar(out, 'R');
    if (isByteReg(name)) {
        num -= 16;
        if (num < 8) {
            out = outChar(out, 'H');
        } else {
            num -= 8;
            out = outChar(out, 'L');
        }
    } else if (isLongReg(name)) {
        num -= 32;
        out = outChar(out, 'R');
    } else if (isQuadReg(name)) {
        num -= 48;
        out = outChar(out, 'Q');
    }
    if (num >= 10) {
        *out++ = '1';
        num -= 10;
    }
    *out++ = num + '0';
    *out = 0;
    return out;
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
        return TableZ8000.segmentedModel() ? decodeLongReg(num)
                                           : decodeWordReg(num);
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
    return name == REG_PSAPSEG || name == REG_PSAPOFF || name == REG_NSPSEG ||
           name == REG_NSPOFF;
}

static bool isNonSegCtlReg(RegName name) {
    return name == REG_PSAP || name == REG_NSP;
}

RegName RegZ8000::parseCtlReg(const char *line) {
    const NameEntry *entry = searchText(line, ARRAY_RANGE(CTL_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

uint8_t RegZ8000::ctlRegLen(RegName name) {
    return nameLen(uint8_t(name), ARRAY_RANGE(CTL_TABLE));
}

char *RegZ8000::outCtlName(char *out, RegName name) const {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(CTL_TABLE));
    if (entry)
        out = outText(out, entry->text());
    return out;
}

RegName RegZ8000::decodeCtlReg(uint8_t num) {
    num &= 7;
    const NameEntry *entry = searchName(num + 64, ARRAY_RANGE(CTL_TABLE));
    RegName name = entry ? RegName(entry->name()) : REG_ILLEGAL;
    if (!TableZ8000.segmentedModel() && isSegCtlReg(name)) {
        name = RegName(entry->name() + 8);
        return isNonSegCtlReg(name) ? name : REG_ILLEGAL;
    }
    return name;
}

int8_t RegZ8000::encodeCtlReg(RegName name) {
    const int8_t num = (static_cast<int8_t>(name) - 64) & 7;
    if (TableZ8000.segmentedModel())
        return isNonSegCtlReg(name) ? -1 : num;
    return isSegCtlReg(name) ? -1 : num;
}

static const char TEXT_INTR_VI[] PROGMEM = "VI";
static const char TEXT_INTR_NVI[] PROGMEM = "NVI";
static const RegBase::NameEntry INTR_TABLE[] PROGMEM = {
        NAME_ENTRY(INTR_NVI),
        NAME_ENTRY(INTR_VI),
};

IntrName RegZ8000::parseIntrName(const char *line) {
    const NameEntry *entry = searchText(line, ARRAY_RANGE(INTR_TABLE));
    return entry ? IntrName(entry->name()) : INTR_UNDEF;
}

uint8_t RegZ8000::intrNameLen(IntrName name) {
    return nameLen(uint8_t(name), ARRAY_RANGE(INTR_TABLE));
}

char *RegZ8000::outIntrNames(char *out, uint8_t intrs) const {
    char c = 0;
    if ((intrs & int8_t(INTR_VI)) == 0) {
        out = outText(out, TEXT_INTR_VI);
        c = ',';
    }
    if ((intrs & int8_t(INTR_NVI)) == 0) {
        if (c)
            *out++ = c;
        out = outText(out, TEXT_INTR_NVI);
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

CcName RegZ8000::parseCcName(const char *line) {
    const NameEntry *entry = searchText(line, ARRAY_RANGE(CC_TABLE));
    const CcName name = entry ? CcName(entry->name()) : CC_UNDEF;
    return name == CC_T ? CC_UNDEF : name;
}

uint8_t RegZ8000::ccNameLen(const CcName name) {
    return nameLen(uint8_t(name), ARRAY_RANGE(CC_TABLE));
}

uint8_t RegZ8000::encodeCcName(CcName name) {
    return uint8_t(name) & 0xF;
}

CcName RegZ8000::decodeCcNum(uint8_t num) {
    return CcName(num & 0xF);
}

char *RegZ8000::outCcName(char *out, CcName name) const {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(CC_TABLE));
    if (entry)
        out = outText(out, entry->text());
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

FlagName RegZ8000::parseFlagName(const char *line) {
    const NameEntry *entry = searchText(line, ARRAY_RANGE(FLAG_TABLE));
    return entry ? FlagName(entry->name()) : FLAG_UNDEF;
}

uint8_t RegZ8000::flagNameLen(FlagName name) {
    return name == FLAG_UNDEF ? 0 : 1;
}

char *RegZ8000::outFlagNames(char *out, uint8_t flags) const {
    char sep = 0;
    for (uint8_t bit = 0x8; bit; bit >>= 1) {
        if (flags & bit) {
            if (sep)
                *out++ = sep;
            sep = ',';
            const NameEntry *entry = searchName(bit, ARRAY_RANGE(FLAG_TABLE));
            out = outText(out, entry->text());
        }
    }
    return out;
}

uint8_t RegZ8000::encodeFlagName(FlagName name) {
    return uint8_t(name) & 0xF;
}

bool RegZ8000::checkOverwrap(RegName dst, RegName src, RegName cnt) {
    const uint8_t dnum = encodeGeneralRegName(dst);
    const uint8_t ds = isByteReg(dst) && dnum >= 8 ? dnum - 8 : dnum;
    const uint8_t de = isLongReg(dst) ? ds + 1 : ds;
    const uint8_t ss = encodeGeneralRegName(src);
    const uint8_t se = isLongReg(src) ? ss + 1 : ss;
    const uint8_t c = encodeGeneralRegName(cnt);
    return ds == ss || ds == se || de == ss || de == se || ds == c || de == c ||
           ss == c || se == c;
}

}  // namespace z8000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
