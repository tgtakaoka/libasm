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

#ifndef __LIBASM_REG_TLCS900_H__
#define __LIBASM_REG_TLCS900_H__

#include <stdint.h>
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {

struct ValueParser;

namespace tlcs900 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    // 8-bit registers: M_REG8
    REG_W = 0,
    REG_A = 1,
    REG_B = 2,
    REG_C = 3,
    REG_D = 4,
    REG_E = 5,
    REG_H = 6,
    REG_L = 7,
    // 16-bit registers: M_REG16
    REG_WA = 0 + 8,
    REG_BC = 1 + 8,
    REG_DE = 2 + 8,
    REG_HL = 3 + 8,
    REG_IX = 4 + 8,
    REG_IY = 5 + 8,
    REG_IZ = 6 + 8,
    REG_SP = 7 + 8,
    // 32-bit registers: M_REG32
    REG_XWA = 0 + 16,
    REG_XBC = 1 + 16,
    REG_XDE = 2 + 16,
    REG_XHL = 3 + 16,
    REG_XIX = 4 + 16,
    REG_XIY = 5 + 16,
    REG_XIZ = 6 + 16,
    REG_XSP = 7 + 16,
    // Special register
    REG_SR = 24,
    // TLCS-900
    REG_NXSP = 32,
};

enum MapReg : uint8_t {
    // 8-bit
    MAP_RA = 0x00,
    MAP_RW = 0x01,
    MAP_QA = 0x02,  // E2
    MAP_QW = 0x03,  // E3
    MAP_RC = 0x04,
    MAP_RB = 0x05,
    MAP_QC = 0x06,  // E6
    MAP_QB = 0x07,  // E7
    MAP_RE = 0x08,
    MAP_RD = 0x09,
    MAP_QE = 0x0A,  // EA
    MAP_QD = 0x0B,  // EB
    MAP_RL = 0x0C,
    MAP_RH = 0x0D,
    MAP_QL = 0x0E,  // EE
    MAP_QH = 0x0F,  // EF
    // 16-bit
    MAP_RWA = 0x10,
    MAP_QWA = 0x12,
    MAP_RBC = 0x14,
    MAP_QBC = 0x16,
    MAP_RDE = 0x18,
    MAP_QDE = 0x1A,
    MAP_RHL = 0x1C,
    MAP_QHL = 0x1E,
    // 32-bit
    MAP_XWA = 0x20,
    MAP_XBC = 0x24,
    MAP_XDE = 0x28,
    MAP_XHL = 0x2C,
    // 16-bit
    MAP_WA = 0x30,   // E0
    MAP_BC = 0x34,   // E4
    MAP_DE = 0x38,   // E8
    MAP_HL = 0x3C,   // EC
    MAP_IX = 0x40,   // F0
    MAP_QIX = 0x42,  // F2
    MAP_IY = 0x44,   // F4
    MAP_QIY = 0x46,  // F6
    MAP_IZ = 0x48,   // F8
    MAP_QIZ = 0x4A,  // FA
    MAP_SP = 0x4C,   // FC
    MAP_QSP = 0x4E,  // FE
    // 32-bit
    MAP_XIX = 0x50,  // F0
    MAP_XIY = 0x54,  // F4
    MAP_XIZ = 0x58,  // F8
    MAP_XSP = 0x5C,  // FC
    // 8-bit
    MAP_A = 0xE0,
    MAP_W = 0xE1,
    MAP_C = 0xE4,
    MAP_B = 0xE5,
    MAP_E = 0xE8,
    MAP_D = 0xE9,
    MAP_L = 0xEC,
    MAP_H = 0xED,
    MAP_IXL = 0xF0,
    MAP_IXH = 0xF1,
    MAP_QIXL = 0xF2,
    MAP_QIXH = 0xF3,
    MAP_IYL = 0xF4,
    MAP_IYH = 0xF5,
    MAP_QIYL = 0xF6,
    MAP_QIYH = 0xF7,
    MAP_IZL = 0xF8,
    MAP_IZH = 0xF9,
    MAP_QIZL = 0xFA,
    MAP_QIZH = 0xFB,
    MAP_SPL = 0xFC,
    MAP_SPH = 0xFD,
    MAP_QSPL = 0xFE,
    MAP_QSPH = 0xFF,
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
    CC_ALIAS = 16,
    CC_PE = CC_OV + CC_ALIAS,   // == OV(4)
    CC_M = CC_MI + CC_ALIAS,    // == MI(5)
    CC_EQ = CC_Z + CC_ALIAS,    // == Z(6)
    CC_ULT = CC_C + CC_ALIAS,   // == C(7)
    CC_PO = CC_NOV + CC_ALIAS,  // == NOV(12)
    CC_P = CC_PL + CC_ALIAS,    // == PL(13)
    CC_NE = CC_NZ + CC_ALIAS,   // == NZ(14)
    CC_UGE = CC_NC + CC_ALIAS,  // == NC(15)
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
}  // namespace tlcs900
}  // namespace libasm

#endif  // __LIBASM_REG_TLCS900_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
