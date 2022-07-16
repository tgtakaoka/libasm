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

#include "reg_tms9900.h"

#include <ctype.h>

namespace libasm {
namespace tms9900 {

RegName RegTms9900::parseRegName(StrScanner &scan) {
    StrScanner p(scan);
    if (toupper(*p++) != 'R')
        return REG_UNDEF;
    const char c1 = *p++;
    const char c2 = *p;
    if (isdigit(c1) && !isidchar(c2)) {
        scan = p;
        return RegName(c1 - '0');
    }
    if (c1 == '1' && (c2 >= '0' && c2 < '6') && !isidchar(*++p)) {
        scan = p;
        return RegName((c2 - '0') + 10);
    }
    return REG_UNDEF;
}

StrBuffer &RegTms9900::outRegName(StrBuffer &out, uint8_t num) const {
    num &= 0x0f;
    out.letter('R', isUppercase());
    if (num < 10) {
        out.letter('0' + num);
    } else {
        out.letter('1').letter('0' + num - 10);
    }
    return out;
}

uint8_t RegTms9900::encodeRegNumber(RegName name) {
    return uint8_t(name);
}

}  // namespace tms9900
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
