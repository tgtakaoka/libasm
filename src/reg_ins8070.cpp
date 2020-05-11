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

#include "config_ins8070.h"
#include "reg_ins8070.h"

#include <ctype.h>

namespace libasm {
namespace ins8070 {

static constexpr RegName ALL_REGS[] PROGMEM = {
    REG_PC, REG_SP, REG_P2, REG_P3,
    REG_A,  REG_E,  REG_EA, REG_T,  REG_S
};

static constexpr RegName POINTER_REGS[] PROGMEM = {
    REG_PC, REG_SP, REG_P2, REG_P3
};

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static bool regCharCaseEqual(char c, char regChar) {
    return toupper(c) == toupper(regChar);
}

char RegIns8070::regName1stChar(const RegName regName) const {
    char r = char(regName);
    if (isdigit(r)) r = 'p';
    return _uppercase ? toupper(r) : tolower(r);
}

char RegIns8070::regName2ndChar(const RegName regName) const {
    char r = 0;
    if (regName == REG_EA) r = 'a';
    if (regName == REG_PC) r = 'c';
    if (regName == REG_SP) r = 'p';
    if (isdigit(char(regName))) r = char(regName);
    return _uppercase ? toupper(r) : tolower(r);
}

bool RegIns8070::compareRegName(const char *line, RegName regName) const {
    if (!regCharCaseEqual(*line++, regName1stChar(regName))) return false;
    const char r2 = regName2ndChar(regName);
    if (r2 && !regCharCaseEqual(*line++, r2)) return false;
    return !isidchar(*line);
}

host::uint_t RegIns8070::regNameLen(RegName regName) const {
    if (regName == REG_UNDEF) return 0;
    return regName2ndChar(regName) ? 2 : 1;
}

char *RegIns8070::outRegName(char *out, const RegName regName) const {
    *out++ = regName1stChar(regName);
    const char r2 = regName2ndChar(regName);
    if (r2) *out++ = r2;
    *out = 0;
    return out;
}

RegName RegIns8070::parseRegName(
    const char *line, const RegName *table, const RegName *end) const {
    for (const RegName *p = table; p < end; p++) {
        const RegName regName = RegName(pgm_read_byte(p));
        if (compareRegName(line, regName)) return regName;
    }
    return REG_UNDEF;
}

RegName RegIns8070::parseRegister(const char *line) const {
    return parseRegName(line, ARRAY_RANGE(ALL_REGS));
}

RegName RegIns8070::parsePointerReg(const char *line) const {
    return parseRegName(line, ARRAY_RANGE(POINTER_REGS));
}

host::int_t RegIns8070::encodePointerReg(RegName regName) {
    if (regName == REG_PC) return 0;
    if (regName == REG_SP) return 1;
    return char(regName) - '0';
}

RegName RegIns8070::decodePointerReg(uint8_t regNum) {
    regNum &= 3;
    if (regNum == 0) return REG_PC;
    if (regNum == 1) return REG_SP;
    return RegName(regNum + '0');
}

} // namespace ins8070
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
