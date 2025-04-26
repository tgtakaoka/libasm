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

#ifndef __LIBASM_REG_MC68HC12_H__
#define __LIBASM_REG_MC68HC12_H__

#include <stdint.h>
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {

struct ValueParser;

namespace mc68hc12 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    REG_A = 0,
    REG_B = 1,
    REG_CCR = 2,
    REG_D = 4,
    REG_X = 5,
    REG_Y = 6,
    REG_SP = 7,
    REG_PC = 8,
};

namespace reg {

RegName parseRegName(StrScanner &scan, const ValueParser &pareser);
StrBuffer &outRegName(StrBuffer &out, RegName name);
RegName decodeRegNum(uint_fast8_t num);
uint_fast8_t regSize(RegName name);
uint_fast8_t encodeRegNum(RegName name);

}  // namespace reg
}  // namespace mc68hc12
}  // namespace libasm

#endif  // __LIBASM_REG_MC68HC12_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
