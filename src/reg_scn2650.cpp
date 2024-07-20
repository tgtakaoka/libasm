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

#include "reg_base.h"
#include "text_scn2650.h"

using namespace libasm::reg;
using namespace libasm::text::scn2650;

namespace libasm {
namespace scn2650 {
namespace reg {

namespace {
// clang-format off

constexpr NameEntry CC_ENTRIES[] PROGMEM = {
    { TEXT_CC_EQ, CC_EQ },
    { TEXT_CC_GT, CC_GT },
    { TEXT_CC_LT, CC_LT },
    { TEXT_CC_UN, CC_UN },
};

PROGMEM constexpr NameTable CC_TABLE{ARRAY_RANGE(CC_ENTRIES)};

// clang-format on
}  // namespace

RegName parseRegName(StrScanner &scan) {
    auto p = scan;
    if (p.iexpect('R')) {
        const auto num = parseRegNumber(p);
        if (num < 0 || num >= 4)
            return REG_UNDEF;
        scan = p;
        return RegName(REG_R0 + num);
    }
    return REG_UNDEF;
}

uint8_t encodeRegName(RegName name) {
    return uint8_t(name);
}

RegName decodeRegName(uint8_t opc) {
    return RegName(opc & 0x3);
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    return out.letter('R').int16(int8_t(name));
}

CcName parseCcName(StrScanner &scan) {
    const auto *entry = CC_TABLE.searchText(scan);
    return entry ? CcName(entry->name()) : CC_UNDEF;
}

uint8_t encodeCcName(CcName name) {
    return uint8_t(name);
}

CcName decodeCcName(uint8_t opc) {
    return CcName(opc & 0x3);
}

StrBuffer &outCcName(StrBuffer &out, CcName name) {
    const auto *entry = CC_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

}  // namespace reg
}  // namespace scn2650
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
