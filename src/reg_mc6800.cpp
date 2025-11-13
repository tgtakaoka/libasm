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

#include "reg_mc6800.h"

#include "reg_base.h"
#include "text_mc6800.h"

using namespace libasm::reg;
using namespace libasm::text::mc6800;

namespace libasm {
namespace mc6800 {
namespace reg {

namespace {
// clang-format off
constexpr NameEntry CC_ENTRIES[] PROGMEM = {
    { TEXT_CC_CC, CC_CC },
    { TEXT_CC_CS, CC_CS },
    { TEXT_CC_EQ, CC_EQ },
    { TEXT_CC_GE, CC_GE },
    { TEXT_CC_GT, CC_GT },
    { TEXT_CC_HI, CC_HI },
    { TEXT_CC_HS, CC_HS },
    { TEXT_CC_LE, CC_LE },
    { TEXT_CC_LO, CC_LO },
    { TEXT_CC_LS, CC_LS },
    { TEXT_CC_LT, CC_LT },
    { TEXT_CC_MI, CC_MI },
    { TEXT_CC_NE, CC_NE },
    { TEXT_CC_PL, CC_PL },
    { TEXT_CC_RA, CC_RA },
    { TEXT_CC_VC, CC_VC },
    { TEXT_CC_VS, CC_VS },
};
// clang-format on

PROGMEM constexpr NameTable CC_TABLE{ARRAY_RANGE(CC_ENTRIES)};
}  // namespace

RegName parseRegName(StrScanner &scan) {
    if (scan.iexpectWord_P(TEXT_REG_X))
        return REG_X;
    if (scan.iexpectWord_P(TEXT_REG_Y))
        return REG_Y;
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    return out.letter(char(name));
}

CcName parseCcName(StrScanner &scan) {
    const auto *entry = CC_TABLE.searchText(scan);
    if (entry) {
        return CcName(entry->name());
    }
    return CC_UNDEF;
}

StrBuffer &outCcName(StrBuffer &out, CcName name) {
    const auto *entry = CC_TABLE.searchName(name);
    return entry ? entry->outText(out) : out;
}

uint_fast8_t encodeCcName(CcName name) {
    return uint8_t(name);
}

CcName decodeCcName(uint_fast8_t num) {
    num &= 0x0F;
    return num == 1 ? CC_UNDEF : CcName(num & 0x0F);
}

}  // namespace reg
}  // namespace mc6800
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
