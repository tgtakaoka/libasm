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

#include "reg_cdp1802.h"

#include <ctype.h>

namespace libasm {
namespace cdp1802 {

RegName RegCdp1802::parseRegName(StrScanner &scan) {
    StrScanner p(scan);
    if (p.iexpect('R')) {
        const auto num = parseRegNumber(p, 16);
        if (num >= 0) {
            scan = p;
            return RegName(num);
        }
    }
    return REG_UNDEF;
}

StrBuffer &RegCdp1802::outRegName(StrBuffer &out, RegName name) const {
    return outRegNumber(out.letter('R', isUppercase()), int8_t(name));
}

}  // namespace cdp1802
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
