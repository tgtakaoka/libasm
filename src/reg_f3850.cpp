/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "reg_f3850.h"

#include "text_f3850.h"

using namespace libasm::text::f3850;

namespace libasm {
namespace f3850 {

static constexpr RegBase::NameEntry REG_TABLE[] PROGMEM = {
        NAME_ENTRY(REG_A),
        NAME_ENTRY(REG_W),
        NAME_ENTRY(REG_IS),
        NAME_ENTRY(REG_KU),
        NAME_ENTRY(REG_KL),
        NAME_ENTRY(REG_K),
        NAME_ENTRY(REG_QU),
        NAME_ENTRY(REG_QL),
        NAME_ENTRY(REG_Q),
        NAME_ENTRY(REG_HU),
        NAME_ENTRY(REG_HL),
        NAME_ENTRY(REG_H),
        NAME_ENTRY(REG_P),
        NAME_ENTRY(REG_P0),
        NAME_ENTRY(REG_DC),
        NAME_ENTRY(REG_J),
        NAME_ENTRY(REG_S),
        NAME_ENTRY(REG_I),
        NAME_ENTRY(REG_D),
};

RegName RegF3850::parseRegName(StrScanner &scan) {
    auto p = scan;
    const auto *entry = searchText(p, ARRAY_RANGE(REG_TABLE));
    if (entry == nullptr)
        return REG_UNDEF;
    const auto name = RegName(entry->name());
    if ((name == REG_H || name == REG_D) && p.expect('\''))
        return REG_UNDEF;       // H'xx' and D'xx'
    scan = p;
    return name;
}

RegName RegF3850::decodeRegName(Config::opcode_t opc) {
    static constexpr auto regJ = uint8_t(REG_J) - uint8_t(REG_alias);
    static constexpr auto regD = uint8_t(REG_D) - uint8_t(REG_alias);
    const auto regno = opc & 0xF;
    if (regno >= regJ && regno <= regD)
        return RegName(regno + uint8_t(REG_alias));
    return REG_UNDEF;
}

StrBuffer &RegF3850::outRegName(StrBuffer &out, RegName name) const {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        out.text_P(entry->text_P(), isUppercase());
    return out;
}

}  // namespace f3850
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
