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

#ifndef __LIBASM_REG_Z8_H__
#define __LIBASM_REG_Z8_H__

#include <stdint.h>
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {

struct ValueParser;

namespace z8 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    REG_ILLEGAL = -2,
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
    // Register pair
    REG_RR0 = 16 + 0,
    REG_RR2 = 16 + 2,
    REG_RR4 = 16 + 4,
    REG_RR6 = 16 + 6,
    REG_RR8 = 16 + 8,
    REG_RR10 = 16 + 10,
    REG_RR12 = 16 + 12,
    REG_RR14 = 16 + 14,
};

enum CcName : int8_t {
    CC_UNDEF = -1,
    CC_F = 0,
    CC_LT = 1,
    CC_LE = 2,
    CC_ULE = 3,
    CC_OV = 4,
    CC_MI = 5,
    CC_Z = 6,
    CC_C = 7,
    CC_T = 8,
    CC_GE = 9,
    CC_GT = 10,
    CC_UGT = 11,
    CC_NOV = 12,
    CC_PL = 13,
    CC_NZ = 14,
    CC_NC = 15,
    // Aliases
    CC_EQ = 6 + 16,    // == Z(6)
    CC_ULT = 7 + 16,   // == C(7)
    CC_NE = 14 + 16,   // == NZ(14)
    CC_UGE = 15 + 16,  // == NC(15)
};

namespace reg {

RegName parseRegName(StrScanner &scan);
StrBuffer &outRegName(StrBuffer &out, RegName name);
bool isPairReg(RegName);
uint8_t encodeRegName(RegName name);
RegName decodeRegNum(uint8_t num);
RegName decodePairRegNum(uint8_t num);

CcName parseCcName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outCcName(StrBuffer &out, CcName name);
uint8_t encodeCcName(CcName name);
CcName decodeCcNum(uint8_t num);

bool isWorkRegAlias(bool super8, uint8_t addr);
uint8_t encodeWorkRegAddr(bool super8, RegName name);

}  // namespace reg
}  // namespace z8
}  // namespace libasm

#endif  // __LIBASM_REG_Z8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
