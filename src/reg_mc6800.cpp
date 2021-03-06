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

#include "reg_mc6800.h"

#include <ctype.h>

#include "config_mc6800.h"
#include "table_mc6800.h"

namespace libasm {
namespace mc6800 {

RegName RegMc6800::parseRegName(const char *line) {
    const char r = *line++;
    if (isidchar(*line))
        return REG_UNDEF;
    switch (toupper(r)) {
    case 'X':
        return REG_X;
    case 'Y':
        return REG_Y;
    default:
        return REG_UNDEF;
    }
}

uint8_t RegMc6800::regNameLen(RegName name) {
    return name == REG_UNDEF ? 0 : 1;
}

char *RegMc6800::outRegName(char *out, const RegName name) const {
    return outChar(out, char(name));
}

}  // namespace mc6800
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
