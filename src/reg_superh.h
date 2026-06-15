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

#ifndef __LIBASM_REG_SUPERH_H__
#define __LIBASM_REG_SUPERH_H__

#include <stdint.h>

#include "reg_base.h"
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {

struct ValueParser;

namespace superh {

// 32-bit general registers R0-R15, encoded as 4-bit field. Special
// registers (SR/GBR/VBR/MACH/MACL/PR/PC) share the enum; they don't
// participate in numeric register encoding.
enum RegName : int8_t {
    REG_UNDEF = -1,
    REG_R0    = 0,
    REG_R1    = 1,
    REG_R2    = 2,
    REG_R3    = 3,
    REG_R4    = 4,
    REG_R5    = 5,
    REG_R6    = 6,
    REG_R7    = 7,
    REG_R8    = 8,
    REG_R9    = 9,
    REG_R10   = 10,
    REG_R11   = 11,
    REG_R12   = 12,
    REG_R13   = 13,
    REG_R14   = 14,
    REG_R15   = 15,
    REG_SR    = 16,
    REG_GBR   = 17,
    REG_VBR   = 18,
    REG_MACH  = 19,
    REG_MACL  = 20,
    REG_PR    = 21,
    REG_PC    = 22,
};

namespace reg {

RegName parseRegName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outRegName(StrBuffer &out, RegName reg);
RegName decodeRn(uint16_t word);   // extract 4-bit field from bits[11:8]
RegName decodeRm(uint16_t word);   // extract 4-bit field from bits[7:4]

inline bool isGpr(RegName r) { return r >= REG_R0 && r <= REG_R15; }
uint8_t encodeRegNum(RegName r);   // 0..15 for GPRs; undefined for special regs

}  // namespace reg
}  // namespace superh
}  // namespace libasm

#endif  // __LIBASM_REG_SUPERH_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
