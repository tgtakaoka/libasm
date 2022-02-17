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

#include "reg_i8048.h"

namespace libasm {
namespace i8048 {

// clang-format off
static const char TEXT_REG_A[]    PROGMEM = "A";
static const char TEXT_REG_BUS[]  PROGMEM = "BUS";
static const char TEXT_REG_C[]    PROGMEM = "C";
static const char TEXT_REG_CLK[]  PROGMEM = "CLK";
static const char TEXT_REG_CNT[]  PROGMEM = "CNT";
static const char TEXT_REG_F0[]   PROGMEM = "F0";
static const char TEXT_REG_F1[]   PROGMEM = "F1";
static const char TEXT_REG_I[]    PROGMEM = "I";
static const char TEXT_REG_MB0[]  PROGMEM = "MB0";
static const char TEXT_REG_MB1[]  PROGMEM = "MB1";
static const char TEXT_REG_P[]    PROGMEM = "P";
static const char TEXT_REG_P1[]   PROGMEM = "P1";
static const char TEXT_REG_P2[]   PROGMEM = "P2";
static const char TEXT_REG_P4[]   PROGMEM = "P4";
static const char TEXT_REG_P5[]   PROGMEM = "P5";
static const char TEXT_REG_P6[]   PROGMEM = "P6";
static const char TEXT_REG_P7[]   PROGMEM = "P7";
static const char TEXT_REG_PSW[]  PROGMEM = "PSW";
static const char TEXT_REG_R0[]   PROGMEM = "R0";
static const char TEXT_REG_R1[]   PROGMEM = "R1";
static const char TEXT_REG_R2[]   PROGMEM = "R2";
static const char TEXT_REG_R3[]   PROGMEM = "R3";
static const char TEXT_REG_R4[]   PROGMEM = "R4";
static const char TEXT_REG_R5[]   PROGMEM = "R5";
static const char TEXT_REG_R6[]   PROGMEM = "R6";
static const char TEXT_REG_R7[]   PROGMEM = "R7";
static const char TEXT_REG_RB0[]  PROGMEM = "RB0";
static const char TEXT_REG_RB1[]  PROGMEM = "RB1";
static const char TEXT_REG_T[]    PROGMEM = "T";
static const char TEXT_REG_TCNT[] PROGMEM = "TCNT";
static const char TEXT_REG_TCNTI[] PROGMEM = "TCNTI";

// clang-format on
static constexpr RegBase::NameEntry REG_TABLE[] PROGMEM = {
        NAME_ENTRY(REG_A),
        NAME_ENTRY(REG_BUS),
        NAME_ENTRY(REG_C),
        NAME_ENTRY(REG_CLK),
        NAME_ENTRY(REG_CNT),
        NAME_ENTRY(REG_F0),
        NAME_ENTRY(REG_F1),
        NAME_ENTRY(REG_I),
        NAME_ENTRY(REG_MB0),
        NAME_ENTRY(REG_MB1),
        NAME_ENTRY(REG_P),
        NAME_ENTRY(REG_P1),
        NAME_ENTRY(REG_P2),
        NAME_ENTRY(REG_P4),
        NAME_ENTRY(REG_P5),
        NAME_ENTRY(REG_P6),
        NAME_ENTRY(REG_P7),
        NAME_ENTRY(REG_PSW),
        NAME_ENTRY(REG_R0),
        NAME_ENTRY(REG_R1),
        NAME_ENTRY(REG_R2),
        NAME_ENTRY(REG_R3),
        NAME_ENTRY(REG_R4),
        NAME_ENTRY(REG_R5),
        NAME_ENTRY(REG_R6),
        NAME_ENTRY(REG_R7),
        NAME_ENTRY(REG_RB0),
        NAME_ENTRY(REG_RB1),
        NAME_ENTRY(REG_T),
        NAME_ENTRY(REG_TCNT),
        NAME_ENTRY(REG_TCNTI),
};

RegName RegI8048::parseRegName(StrScanner &scan) {
    const NameEntry *entry = searchText(scan, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &RegI8048::outRegName(StrBuffer &out, const RegName name) const {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        outText(out, entry->text());
    return out;
}

bool RegI8048::isRReg(RegName name) {
    const int8_t num = int8_t(name);
    return num >= int8_t(REG_R0) && num <= int8_t(REG_R7);
}

bool RegI8048::isPort(RegName name) {
    const int8_t num = int8_t(name);
    return num >= int8_t(REG_P1) && num <= int8_t(REG_P7);
}

bool RegI8048::isTimer(RegName name) {
    const int8_t num = int8_t(name);
    return num >= int8_t(REG_T) && num <= int8_t(REG_TCNTI);
}

uint8_t RegI8048::encodeRReg(RegName name) {
    return int8_t(name);
}

RegName RegI8048::decodeRReg(const uint8_t num) {
    return RegName(num & 7);
}


uint8_t RegI8048::encodePort(RegName name) {
    return int8_t(name) - int8_t(REG_BUS);
}

RegName RegI8048::decodePort(const uint8_t num) {
    return RegName((num & 7) + int8_t(REG_BUS));
}

}  // namespace i8048
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
