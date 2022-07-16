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

#include "reg_mos6502.h"

namespace libasm {
namespace mos6502 {

RegName RegMos6502::parseRegName(StrScanner &scan) {
    StrScanner p(scan);
    const char r = *p++;
    if (isidchar(*p))
        return REG_UNDEF;
    RegName reg = REG_UNDEF;
    switch (toupper(r)) {
    case 'A':
        reg = REG_A;
        break;
    case 'X':
        reg = REG_X;
        break;
    case 'Y':
        reg = REG_Y;
        break;
    case 'S':
        reg = REG_S;
        break;
    default:
        return REG_UNDEF;
    }
    scan = p;
    return reg;
}

StrBuffer &RegMos6502::outRegName(StrBuffer &out, const RegName name) const {
    return out.letter(char(name), isUppercase());
}

}  // namespace mos6502
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
