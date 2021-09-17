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

#include "reg_tms32010.h"

#include <ctype.h>

#include "config_tms32010.h"

namespace libasm {
namespace tms32010 {

StrBuffer &RegTms32010::outRegName(StrBuffer &out, RegName name) const {
    if (name == REG_AR0 || name == REG_AR1) {
        outChar(out, 'A');
        outChar(out, 'R');
        out.letter(int8_t(name) + '0');
    } else {
        const int8_t num = int8_t(name) - int8_t(REG_PA0);
        if (num >= 0 && num < 8) {
            outChar(out, 'P');
            outChar(out, 'A');
            out.letter(num + '0');
        }
    }
    return out;
}

}  // namespace tms32010
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
