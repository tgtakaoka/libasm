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

// 8-bit register encoding: bit3=0 (high byte), bit3=1 (low byte), bits[2:0]=reg number
// R0H=0x00, R0L=0x08, R1H=0x01, ..., R7H=0x07, R7L=0x0F
// 16-bit register encoding: bits[2:0]=reg number, bit3=0
// R0=0, R1=1, ..., R7=7

enum RegName : int8_t {
    REG_UNDEF = -1,
    // 8-bit registers (encoded as nibble: bit3=low/high, bits[2:0]=number)
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
    // 16-bit registers (encoded as 0x10|number so they don't overlap 8-bit)
    REG_R0 = 0x10,
    REG_R1 = 0x11,
    REG_R2 = 0x12,
    REG_R3 = 0x13,
    REG_R4 = 0x14,
    REG_R5 = 0x15,
    REG_R6 = 0x16,
    REG_R7 = 0x17,
    // SP is an alias for R7
    REG_SP = 0x18,
    // Control registers
    REG_CCR = 0x20,
};

namespace reg {

RegName parseRegName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outRegName(StrBuffer &out, RegName name);

uint_fast8_t encodeReg8(RegName reg);   // returns 4-bit encoding
uint_fast8_t encodeReg16(RegName reg);  // returns 3-bit encoding

RegName decodeReg8(uint_fast8_t nibble);   // decode 4-bit field
RegName decodeReg16(uint_fast8_t nibble);  // decode 3-bit field (0..7)

bool isReg8(RegName reg);
bool isReg16(RegName reg);

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
