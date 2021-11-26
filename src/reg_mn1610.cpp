/*
 * Copyright 2021 Tadashi G. Takaoka
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
#include "reg_mn1610.h"

#include <ctype.h>

#include "config_mn1610.h"
#include "table_mn1610.h"

namespace libasm {
namespace mn1610 {

// clang-format off
static const char TEXT_REG_R0[]   PROGMEM = "R0";
static const char TEXT_REG_R1[]   PROGMEM = "R1";
static const char TEXT_REG_R2[]   PROGMEM = "R2";
static const char TEXT_REG_R3[]   PROGMEM = "R3";
static const char TEXT_REG_R4[]   PROGMEM = "R4";
static const char TEXT_REG_SP[]   PROGMEM = "SP";
static const char TEXT_REG_STR[]  PROGMEM = "STR";
static const char TEXT_REG_IC[]  PROGMEM = "IC";
static const char TEXT_REG_DR0[]  PROGMEM = "DR0";
static const char TEXT_REG_X0[]   PROGMEM = "X0";
static const char TEXT_REG_X1[]   PROGMEM = "X1";
static const char TEXT_REG_CSBR[] PROGMEM = "CSBR";
static const char TEXT_REG_SSBR[] PROGMEM = "SSBR";
static const char TEXT_REG_TSR0[] PROGMEM = "TSR0";
static const char TEXT_REG_TSR1[] PROGMEM = "TSR1";
static const char TEXT_REG_OSR0[] PROGMEM = "OSR0";
static const char TEXT_REG_OSR1[] PROGMEM = "OSR1";
static const char TEXT_REG_OSR2[] PROGMEM = "OSR2";
static const char TEXT_REG_OSR3[] PROGMEM = "OSR3";
static const char TEXT_REG_TCR[]  PROGMEM = "TCR";
static const char TEXT_REG_TIR[]  PROGMEM = "TIR";
static const char TEXT_REG_TSR[]  PROGMEM = "TSR";
static const char TEXT_REG_SCR[]  PROGMEM = "SCR";
static const char TEXT_REG_SSR[]  PROGMEM = "SSR";
static const char TEXT_REG_SIR[]  PROGMEM = "SIR";
static const char TEXT_REG_SOR[]  PROGMEM = "SOR";
static const char TEXT_REG_IISR[] PROGMEM = "IISR";
static const char TEXT_REG_SBRB[] PROGMEM = "SBRB";
static const char TEXT_REG_ICB[]  PROGMEM = "ICB";
static const char TEXT_REG_NPP[]  PROGMEM = "NPP";
static const RegBase::NameEntry REG_TABLE[] PROGMEM = {
        NAME_ENTRY(REG_R0),
        NAME_ENTRY(REG_R1),
        NAME_ENTRY(REG_R2),
        NAME_ENTRY(REG_R3),
        NAME_ENTRY(REG_R4),
        NAME_ENTRY(REG_SP),
        NAME_ENTRY(REG_STR),
        // None generic register
        NAME_ENTRY(REG_IC),
        NAME_ENTRY(REG_DR0),
        // Register alias
        NAME_ENTRY(REG_X0),
        NAME_ENTRY(REG_X1),
        // none generic register
        NAME_ENTRY(REG_IC),
        NAME_ENTRY(REG_DR0),
        // Segment base register
        NAME_ENTRY(REG_CSBR),
        NAME_ENTRY(REG_SSBR),
        NAME_ENTRY(REG_TSR0),
        NAME_ENTRY(REG_TSR1),
        NAME_ENTRY(REG_OSR0),
        NAME_ENTRY(REG_OSR1),
        NAME_ENTRY(REG_OSR2),
        NAME_ENTRY(REG_OSR3),
        // Hardware control register
        NAME_ENTRY(REG_TCR),
        NAME_ENTRY(REG_TIR),
        NAME_ENTRY(REG_TSR),
        NAME_ENTRY(REG_SCR),
        NAME_ENTRY(REG_SSR),
        NAME_ENTRY(REG_SIR),
        NAME_ENTRY(REG_SOR),
        NAME_ENTRY(REG_IISR),
        // Special register
        NAME_ENTRY(REG_SBRB),
        NAME_ENTRY(REG_ICB),
        NAME_ENTRY(REG_NPP),
};
// clang-format on

RegName RegMn1610::parseRegName(const char *line) {
    const auto *entry = searchText(line, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

uint8_t RegMn1610::regNameLen(RegName name) {
    return nameLen(uint8_t(name), ARRAY_RANGE(REG_TABLE));
}

bool RegMn1610::isGeneric(RegName name) {
    if (name == REG_X0 || name == REG_X1)
        return true;
    const auto n = int8_t(name);
    return n >= int8_t(REG_R0) && n <= int8_t(REG_STR);
}

bool RegMn1610::isIndex(RegName name) {
    return name == REG_X0 || name == REG_X1 || name == REG_R3 || name == REG_R4;
}

bool RegMn1610::isIndirect(RegName name) {
    if (name == REG_X0 || name == REG_X1)
        return true;
    const auto n = int8_t(name);
    return n >= int8_t(REG_R1) && n <= int8_t(REG_R4);
}

bool RegMn1610::isSegmentBase(RegName name) {
    const auto n = int8_t(name);
    return n >= int8_t(REG_CSBR) && n <= int8_t(REG_TSR1);
}

bool RegMn1610::isSegment(RegName name) {
    const auto n = int8_t(name);
    return n >= int8_t(REG_CSBR) && n <= int8_t(REG_OSR3);
}

bool RegMn1610::isHardware(RegName name) {
    const auto n = int8_t(name);
    return n >= int8_t(REG_TCR) && n <= int8_t(REG_SOR);
}

bool RegMn1610::isSpecial(RegName name) {
    const auto n = int8_t(name);
    return n >= int8_t(REG_SBRB) && n <= int8_t(REG_NPP);
}

uint16_t RegMn1610::encodeGeneric(RegName name) {
    uint8_t n = int8_t(name);
    if (name == REG_X0 || name == REG_X1)
        n -= 8;
    return n;
}

uint16_t RegMn1610::encodeIndex(RegName name) {
    return (name == REG_X0 || name == REG_R3) ? 4 : 5;
}

uint16_t RegMn1610::encodeIndirect(RegName name) {
    uint8_t n = int8_t(name);
    if (name == REG_X0 || name == REG_X1)
        n -= 8;
    return n - 1;
}

uint16_t RegMn1610::encodeSegment(RegName name) {
    return int8_t(name) - 16;
}

uint16_t RegMn1610::encodeHardware(RegName name) {
    if (name == REG_SOR)
        return 5;
    return int8_t(name) - 24;
}

uint16_t RegMn1610::encodeSpecial(RegName name) {
    return int8_t(name) - 32;
}

RegName RegMn1610::decodeRegNum(uint8_t num) {
    const auto r = num & 7;
    return r == 7 ? REG_UNDEF : RegName(r);
}

RegName RegMn1610::decodeIndirect(uint8_t num) {
    return RegName((num & 3) + 1);
}

RegName RegMn1610::decodeSegment(uint8_t num) {
    return RegName((num & 7) + 16);
}

RegName RegMn1610::decodeHardware(uint8_t num) {
    const auto r = num & 7;
    return r == 7 ? REG_UNDEF : RegName(r + 24);
}

RegName RegMn1610::decodeSpecial(uint8_t num) {
    const auto r = num & 7;
    return r >= 3 ? REG_UNDEF : RegName(r + 32);
}

StrBuffer &RegMn1610::outRegName(StrBuffer &out, RegName name) const {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        outText(out, entry->text());
    return out;
}

// clang-format off
static const char TEXT_CC_SKP[] PROGMEM = "SKP";
static const char TEXT_CC_M[]   PROGMEM = "M";
static const char TEXT_CC_PZ[]  PROGMEM = "PZ";
static const char TEXT_CC_Z[]   PROGMEM = "Z";
static const char TEXT_CC_NZ[]  PROGMEM = "NZ";
static const char TEXT_CC_MZ[]  PROGMEM = "MZ";
static const char TEXT_CC_P[]   PROGMEM = "P";
static const char TEXT_CC_EZ[]  PROGMEM = "EZ";
static const char TEXT_CC_ENZ[] PROGMEM = "ENZ";
static const char TEXT_CC_OZ[]  PROGMEM = "OZ";
static const char TEXT_CC_ONZ[] PROGMEM = "ONZ";
static const char TEXT_CC_LMZ[] PROGMEM = "LMZ";
static const char TEXT_CC_LP[]  PROGMEM = "LP";
static const char TEXT_CC_LPZ[] PROGMEM = "LPZ";
static const char TEXT_CC_LM[]  PROGMEM = "LM";
static const char TEXT_CC_E[]   PROGMEM = "E";
static const char TEXT_CC_NE[]  PROGMEM = "NE";
static const char TEXT_CC_C[]   PROGMEM = "C";
static const char TEXT_CC_RE[]  PROGMEM = "RE";
static const char TEXT_CC_SE[]  PROGMEM = "SE";
static const char TEXT_CC_CE[]  PROGMEM = "CE";
static const char TEXT_CC_NONE[] PROGMEM = "";
// clang-format on
static const RegBase::NameEntry CC_TABLE[] PROGMEM = {
        NAME_ENTRY(CC_SKP),
        NAME_ENTRY(CC_M),
        NAME_ENTRY(CC_PZ),
        NAME_ENTRY(CC_Z),
        NAME_ENTRY(CC_NZ),
        NAME_ENTRY(CC_MZ),
        NAME_ENTRY(CC_P),
        NAME_ENTRY(CC_EZ),
        NAME_ENTRY(CC_ENZ),
        NAME_ENTRY(CC_OZ),
        NAME_ENTRY(CC_ONZ),
        NAME_ENTRY(CC_LMZ),
        NAME_ENTRY(CC_LP),
        NAME_ENTRY(CC_LPZ),
        NAME_ENTRY(CC_LM),
        // Aliases
        NAME_ENTRY(CC_E),
        NAME_ENTRY(CC_NE),
        // Carry mode
        NAME_ENTRY(CC_C),
        // E register operation
        NAME_ENTRY(CC_RE),
        NAME_ENTRY(CC_SE),
        NAME_ENTRY(CC_CE),
        // Empty text.
        NAME_ENTRY(CC_NONE),
};

CcName RegMn1610::parseCcName(const char *line) {
    const auto *entry = searchText(line, ARRAY_RANGE(CC_TABLE));
    const auto name = entry ? CcName(entry->name()) : CC_UNDEF;
    return name == CC_NONE ? CC_UNDEF : name;
}

uint8_t RegMn1610::ccNameLen(CcName name) {
    return nameLen(uint8_t(name), ARRAY_RANGE(CC_TABLE));
}

bool RegMn1610::isSkip(CcName name) {
    const auto n = int8_t(name);
    return n >= int8_t(CC_SKP) && n <= int8_t(CC_NE);
}

bool RegMn1610::isCop(CcName name) {
    return name == CC_C;
}

bool RegMn1610::isEop(CcName name) {
    const auto n = int8_t(name);
    return n >= int8_t(CC_RE);
}

uint16_t RegMn1610::encodeSkip(CcName name) {
    return int8_t(name) & 0x0F;
}

uint16_t RegMn1610::encodeCop(CcName name) {
    return name == CC_NONE ? 0 : 1;
}

uint16_t RegMn1610::encodeEop(CcName name) {
    return name == CC_NONE ? 0 : int8_t(name) - 48;
}

CcName RegMn1610::decodeSkip(uint8_t num) {
    return CcName(num & 0xF);
}

CcName RegMn1610::decodeCop(uint8_t num) {
    return (num & 1) == 0 ? CC_NONE : CC_C;
}

CcName RegMn1610::decodeEop(uint8_t num) {
    const auto eop = num & 3;
    return eop == 0 ? CC_NONE : CcName(eop + 48);
}

StrBuffer &RegMn1610::outCcName(StrBuffer &out, CcName name) const {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(CC_TABLE));
    if (entry)
        outText(out, entry->text());
    return out;
}

}  // namespace mn1610
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
