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

#ifndef __LIBASM_REG_Z80_H__
#define __LIBASM_REG_Z80_H__

#include <stdint.h>
#include "config_z80.h"
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {

struct ValueParser;

namespace z80 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    // 16-bit registers
    // Pointer registers
    REG_BC = 0,
    REG_DE = 1,
    REG_HL = 2,
    REG_SP = 3,
    REG_IX = 4,
    REG_IY = 5,
    REG_AF = 6,
    // 8-bit registers
    // Data registers
    REG_B = 7,
    REG_C = 8,
    REG_D = 9,
    REG_E = 10,
    REG_H = 11,
    REG_L = 12,
    REG_A = 14,    // Missing 13 is (HL)
    REG_IXH = 15,  // Z280
    REG_IXL = 16,  // Z280
    REG_IYH = 17,  // Z280
    REG_IYL = 18,  // Z280
    // Other registers
    REG_I = 19,
    REG_R = 20,
    REG_DEHL = 21,  // Z280
    REG_USP = 22,   // Z280
    REG_PC = 23,    // Z280
    // Alternate register
    ALT_BASE = 24,
    REG_AFP = REG_AF + ALT_BASE,
};

enum CcName : int8_t {
    CC_UNDEF = -1,
    CC_NZ = 0,
    CC_Z = 1,
    CC_NC = 2,
    CC_C = 3,
    CC_PO = 4,
    CC_PE = 5,
    CC_P = 6,
    CC_M = 7,
    // alias (Z280)
    CC_alias = 8,
    CC_NV = CC_PO + CC_alias,
    CC_V = CC_PE + CC_alias,
    CC_NS = CC_P + CC_alias,
    CC_S = CC_M + CC_alias,
};

namespace reg {

RegName parseRegName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outRegName(StrBuffer &out, RegName name);
uint8_t encodeDataReg(RegName name);
RegName decodeDataReg(uint8_t num);
uint8_t encodePointerReg(RegName name);
uint8_t encodePointerRegIx(RegName name, RegName ix);
uint8_t encodeStackReg(RegName name);
RegName decodeStackReg(uint8_t num);
uint8_t encodeIndirectBase(RegName name);
RegName decodeIndirectBase(uint8_t num);
bool isIndexReg(RegName name);

CcName parseCcName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outCcName(StrBuffer &out, CcName cc);
bool isCc4Name(CcName name);
bool isCcAlias(CcName name);
uint8_t encodeCcName(CcName name);
CcName decodeCcName(uint8_t num);

}  // namespace reg
}  // namespace z80
}  // namespace libasm

#endif  // __LIBASM_REG_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
