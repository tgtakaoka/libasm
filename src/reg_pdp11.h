/*
 * Copyright 2024 Tadashi G. Takaoka
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

#ifndef __LIBASM_REG_PDP11_H__
#define __LIBASM_REG_PDP11_H__

#include <stdint.h>
#include "entry_pdp11.h"
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {

struct ValueParser;

namespace pdp11 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    // General registers.
    REG_R0 = 0 + 0,
    REG_R1 = 1 + 0,
    REG_R2 = 2 + 0,
    REG_R3 = 3 + 0,
    REG_R4 = 4 + 0,
    REG_R5 = 5 + 0,
    REG_SP = 6 + 0,
    REG_PC = 7 + 0,
    // FP11 registers
    REG_AC0 = 0 + 8,
    REG_AC1 = 1 + 8,
    REG_AC2 = 2 + 8,
    REG_AC3 = 3 + 8,
    REG_AC4 = 4 + 8,
    REG_AC5 = 5 + 8,
};

namespace reg {

RegName parseRegName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outRegName(StrBuffer &out, RegName name);
bool isGeneralReg(RegName name);
bool isAc03Reg(RegName name);
bool isFloatReg(RegName name);
Config::opcode_t encodeRegNo(RegName reg);
RegName decodeGeneralReg(uint8_t regno);
RegName decodeFloatReg(uint8_t regno);

}  // namespace reg
}  // namespace pdp11
}  // namespace libasm

#endif  // __LIBASM_REG_PDP11_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
