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

#ifndef __LIBASM_REG_AM29000_H__
#define __LIBASM_REG_AM29000_H__

#include <stdint.h>

#include "config_am29000.h"
#include "str_buffer.h"

namespace libasm {
namespace am29000 {

// Special-purpose registers.  The enumerator is an index into the table of
// register numbers, because the numbers themselves run up to 164 and a
// reg::NameEntry holds only an int8_t.  Protected registers come first, then
// the unprotected ones, then those the arithmetic software virtualizes.
// Registers which only some family members implement are kept in contiguous
// runs so that a range check decides whether one exists, see isMmuReg,
// isAm29050Reg and isCacheReg.
enum RegName : int8_t {
    REG_UNDEF = -1,
    REG_VAB = 0,
    REG_OPS = 1,
    REG_CPS = 2,
    REG_CFG = 3,
    REG_CHA = 4,
    REG_CHD = 5,
    REG_CHC = 6,
    REG_RBP = 7,
    REG_TMC = 8,
    REG_TMR = 9,
    REG_PC0 = 10,
    REG_PC1 = 11,
    REG_PC2 = 12,
    // Memory management unit; absent from the Am29200 and Am29205.
    REG_MMU = 13,
    REG_LRU = 14,
    // Region mapping, shadow program counters, instruction breakpoints and the
    // exception opcode; implemented by the Am29050 alone.
    REG_RSN = 15,
    REG_RMA0 = 16,
    REG_RMC0 = 17,
    REG_RMA1 = 18,
    REG_RMC1 = 19,
    REG_SPC0 = 20,
    REG_SPC1 = 21,
    REG_SPC2 = 22,
    REG_IBA0 = 23,
    REG_IBC0 = 24,
    REG_IBA1 = 25,
    REG_IBC1 = 26,
    REG_EXOP = 27,
    // Cache interface; the Am29200 has no cache and the Am29050 does not
    // expose one through a special-purpose register.
    REG_CIR = 28,
    REG_CDR = 29,
    REG_IPC = 30,
    REG_IPA = 31,
    REG_IPB = 32,
    REG_Q = 33,
    REG_ALU = 34,
    REG_BP = 35,
    REG_FC = 36,
    REG_CR = 37,
    REG_FPE = 38,
    REG_INTE = 39,
    REG_FPS = 40,
};

namespace reg {

// General-purpose registers.  There are 128 global and 128 local registers,
// addressed as one bank of 256 absolute register numbers.
constexpr uint8_t LOCAL_BASE = 128;

StrBuffer &outGenReg(StrBuffer &out, uint8_t regno);

// Which special-purpose registers exist depends on the CPU: the Am29200 has
// neither the memory management unit nor a cache, the Am29050 adds a set of
// its own and has no cache registers, and the rest have both.
StrBuffer &outSpReg(StrBuffer &out, RegName name);
RegName decodeSpReg(uint8_t number, CpuType cpuType);

}  // namespace reg
}  // namespace am29000
}  // namespace libasm

#endif  // __LIBASM_REG_AM29000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
