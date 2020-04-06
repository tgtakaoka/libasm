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

#include "config_i8080.h"
#include "reg_i8080.h"

#include <ctype.h>

namespace libasm {
namespace i8080 {

static constexpr RegName ALL_REGS[] PROGMEM = {
    REG_A, REG_B, REG_C, REG_D, REG_E, REG_H, REG_L, REG_SP, REG_PSW
};

static constexpr RegName POINTER_REGS[] PROGMEM = {
    REG_B, REG_D, REG_H, REG_SP
};
static constexpr RegName STACK_REGS[] PROGMEM = {
    REG_B, REG_D, REG_H, REG_PSW
};
static constexpr RegName INDEX_REGS[] PROGMEM = {
    REG_B, REG_D
};
static constexpr RegName DATA_REGS[] PROGMEM = {
    REG_B, REG_C, REG_D, REG_E, REG_H, REG_L, REG_M, REG_A
};

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static bool regCharCaseEqual(char c, char regChar) {
    return toupper(c) == toupper(regChar);
}

char RegI8080::regName1stChar(const RegName regName) const {
    const char r = char(regName);
    return _uppercase ? r : tolower(r);
}

char RegI8080::regName2ndChar(const RegName regName) const {
    if (regName == REG_SP)
        return _uppercase ? 'P' : 'p';
    if (regName == REG_PSW)
        return _uppercase ? 'S' : 's';
    return 0;
}

char RegI8080::regName3rdChar(const RegName regName) const {
    if (regName == REG_PSW)
        return _uppercase ? 'W' : 'w';
    return 0;
}

bool RegI8080::compareRegName(const char *line, RegName regName) const {
    if (!regCharCaseEqual(*line++, regName1stChar(regName))) return false;
    const char r2 = regName2ndChar(regName);
    if (r2 && !regCharCaseEqual(*line++, r2)) return false;
    const char r3 = regName3rdChar(regName);
    if (r3 && !regCharCaseEqual(*line++, r3)) return false;
    return !isidchar(*line);
}

host::uint_t RegI8080::regNameLen(RegName regName) const {
    return regName2ndChar(regName) == 0 ? 1
        : (regName3rdChar(regName) == 0 ? 2 : 3);
}

char *RegI8080::outRegName(char *out, const RegName regName) const {
    *out++ = regName1stChar(regName);
    const char r2 = regName2ndChar(regName);
    if (r2) {
        *out++ = r2;
        const char r3 = regName3rdChar(regName);
        if (r3) *out++ = r3;
    }
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

RegName RegI8080::parseRegName(
    const char *line, const RegName *table, const RegName *end) const {
    for (const RegName *p = table; p < end; p++) {
        const RegName regName = RegName(pgm_read_byte(p));
        if (compareRegName(line, regName)) return regName;
    }
    return REG_UNDEF;
}

static RegName decodeRegNumber(
    const host::uint_t regNum, const RegName *table, const RegName *end) {
    const RegName *entry = &table[regNum];
    return entry < end ? RegName(pgm_read_byte(entry)) : REG_UNDEF;
}

RegName RegI8080::parseRegister(const char *line) const {
    return parseRegName(line, ARRAY_RANGE(ALL_REGS));
}

RegName RegI8080::parsePointerReg(const char *line) const {
    return parseRegName(line, ARRAY_RANGE(POINTER_REGS));
}

RegName RegI8080::parseStackReg(const char *line) const {
    return parseRegName(line, ARRAY_RANGE(STACK_REGS));
}

RegName RegI8080::parseIndexReg(const char *line) const {
    return parseRegName(line, ARRAY_RANGE(INDEX_REGS));
}

RegName RegI8080::parseDataReg(const char *line) const {
    return parseRegName(line, ARRAY_RANGE(DATA_REGS));
}

host::int_t RegI8080::encodePointerReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(POINTER_REGS));
}

host::int_t RegI8080::encodeStackReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(STACK_REGS));
}

host::int_t RegI8080::encodeIndexReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(INDEX_REGS));
}

host::int_t RegI8080::encodeDataReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(DATA_REGS));
}

RegName RegI8080::decodePointerReg(uint8_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(POINTER_REGS));
}

RegName RegI8080::decodeStackReg(uint8_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(STACK_REGS));
}

RegName RegI8080::decodeIndexReg(uint8_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(INDEX_REGS));
}

RegName RegI8080::decodeDataReg(uint8_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(DATA_REGS));
}

} // namespace i8080
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
