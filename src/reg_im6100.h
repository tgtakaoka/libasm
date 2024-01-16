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

#ifndef __LIBASM_REG_IM6100_H__
#define __LIBASM_REG_IM6100_H__

#include "config_im6100.h"

namespace libasm {
namespace im6100 {
namespace reg {

constexpr uint8_t fieldOf(Config::uintptr_t addr) {
    return (addr >> 12) & 07;
}

constexpr uint8_t pageOf(Config::uintptr_t addr) {
    return addr >> 7;
}

constexpr uint8_t offsetOf(Config::opcode_t opc) {
    return opc & 0177;
}

constexpr Config::uintptr_t memoryAddress(uint8_t offset, uint8_t page, uint8_t field = 0) {
    return offset | (static_cast<Config::uintptr_t>(page) << 7) |
           (static_cast<Config::uintptr_t>(field) << 12);
}

}  // namespace reg
}  // namespace im6100
}  // namespace libasm

#endif  // __LIBASM_REG_IM6100_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
