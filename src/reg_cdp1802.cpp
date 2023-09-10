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

#include "reg_base.h"

using namespace libasm::reg;

namespace libasm {
namespace cdp1802 {
namespace reg {

RegName parseRegName(StrScanner &scan) {
    auto save = scan;
    if (scan.iexpect('R')) {
        const auto num = parseRegNumber(scan);
        if (num >= 0 && num < 16)
            return RegName(num);
    }
    scan = save;
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    return out.letter('R').uint8(int8_t(name));
}

}  // namespace reg
}  // namespace cdp1802
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
