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

#ifndef __LIBASM_REG_H8300_H__
#define __LIBASM_REG_H8300_H__

#include <stdint.h>

#include "entry_h8300.h"
#include "str_buffer.h"
#include "str_scanner.h"
#include "value_parser.h"

namespace libasm {
namespace h8300 {

// Register encodings: 4-bit nibble for 8-bit (R0H..R7L) and 16-bit (R/E),
// 3-bit nibble for 32-bit (ER).

enum RegName : int8_t {
    REG_UNDEF = -1,
    // 8-bit registers
    REG_R0H = 0x00,
    REG_R1H = 0x01,
    REG_R2H = 0x02,
    REG_R3H = 0x03,
    REG_R4H = 0x04,
    REG_R5H = 0x05,
    REG_R6H = 0x06,
    REG_R7H = 0x07,
    REG_R0L = 0x08,
    REG_R1L = 0x09,
    REG_R2L = 0x0A,
    REG_R3L = 0x0B,
    REG_R4L = 0x0C,
    REG_R5L = 0x0D,
    REG_R6L = 0x0E,
    REG_R7L = 0x0F,
    // 16-bit R-half (numbered contiguously with E-halves).
    REG_R0 = 0x10,
    REG_R1 = 0x11,
    REG_R2 = 0x12,
    REG_R3 = 0x13,
    REG_R4 = 0x14,
    REG_R5 = 0x15,
    REG_R6 = 0x16,
    REG_R7 = 0x17,
    // 16-bit E-half (H8/300H): upper halves of ER0..ER7.
    REG_E0 = 0x18,
    REG_E1 = 0x19,
    REG_E2 = 0x1A,
    REG_E3 = 0x1B,
    REG_E4 = 0x1C,
    REG_E5 = 0x1D,
    REG_E6 = 0x1E,
    REG_E7 = 0x1F,
    // 32-bit ER registers (H8/300H).
    REG_ER0 = 0x20,
    REG_ER1 = 0x21,
    REG_ER2 = 0x22,
    REG_ER3 = 0x23,
    REG_ER4 = 0x24,
    REG_ER5 = 0x25,
    REG_ER6 = 0x26,
    REG_ER7 = 0x27,
    // SP alias: R7 on H8/300, ER7 on H8/300H.
    REG_SP = 0x28,
    // Control registers.
    REG_CCR = 0x30,
    REG_EXR = 0x31,  // H8S extended control register
};

namespace reg {

RegName parseRegName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outRegName(StrBuffer &out, RegName name);

uint_fast8_t encodeReg8(RegName reg);
uint_fast8_t encodeReg16(RegName reg);
uint_fast8_t encodeReg32(RegName reg);
uint_fast8_t encodeAddrReg(RegName reg);

RegName decodeReg8(uint_fast8_t nibble);
RegName decodeReg16(uint_fast8_t nibble);
RegName decodeReg32(uint_fast8_t nibble);

bool isReg8(RegName reg);
bool isReg16(RegName reg);
bool isReg32(RegName reg);
// Matches R0..R7 on H8/300, and both R0..R7 (alias) and ER0..ER7 on H8/300H
bool isAddrReg(RegName reg, bool hasReg32);

char sizeSuffix(OprSize size);

}  // namespace reg
}  // namespace h8300
}  // namespace libasm

#endif  // __LIBASM_REG_H8300_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
