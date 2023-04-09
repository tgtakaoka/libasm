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

#ifndef __REG_CDP1802_H__
#define __REG_CDP1802_H__

#include <stdint.h>

#include "str_scanner.h"
#include "str_buffer.h"

namespace libasm {
namespace cdp1802 {

/**
 * SCRT (Standard CALL and RETURN Technique (Fig. 75 of CDP1802 Users Manual).
 * R0: Direct Memory Access
 * R1: Interrupt
 * R2: Stack Pointer
 * R3: Program Counter
 * R4: Subroutine Call address
 * R5: Link register
 * R6: Argument pointer
 * R7-R15: Working register
 */

enum RegName : int8_t {
    REG_UNDEF = -1,
    REG_R0 = 0,
    REG_R1 = 1,
    REG_R2 = 2,
    REG_R3 = 3,
    REG_R4 = 4,
    REG_R5 = 5,
    REG_R6 = 6,
    REG_R7 = 7,
    REG_R8 = 8,
    REG_R9 = 9,
    REG_R10 = 10,
    REG_R11 = 11,
    REG_R12 = 12,
    REG_R13 = 13,
    REG_R14 = 14,
    REG_R15 = 15,
};

namespace reg {

RegName parseRegName(StrScanner &scan);
StrBuffer &outRegName(StrBuffer &out, RegName name);

}  // namespace reg
}  // namespace cdp1802
}  // namespace libasm

#endif  // __REG_CDP1802_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
