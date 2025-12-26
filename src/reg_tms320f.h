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

#ifndef __LIBASM_REG_TMS320F_H__
#define __LIBASM_REG_TMS320F_H__

#include <stdint.h>

namespace libasm {

struct StrBuffer;
struct StrScanner;
struct ValueParser;

namespace tms320f {

enum RegName : int8_t {
    REG_UNDEF = -1,
    REG_R0 = 0,    // Extended-precision register 0
    REG_R1 = 1,    // Extended-precision register 1
    REG_R2 = 2,    // Extended-precision register 2
    REG_R3 = 3,    // Extended-precision register 3
    REG_R4 = 4,    // Extended-precision register 4
    REG_R5 = 5,    // Extended-precision register 5
    REG_R6 = 6,    // Extended-precision register 6
    REG_R7 = 7,    // Extended-precision register 7
    REG_AR0 = 8,   // Auxiliary register 0
    REG_AR1 = 9,   // Auxiliary register 1
    REG_AR2 = 10,  // Auxiliary register 2
    REG_AR3 = 11,  // Auxiliary register 3
    REG_AR4 = 12,  // Auxiliary register 4
    REG_AR5 = 13,  // Auxiliary register 5
    REG_AR6 = 14,  // Auxiliary register 6
    REG_AR7 = 15,  // Auxiliary register 7
    REG_DP = 16,   // Data-page Pointer
    REG_IR0 = 17,  // Index register 0
    REG_IR1 = 18,  // Index register 1
    REG_BK = 19,   // Block-size register
    REG_SP = 20,   // Active stack pointer
    REG_ST = 21,   // Status register
    REG_IE = 22,   // Interrupt enable
    REG_IF = 23,   // Interrupt flags
    REG_IOF = 24,  // I/O flags
    REG_RS = 25,   // Repeat start address
    REG_RE = 26,   // Repeat end address
    REG_RC = 27,   // Repeat counter
    _REG_MAX = 28,
};

namespace reg {

RegName parseRegName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outRegName(StrBuffer &out, RegName name);
uint_fast8_t encodeRegName(RegName name);
RegName decodeRegName(uint_fast16_t num);

}  // namespace reg
}  // namespace tms320f
}  // namespace libasm

#endif  // __LIBASM_REG_TMS320F_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
