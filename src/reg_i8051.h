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

#ifndef __LIBASM_REG_I8051_H__
#define __LIBASM_REG_I8051_H__

#include <stdint.h>
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {

struct ValueParser;

namespace i8051 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    // R registers.
    REG_R0 = 0,
    REG_R1 = 1,
    REG_R2 = 2,
    REG_R3 = 3,
    REG_R4 = 4,
    REG_R5 = 5,
    REG_R6 = 6,
    REG_R7 = 7,
    // Other registers.
    REG_A = 0 + 8,
    REG_AB = 1 + 8,
    REG_C = 2 + 8,
    REG_PC = 3 + 8,
    REG_DPTR = 4 + 8,
};

namespace reg {

RegName parseRegName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outRegName(StrBuffer &out, RegName name);

bool isRReg(RegName name);
uint8_t encodeRRegName(RegName name);
RegName decodeRRegNum(uint8_t num);

}  // namespace reg
}  // namespace i8051
}  // namespace libasm

#endif  // __LIBASM_REG_I8051_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
