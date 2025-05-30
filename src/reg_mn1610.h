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

#ifndef __LIBASM_REG_MN1610_H__
#define __LIBASM_REG_MN1610_H__

#include <stdint.h>
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {

struct ValueParser;

namespace mn1610 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    // Generic register
    REG_R0 = 0,
    REG_R1 = 1,
    REG_R2 = 2,
    REG_R3 = 3,
    REG_R4 = 4,
    REG_SP = 5,
    REG_STR = 6,
    // None generic register
    REG_IC = 7,
    REG_DR0 = 8,
    // Register Alias
    REG_X0 = REG_R3 + 8,  // R3
    REG_X1 = REG_R4 + 8,  // R4
    // Segment base register
    REG_CSBR = 0 + 16,
    REG_SSBR = 1 + 16,
    REG_TSR0 = 2 + 16,
    REG_TSR1 = 3 + 16,
    REG_OSR0 = 4 + 16,
    REG_OSR1 = 5 + 16,
    REG_OSR2 = 6 + 16,
    REG_OSR3 = 7 + 16,
    // Hardware control register
    REG_TCR = 0 + 24,
    REG_TIR = 1 + 24,
    REG_TSR = 2 + 24,
    REG_SCR = 3 + 24,
    REG_SSR = 4 + 24,
    REG_SIR = 5 + 24,
    REG_IISR = 6 + 24,
    REG_SOR = 7 + 24,
    // Special register
    REG_SBRB = 0 + 32,
    REG_ICB = 1 + 32,
    REG_NPP = 2 + 32,
};

enum CcName : int8_t {
    CC_UNDEF = -1,
    CC_NONE = 0,
    CC_SKP = 1,
    CC_M = 2,
    CC_PZ = 3,
    CC_Z = 4,
    CC_NZ = 5,
    CC_MZ = 6,
    CC_P = 7,
    CC_EZ = 8,
    CC_ENZ = 9,
    CC_OZ = 10,
    CC_ONZ = 11,
    CC_LMZ = 12,
    CC_LP = 13,
    CC_LPZ = 14,
    CC_LM = 15,
    // Alias
    CC_E = CC_Z + 16,
    CC_NE = CC_NZ + 16,
    // Carry mode
    CC_C = 32,
    // E register operation
    CC_RE = 1 + 48,
    CC_SE = 2 + 48,
    CC_CE = 3 + 48,
};

namespace reg {

RegName parseRegName(StrScanner &scan, const ValueParser &parser);
bool isGeneric(RegName name);
bool isIndex(RegName name);
bool isIndirect(RegName name);
bool isSegmentBase(RegName name);
bool isSegment(RegName name);
bool isHardware(RegName name);
bool isSpecial(RegName name);
uint16_t encodeGeneric(RegName name);
uint16_t encodeIndex(RegName name);
uint16_t encodeIndirect(RegName name);
uint16_t encodeSegment(RegName name);
uint16_t encodeHardware(RegName name);
uint16_t encodeSpecial(RegName name);
RegName decodeRegNum(uint8_t num);
RegName decodeIndirect(uint8_t num);
RegName decodeSegment(uint8_t num);
RegName decodeHardware(uint8_t num);
RegName decodeSpecial(uint8_t num);
StrBuffer &outRegName(StrBuffer &out, RegName name);

CcName parseCcName(StrScanner &scan, const ValueParser &parser);
bool isSkip(CcName name);
bool isCop(CcName name);
bool isEop(CcName name);
uint16_t encodeSkip(CcName name);
uint16_t encodeCop(CcName name);
uint16_t encodeEop(CcName name);
CcName decodeSkip(uint8_t num);
CcName decodeCop(uint8_t num);
CcName decodeEop(uint8_t num);
StrBuffer &outCcName(StrBuffer &out, CcName name);

}  // namespace reg
}  // namespace mn1610
}  // namespace libasm

#endif  // __LIBASM_REG_MN1610_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
