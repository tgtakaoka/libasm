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

#include "reg_i8086.h"

#include "text_i8086.h"

using namespace libasm::text::i8086;
using namespace libasm::reg;

namespace libasm {
namespace i8086 {
namespace reg {

static constexpr NameEntry REG_TABLE[] PROGMEM = {
        NAME_ENTRY(REG_AL),
        NAME_ENTRY(REG_AH),
        NAME_ENTRY(REG_AX),
        NAME_ENTRY(REG_BL),
        NAME_ENTRY(REG_BH),
        NAME_ENTRY(REG_BX),
        NAME_ENTRY(REG_CL),
        NAME_ENTRY(REG_CH),
        NAME_ENTRY(REG_CX),
        NAME_ENTRY(REG_DL),
        NAME_ENTRY(REG_DH),
        NAME_ENTRY(REG_DX),
        NAME_ENTRY(REG_BP),
        NAME_ENTRY(REG_SP),
        NAME_ENTRY(REG_SI),
        NAME_ENTRY(REG_DI),
        NAME_ENTRY(REG_CS),
        NAME_ENTRY(REG_DS),
        NAME_ENTRY(REG_ES),
        NAME_ENTRY(REG_SS),
        NAME_ENTRY(REG_PTR),
        NAME_ENTRY(REG_BYTE),
        NAME_ENTRY(REG_WORD),
};

RegName parseRegName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        out.text_P(entry->text_P());
    return out;
}

RegName decodeByteReg(uint8_t num) {
    return RegName((num & 7) + 8);
}

RegName decodeWordReg(uint8_t num) {
    return RegName(num & 7);
}

RegName decodeSegReg(uint8_t num) {
    return RegName((num & 3) + 16);
}

bool isGeneralReg(RegName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 16;
}

bool isSegmentReg(RegName name) {
    const auto num = int8_t(name);
    return num >= 16 && num < 20;
}

OprSize generalRegSize(RegName name) {
    const auto num = int8_t(name);
    return num < 8 ? SZ_WORD : SZ_BYTE;
}

uint8_t encodeRegNum(RegName name) {
    const auto num = uint8_t(name);
    if (num < 8)
        return num;
    if (num < 16)
        return num - 8;
    return num - 16;
}

}  // namespace reg
}  // namespace i8086
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
