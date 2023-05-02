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

#include "pseudos.h"

#include "asm_base.h"
#include "text_common.h"

using namespace libasm::text::common;

namespace libasm {
namespace pseudo {

// clang-format off
static constexpr Pseudo COMMONS[] PROGMEM = {
    Pseudo{TEXT_ORG, &Assembler::defineOrigin, 0},
    Pseudo{TEXT_ALIGN, &Assembler::alignOrigin, 0},
};

static constexpr uint8_t INDEX_COMMONS[] PROGMEM = {
};

// clang-format on

}  // namespace pseudo
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
