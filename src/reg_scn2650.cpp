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

#include "text_scn2650.h"

using namespace libasm::text::scn2650;

namespace libasm {
namespace scn2650 {

RegName RegScn2650::parseRegName(StrScanner &scan) {
    auto p = scan;
    if (p.iexpect('R')) {
        const auto num = parseRegNumber(p, 4);
        if (num >= 0) {
            scan = p;
            return RegName(num);
        }
    }
    return REG_UNDEF;
}

uint8_t RegScn2650::encodeRegName(RegName name) {
    return uint8_t(name);
}

RegName RegScn2650::decodeRegName(uint8_t opc) {
    return RegName(opc & 0x3);
}

StrBuffer &RegScn2650::outRegName(StrBuffer &out, RegName name) const {
    return outRegNumber(out.letter('R'), int8_t(name));
}

static constexpr RegBase::NameEntry CC_TABLE[] PROGMEM = {
        NAME_ENTRY(CC_EQ),
        NAME_ENTRY(CC_GT),
        NAME_ENTRY(CC_LT),
        NAME_ENTRY(CC_UN),
};

CcName RegScn2650::parseCcName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(CC_TABLE));
    return entry ? CcName(entry->name()) : CC_UNDEF;
}

uint8_t RegScn2650::encodeCcName(CcName name) {
    return uint8_t(name);
}

CcName RegScn2650::decodeCcName(uint8_t opc) {
    return CcName(opc & 0x3);
}

StrBuffer &RegScn2650::outCcName(StrBuffer &out, CcName name) const {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(CC_TABLE));
    if (entry)
        out.text_P(entry->text_P());
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
