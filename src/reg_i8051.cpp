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

#include "config_i8051.h"
#include "reg_i8051.h"

#include <ctype.h>

namespace libasm {
namespace i8051 {

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static bool regCharCaseEqual(char c, char regChar) {
    return toupper(c) == toupper(regChar);
}

char RegI8051::regName1stChar(const RegName regName) const {
    char r = char(regName);
    if (isdigit(r)) r = 'R';
    return _uppercase ? toupper(r) : tolower(r);
}

char RegI8051::regName2ndChar(const RegName regName) const {
    char r = char(regName);
    if (isdigit(r)) return r;
    if (isupper(r)) return 0;
    if (regName == REG_AB) r = 'B';
    if (regName == REG_PC) r = 'C';
    if (regName == REG_DPTR) r = 'P';
    return _uppercase ? toupper(r) : tolower(r);
}

char RegI8051::regName3rdChar(const RegName regName) const {
    if (regName != REG_DPTR) return 0;
    return _uppercase ? 'T' : 't';
}

char RegI8051::regName4thChar(const RegName regName) const {
    if (regName != REG_DPTR) return 0;
    return _uppercase ? 'R' : 'r';
}

bool RegI8051::compareRegName(const char *line, RegName regName) const {
    if (!regCharCaseEqual(*line++, regName1stChar(regName))) return false;
    const char r2 = regName2ndChar(regName);
    if (r2 && !regCharCaseEqual(*line++, r2)) return false;
    const char r3 = regName3rdChar(regName);
    if (r3 && !regCharCaseEqual(*line++, r3)) return false;
    const char r4 = regName4thChar(regName);
    if (r4 && !regCharCaseEqual(*line++, r4)) return false;
    return !isidchar(*line);
}

uint8_t RegI8051::regNameLen(RegName regName) const {
    return regName2ndChar(regName) == 0 ? 1
        : (regName4thChar(regName) == 0 ? 2 : 4);
}

char *RegI8051::outRegName(char *out, const RegName regName) const {
    *out++ = regName1stChar(regName);
    const char r2 = regName2ndChar(regName);
    if (r2) {
        *out++ = r2;
        const char r3 = regName3rdChar(regName);
        if (r3) {
            *out++ = r3;
            const char r4 = regName4thChar(regName);
            if (r4) *out++ = r4;
        }
    }
    *out = 0;
    return out;
}

RegName RegI8051::parseRegName(
    const char *line, const RegName *table, const RegName *end) const {
    for (const RegName *p = table; p < end; p++) {
        const RegName regName = RegName(pgm_read_byte(p));
        if (compareRegName(line, regName)) return regName;
    }
    return REG_UNDEF;
}

static constexpr RegName ALL_REGISTERS[] PROGMEM = {
    REG_A, REG_C, REG_DPTR, REG_AB, REG_PC,
    REG_R0, REG_R1, REG_R2, REG_R3, REG_R4, REG_R5, REG_R6, REG_R7,
};

RegName RegI8051::parseRegister(const char *line) const {
    return parseRegName(line, ARRAY_RANGE(ALL_REGISTERS));
}

bool RegI8051::isRReg(RegName regName) const {
    return isdigit(char(regName));
}

int8_t RegI8051::encodeRReg(RegName regName) const {
    const char r = char(regName);
    return isdigit(r) ? r - '0' : -1;
}

RegName RegI8051::decodeRReg(const uint8_t regNum) const {
    return RegName(regNum + '0');
}

} // namespace i8051
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
