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
#include "reg_base.h"
#include "text_tms32010.h"

using namespace libasm::reg;
using namespace libasm::text::tms32010;

namespace libasm {
namespace tms32010 {
namespace reg {

RegName parseRegName(StrScanner &scan) {
    auto p = scan;
    if (p.iexpectText_P(TEXT_REG_AR)) {
        const auto num = parseRegNumber(p);
        if (num >= 0 && num <= 7) {
            scan = p;
            return RegName(REG_AR0 + num);
        }
        return REG_UNDEF;
    }
    if (p.iexpectText_P(TEXT_REG_PA)) {
        const auto num = parseRegNumber(p);
        if (num >= 0 && num <= 15) {
            scan = p;
            return RegName(REG_PA0 + num);
        }
        return REG_UNDEF;
    }
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    if (name >= REG_PA0)
        return out.text_P(TEXT_REG_PA).int16(name - REG_PA0);
    return out.text_P(TEXT_REG_AR).int16(name - REG_AR0);
}

uint_fast8_t encodeRegName(RegName name) {
    return int8_t(name) - (name >= REG_PA0 ? REG_PA0 : REG_AR0);
}

}  // namespace reg
}  // namespace tms32010
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
