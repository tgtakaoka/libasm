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

static constexpr RegName STACK_S_REGS[8] PROGMEM = {
    REG_CC, REG_A, REG_B, REG_DP, REG_X, REG_Y, REG_U, REG_PC
};
static constexpr RegName STACK_U_REGS[8] PROGMEM = {
    REG_CC, REG_A, REG_B, REG_DP, REG_X, REG_Y, REG_S, REG_PC
};
static constexpr char CCR_BITS[8] PROGMEM = {
    'E', 'F', 'H', 'I', 'N', 'Z', 'V', 'C'
};
static constexpr RegName BIT_OP_REGS[] PROGMEM = {
    REG_CC, REG_A, REG_B, REG_UNDEF
};

static constexpr RegName TFM_BASE_REGS[] PROGMEM = {
    REG_D, REG_X, REG_Y, REG_U, REG_S
};
static constexpr char TFM_SRC_MODES[4] PROGMEM = {
    '+', '-', '+',   0
};
static constexpr char TFM_DST_MODES[4] PROGMEM = {
    '+', '-',   0, '+'
};

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static bool regCharCaseEqual(char c, char regChar) {
    return toupper(c) == toupper(regChar);
}

char RegMc6809::regName1stChar(const RegName regName) const {
    const char r = char(regName);
    if (isdigit(r)) return '0';
    return _uppercase ? toupper(r) : tolower(r);
}

char RegMc6809::regName2ndChar(const RegName regName) const {
    if (regName == REG_PC || regName == REG_PCR || regName == REG_CC)
        return _uppercase ? 'C' : 'c';
    if (regName == REG_DP)
        return _uppercase ? 'P' : 'p';
    if (regName == REG_00)
        return '0';
    return 0;
}

char RegMc6809::regName3rdChar(const RegName regName) const {
    if (regName == REG_PCR)
        return _uppercase ? 'R' : 'r';
    return 0;
}

bool RegMc6809::compareRegName(const char *line, RegName regName) const {
    if (!regCharCaseEqual(*line++, regName1stChar(regName))) return false;
    const char r2 = regName2ndChar(regName);
    if (r2 && !regCharCaseEqual(*line++, r2)) return false;
    const char r3 = regName3rdChar(regName);
    if (r3 && !regCharCaseEqual(*line++, r3)) return false;
    return !isidchar(*line);
}

host::uint_t RegMc6809::regNameLen(RegName regName) const {
    return regName2ndChar(regName) == 0 ? 1
        : (regName3rdChar(regName) == 0 ? 2 : 3);
}

char *RegMc6809::outRegName(char *out, const RegName regName) const {
    *out++ = regName1stChar(regName);
    const char r2 = regName2ndChar(regName);
    if (r2) {
        *out++ = r2;
        const char r3 = regName3rdChar(regName);
        if (r3)
            *out++ = r3;
    }
    *out = 0;
    return out;
}

char *RegMc6809::outCCRBits(char *out, uint8_t val) const {
    host::int_t n = 0;
    for (host::uint_t mask = 0x80, i = 0; i < 8; mask >>= 1, i++) {
        if (val & mask) {
            if (n == 1) { char b = *--out; *out++ = '('; *out++ = b; }
            if (n) *out++ = '|';
            char c = pgm_read_byte(&CCR_BITS[i]);
            *out++ = _uppercase ? c : tolower(c);
            n++;
        }
    }
    if (n >= 2) *out++ = ')';
    *out = 0;
    return out;
}

static host::int_t encodeRegNumber(
    RegName regName, const RegName *table, const RegName *end) {
    for (const RegName *p = table; p < end; p++) {
        if (pgm_read_byte(p) == regName) return p - table;
    }
    return -1;
}

RegName RegMc6809::parseRegName(
    const char *line, const RegName *table, const RegName *end) const {
    for (const RegName *p = table; p < end; p++) {
        const RegName regName = RegName(pgm_read_byte(p));
        if (compareRegName(line, regName)) return regName;
    }
    return REG_UNDEF;
}

static RegName decodeRegNumber(
    host::uint_t regNum, const RegName *table, const RegName *end) {
    const RegName *entry = &table[regNum];
    return entry < end ? RegName(pgm_read_byte(entry)) : REG_UNDEF;
}

RegName RegMc6809::getStackReg(host::uint_t bit, target::insn_t insnCode) {
    const RegName *table = (insnCode & 2) == 0
        ? &STACK_S_REGS[0] : &STACK_U_REGS[0];
    return RegName(pgm_read_byte(&table[bit]));
}

RegName RegMc6809::parseBitOpReg(const char *line) const {
    return parseRegName(line, ARRAY_RANGE(BIT_OP_REGS));
}

RegName RegMc6809::parseTfmBaseReg(const char *line) const {
    return parseRegName(line, ARRAY_RANGE(TFM_BASE_REGS));
}

host::int_t RegMc6809::encodeBitOpReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(BIT_OP_REGS));
}

host::int_t RegMc6809::encodeTfmBaseReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(TFM_BASE_REGS));
}

RegName RegMc6809::decodeBitOpReg(uint8_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(BIT_OP_REGS));
}

RegName RegMc6809::decodeTfmBaseReg(uint8_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(TFM_BASE_REGS));
}

char RegMc6809::tfmSrcModeChar(host::uint_t mode) {
    return pgm_read_byte(&TFM_SRC_MODES[mode]);
}

char RegMc6809::tfmDstModeChar(host::uint_t mode) {
    return pgm_read_byte(&TFM_DST_MODES[mode]);
}

static constexpr RegName MC6809_INDEX_REGS[] PROGMEM = {
    REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_B, REG_A, REG_UNDEF,
    REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_D
};

static constexpr RegName HD6309_INDEX_REGS[] PROGMEM = {
    REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_B, REG_A, REG_E,
    REG_UNDEF, REG_UNDEF, REG_F, REG_D, REG_UNDEF, REG_UNDEF, REG_W
};

static constexpr RegName MC6809_BASE_REGS[] PROGMEM = {
    REG_X, REG_Y, REG_U, REG_S
};

static constexpr RegName HD6309_BASE_REGS[] PROGMEM = {
    REG_X, REG_Y, REG_U, REG_S, REG_W
};

static constexpr RegName MC6809_DATA_REGS[] PROGMEM = {
    REG_D, REG_X, REG_Y,  REG_U, REG_S, REG_PC, REG_UNDEF, REG_UNDEF,
    REG_A, REG_B, REG_CC, REG_DP
};

static constexpr RegName HD6309_DATA_REGS[] PROGMEM = {
    REG_D, REG_X, REG_Y,  REG_U,  REG_S, REG_PC, REG_W, REG_V,
    REG_A, REG_B, REG_CC, REG_DP, REG_0, REG_00, REG_E, REG_F,
    REG_Z
};

RegName RegMc6809::parseIndexReg(const char *line) const {
    return TableMc6809.is6309()
        ? parseRegName(line, ARRAY_RANGE(HD6309_INDEX_REGS))
        : parseRegName(line, ARRAY_RANGE(MC6809_INDEX_REGS));
}

RegName RegMc6809::parseBaseReg(const char *line) const {
    return TableMc6809.is6309()
        ? parseRegName(line, ARRAY_RANGE(HD6309_BASE_REGS))
        : parseRegName(line, ARRAY_RANGE(MC6809_BASE_REGS));
}

RegName RegMc6809::parseDataReg(const char *line) const {
    return TableMc6809.is6309()
        ? parseRegName(line, ARRAY_RANGE(HD6309_DATA_REGS))
        : parseRegName(line, ARRAY_RANGE(MC6809_DATA_REGS));
}

host::int_t RegMc6809::encodeIndexReg(RegName regName) const {
    return TableMc6809.is6309()
        ? encodeRegNumber(regName, ARRAY_RANGE(HD6309_INDEX_REGS))
        : encodeRegNumber(regName, ARRAY_RANGE(MC6809_INDEX_REGS));
}

host::int_t RegMc6809::encodeBaseReg(RegName regName) const {
    return TableMc6809.is6309()
        ? encodeRegNumber(regName, ARRAY_RANGE(HD6309_BASE_REGS))
        : encodeRegNumber(regName, ARRAY_RANGE(MC6809_BASE_REGS));
}

host::int_t RegMc6809::encodeDataReg(RegName regName) const {
    if (TableMc6809.is6309()) {
        if (regName == REG_Z) regName = REG_0;
        return encodeRegNumber(regName, ARRAY_RANGE(HD6309_DATA_REGS));
    } 
    return encodeRegNumber(regName, ARRAY_RANGE(MC6809_DATA_REGS));
}

RegName RegMc6809::decodeIndexReg(uint8_t regNum) const {
    return TableMc6809.is6309()
        ? decodeRegNumber(regNum, ARRAY_RANGE(HD6309_INDEX_REGS))
        : decodeRegNumber(regNum, ARRAY_RANGE(MC6809_INDEX_REGS));
}

RegName RegMc6809::decodeBaseReg(uint8_t regNum) const {
    return TableMc6809.is6309()
        ? decodeRegNumber(regNum, ARRAY_RANGE(HD6309_BASE_REGS))
        : decodeRegNumber(regNum, ARRAY_RANGE(MC6809_BASE_REGS));
}

RegName RegMc6809::decodeRegName(uint8_t regNum) const {
    return TableMc6809.is6309()
        ? decodeRegNumber(regNum, ARRAY_RANGE(HD6309_DATA_REGS))
        : decodeRegNumber(regNum, ARRAY_RANGE(MC6809_DATA_REGS));
}

static constexpr RegName BYTE_REGS[] PROGMEM = {
    REG_A, REG_B, REG_CC, REG_DP, REG_E, REG_F
};
static constexpr RegName WORD_REGS[] PROGMEM = {
    REG_D, REG_X, REG_Y, REG_U, REG_S, REG_PC, REG_W, REG_V
};

OprSize RegMc6809::regSize(RegName regName) {
    if (encodeRegNumber(regName, ARRAY_RANGE(BYTE_REGS)) >= 0)
        return SZ_BYTE;
    if (encodeRegNumber(regName, ARRAY_RANGE(WORD_REGS)) >= 0)
        return SZ_WORD;
    return SZ_NONE; // REG_0, REG_00, REG_Z
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
