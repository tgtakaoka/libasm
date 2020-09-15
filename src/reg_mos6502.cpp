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

#include "config_mos6502.h"
#include "reg_mos6502.h"

namespace libasm {
namespace mos6502 {

RegName RegMos6502::parseRegName(const char *line) {
    const char r = *line++;
    if (isidchar(*line)) return REG_UNDEF;
    switch (toupper(r)) {
    case 'A': return REG_A;
    case 'X': return REG_X;
    case 'Y': return REG_Y;
    case 'S': return REG_S;
    default:  return REG_UNDEF;
    }
}

uint8_t RegMos6502::regNameLen(RegName name) {
    return name == REG_UNDEF ? 0 : 1;
}

char *RegMos6502::outRegName(char *out, const RegName name) const {
    return outChar(out, char(name));
}

} // namespace mos6502
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
