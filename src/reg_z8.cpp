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
    return (addr & 0xF0) == (TableZ8::TABLE.isSuper8() ? 0xC0 : 0xE0);
}

uint8_t RegZ8::encodeWorkRegAddr(RegName name) {
    return encodeRegName(name) | (TableZ8::TABLE.isSuper8() ? 0xC0 : 0xE0);
}

RegName RegZ8::parseRegName(StrScanner &scan) {
    auto p = scan;
    if (p.iexpect('R')) {
        if (p.iexpect('R')) {
            const auto num = parseRegNumber(p, 16);
            if (num >= 0) {
                if (num % 2)
                    return REG_ILLEGAL;
                scan = p;
                return RegName(num + int8_t(REG_RR0));
            }
        } else {
            const auto num = parseRegNumber(p, 16);
            if (num >= 0) {
                scan = p;
                return RegName(num);
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
    const auto num = uint8_t(name);
    if (num < 16)
        return outRegNumber(out.letter('R', isUppercase()), num);
    return outRegNumber(out.text_P(PSTR("RR"), isUppercase()), num - 16);
}

// clang-format off
static constexpr char TEXT_CC_F[]   PROGMEM = "F";
static constexpr char TEXT_CC_LT[]  PROGMEM = "LT";
static constexpr char TEXT_CC_LE[]  PROGMEM = "LE";
static constexpr char TEXT_CC_ULE[] PROGMEM = "ULE";
static constexpr char TEXT_CC_OV[]  PROGMEM = "OV";
static constexpr char TEXT_CC_MI[]  PROGMEM = "MI";
static constexpr char TEXT_CC_Z[]   PROGMEM = "Z";
static constexpr char TEXT_CC_C[]   PROGMEM = "C";
static constexpr char TEXT_CC_T[]   PROGMEM = "";
static constexpr char TEXT_CC_GE[]  PROGMEM = "GE";
static constexpr char TEXT_CC_GT[]  PROGMEM = "GT";
static constexpr char TEXT_CC_UGT[] PROGMEM = "UGT";
static constexpr char TEXT_CC_NOV[] PROGMEM = "NOV";
static constexpr char TEXT_CC_PL[]  PROGMEM = "PL";
static constexpr char TEXT_CC_NZ[]  PROGMEM = "NZ";
static constexpr char TEXT_CC_NC[]  PROGMEM = "NC";
static constexpr char TEXT_CC_EQ[]  PROGMEM = "EQ";
static constexpr char TEXT_CC_ULT[] PROGMEM = "ULT";
static constexpr char TEXT_CC_NE[]  PROGMEM = "NE";
static constexpr char TEXT_CC_UGE[] PROGMEM = "UGE";
// clang-format on
static constexpr RegBase::NameEntry CC_TABLE[] PROGMEM = {
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
    const auto *entry = searchText(scan, ARRAY_RANGE(CC_TABLE));
    const auto name = entry ? CcName(entry->name()) : CC_UNDEF;
    return name == CC_T ? CC_UNDEF : name;
}

StrBuffer &RegZ8::outCcName(StrBuffer &out, CcName name) const {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(CC_TABLE));
    if (entry)
        out.text_P(entry->text_P(), isUppercase());
    return out;
}

uint8_t RegZ8::encodeCcName(CcName name) {
    const auto cc = uint8_t(name);
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
