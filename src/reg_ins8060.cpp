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

#include "reg_ins8060.h"

#include <ctype.h>

#include "config_ins8060.h"

namespace libasm {
namespace ins8060 {

// clang-format off
static const char TEXT_REG_E[]  PROGMEM  = "E";
static const char TEXT_REG_P0[] PROGMEM = "P0";
static const char TEXT_REG_P1[] PROGMEM = "P1";
static const char TEXT_REG_P2[] PROGMEM = "P2";
static const char TEXT_REG_P3[] PROGMEM = "P3";
static const char TEXT_REG_PC[] PROGMEM = "PC";
// clang-format on
static constexpr RegBase::NameEntry REG_TABLE[] PROGMEM = {
        NAME_ENTRY(REG_E),
        NAME_ENTRY(REG_P1),
        NAME_ENTRY(REG_P2),
        NAME_ENTRY(REG_P3),
        NAME_ENTRY(REG_PC),
        NAME_ENTRY(REG_P0),
};

RegName RegIns8060::parseRegName(const char *line) {
    const NameEntry *entry = searchText(line, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

uint8_t RegIns8060::regNameLen(RegName name) {
    return nameLen(uint8_t(name), ARRAY_RANGE(REG_TABLE));
}

StrBuffer &RegIns8060::outRegName(StrBuffer &out, const RegName name) const {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        outText(out, entry->text());
    return out;
}

bool RegIns8060::isPointerReg(RegName name) {
    switch (name) {
    case REG_PC:
    case REG_P1:
    case REG_P2:
    case REG_P3:
    case REG_P0:
        return true;
    default:
        return false;
    }
}

uint8_t RegIns8060::encodePointerReg(RegName name) {
    if (name == REG_P0)
        return 0;
    return uint8_t(name);
}

RegName RegIns8060::decodePointerReg(uint8_t num) {
    return RegName(num & 3);
}

}  // namespace ins8060
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
