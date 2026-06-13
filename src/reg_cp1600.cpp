/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "reg_cp1600.h"
#include "text_cp1600.h"

#include "reg_base.h"

using namespace libasm::reg;
using namespace libasm::text::cp1600;

namespace libasm {
namespace cp1600 {
namespace reg {

StrBuffer &outRegName(StrBuffer &out, uint_fast8_t num) {
    return out.letter('R').int16(num & 7);
}

}  // namespace reg
}  // namespace cp1600
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
