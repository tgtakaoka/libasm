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

#include "config_ins8060.h"
#include "reg_ins8060.h"

#include <ctype.h>

namespace libasm {
namespace ins8060 {

static constexpr RegName ALL_REGS[] PROGMEM = {
    REG_P0, REG_P1, REG_P2, REG_P3, REG_PC, REG_E,
};

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static bool regCharCaseEqual(char c, char regChar) {
    return toupper(c) == toupper(regChar);
}

char RegIns8060::regName1stChar(const RegName regName) const {
    const char r = char(regName);
    if (isdigit(r)) return 'P';
    return _uppercase ? r : tolower(r);
}

char RegIns8060::regName2ndChar(const RegName regName) const {
    const char r = char(regName);
    if (isdigit(r)) return r;
    if (regName == REG_PC)
        return _uppercase ? 'C' : 'C';
    return 0;
}

bool RegIns8060::compareRegName(const char *line, RegName regName) const {
    if (!regCharCaseEqual(*line++, regName1stChar(regName))) return false;
    const char r2 = regName2ndChar(regName);
    if (r2 && !regCharCaseEqual(*line++, r2)) return false;
    return !isidchar(*line);
}

host::uint_t RegIns8060::regNameLen(RegName regName) const {
    if (regName == REG_UNDEF) return 0;
    return regName2ndChar(regName) ? 2 : 1;
}

char *RegIns8060::outRegName(char *out, const RegName regName) const {
    *out++ = regName1stChar(regName);
    const char r2 = regName2ndChar(regName);
    if (r2) *out++ = r2;
    *out = 0;
    return out;
}

RegName RegIns8060::parseRegName(
    const char *line, const RegName *table, const RegName *end) const {
    for (const RegName *p = table; p < end; p++) {
        const RegName regName = RegName(pgm_read_byte(p));
        if (compareRegName(line, regName)) return regName;
    }
    return REG_UNDEF;
}

RegName RegIns8060::parseRegister(const char *line) const {
    return parseRegName(line, ARRAY_RANGE(ALL_REGS));
}

RegName RegIns8060::parsePointerReg(const char *line) const {
    const RegName regNam = parseRegister(line);
    return (regNam == REG_E) ? REG_UNDEF : regNam;
}

host::int_t RegIns8060::encodePointerReg(RegName regName) {
    if (regName == REG_PC) return 0;
    return char(regName) - '0';
}

RegName RegIns8060::decodePointerReg(uint8_t regNum) {
    if (regNum == 0) return REG_PC;
    return RegName(regNum + '0');
}

} // namespace ins8060
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
