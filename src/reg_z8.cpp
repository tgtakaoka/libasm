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

#include "reg_z8.h"

#include "table_z8.h"

#include <ctype.h>

namespace libasm {
namespace z8 {

bool RegZ8::isWorkRegAlias(uint8_t addr) {
    return (addr & 0xF0) == (TableZ8.isSuper8() ? 0xC0 : 0xE0);
}

uint8_t RegZ8::encodeWorkRegAddr(RegName name) {
    return encodeRegName(name) | (TableZ8.isSuper8() ? 0xC0 : 0xE0);
}

int8_t RegZ8::parseRegNum(StrScanner &scan) {
    const char c1 = *scan;
    if (!isdigit(c1))
        return -1;
    StrScanner p(scan);
    const char c2 = *++p;
    if (!isidchar(c2)) {
        scan = p;
        return c1 - '0';
    }
    if (c1 != '1' || c2 < '0' || c2 >= '7' || isidchar(*++p))
        return -1;
    scan = p;
    return c2 - '0' + 10;
}

RegName RegZ8::parseRegName(StrScanner &scan) {
    StrScanner p(scan);
    if (toupper(*p++) == 'R') {
        if (toupper(*p) == 'R') {
            const int8_t regNum = parseRegNum(++p);
            if (regNum >= 0 && regNum % 2 == 0) {
                scan = p;
                return RegName(regNum + 16);
            }
        } else if (isdigit(*p)) {
            const int8_t regNum = parseRegNum(p);
            if (regNum >= 0) {
                scan = p;
                return RegName(regNum);
            }
        }
    }
    return REG_UNDEF;
}

uint8_t RegZ8::encodeRegName(RegName name) {
    return uint8_t(name) & 0xF;
}

RegName RegZ8::decodeRegNum(uint8_t num) {
    return RegName(num & 0xF);
}

RegName RegZ8::decodePairRegNum(uint8_t num) {
    if (num % 2)
        return REG_UNDEF;
    return RegName((num & 0xF) + 16);
}

bool RegZ8::isPairReg(RegName name) {
    return int8_t(name) >= 16;
}

StrBuffer &RegZ8::outRegName(StrBuffer &out, RegName name) const {
    uint8_t num = uint8_t(name);
    const char r = _uppercase ? 'R' : 'r';
    out.letter(r);
    if (num >= 16) {
        out.letter(r);
        num -= 16;
    }
    if (num >= 10) {
        out.letter('1');
        num -= 10;
    }
    return out.letter(num + '0');
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
        // Empty text
        NAME_ENTRY(CC_T),
};

CcName RegZ8::parseCcName(StrScanner &scan) {
    const NameEntry *entry = searchText(scan, ARRAY_RANGE(CC_TABLE));
    const CcName name = entry ? CcName(entry->name()) : CC_UNDEF;
    return name == CC_T ? CC_UNDEF : name;
}

StrBuffer &RegZ8::outCcName(StrBuffer &out, CcName name) const {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(CC_TABLE));
    if (entry)
        outText(out, entry->text());
    return out;
}

uint8_t RegZ8::encodeCcName(CcName name) {
    const uint8_t cc = uint8_t(name);
    if (cc < 16)
        return cc;
    return cc - 16;  // Aliases
}

CcName RegZ8::decodeCcNum(uint8_t num) {
    return CcName(num);
}

}  // namespace z8
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
