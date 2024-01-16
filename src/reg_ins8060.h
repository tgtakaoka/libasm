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

#ifndef __LIBASM_REG_INS8060_H__
#define __LIBASM_REG_INS8060_H__

#include <stdint.h>

#include "config_ins8060.h"
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {
namespace ins8060 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    // Pointer registers.
    REG_PC = 0,
    REG_P1 = 1,
    REG_P2 = 2,
    REG_P3 = 3,
    // Other registers.
    REG_P0 = 0 + 4,  // Alias of PC
    REG_E = 1 + 4,
};

namespace reg {

constexpr Config::uintptr_t page(Config::uintptr_t addr) {
    return addr & ~0xFFF;
}

constexpr Config::uintptr_t offset(Config::uintptr_t addr) {
    return addr & 0xFFF;
}

RegName parseRegName(StrScanner &scan);
StrBuffer &outRegName(StrBuffer &out, RegName name);

bool isPointerReg(RegName name);
uint8_t encodePointerReg(RegName name);
RegName decodePointerReg(uint8_t num);

}  // namespace reg
}  // namespace ins8060
}  // namespace libasm

#endif  // __LIBASM_REG_INS8060_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
