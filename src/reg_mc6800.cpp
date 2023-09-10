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

#include "reg_base.h"
#include "text_mc6800.h"

using namespace libasm::reg;
using namespace libasm::text::mc6800;

namespace libasm {
namespace mc6800 {
namespace reg {

RegName parseRegName(StrScanner &scan) {
    if (scan.iexpectText_P(TEXT_REG_X, 1, true))
        return REG_X;
    if (scan.iexpectText_P(TEXT_REG_Y, 1, true))
        return REG_Y;
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    return out.letter(char(name));
}

}  // namespace reg
}  // namespace mc6800
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
