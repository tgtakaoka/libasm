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

#ifndef __LIBASM_REG_F3850_H__
#define __LIBASM_REG_F3850_H__

#include <stdint.h>

#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {
namespace f3850 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    REG_A = 1,         // Accumulator
    REG_W = 2,         // Status register
    REG_IS = 3,        // ISAR (Indirect scratchpad address register)
    REG_KU = 4,        // High 8-bit of K (R12)
    REG_KL = 5,        // Low 8-bit of K (R13)
    REG_QU = 6,        // High 8-bit of Q (R14)
    REG_QL = 7,        // Low 8-bit of Q (R15)
    REG_DC = 8,        // Data counter (DC0)
    REG_P = 9,         // Stack register (PC1)
    REG_P0 = 10,       // Program counter (PC0)
    REG_H = 11,        // Data pointer scratchpad (R10:R11)
    REG_K = 12,        // Stack pointer scratchpad (R12:R13)
    REG_Q = 13,        // Program counter scratchpad (R14:R15)
    REG_alias = 16,    // register number alias base
    REG_J = 9 + 16,    // R9: Status register scratchpad
    REG_HU = 10 + 16,  // R10: High 8-bit of H
    REG_HL = 11 + 16,  // R11: Low 8-bit of H
    REG_S = 12 + 16,   // Indirect ISAR
    REG_I = 13 + 16,   // Indirect ISAR with post increment
    REG_D = 14 + 16,   // Indirect ISAR with post decrement
};

namespace reg {

RegName parseRegName(StrScanner &scan);
RegName decodeRegName(uint8_t opc);
StrBuffer &outRegName(StrBuffer &out, RegName name);

}  // namespace reg
}  // namespace f3850
}  // namespace libasm

#endif  // __REG_F3850_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
