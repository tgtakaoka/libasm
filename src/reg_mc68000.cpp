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

#include "reg_mc68000.h"

namespace libasm {
namespace mc68000 {

static const char TEXT_REG_D0[] PROGMEM = "D0";
static const char TEXT_REG_D1[] PROGMEM = "D1";
static const char TEXT_REG_D2[] PROGMEM = "D2";
static const char TEXT_REG_D3[] PROGMEM = "D3";
static const char TEXT_REG_D4[] PROGMEM = "D4";
static const char TEXT_REG_D5[] PROGMEM = "D5";
static const char TEXT_REG_D6[] PROGMEM = "D6";
static const char TEXT_REG_D7[] PROGMEM = "D7";
static const char TEXT_REG_A0[] PROGMEM = "A0";
static const char TEXT_REG_A1[] PROGMEM = "A1";
static const char TEXT_REG_A2[] PROGMEM = "A2";
static const char TEXT_REG_A3[] PROGMEM = "A3";
static const char TEXT_REG_A4[] PROGMEM = "A4";
static const char TEXT_REG_A5[] PROGMEM = "A5";
static const char TEXT_REG_A6[] PROGMEM = "A6";
static const char TEXT_REG_A7[] PROGMEM = "A7";
static const char TEXT_REG_PC[] PROGMEM = "PC";
static const char TEXT_REG_SR[] PROGMEM = "SR";
static const char TEXT_REG_CCR[] PROGMEM = "CCR";
static const char TEXT_REG_USP[] PROGMEM = "USP";

static const RegBase::NameEntry REG_TABLE[] PROGMEM = {
        NAME_ENTRY(REG_D0),
        NAME_ENTRY(REG_D1),
        NAME_ENTRY(REG_D2),
        NAME_ENTRY(REG_D3),
        NAME_ENTRY(REG_D4),
        NAME_ENTRY(REG_D5),
        NAME_ENTRY(REG_D6),
        NAME_ENTRY(REG_D7),
        NAME_ENTRY(REG_A0),
        NAME_ENTRY(REG_A1),
        NAME_ENTRY(REG_A2),
        NAME_ENTRY(REG_A3),
        NAME_ENTRY(REG_A4),
        NAME_ENTRY(REG_A5),
        NAME_ENTRY(REG_A6),
        NAME_ENTRY(REG_A7),
        NAME_ENTRY(REG_PC),
        NAME_ENTRY(REG_SR),
        NAME_ENTRY(REG_CCR),
        NAME_ENTRY(REG_USP),
};

RegName RegMc68000::parseRegName(StrScanner &scan) {
    const NameEntry *entry = searchText(scan, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &RegMc68000::outRegName(StrBuffer &out, RegName name) {
    const NameEntry *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        out.text_P(entry->text_P(), isUppercase());
    return out;
}

bool RegMc68000::isDataReg(RegName name) {
    const int8_t num = int8_t(name);
    return num >= 0 && num < 8;
}

bool RegMc68000::isAddrReg(RegName name) {
    const int8_t num = int8_t(name) - 8;
    return num >= 0 && num < 8;
}

bool RegMc68000::isGeneralReg(RegName name) {
    const int8_t num = int8_t(name);
    return num >= 0 && num < 16;
}

Config::opcode_t RegMc68000::encodeGeneralRegNo(RegName name) {
    return int8_t(name) & 7;
}

uint8_t RegMc68000::encodeGeneralRegPos(RegName name) {
    return uint8_t(name);
}

RegName RegMc68000::decodeGeneralReg(uint8_t regno) {
    return RegName(regno & 0xF);
}

RegName RegMc68000::decodeDataReg(uint8_t regno) {
    return RegName(regno & 7);
}

RegName RegMc68000::decodeAddrReg(uint8_t regno) {
    return RegName((regno & 7) + 8);
}

OprSize RegMc68000::parseSize(StrScanner &scan) {
    StrScanner p(scan);
    if (p.expect('.')) {
        OprSize size = SZ_ERROR;
        if (p.iexpect('B')) {
            size = SZ_BYTE;
        } else if (p.iexpect('W')) {
            size = SZ_WORD;
        } else if (p.iexpect('L')) {
            size = SZ_LONG;
        }
        if (size == SZ_ERROR || isidchar(*p))
            return SZ_ERROR;
        scan = p;
        return size;
    }
    return SZ_NONE;
}

uint8_t RegMc68000::sizeNameLen(OprSize size) {
    return size == SZ_NONE || size == SZ_ERROR ? 0 : 2;
}

char RegMc68000::sizeSuffix(OprSize size) const {
    char base = isUppercase() ? 0 : 'a' - 'A';
    switch (size) {
    case SZ_BYTE:
        return base + 'B';
    case SZ_WORD:
        return base + 'W';
        break;
    case SZ_LONG:
        return base + 'L';
    default:
        return 0;
    }
}

Config::opcode_t EaMc68000::encodeMode(AddrMode mode) {
    const uint8_t m = static_cast<uint8_t>(mode);
    return m >= 8 ? 7 : m;
}

Config::opcode_t EaMc68000::encodeRegNo(AddrMode mode, RegName reg) {
    const uint8_t m = static_cast<uint8_t>(mode);
    if (m < 8)
        return RegMc68000::encodeGeneralRegNo(reg);
    return m - 8;
}

EaMc68000::EaMc68000(OprSize size_, uint8_t raw_mode, uint8_t regno) {
    regno &= 7;
    size = size_;
    if ((raw_mode &= 7) == 7) {
        mode = (regno < 5) ? AddrMode(regno + 8) : M_ERROR;
        reg = REG_UNDEF;
    } else {
        mode = AddrMode(raw_mode);
        reg = (mode == M_DREG) ? RegMc68000::decodeDataReg(regno)
                               : RegMc68000::decodeAddrReg(regno);
    }
}

OprSize BriefExt::indexSize() const {
    return (word & 0x800) ? SZ_LONG : SZ_WORD;
}

RegName BriefExt::index() const {
    return RegMc68000::decodeGeneralReg(word >> 12);
}

uint8_t BriefExt::disp() const {
    return static_cast<uint8_t>(word);
}

}  // namespace mc68000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
