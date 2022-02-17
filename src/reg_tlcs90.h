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

#ifndef __REG_TLCS90_H__
#define __REG_TLCS90_H__

#include "insn_tlcs90.h"
#include "reg_base.h"

namespace libasm {
namespace tlcs90 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    // 16-bit registers: M_REG16
    REG_BC = 0 + 0,
    REG_DE = 0 + 1,
    REG_HL = 0 + 2,
    // Index registers: M_REGIX
    REG_IX = 4 + 0,
    REG_IY = 4 + 1,
    REG_SP = 4 + 2,
    // 16-bit registers for stack: M_STACK
    REG_AF = 8 + 6,
    REG_AFP = 8 + 7,
    // 8-bit registers: M_REG8
    REG_B = 16 + 0,
    REG_C = 16 + 1,
    REG_D = 16 + 2,
    REG_E = 16 + 3,
    REG_H = 16 + 4,
    REG_L = 16 + 5,
    REG_A = 16 + 6,
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
    CC_PE = 16 + 4,    // == OV(4)
    CC_M = 16 + 5,     // == MI(5)
    CC_EQ = 16 + 6,    // == Z(6)
    CC_ULT = 16 + 7,   // == C(7)
    CC_PO = 16 + 12,   // == NOV(12)
    CC_P = 16 + 13,    // == PL(13)
    CC_NE = 16 + 14,   // == NZ(14)
    CC_UGE = 16 + 15,  // == NC(15)
};

class RegTlcs90 : public RegBase {
public:
    static RegName parseRegName(StrScanner &scan);
    StrBuffer &outRegName(StrBuffer &out, const RegName name) const;
    static bool isReg16(RegName name);
    static bool isRegIndex(RegName name);

    static uint8_t encodeReg8(RegName name);
    static RegName decodeReg8(uint8_t num);

    static uint8_t encodeReg16(RegName name);
    static RegName decodeReg16(uint8_t num);

    static uint8_t encodeIndexReg(RegName name);
    static RegName decodeIndexReg(uint8_t num);

    static uint8_t encodeStackReg(RegName name);
    static RegName decodeStackReg(uint8_t num);

    static CcName parseCcName(StrScanner &scan);
    StrBuffer &outCcName(StrBuffer &out, CcName cc) const;
    static uint8_t encodeCcName(CcName name);
    static CcName decodeCcName(uint8_t num);
};

}  // namespace tlcs90
}  // namespace libasm

#endif  // __REG_TLCS90_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
