/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "reg_mc6805.h"

#include <ctype.h>

#include "config_mc6805.h"
#include "table_mc6805.h"

namespace libasm {
namespace mc6805 {

RegName RegMc6805::parseRegName(const char *line) {
    const char r = *line++;
    if (isidchar(*line))
        return REG_UNDEF;
    switch (toupper(r)) {
    case 'X':
        return REG_X;
    default:
        return REG_UNDEF;
    }
}

uint8_t RegMc6805::regNameLen(RegName name) {
    return name == REG_UNDEF ? 0 : 1;
}

StrBuffer &RegMc6805::outRegName(StrBuffer &out, const RegName name) const {
    return outChar(out, char(name));
}

}  // namespace mc6805
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
