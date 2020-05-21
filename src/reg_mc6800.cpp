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

#include "config_mc6800.h"
#include "reg_mc6800.h"

#include <ctype.h>

namespace libasm {
namespace mc6800 {

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static bool regCharCaseEqual(char c, char regChar) {
    return toupper(c) == toupper(regChar);
}

char RegMc6800::regName1stChar(const RegName regName) const {
    const char r = char(regName);
    return _uppercase ? toupper(r) : tolower(r);
}

bool RegMc6800::compareRegName(const char *line, RegName regName) const {
    if (!regCharCaseEqual(*line++, regName1stChar(regName))) return false;
    return !isidchar(*line);
}

host::uint_t RegMc6800::regNameLen(RegName regName) const {
    return regName == REG_UNDEF ? 0 : 1;
}

char *RegMc6800::outRegName(char *out, const RegName regName) const {
    *out++ = regName1stChar(regName);
    *out = 0;
    return out;
}

RegName RegMc6800::parseRegName(const char *line) const {
    if (compareRegName(line, REG_A)) return REG_A;
    if (compareRegName(line, REG_B)) return REG_B;
    if (compareRegName(line, REG_X)) return REG_X;
    return REG_UNDEF;
}

} // namespace mc6800
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
