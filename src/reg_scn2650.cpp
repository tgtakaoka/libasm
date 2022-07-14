/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include "reg_scn2650.h"

#include "table_scn2650.h"

#include <ctype.h>

namespace libasm {
namespace scn2650 {

RegName RegScn2650::parseRegName(StrScanner &scan) {
    StrScanner p(scan);
    if (p.iexpect('R')) {
        const auto num = p.expect([](char c) { return c >= '0' && c <= '3'; });
        if (num) {
            scan = p;
            return RegName(num - '0');
        }
    }
    return REG_UNDEF;
}

uint8_t RegScn2650::encodeRegName(RegName name) {
    return uint8_t(name);
}

StrBuffer &RegScn2650::outRegName(StrBuffer &out, RegName name) const {
    return out.letter('R', isUppercase()).letter('0' + uint8_t(name));
}

// clang-format off
static const char TEXT_CC_EQ[] PROGMEM = "EQ";
static const char TEXT_CC_GT[] PROGMEM = "GT";
static const char TEXT_CC_LT[] PROGMEM = "LT";
static const char TEXT_CC_UN[] PROGMEM = "UN";
// clang-format on
static const RegBase::NameEntry CC_TABLE[] PROGMEM = {
        NAME_ENTRY(CC_EQ),
        NAME_ENTRY(CC_GT),
        NAME_ENTRY(CC_LT),
        NAME_ENTRY(CC_UN),
};

CcName RegScn2650::parseCcName(StrScanner &scan) {
    const NameEntry *entry = searchText(scan, ARRAY_RANGE(CC_TABLE));
    return entry ? CcName(entry->name()) : CC_UNDEF;
}

uint8_t RegScn2650::encodeCcName(CcName name) {
    return uint8_t(name);
}

StrBuffer &RegScn2650::outCcName(StrBuffer &out, CcName name) const {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(CC_TABLE));
    if (entry)
        out.text_P(entry->text_P(), isUppercase());
    return out;
}

}  // namespace scn2650
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
