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

#include <ctype.h>

#include "config_mc68000.h"

namespace libasm {
namespace mc68000 {

RegName RegMc68000::parseRegName(const char *line) {
    const char c1 = toupper(*line++);
    const char c2 = toupper(*line++);
    if (c1 == 'A' || c1 == 'D') {
        if (c2 < '0' || c2 >= '8' || isidchar(*line))
            return REG_UNDEF;
        return c1 == 'D' ? decodeDataReg(c2 - '0') : decodeAddrReg(c2 - '0');
    }
    const char c3 = toupper(*line++);
    if (c1 == 'P' && c2 == 'C' && !isidchar(c3))
        return REG_PC;
    if (c1 == 'S' && c2 == 'R' && !isidchar(c3))
        return REG_SR;
    if (c1 == 'C' && c2 == 'C' && c3 == 'R' && !isidchar(*line))
        return REG_CCR;
    if (c1 == 'U' && c2 == 'S' && c3 == 'P' && !isidchar(*line))
        return REG_USP;
    return REG_UNDEF;
}

uint8_t RegMc68000::regNameLen(RegName name) {
    if (name == REG_UNDEF)
        return 0;
    return (name == REG_CCR || name == REG_USP) ? 3 : 2;
}

static const char TEXT_REG_PC[] PROGMEM = "PC";
static const char TEXT_REG_SR[] PROGMEM = "SR";
static const char TEXT_REG_CCR[] PROGMEM = "CCR";
static const char TEXT_REG_USP[] PROGMEM = "USP";

StrBuffer &RegMc68000::outRegName(StrBuffer &out, RegName name) {
    if (isDataReg(name))
        outChar(out, 'D').letter('0' + int8_t(name));
    if (isAddrReg(name))
        return outChar(out, 'A').letter('0' + int8_t(name) - 8);
    if (name == REG_CCR)
        return outText(out, TEXT_REG_CCR);
    if (name == REG_PC)
        return outText(out, TEXT_REG_PC);
    if (name == REG_SR)
        return outText(out, TEXT_REG_SR);
    if (name == REG_USP)
        return outText(out, TEXT_REG_USP);
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

OprSize RegMc68000::parseSize(const char *line) {
    if (*line++ != '.')
        return SZ_NONE;
    const char c = *line++;
    if (isidchar(*line))
        return SZ_ERROR;
    switch (c & ~0x20) {
    case 'B':
        return SZ_BYTE;
    case 'W':
        return SZ_WORD;
    case 'L':
        return SZ_LONG;
    default:
        return SZ_ERROR;
    }
}

uint8_t RegMc68000::sizeNameLen(OprSize size) {
    return size == SZ_NONE || size == SZ_ERROR ? 0 : 2;
}

char RegMc68000::sizeSuffix(OprSize size) const {
    char suffix;
    switch (size) {
    case SZ_BYTE:
        suffix = 'B';
        break;
    case SZ_WORD:
        suffix = 'W';
        break;
    case SZ_LONG:
        suffix = 'L';
        break;
    default:
        return 0;
    }
    if (_uppercase)
        return suffix;
    return suffix | 0x20;
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
