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

namespace libasm {
namespace i8086 {

// clang-format off
static const char TEXT_REG_AL[]   PROGMEM = "AL";
static const char TEXT_REG_BL[]   PROGMEM = "BL";
static const char TEXT_REG_CL[]   PROGMEM = "CL";
static const char TEXT_REG_DL[]   PROGMEM = "DL";
static const char TEXT_REG_AH[]   PROGMEM = "AH";
static const char TEXT_REG_BH[]   PROGMEM = "BH";
static const char TEXT_REG_CH[]   PROGMEM = "CH";
static const char TEXT_REG_DH[]   PROGMEM = "DH";
static const char TEXT_REG_AX[]   PROGMEM = "AX";
static const char TEXT_REG_BX[]   PROGMEM = "BX";
static const char TEXT_REG_CX[]   PROGMEM = "CX";
static const char TEXT_REG_DX[]   PROGMEM = "DX";
static const char TEXT_REG_SP[]   PROGMEM = "SP";
static const char TEXT_REG_BP[]   PROGMEM = "BP";
static const char TEXT_REG_SI[]   PROGMEM = "SI";
static const char TEXT_REG_DI[]   PROGMEM = "DI";
static const char TEXT_REG_CS[]   PROGMEM = "CS";
static const char TEXT_REG_DS[]   PROGMEM = "DS";
static const char TEXT_REG_ES[]   PROGMEM = "ES";
static const char TEXT_REG_SS[]   PROGMEM = "SS";
static const char TEXT_REG_PTR[]  PROGMEM  = "PTR";
static const char TEXT_REG_BYTE[] PROGMEM = "BYTE";
static const char TEXT_REG_WORD[] PROGMEM = "WORD";
// clang-format on
static const RegBase::NameEntry REG_TABLE[] PROGMEM = {
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

RegName RegI8086::parseRegName(StrScanner &scan) {
    const NameEntry *entry = searchText(scan, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &RegI8086::outRegName(StrBuffer &out, RegName name) const {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        out.text_P(entry->text_P(), isUppercase());
    return out;
}

RegName RegI8086::decodeByteReg(uint8_t num) {
    return RegName((num & 7) + 8);
}

RegName RegI8086::decodeWordReg(uint8_t num) {
    return RegName(num & 7);
}

RegName RegI8086::decodeSegReg(uint8_t num) {
    return RegName((num & 3) + 16);
}

bool RegI8086::isGeneralReg(RegName name) {
    const int8_t num = int8_t(name);
    return num >= 0 && num < 16;
}

bool RegI8086::isSegmentReg(RegName name) {
    const int8_t num = int8_t(name);
    return num >= 16 && num < 20;
}

OprSize RegI8086::generalRegSize(RegName name) {
    const int8_t num = int8_t(name);
    return num < 8 ? SZ_WORD : SZ_BYTE;
}

uint8_t RegI8086::encodeRegNum(RegName name) {
    const uint8_t num = uint8_t(name);
    if (num < 8)
        return num;
    if (num < 16)
        return num - 8;
    return num - 16;
}

}  // namespace i8086
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
