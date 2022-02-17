/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "reg_base.h"

#ifndef __REG_I8048_H__
#define __REG_I8048_H__

namespace libasm {
namespace i8048 {

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
    // I/O ports.
    REG_BUS = 0 + 8,
    REG_P1 = 1 + 8,
    REG_P2 = 2 + 8,
    REG_P4 = 4 + 8,
    REG_P5 = 5 + 8,
    REG_P6 = 6 + 8,
    REG_P7 = 7 + 8,
    // Accumulator & Program Status Word.
    REG_A  = 0 + 16,
    REG_PSW = 1 + 16,
    REG_P = 2 + 16,             // MSM80C39/MSM80C48
    // Flags.
    REG_F0 = 0 + 24,
    REG_F1 = 1 + 24,
    REG_C  = 2 + 24,
    REG_I  = 3 + 24,
    REG_RB0 = 4 + 24,
    REG_RB1 = 5 + 24,
    REG_MB0 = 6 + 24,
    REG_MB1 = 7 + 24,
    // Timer & Counter.
    REG_T  = 0 + 32,
    REG_CLK = 1 + 32,
    REG_CNT = 2 + 32,
    REG_TCNT = 3 + 32,
    REG_TCNTI = 4 + 32,
};

class RegI8048 : public RegBase {
public:
    static RegName parseRegName(StrScanner &scan);
    StrBuffer &outRegName(StrBuffer &out, const RegName name) const;

    static bool isRReg(RegName name);
    static bool isPort(RegName name);
    static bool isTimer(RegName name);
    static uint8_t encodeRReg(RegName name);
    static RegName decodeRReg(uint8_t num);
    static uint8_t encodePort(RegName name);
    static RegName decodePort(uint8_t num);
};

}  // namespace i8048
}  // namespace libasm

#endif  // __REG_I8048_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
