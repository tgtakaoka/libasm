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

#include "reg_i8051.h"

#include "text_i8051.h"

using namespace libasm::text::i8051;
using namespace libasm::reg;

namespace libasm {
namespace i8051 {
namespace reg {

static constexpr NameEntry REG_TABLE[] PROGMEM = {
        NAME_ENTRY(REG_A),
        NAME_ENTRY(REG_C),
        NAME_ENTRY(REG_AB),
        NAME_ENTRY(REG_R0),
        NAME_ENTRY(REG_R1),
        NAME_ENTRY(REG_R2),
        NAME_ENTRY(REG_R3),
        NAME_ENTRY(REG_R4),
        NAME_ENTRY(REG_R5),
        NAME_ENTRY(REG_R6),
        NAME_ENTRY(REG_R7),
        NAME_ENTRY(REG_PC),
        NAME_ENTRY(REG_DPTR),
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
    return num >= 0 && num < 8;
}

uint8_t encodeRRegName(RegName name) {
    return uint8_t(name);
}

RegName decodeRRegNum(const uint8_t num) {
    return RegName(num);
}

}  // namespace i8051
}  // namespace reg
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
