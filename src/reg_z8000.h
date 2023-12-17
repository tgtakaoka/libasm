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

#ifndef __LIBASM_REG_Z8000_H__
#define __LIBASM_REG_Z8000_H__

#include <stdint.h>

#include "entry_z8000.h"
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {
namespace z8000 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    REG_ILLEGAL = -2,
    // Word Register: 0~15
    REG_R0 = 0,
    REG_R1 = REG_R0 + 1,
    REG_R2 = REG_R0 + 2,
    REG_R3 = REG_R0 + 3,
    REG_R4 = REG_R0 + 4,
    REG_R5 = REG_R0 + 5,
    REG_R6 = REG_R0 + 6,
    REG_R7 = REG_R0 + 7,
    REG_R8 = REG_R0 + 8,
    REG_R9 = REG_R0 + 9,
    REG_R10 = REG_R0 + 10,
    REG_R11 = REG_R0 + 11,
    REG_R12 = REG_R0 + 12,
    REG_R13 = REG_R0 + 13,
    REG_R14 = REG_R0 + 14,
    REG_R15 = REG_R0 + 15,
    // Byte Register: 16~31
    REG_RH0 = 16,
    REG_RH1 = REG_RH0 + 1,
    REG_RH2 = REG_RH0 + 2,
    REG_RH3 = REG_RH0 + 3,
    REG_RH4 = REG_RH0 + 4,
    REG_RH5 = REG_RH0 + 5,
    REG_RH6 = REG_RH0 + 6,
    REG_RH7 = REG_RH0 + 7,
    REG_RL0 = REG_RH0 + 8,
    REG_RL1 = REG_RL0 + 1,
    REG_RL2 = REG_RL0 + 2,
    REG_RL3 = REG_RL0 + 3,
    REG_RL4 = REG_RL0 + 4,
    REG_RL5 = REG_RL0 + 5,
    REG_RL6 = REG_RL0 + 6,
    REG_RL7 = REG_RL0 + 7,
    // Long Register:32 ~ 47
    REG_RR0 = 32,
    REG_RR2 = REG_RR0 + 2,
    REG_RR4 = REG_RR0 + 4,
    REG_RR6 = REG_RR0 + 6,
    REG_RR8 = REG_RR0 + 8,
    REG_RR10 = REG_RR0 + 10,
    REG_RR12 = REG_RR0 + 12,
    REG_RR14 = REG_RR0 + 14,
    // Quad Register: 48~63
    REG_RQ0 = 48,
    REG_RQ4 = REG_RQ0 + 4,
    REG_RQ8 = REG_RQ0 + 8,
    REG_RQ12 = REG_RQ0 + 12,
    // Special Register: 64~
    REG_CTLBASE = 64,
    REG_FLAGS = REG_CTLBASE + 1,
    REG_FCW = REG_CTLBASE + 2,
    REG_REFRESH = REG_CTLBASE + 3,
    REG_PSAPSEG = REG_CTLBASE + 4,
    REG_PSAPOFF = REG_CTLBASE + 5,
    REG_NSPSEG = REG_CTLBASE + 6,
    REG_NSPOFF = REG_CTLBASE + 7,
    REG_PSAP = REG_PSAPOFF + 8,
    REG_NSP = REG_NSPOFF + 8,
};

enum CcName : int8_t {
    CC_UNDEF = -1,
    CC_F = 0,     // Always false
    CC_LT = 1,    // Less than
    CC_LE = 2,    // Less than or equal
    CC_ULE = 3,   // Unsigned less than or equal
    CC_OV = 4,    // Overflow
    CC_MI = 5,    // Minus
    CC_Z = 6,     // Zero
    CC_C = 7,     // Carry
    CC_T = 8,     // Always true
    CC_GE = 9,    // Greater than or equal
    CC_GT = 10,   // Greater than
    CC_UGT = 11,  // Unsigned greater than
    CC_NOV = 12,  // No overflow
    CC_PL = 13,   // Plus
    CC_NZ = 14,   // Not zero
    CC_NC = 15,   // No carry
    // Alias
    CC_EQ = 6 + 16,    // Z: Equal
    CC_ULT = 7 + 16,   // C: Unsigned less than
    CC_NE = 14 + 16,   // NZ: Nor equal
    CC_UGE = 15 + 16,  // NC: Unsigned greater than or equal
};

enum FlagName : int8_t {
    FLAG_UNDEF = 0,
    FLAG_C = 8,  // Carry
    FLAG_Z = 4,  // Zero
    FLAG_S = 2,  // Sign
    FLAG_P = 1,  // Parity
    // Alias
    FLAG_V = 1 + 16,  // Overflow
};

enum IntrName : int8_t {
    INTR_UNDEF = 0,
    INTR_NVI = 1,  // Non-Vectored Interrupt
    INTR_VI = 2,   // Vectored Interrupt
};

namespace reg {

RegName parseRegName(StrScanner &scan);
StrBuffer &outRegName(StrBuffer &out, RegName name);
uint8_t encodeGeneralRegName(RegName name);
bool decodeRegNum(bool segmentedModel, uint8_t num, OprSize size, RegName &name);
RegName decodeByteReg(uint8_t num);
RegName decodeWordReg(uint8_t num);
RegName decodeLongReg(uint8_t num);
RegName decodeQuadReg(uint8_t num);
bool isByteReg(RegName name);
bool isWordReg(RegName name);
bool isLongReg(RegName name);
bool isQuadReg(RegName name);

RegName parseCtlReg(StrScanner &scan);
RegName decodeCtlReg(bool segmentedModel, uint8_t num);
bool isCtlReg(RegName name);
int8_t encodeCtlReg(bool segmentedModel, RegName name);

CcName parseCcName(StrScanner &scan);
StrBuffer &outCcName(StrBuffer &out, CcName name);
uint8_t encodeCcName(CcName name);
CcName decodeCcNum(uint8_t num);

FlagName parseFlagName(StrScanner &scan);
StrBuffer &outFlagNames(StrBuffer &out, uint8_t flags);
uint8_t encodeFlagName(FlagName name);

IntrName parseIntrName(StrScanner &scan);
StrBuffer &outIntrNames(StrBuffer &out, uint8_t intrs);
uint8_t encodeIntrName(IntrName name);

bool checkOverlap(RegName dst, RegName src, RegName cnt = REG_UNDEF);

StrBuffer &outCtlName(StrBuffer &out, RegName name);
uint8_t ctlRegLen(RegName name);

}  // namespace reg
}  // namespace z8000
}  // namespace libasm

#endif  // __REG_Z8000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
