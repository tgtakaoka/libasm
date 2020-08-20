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

#ifndef __REG_Z8000_H__
#define __REG_Z8000_H__

#include "insn_z8000.h"
#include "reg_base.h"

namespace libasm {
namespace z8000 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    REG_ILLEGAL = -2,
    // Word Register
    REG_R0  = 0,
    REG_R1  = 1,
    REG_R2  = 2,
    REG_R3  = 3,
    REG_R4  = 4,
    REG_R5  = 5,
    REG_R6  = 6,
    REG_R7  = 7,
    REG_R8  = 8,
    REG_R9  = 9,
    REG_R10 = 10,
    REG_R11 = 11,
    REG_R12 = 12,
    REG_R13 = 13,
    REG_R14 = 14,
    REG_R15 = 15,
    // Byte Register
    REG_RH0 = 16 + 0,
    REG_RH1 = 16 + 1,
    REG_RH2 = 16 + 2,
    REG_RH3 = 16 + 3,
    REG_RH4 = 16 + 4,
    REG_RH5 = 16 + 5,
    REG_RH6 = 16 + 6,
    REG_RH7 = 16 + 7,
    REG_RL0 = 16 + 8,
    REG_RL1 = 16 + 9,
    REG_RL2 = 16 + 10,
    REG_RL3 = 16 + 11,
    REG_RL4 = 16 + 12,
    REG_RL5 = 16 + 13,
    REG_RL6 = 16 + 14,
    REG_RL7 = 16 + 15,
    // Long Register
    REG_RR0  = 32 + 0,
    REG_RR2  = 32 + 2,
    REG_RR4  = 32 + 4,
    REG_RR6  = 32 + 6,
    REG_RR8  = 32 + 8,
    REG_RR10 = 32 + 10,
    REG_RR12 = 32 + 12,
    REG_RR14 = 32 + 14,
    // Quad Register
    REG_RQ0  = 48 + 0,
    REG_RQ4  = 48 + 4,
    REG_RQ8  = 48 + 8,
    REG_RQ12 = 48 + 12,
    // Special Register
    REG_FLAGS   = 64 + 1,
    REG_FCW     = 64 + 2,
    REG_REFRESH = 64 + 3,
    REG_PSAPSEG = 64 + 4,
    REG_PSAPOFF = 64 + 5,
    REG_NSPSEG  = 64 + 6,
    REG_NSPOFF  = 64 + 7,
};

enum CcName : int8_t {
    CC_UNDEF = -1,
    CC_F   = 0,  // Always false
    CC_LT  = 1,  // Less than
    CC_LE  = 2,  // Less than or equal
    CC_ULE = 3,  // Unsigned less than or equal
    CC_OV  = 4,  // Overflow
    CC_MI  = 5,  // Minus
    CC_Z   = 6,  // Zero
    CC_C   = 7,  // Carry
    CC_T   = 8,  // Always true
    CC_GE  = 9,  // Greater than or equal
    CC_GT  = 10, // Greater than
    CC_UGT = 11, // Unsigned greater than
    CC_NOV = 12, // No overflow
    CC_PL  = 13, // Plus
    CC_NZ  = 14, // Not zero
    CC_NC  = 15, // No carry
    // Alias
    CC_EQ  = 16,  // Z: Equal
    CC_ULT = 17,  // C: Unsigned less than
    CC_NE  = 18,  // NZ: Nor equal
    CC_UGE = 19,  // NC: Unsigned greater than or equal
};

enum FlagName : int8_t {
    FL_UNDEF = 0,
    FL_C = 8,  // Carry
    FL_Z = 4,  // Zero
    FL_S = 2,  // Sign
    FL_P = 1,  // Parity
    // Alias
    FL_V = 16, // Overflow
};

enum IntrName : int8_t {
    IT_UNDEF = 0,
    IT_NVI = 1, // Non-Vectored Interrupt
    IT_VI  = 2, // Vectored Interrupt
};

class RegZ8000 : public RegBase {
public:
    RegName parseRegName(const char *line) const;
    uint8_t regNameLen(RegName regName) const;
    uint8_t encodeRegName(RegName regName) const;
    RegName decodeRegNum(uint8_t regNum, OprSize size) const;
    RegName decodeByteReg(uint8_t regNum) const;
    RegName decodeWordReg(uint8_t regNum) const;
    RegName decodeLongReg(uint8_t regNum) const;
    RegName decodeQuadReg(uint8_t regNum) const;
    bool isByteReg(RegName) const;
    bool isWordReg(RegName) const;
    bool isLongReg(RegName) const;
    bool isQuadReg(RegName) const;
    char *outRegName(char *out, RegName regName) const;

    RegName parseCtlReg(const char *line) const;
    RegName decodeCtlReg(uint8_t ctlNum) const;
    bool isCtlReg(RegName) const;
    int8_t encodeCtlReg(RegName ctlReg) const;

    CcName parseCcName(const char *line) const;
    uint8_t ccNameLen(const CcName ccName) const;
    int8_t encodeCcName(CcName ccName) const;
    CcName decodeCcNum(uint8_t ccNum) const;
    char *outCcName(char *out, CcName ccName) const;

    char *outFlagNames(char *out, uint8_t flagNames) const;
    FlagName parseFlagName(const char *line) const;
    uint8_t flagNameLen(FlagName flag) const;
    uint8_t encodeFlagName(FlagName flag) const;

    char *outIntrNames(char *out, uint8_t intrNames) const;
    IntrName parseIntrName(const char *line) const;
    uint8_t intrNameLen(IntrName intrName) const;
    uint8_t encodeIntrName(IntrName intrName) const;

private:
    char *outCtlName(char *out, RegName regName) const;
    uint8_t ctlRegLen(RegName) const;
    char *outChar(char *out, char c) const;
    char *outText(char *out, const char *text) const;
    bool compareText(const char *p, const char *text) const;
};

} // namespace z8000
} // namespace libasm

#endif // __REG_Z8000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4: