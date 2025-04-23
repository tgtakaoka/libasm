/*
 * Copyright 2025 Tadashi G. Takaoka
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

#ifndef __LIBASM_REG_TMS370_H__
#define __LIBASM_REG_TMS370_H__

#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {
namespace tms370 {

enum RegName : int16_t {
    REG_UNDEF = -1,
    REG_A = 'A',
    REG_B = 'B',
    REG_ST = 'S',
    REG_SP = 'P',
    REG_RN = 0x100,  // R0~R255
    REG_PN = 0x200,  // P0~P256
};

namespace reg {

RegName parseRegName(StrScanner &scan);
StrBuffer &outRegName(StrBuffer &out, RegName name);

constexpr bool isRegName(RegName name) {
    return name >= REG_RN && name < REG_PN;
}

uint_fast8_t toRegNum(RegName name);

constexpr RegName toRegName(uint_fast8_t num) {
    return RegName(REG_RN + num);
}

constexpr bool isPortName(RegName name) {
    return name >= REG_PN;
}

constexpr uint_fast8_t toPortNum(RegName name) {
    return RegName(name - REG_PN);
}

constexpr RegName toPortName(uint_fast8_t num) {
    return RegName(REG_PN + num);
}

}  // namespace reg
}  // namespace tms370
}  // namespace libasm

#endif  // __LIBASM_REG_TMS370_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
