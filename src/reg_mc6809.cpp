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

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

struct RegEntry {
    RegName _name;
    const /*PROGMEM*/ char *_text;

    static const RegEntry *searchName(
            RegName name, const RegEntry *begin, const RegEntry *end) {
        for (const RegEntry *entry = begin; entry < end; entry++) {
            if (entry->name() == name) return entry;
        }
        return nullptr;
    }
    static const RegEntry *searchText(
            const char *str, const RegEntry *begin, const RegEntry *end) {
        for (const RegEntry *entry = begin; entry < end; entry++) {
            const char *text = entry->text();
            const uint8_t len = strlen_P(text);
            if (strncasecmp_P(str, text, len) == 0 && !isidchar(str[len]))
                return entry;
        }
        return nullptr;
    }
    static uint8_t nameLen(RegName name) {
        if (name == REG_UNDEF)
            return 0;
        if (name == REG_PCR)
            return 3;
        if (name == REG_PC || name == REG_CC || name == REG_DP)
            return 2;
        return 1;
    }
    
    inline RegName name() const {
        return RegName(pgm_read_byte(&this->_name));
    }
    inline const /*PROGMEM*/ char *text() const {
        return reinterpret_cast<const char *>(pgm_read_ptr(&this->_text));
    }
};

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
static constexpr RegEntry REG_TABLE[] PROGMEM = {
    { REG_A,   TEXT_REG_A },
    { REG_B,   TEXT_REG_B },
    { REG_D,   TEXT_REG_D },
    { REG_X,   TEXT_REG_X },
    { REG_Y,   TEXT_REG_Y },
    { REG_U,   TEXT_REG_U },
    { REG_S,   TEXT_REG_S },
    { REG_PC,  TEXT_REG_PC },
    { REG_CC,  TEXT_REG_CC },
    { REG_DP,  TEXT_REG_DP },
    { REG_PCR, TEXT_REG_PCR },
    { REG_W,   TEXT_REG_W },
    { REG_E,   TEXT_REG_E },
    { REG_F,   TEXT_REG_F },
    { REG_V,   TEXT_REG_V },
    { REG_Z,   TEXT_REG_Z },
    { REG_0,   TEXT_REG_0 },
};

RegName RegMc6809::parseRegName(const char *line) const {
    const RegEntry *entry = RegEntry::searchText(line, ARRAY_RANGE(REG_TABLE));
    return entry ? entry->name() : REG_UNDEF;
}

uint8_t RegMc6809::regNameLen(RegName name) const {
    return RegEntry::nameLen(name);
}

RegSize RegMc6809::regSize(RegName name) const {
    const int8_t num = int8_t(name) - 8;
    if (num >= 0 && num < 8) return SZ_WORD;
    if (num >= 8 && num < 12) return SZ_BYTE;
    if (num >= 14) return SZ_BYTE;
    return SZ_NONE;
}

char *RegMc6809::outRegName(char *out, const RegName name) const {
    const RegEntry *entry = RegEntry::searchName(name, ARRAY_RANGE(REG_TABLE));
    if (entry) {
        const char *text = entry->text();
        char c;
        while ((c = pgm_read_byte(text++)) != 0)
            *out++ = _uppercase ? c : tolower(c);
        *out = 0;
    }
    return out;
}

RegName RegMc6809::decodeDataReg(uint8_t num) const {
    if (num == 13) num = 12;
    if (TableMc6809.cpuType() == MC6809) {
        if (num == 6 || num == 7 || num >= 12)
            return REG_UNDEF;
    }
    return RegName(num + 8);
}

int8_t RegMc6809::encodeDataReg(RegName name) const {
    int8_t num = int8_t(name) - 8;
    if (num < 0) return -1;
    if (num == 13) num = 12;
    if (TableMc6809.cpuType() == MC6809) {
        if (num == 6 || num == 7 || num >= 12)
            return -1;
    }
    return num;
}

RegName RegMc6809::decodeBaseReg(uint8_t num) const {
    if (num == 4 && TableMc6809.cpuType() == HD6309)
        return REG_W;
    switch (num) {
    case 0:  return REG_X;
    case 1:  return REG_Y;
    case 2:  return REG_U;
    case 3:  return REG_S;
    default: return REG_UNDEF;
    }
}

int8_t RegMc6809::encodeBaseReg(RegName name) const {
    if (TableMc6809.cpuType() == HD6309 && name == REG_W)
        return 4;
    switch (name) {
    case REG_X: return 0;
    case REG_Y: return 1;
    case REG_U: return 2;
    case REG_S: return 3;
    default:    return -1;
    }
}

bool RegMc6809::isBaseReg(RegName name) const {
    switch (name) {
    case REG_X:
    case REG_Y:
    case REG_U:
    case REG_S:
    case REG_W: return true;
    default:    return false;
    }
}

int8_t RegMc6809::encodeIndexReg(RegName name) const {
    if (TableMc6809.cpuType() == HD6309) {
        switch (name) {
        case REG_W: return 0xE;
        case REG_E: return 0x7;
        case REG_F: return 0xA;
        default: break;
        }
    }
    switch (name) {
    case REG_D: return 0xB;
    case REG_A: return 0x6;
    case REG_B: return 0x5;
    default:    return -1;
    }
}

bool RegMc6809::isIndexReg(RegName name) const {
    switch (name) {
    case REG_D:
    case REG_A:
    case REG_B:
    case REG_W:
    case REG_E:
    case REG_F: return true;
    default:    return false;
    }
}

bool RegMc6809::isIndexedBase(RegName name) const {
    return isBaseReg(name)
        || name == REG_PCR
        || name == REG_PC;
}

static constexpr RegName STACK_REGS[8] PROGMEM = {
    REG_CC, REG_A, REG_B, REG_DP, REG_X, REG_Y, REG_U, REG_PC
};

uint8_t RegMc6809::encodeStackReg(RegName name, bool onUserStack) const {
    if (onUserStack) {
        if (name == REG_U) return 0;
        if (name == REG_S) name = REG_U;
    }
    if (name == REG_D) return 0x06;
    for (uint8_t i = 0; i < sizeof(STACK_REGS); i++) {
        if (pgm_read_byte(&STACK_REGS[i]) == name) return 1 << i;
    }
    return 0;
}

RegName RegMc6809::decodeStackReg(uint8_t bitPos, bool onUserStack) const {
    const RegName name = RegName(pgm_read_byte(&STACK_REGS[bitPos]));
    return (onUserStack && name == REG_U) ? REG_S : name;
}

RegName RegMc6809::decodeBitOpReg(uint8_t num) const {
    switch (num) {
    case 0: return REG_CC;
    case 1: return REG_A;
    case 2: return REG_B;
    default: return REG_UNDEF;
    }
}

bool RegMc6809::isBitOpReg(RegName name) const {
    return name == REG_CC || name == REG_A || name == REG_B;
}

int8_t RegMc6809::encodeBitOpReg(RegName name) const {
    switch (name) {
    case REG_CC: return 0;
    case REG_A:  return 1;
    case REG_B:  return 2;
    default:     return -1;
    }
}


RegName RegMc6809::decodeTfmBaseReg(uint8_t num) const {
    return num < 5 ? RegName(num + 8) : REG_UNDEF;
}

bool RegMc6809::isTfmBaseReg(RegName name) const {
    return encodeTfmBaseReg(name) >= 0;
}

char RegMc6809::tfmSrcModeChar(uint8_t mode) const {
    if (mode == 0 || mode == 2) return '+';
    return mode == 1 ? '-' : 0;
}

char RegMc6809::tfmDstModeChar(uint8_t mode) const {
    if (mode == 0 || mode == 3) return '+';
    return mode == 1 ? '-' : 0;
}

int8_t RegMc6809::encodeTfmBaseReg(RegName name) const {
    const int8_t num = int8_t(name) - 8;
    return num >= 0 && num < 5 ? num : -1;
}

} // namespace mc6809
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
