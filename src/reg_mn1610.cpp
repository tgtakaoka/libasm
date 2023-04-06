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

#include "text_mn1610.h"

using namespace libasm::text::mn1610;
using namespace libasm::reg;

namespace libasm {
namespace mn1610 {
namespace reg {

static constexpr NameEntry REG_TABLE[] PROGMEM = {
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

RegName parseRegName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

bool isGeneric(RegName name) {
    if (name == REG_X0 || name == REG_X1)
        return true;
    const auto n = int8_t(name);
    return n >= int8_t(REG_R0) && n <= int8_t(REG_STR);
}

bool isIndex(RegName name) {
    return name == REG_X0 || name == REG_X1 || name == REG_R3 || name == REG_R4;
}

bool isIndirect(RegName name) {
    if (name == REG_X0 || name == REG_X1)
        return true;
    const auto n = int8_t(name);
    return n >= int8_t(REG_R1) && n <= int8_t(REG_R4);
}

bool isSegmentBase(RegName name) {
    const auto n = int8_t(name);
    return n >= int8_t(REG_CSBR) && n <= int8_t(REG_TSR1);
}

bool isSegment(RegName name) {
    const auto n = int8_t(name);
    return n >= int8_t(REG_CSBR) && n <= int8_t(REG_OSR3);
}

bool isHardware(RegName name) {
    const auto n = int8_t(name);
    return n >= int8_t(REG_TCR) && n <= int8_t(REG_SOR);
}

bool isSpecial(RegName name) {
    const auto n = int8_t(name);
    return n >= int8_t(REG_SBRB) && n <= int8_t(REG_NPP);
}

uint16_t encodeGeneric(RegName name) {
    uint8_t n = int8_t(name);
    if (name == REG_X0 || name == REG_X1)
        n -= 8;
    return n;
}

uint16_t encodeIndex(RegName name) {
    return (name == REG_X0 || name == REG_R3) ? 4 : 5;
}

uint16_t encodeIndirect(RegName name) {
    uint8_t n = int8_t(name);
    if (name == REG_X0 || name == REG_X1)
        n -= 8;
    return n - 1;
}

uint16_t encodeSegment(RegName name) {
    return int8_t(name) - 16;
}

uint16_t encodeHardware(RegName name) {
    if (name == REG_SOR)
        return 5;
    return int8_t(name) - 24;
}

uint16_t encodeSpecial(RegName name) {
    return int8_t(name) - 32;
}

RegName decodeRegNum(uint8_t num) {
    const auto r = num & 7;
    return r == 7 ? REG_UNDEF : RegName(r);
}

RegName decodeIndirect(uint8_t num) {
    return RegName((num & 3) + 1);
}

RegName decodeSegment(uint8_t num) {
    return RegName((num & 7) + 16);
}

RegName decodeHardware(uint8_t num) {
    const auto r = num & 7;
    return r == 7 ? REG_UNDEF : RegName(r + 24);
}

RegName decodeSpecial(uint8_t num) {
    const auto r = num & 7;
    return r >= 3 ? REG_UNDEF : RegName(r + 32);
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        out.text_P(entry->text_P());
    return out;
}

static constexpr NameEntry CC_TABLE[] PROGMEM = {
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

CcName parseCcName(StrScanner &scan) {
    auto p = scan;
    const auto *entry = searchText(p, ARRAY_RANGE(CC_TABLE));
    const auto name = entry ? CcName(entry->name()) : CC_UNDEF;
    if (name == CC_C && *p == '\'')
        return CC_UNDEF;  // Letter constant C'x'
    scan = p;
    return name == CC_NONE ? CC_UNDEF : name;
}

bool isSkip(CcName name) {
    const auto n = int8_t(name);
    return n >= int8_t(CC_SKP) && n <= int8_t(CC_NE);
}

bool isCop(CcName name) {
    return name == CC_C;
}

bool isEop(CcName name) {
    const auto n = int8_t(name);
    return n >= int8_t(CC_RE);
}

uint16_t encodeSkip(CcName name) {
    return int8_t(name) & 0x0F;
}

uint16_t encodeCop(CcName name) {
    return name == CC_NONE ? 0 : 1;
}

uint16_t encodeEop(CcName name) {
    return name == CC_NONE ? 0 : int8_t(name) - 48;
}

CcName decodeSkip(uint8_t num) {
    return CcName(num & 0xF);
}

CcName decodeCop(uint8_t num) {
    return (num & 1) == 0 ? CC_NONE : CC_C;
}

CcName decodeEop(uint8_t num) {
    const auto eop = num & 3;
    return eop == 0 ? CC_NONE : CcName(eop + 48);
}

StrBuffer &outCcName(StrBuffer &out, CcName name) {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(CC_TABLE));
    if (entry)
        out.text_P(entry->text_P());
    return out;
}

}  // namespace reg
}  // namespace mn1610
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
