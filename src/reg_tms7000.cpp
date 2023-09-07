/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "reg_tms7000.h"

#include "reg_base.h"
#include "text_tms7000.h"

using namespace libasm::reg;
using namespace libasm::text::tms7000;

namespace libasm {
namespace tms7000 {
namespace reg {


StrBuffer &outRegName(StrBuffer &out, RegName name) {
    if (name == REG_A || name == REG_B)
        return out.letter(char(name));
    if (isRegName(name))
        return out.letter('R').uint8(toRegNum(name));
    if (isPortName(name))
        return out.letter('P').uint8(toPortNum(name));
    if (name == REG_ST)
        return out.text_P(TEXT_REG_ST);
    return out;
}

}  // namespace reg
}  // namespace tms7000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
