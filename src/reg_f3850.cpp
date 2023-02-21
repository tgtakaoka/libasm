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

#include "table_f3850.h"

#include <ctype.h>

namespace libasm {
namespace f3850 {

// clang-format off
static constexpr char TEXT_REG_A[]  PROGMEM = "A";
static constexpr char TEXT_REG_W[]  PROGMEM = "W";
static constexpr char TEXT_REG_IS[] PROGMEM = "IS";
static constexpr char TEXT_REG_KU[] PROGMEM = "KU";
static constexpr char TEXT_REG_KL[] PROGMEM = "KL";
static constexpr char TEXT_REG_QU[] PROGMEM = "QU";
static constexpr char TEXT_REG_QL[] PROGMEM = "QL";
static constexpr char TEXT_REG_H[]  PROGMEM = "H";
static constexpr char TEXT_REG_K[]  PROGMEM = "K";
static constexpr char TEXT_REG_Q[]  PROGMEM = "Q";
static constexpr char TEXT_REG_P[]  PROGMEM = "P";
static constexpr char TEXT_REG_P0[] PROGMEM = "P0";
static constexpr char TEXT_REG_DC[] PROGMEM = "DC";
// register alias
static constexpr char TEXT_REG_J[]  PROGMEM = "J";
static constexpr char TEXT_REG_HU[] PROGMEM = "HU";
static constexpr char TEXT_REG_HL[] PROGMEM = "HL";
static constexpr char TEXT_REG_S[]  PROGMEM = "S";
static constexpr char TEXT_REG_I[]  PROGMEM = "I";
static constexpr char TEXT_REG_D[]  PROGMEM = "D";
// clang-format on
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
    StrScanner p(scan);
    const NameEntry *entry = searchText(p, ARRAY_RANGE(REG_TABLE));
    if (entry == nullptr || p.expect('\''))
        return REG_UNDEF;       // maybe H'xx' number
    scan = p;
    return RegName(entry->name());
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
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
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
