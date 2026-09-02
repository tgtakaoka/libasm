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

#ifndef __LIBASM_REG_I960_H__
#define __LIBASM_REG_I960_H__

#include <stdint.h>

#include "config_i960.h"
#include "str_buffer.h"
#include "str_scanner.h"
#include "value_parser.h"

namespace libasm {
namespace i960 {
namespace reg {

// Register numbers 0 to 15 are the local registers r0 to r15 and 16 to 31 the
// global registers g0 to g15.  Four of them carry an architectural name which
// is the only spelling GNU as accepts, so the disassembler always writes it.
static constexpr uint8_t REG_PFP = 0;   // r0, previous frame pointer
static constexpr uint8_t REG_SP = 1;    // r1, stack pointer
static constexpr uint8_t REG_RIP = 2;   // r2, return instruction pointer
static constexpr uint8_t REG_FP = 31;   // g15, frame pointer
static constexpr uint8_t GLOBAL_BASE = 16;

// A register number no operand can hold, used to say a memory operand names
// no address base or index register.
static constexpr uint8_t NO_REG = 32;

// The 80960KB has four floating-point registers.
static constexpr uint8_t FLOAT_REGS = 4;

// Reads r0 to r15, g0 to g15, and the four architectural names.  Returns
// NO_REG when |scan| does not start with a register name.
uint8_t parseRegName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outRegName(StrBuffer &out, uint8_t regno);

// Reads fp0 to fp3, returning NO_REG when |scan| names none of them.
uint8_t parseFloatRegName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outFloatRegName(StrBuffer &out, uint8_t regno);

}  // namespace reg
}  // namespace i960
}  // namespace libasm

#endif  // __LIBASM_REG_I960_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
