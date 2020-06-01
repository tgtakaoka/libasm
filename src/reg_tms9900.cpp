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

#include "config_tms9900.h"
#include "reg_tms9900.h"

namespace libasm {
namespace tms9900 {

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

uint8_t RegTms9900::regNameLen(RegName regName) const {
    const char r = char(regName);
    if (isdigit(r)) return 2;
    if (isalpha(r)) return 3;
    return 0;
}

RegName RegTms9900::parseRegName(const char *line) const {
    if (toupper(*line++) != 'R')
        return REG_UNDEF;
    if (isdigit(*line) && !isidchar(line[1]))
        return RegName(*line);
    if (*line++ == '1' && (*line >= '0' && *line < '6') && !isidchar(line[1]))
        return RegName('A' + (*line - '0'));
    return REG_UNDEF;
}

char *RegTms9900::outRegName(char *out, uint8_t regno) const {
    regno &= 0x0f;
    *out++ = _uppercase ? 'R' : 'r';
    if (regno < 10) {
        *out++ = regno + '0';
    } else {
        *out++ = '1';
        *out++ = regno - 10 + '0';
    }
    *out = 0;
    return out;
}

uint16_t RegTms9900::encodeRegNumber(RegName regName) const {
    const char r = char(regName);
    if (isdigit(r)) return r - '0';
    return r - 'A' + 10;
}

} // namespace tms9900
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
