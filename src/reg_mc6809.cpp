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

#include "config_mc6809.h"
#include "reg_mc6809.h"
#include "table_mc6809.h"

#include <ctype.h>

namespace libasm {
namespace mc6809 {

static const char TEXT_REG_A[]   PROGMEM = "A";
static const char TEXT_REG_B[]   PROGMEM = "B";
static const char TEXT_REG_D[]   PROGMEM = "D";
static const char TEXT_REG_X[]   PROGMEM = "X";
static const char TEXT_REG_Y[]   PROGMEM = "Y";
static const char TEXT_REG_U[]   PROGMEM = "U";
static const char TEXT_REG_S[]   PROGMEM = "S";
static const char TEXT_REG_PC[]  PROGMEM = "PC";
static const char TEXT_REG_CC[]  PROGMEM = "CC";
static const char TEXT_REG_DP[]  PROGMEM = "DP";
static const char TEXT_REG_PCR[] PROGMEM = "PCR";
static const char TEXT_REG_W[]   PROGMEM = "W";
static const char TEXT_REG_E[]   PROGMEM = "E";
static const char TEXT_REG_F[]   PROGMEM = "F";
static const char TEXT_REG_V[]   PROGMEM = "V";
static const char TEXT_REG_Z[]   PROGMEM = "Z";
static const char TEXT_REG_0[]   PROGMEM = "0";
static constexpr RegBase::NameEntry REG_TABLE[] PROGMEM = {
    NAME_ENTRY(REG_A)
    NAME_ENTRY(REG_B)
    NAME_ENTRY(REG_D)
    NAME_ENTRY(REG_X)
    NAME_ENTRY(REG_Y)
    NAME_ENTRY(REG_U)
    NAME_ENTRY(REG_S)
    NAME_ENTRY(REG_PC)
    NAME_ENTRY(REG_CC)
    NAME_ENTRY(REG_DP)
    NAME_ENTRY(REG_PCR)
    NAME_ENTRY(REG_W)
    NAME_ENTRY(REG_E)
    NAME_ENTRY(REG_F)
    NAME_ENTRY(REG_V)
    NAME_ENTRY(REG_Z)
    NAME_ENTRY(REG_0)
};

RegName RegMc6809::parseRegName(const char *line) {
    const NameEntry *entry = searchText(line, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

uint8_t RegMc6809::regNameLen(RegName name) {
    return nameLen(uint8_t(name), ARRAY_RANGE(REG_TABLE));
}

RegSize RegMc6809::regSize(RegName name) {
    const int8_t num = int8_t(name);
    if (num < 0) return SZ_NONE;   // REG_UNDEF
    if (num < 8) return SZ_WORD;   // REG_D..REG_V
    if (num < 12) return SZ_BYTE;  // REG_A..REG_DP
    if (num >= 14) return SZ_BYTE; // REG_E..REG_F
    return SZ_NONE;                // REG_Z..REG_0

}

char *RegMc6809::outRegName(char *out, const RegName name) const {
    const NameEntry *entry = searchName(name, ARRAY_RANGE(REG_TABLE));
    if (entry)
        out = outText(out, entry->text());
    return out;
}

RegName RegMc6809::decodeDataReg(uint8_t num) {
    num &= 0xF;
    const RegName name = RegName(num);
    if (TableMc6809.cpuType() == MC6809) {
        if (name == REG_W || name == REG_V || num >= 12)
            return REG_UNDEF;
    }
    return (name == REG_0) ? REG_Z : name;
}

bool RegMc6809::isDataReg(RegName name) {
    if (name == REG_UNDEF) return false;
    switch (name) {
    case REG_W: case REG_V:
    case REG_E: case REG_F: case REG_Z: case REG_0:
        return TableMc6809.cpuType() == HD6309;
    case REG_PCR:
        return false;
    default:
        return true;
    }
}

uint8_t RegMc6809::encodeDataReg(RegName name) {
    if (name == REG_0) name = REG_Z;
    return uint8_t(name);
}

RegName RegMc6809::decodeBaseReg(uint8_t num) {
    return RegName((num & 3) + 1);
}

bool RegMc6809::isBaseReg(RegName name) {
    switch (name) {
    case REG_X: case REG_Y: case REG_U: case REG_S:
    case REG_W:
        return true;
    default:
        return false;
    }
}

bool RegMc6809::isIndexedBase(RegName name) {
    switch (name) {
    case REG_X: case REG_Y: case REG_U: case REG_S:
    case REG_PC: case REG_PCR:
    case REG_W:
        return true;
    default:
        return false;
    }
}

uint8_t RegMc6809::encodeBaseReg(RegName name) {
    // REG_W is handled separately in TableMc6809::searchPostSpec().
    return uint8_t(name) - 1;
}

static constexpr RegName STACK_REGS[8] PROGMEM = {
    REG_CC, REG_A, REG_B, REG_DP, REG_X, REG_Y, REG_U, REG_PC
};

uint8_t RegMc6809::encodeStackReg(RegName name) {
    if (name == REG_D) return 0x06;
    uint8_t bit = 0x01;
    for (const /*PROGMEM*/ RegName *r = ARRAY_BEGIN(STACK_REGS);
         r < ARRAY_END(STACK_REGS);
         r++, bit <<= 1) {
        if (pgm_read_byte(r) == name) return bit;
    }
    return bit;
}

RegName RegMc6809::decodeStackReg(uint8_t bitPos) {
    return RegName(pgm_read_byte(&STACK_REGS[bitPos]));
}

RegName RegMc6809::decodeBitOpReg(uint8_t num) {
    switch (num) {
    case 0: return REG_CC;
    case 1: return REG_A;
    case 2: return REG_B;
    default: return REG_UNDEF;
    }
}

bool RegMc6809::isBitOpReg(RegName name) {
    const uint8_t num = uint8_t(name);
    return num >= 8 && num < 11;
}

uint8_t RegMc6809::encodeBitOpReg(RegName name) {
    if (name == REG_CC) return 0;
    return uint8_t(name) - uint8_t(REG_A) + 1;
}


RegName RegMc6809::decodeTfmBaseReg(uint8_t num) {
    num &= 0xF;
    return num < 5 ? RegName(num) : REG_UNDEF;
}

bool RegMc6809::isTfmBaseReg(RegName name) {
    const int8_t num = int8_t(name);
    return num >= 0 && num < 5;
}

uint8_t RegMc6809::encodeTfmBaseReg(RegName name) {
    return uint8_t(name);
}

char RegMc6809::tfmSrcModeChar(uint8_t mode) {
    if (mode == 0 || mode == 2) return '+';
    return mode == 1 ? '-' : 0;
}

char RegMc6809::tfmDstModeChar(uint8_t mode) {
    if (mode == 0 || mode == 3) return '+';
    return mode == 1 ? '-' : 0;
}

} // namespace mc6809
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
