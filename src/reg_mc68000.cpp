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

#include "config_mc68000.h"
#include "reg_mc68000.h"

#include <ctype.h>

namespace libasm {
namespace mc68000 {

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static void toUppercase(char *p) {
    while (*p) {
        *p = toupper(*p);
        p++;
    }
}

char *RegMc68000::outRegName(char *out, RegName regName) const {
    char *p = out;
    if (isDreg(regName)) {
        *out++ = 'd';
        *out++ = char(regName) - char(REG_D0) + '0';
    } else if (isAreg(regName)) {
        *out++ = 'a';
        *out++ = char(regName) - char(REG_A0) + '0';
    } else if (regName == REG_CCR) {
        *out++ = 'c';
        *out++ = 'c';
        *out++ = 'r';
    } else if (regName == REG_PC) {
        *out++ = 'p';
        *out++ = 'c';
    } else if (regName == REG_SR) {
        *out++ = 's';
        *out++ = 'r';
    } else if (regName == REG_USP) {
        *out++ = 'u';
        *out++ = 's';
        *out++ = 'p';
    }
    *out = 0;
    if (_uppercase) toUppercase(p);
    return out;
}

char *RegMc68000::outEaSize(char *out, EaSize size) const {
    char *p = out;
    *out++ = '.';
    switch (size) {
    case SZ_BYTE: *out++ = 'b'; break;
    case SZ_WORD: *out++ = 'w'; break;
    case SZ_LONG: *out++ = 'l'; break;
    default: out--; break;
    }
    *out = 0;
    if (_uppercase) toUppercase(p);
    return out;
}

bool RegMc68000::isDreg(RegName reg) {
    const char r = char(reg);
    return r >= char(REG_D0) && r <= char(REG_D7);
}

bool RegMc68000::isAreg(RegName reg) {
    const char r = char(reg);
    return r >= char(REG_A0) && r <= char(REG_A7);
}

bool RegMc68000::isADreg(RegName reg) {
    return isAreg(reg) || isDreg(reg);
}

Config::opcode_t RegMc68000::encodeRegNo(RegName reg) {
    return isDreg(reg)
        ? char(reg) - char(REG_D0)
        : char(reg) - char(REG_A0);
}

uint8_t RegMc68000::encodeRegPos(RegName reg) {
    return isDreg(reg)
        ? char(reg) - char(REG_D0)
        : char(reg) - char(REG_A0) + 8;
}

RegName RegMc68000::decodeDataReg(uint8_t regno) {
    return RegName(char(REG_D0) + (regno & 7));
}

RegName RegMc68000::decodeAddrReg(uint8_t regno) {
    return RegName(char(REG_A0) + (regno & 7));
}

uint8_t RegMc68000::regNameLen(RegName regName) {
    if (regName == REG_UNDEF) return 0;
    return (regName == REG_CCR || regName == REG_USP) ? 3 : 2;
}

RegName RegMc68000::parseRegName(const char *line) {
    const char c1 = toupper(*line++);
    const char c2 = toupper(*line++);
    if (c1 == 'A' || c1 == 'D') {
        if (c2 < '0' || c2 >= '8' || isidchar(*line)) return REG_UNDEF;
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

static EaMode parseEaMode(uint8_t mode, uint8_t regno) {
    if (mode == 7) {
        switch (regno) {
        case 0: return M_ABS_SHORT;
        case 1: return M_ABS_LONG;
        case 2: return M_PC_DISP;
        case 3: return M_PC_INDX;
        case 4: return M_IMM_DATA;
        default: return M_ILLEGAL;
        }
    }
    return EaMode(mode);
}

Config::opcode_t EaMc68000::encodeMode(EaMode mode) {
    const uint8_t m = static_cast<uint8_t>(mode);
    return m >= 8 ? 7 : m;
}

Config::opcode_t EaMc68000::encodeRegNo(EaMode mode, RegName regName) {
    const uint8_t m = static_cast<uint8_t>(mode);
    if (m < 8) return RegMc68000::encodeRegNo(regName);
    if (m < 16) return m - 8;
    return 0;
}

static EaCat getCategories(EaMode mode) {
    switch (mode) {
    case M_DREG:
        return EaCat::DATA | EaCat::ALTERABLE;
    case M_AREG:
        return EaCat::ALTERABLE;
    case M_AIND:
    case M_DISP:
    case M_INDX:
    case M_ABS_SHORT:
    case M_ABS_LONG:
    case M_LABEL:
        return EaCat::DATA | EaCat::MEMORY | EaCat::CONTROL | EaCat::ALTERABLE;
    case M_PINC:
    case M_PDEC:
        return EaCat::DATA | EaCat::MEMORY | EaCat::ALTERABLE;
    case M_PC_DISP:
    case M_PC_INDX:
        return EaCat::DATA | EaCat::MEMORY | EaCat::CONTROL;
    case M_IMM_DATA:
        return EaCat::DATA;
    /* for assembler operand parsing */
    case M_NONE:
        return EaCat::NONE;
    case M_MULT_REGS:
        return EaCat::ALTERABLE;
    default:
        return EaCat::NONE;
    }
}

const char *EaMc68000::eaCategory(EaMode mode) {
    const EaCat categories = getCategories(mode);
    static char buf[5];
    buf[0] = (categories & EaCat::DATA) ? 'D' : '_';
    buf[1] = (categories & EaCat::MEMORY) ? 'M' : '_';
    buf[2] = (categories & EaCat::CONTROL) ? 'C' : '_';
    buf[3] = (categories & EaCat::ALTERABLE) ? 'A' : '_';
    buf[4] = 0;
    return buf;
}

static RegName encodeRegName(EaMode mode, uint8_t regno) {
    switch (mode) {
    case M_DREG:
        return RegMc68000::decodeDataReg(regno);
    case M_ABS_SHORT:
    case M_ABS_LONG:
    case M_PC_DISP:
    case M_IMM_DATA:
    case M_ILLEGAL:
        return REG_UNDEF;
    default:
        return RegMc68000::decodeAddrReg(regno);
    }
}

EaMc68000::EaMc68000(Config::opcode_t insnCode) {
    const uint8_t regno = insnCode & 7;
    size = EaSize((insnCode >> 6) & 3);
    mode = parseEaMode((insnCode >> 3) & 7, regno);
    reg = encodeRegName(mode, regno);
}

EaMc68000::EaMc68000(EaSize size_, uint8_t raw_mode, uint8_t regno) {
    regno &= 7;
    size = size_;
    mode = parseEaMode(raw_mode & 7, regno);
    reg = encodeRegName(mode, regno);
}

EaMc68000::EaMc68000(EaSize size_, EaMode mode_, uint8_t regno) {
    size = size_;
    mode = mode_;
    reg = encodeRegName(mode, regno);
}

bool EaMc68000::satisfy(EaMode mode, EaCat categories) {
    return (getCategories(mode) & categories) == categories;
}

EaSize BriefExt::indexSize() const {
    return (word & 0x800) ? SZ_LONG : SZ_WORD;
}

RegName BriefExt::index() const {
    const RegName base = (word & 0x8000) ? REG_A0 : REG_D0;
    const uint8_t regno = (word >> 12) & 7;
    return RegName(char(base) + regno);
}

uint8_t BriefExt::disp() const {
    return static_cast<uint8_t>(word);
}

} // namespace mc68000
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
