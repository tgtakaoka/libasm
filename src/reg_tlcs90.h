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

#ifndef __LIBASM_REG_TLCS90_H__
#define __LIBASM_REG_TLCS90_H__

#include <stdint.h>
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {

struct ValueParser;

namespace tlcs90 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    // 16-bit registers: M_REG16
    REG_BC = 0,
    REG_DE = 1,
    REG_HL = 2,
    // Index registers: M_REGIX
    REG_IX = 4,
    REG_IY = 5,
    REG_SP = 6,
    // 16-bit registers for stack: M_STACK
    REG_AF = 7,
    // 8-bit registers: M_REG8
    REG_B = 0 + 8,
    REG_C = 1 + 8,
    REG_D = 2 + 8,
    REG_E = 3 + 8,
    REG_H = 4 + 8,
    REG_L = 5 + 8,
    REG_A = 6 + 8,
    // Alternate register
    REG_AFP = 15,
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
    ALIAS_BASE = 16,
    CC_PE = CC_OV + ALIAS_BASE,   // == OV(4)
    CC_M = CC_MI + ALIAS_BASE,    // == MI(5)
    CC_EQ = CC_Z + ALIAS_BASE,    // == Z(6)
    CC_ULT = CC_C + ALIAS_BASE,   // == C(7)
    CC_PO = CC_NOV + ALIAS_BASE,  // == NOV(12)
    CC_P = CC_PL + ALIAS_BASE,    // == PL(13)
    CC_NE = CC_NZ + ALIAS_BASE,   // == NZ(14)
    CC_UGE = CC_NC + ALIAS_BASE,  // == NC(15)
};

namespace reg {

RegName parseRegName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outRegName(StrBuffer &out, RegName name);
bool isReg16(RegName name);
bool isRegIndex(RegName name);

uint8_t encodeReg8(RegName name);
RegName decodeReg8(uint8_t num);

uint8_t encodeReg16(RegName name);
RegName decodeReg16(uint8_t num);

uint8_t encodeIndexReg(RegName name);
RegName decodeIndexReg(uint8_t num);

uint8_t encodeStackReg(RegName name);
RegName decodeStackReg(uint8_t num);

CcName parseCcName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outCcName(StrBuffer &out, CcName cc);
uint8_t encodeCcName(CcName name);
CcName decodeCcName(uint8_t num);

}  // namespace reg
}  // namespace tlcs90
}  // namespace libasm

#endif  // __LIBASM_REG_TLCS90_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
