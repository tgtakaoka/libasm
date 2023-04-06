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

#include "text_ins8060.h"

using namespace libasm::text::ins8060;
using namespace libasm::reg;

namespace libasm {
namespace ins8060 {
namespace reg {

static constexpr NameEntry REG_TABLE[] PROGMEM = {
        NAME_ENTRY(REG_E),
        NAME_ENTRY(REG_P1),
        NAME_ENTRY(REG_P2),
        NAME_ENTRY(REG_P3),
        NAME_ENTRY(REG_PC),
        NAME_ENTRY(REG_P0),
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

bool isPointerReg(RegName name) {
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

uint8_t encodePointerReg(RegName name) {
    if (name == REG_P0)
        return 0;
    return uint8_t(name);
}

RegName decodePointerReg(uint8_t num) {
    return RegName(num & 3);
}

}  // namespace reg
}  // namespace ins8060
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
