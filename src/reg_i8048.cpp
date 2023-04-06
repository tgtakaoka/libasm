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

#include "text_i8048.h"

using namespace libasm::text::i8048;

namespace libasm {
namespace i8048 {

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
    const auto *entry = searchText(scan, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &RegI8048::outRegName(StrBuffer &out, const RegName name) const {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        out.text_P(entry->text_P(), isUppercase());
    return out;
}

bool RegI8048::isRReg(RegName name) {
    const auto num = int8_t(name);
    return num >= int8_t(REG_R0) && num <= int8_t(REG_R7);
}

bool RegI8048::isPort(RegName name) {
    const auto num = int8_t(name);
    return num >= int8_t(REG_P1) && num <= int8_t(REG_P7);
}

bool RegI8048::isTimer(RegName name) {
    const auto num = int8_t(name);
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
