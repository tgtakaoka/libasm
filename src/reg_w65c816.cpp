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

#include <ctype.h>

#include "config_w65c816.h"
#include "reg_w65c816.h"

namespace libasm {
namespace w65c816 {

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static bool regCharCaseEqual(char c, char regChar) {
    return toupper(c) == toupper(regChar);
}

uint8_t RegW65C816::regNameLen(RegName regName) const {
    return regName == REG_UNDEF ? 0 : 1;
}

bool RegW65C816::compareRegName(const char *line, RegName regName) const {
    if (!regCharCaseEqual(*line++, char(regName)))
        return false;
    return !isidchar(*line);
}

RegName RegW65C816::parseIndexReg(const char *line) const {
    if (compareRegName(line, REG_X)) return REG_X;
    if (compareRegName(line, REG_Y)) return REG_Y;
    return REG_UNDEF;
}

char *RegW65C816::outRegName(char *out, const RegName regName) const {
    const char r = char(regName);
    *out++ = _uppercase ? r : tolower(r);
    *out = 0;
    return out;
}

} // namespace w65c816
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
