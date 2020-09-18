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

RegName RegTms9900::parseRegName(const char *line) {
    if (toupper(*line++) != 'R')
        return REG_UNDEF;
    if (isdigit(*line) && !isidchar(line[1]))
        return RegName(*line - '0');
    if (*line++ == '1' && (*line >= '0' && *line < '6') && !isidchar(line[1]))
        return RegName((*line - '0') + 10);
    return REG_UNDEF;
}

uint8_t RegTms9900::regNameLen(RegName name) {
    if (name == REG_UNDEF) return 0;
    const uint8_t r = uint8_t(name);
    return r < 10 ? 2 : 3;
}

char *RegTms9900::outRegName(char *out, uint8_t num) const {
    num &= 0x0f;
    out = outChar(out, 'R');
    if (num < 10) {
        out = outChar(out, '0' + num);
    } else {
        out = outChar(out, '1');
        out = outChar(out, '0' + num - 10);
    }
    *out = 0;
    return out;
}

uint8_t RegTms9900::encodeRegNumber(RegName name) {
    return uint8_t(name);
}

} // namespace tms9900
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
