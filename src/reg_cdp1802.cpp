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

#include "config_cdp1802.h"
#include "table_cdp1802.h"

namespace libasm {
namespace cdp1802 {

RegName RegCdp1802::parseRegName(const char *line) {
    if (toupper(*line++) == 'R' && isdigit(*line)) {
        int8_t num = *line++ - '0';
        if (num == 1 && isdigit(*line))
            num = *line++ - '0' + 10;
        if (num < 16 && !isidchar(*line))
            return RegName(num);
    }
    return REG_UNDEF;
}

uint8_t RegCdp1802::regNameLen(RegName name) {
    const int8_t num = int8_t(name);
    if (num >= 0)
        return num < 10 ? 2 : 3;
    return 0;
}

StrBuffer &RegCdp1802::outRegName(StrBuffer &out, RegName name) const {
    int8_t num = int8_t(name);
    if (num >= 0) {
        outChar(out, 'R');
        if (num >= 10) {
            outChar(out, '1');
            num -= 10;
        }
        outChar(out, num + '0');
    }
    return out;
}

}  // namespace cdp1802
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
