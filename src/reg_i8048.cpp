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
using namespace libasm::reg;

namespace libasm {
namespace i8048 {
namespace reg {

static constexpr NameEntry REG_TABLE[] PROGMEM = {
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

RegName parseRegName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, const RegName name) {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        out.text_P(entry->text_P());
    return out;
}

bool isRReg(RegName name) {
    const auto num = int8_t(name);
    return num >= int8_t(REG_R0) && num <= int8_t(REG_R7);
}

bool isPort(RegName name) {
    const auto num = int8_t(name);
    return num >= int8_t(REG_P1) && num <= int8_t(REG_P7);
}

bool isTimer(RegName name) {
    const auto num = int8_t(name);
    return num >= int8_t(REG_T) && num <= int8_t(REG_TCNTI);
}

uint8_t encodeRRegName(RegName name) {
    return int8_t(name);
}

RegName decodeRRegNum(const uint8_t num) {
    return RegName(num & 7);
}

uint8_t encodePortName(RegName name) {
    return int8_t(name) - int8_t(REG_BUS);
}

RegName decodePortNum(const uint8_t num) {
    return RegName((num & 7) + int8_t(REG_BUS));
}

}  // namespace reg
}  // namespace i8048
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
