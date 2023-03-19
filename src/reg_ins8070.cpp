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

#include "reg_ins8070.h"

namespace libasm {
namespace ins8070 {

// clang-format off
static constexpr char TEXT_REG_A[]  PROGMEM  = "A";
static constexpr char TEXT_REG_E[]  PROGMEM  = "E";
static constexpr char TEXT_REG_EA[] PROGMEM = "EA";
static constexpr char TEXT_REG_T[]  PROGMEM  = "T";
static constexpr char TEXT_REG_S[]  PROGMEM  = "S";
static constexpr char TEXT_REG_PC[] PROGMEM = "PC";
static constexpr char TEXT_REG_SP[] PROGMEM = "SP";
static constexpr char TEXT_REG_P2[] PROGMEM = "P2";
static constexpr char TEXT_REG_P3[] PROGMEM = "P3";
// clang-format on
static constexpr RegBase::NameEntry REG_TABLE[] PROGMEM = {
        NAME_ENTRY(REG_A),
        NAME_ENTRY(REG_E),
        NAME_ENTRY(REG_EA),
        NAME_ENTRY(REG_T),
        NAME_ENTRY(REG_S),
        NAME_ENTRY(REG_PC),
        NAME_ENTRY(REG_SP),
        NAME_ENTRY(REG_P2),
        NAME_ENTRY(REG_P3),
};

RegName RegIns8070::parseRegName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &RegIns8070::outRegName(StrBuffer &out, const RegName name) const {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        out.text_P(entry->text_P(), isUppercase());
    return out;
}

bool RegIns8070::isPointerReg(RegName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 4;
}

uint8_t RegIns8070::encodePointerReg(RegName name) {
    return uint8_t(name);
}

RegName RegIns8070::decodePointerReg(uint8_t num) {
    return RegName(num & 3);
}

}  // namespace ins8070
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
