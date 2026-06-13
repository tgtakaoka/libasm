/*
 * Copyright 2025 Tadashi G. Takaoka
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

#ifndef __LIBASM_REG_H8500_H__
#define __LIBASM_REG_H8500_H__

#include <stdint.h>

#include "str_buffer.h"
#include "str_scanner.h"
#include "value_parser.h"

namespace libasm {
namespace h8500 {

// 16-bit general registers R0-R7, encoded as 3-bit field (0-7)
// R6 = FP (frame pointer), R7 = SP (stack pointer)
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
    REG_FP = REG_R6,
    REG_SP = REG_R7,
};

// Control registers encoded as 3-bit ccc field in LDC/STC/ANDC/ORC/XORC
// 000=SR (word), 001=CCR (byte), 010=invalid, 011=BR (byte)
// 100=EP (byte H8/532), 101=DP (byte H8/532), 110=invalid, 111=TP (byte H8/532)
enum CrName : int8_t {
    CR_UNDEF = -1,
    CR_SR  = 0,  // status register (word)
    CR_CCR = 1,  // condition code register (byte, low of SR)
    CR_BR  = 3,  // base register (byte)
    CR_EP  = 4,  // extra page register (byte, H8/532)
    CR_DP  = 5,  // data page register (byte, H8/532)
    CR_TP  = 7,  // stack page register (byte, H8/532)
};

// Condition codes for Bcc, encoded as 4-bit cc field
enum CcName : int8_t {
    CC_UNDEF = -1,
    CC_T  = 0,   // BRA / BT
    CC_F  = 1,   // BRN / BF
    CC_HI = 2,   // BHI
    CC_LS = 3,   // BLS
    CC_CC = 4,   // BCC / BHS
    CC_CS = 5,   // BCS / BLO
    CC_NE = 6,   // BNE
    CC_EQ = 7,   // BEQ
    CC_VC = 8,   // BVC
    CC_VS = 9,   // BVS
    CC_PL = 10,  // BPL
    CC_MI = 11,  // BMI
    CC_GE = 12,  // BGE
    CC_LT = 13,  // BLT
    CC_GT = 14,  // BGT
    CC_LE = 15,  // BLE
};

namespace reg {

RegName parseRegName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outRegName(StrBuffer &out, RegName name);

CrName parseCrName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outCrName(StrBuffer &out, CrName name);

CcName parseCcName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outCcName(StrBuffer &out, CcName name);

uint8_t encodeReg(RegName reg);    // returns 3-bit encoding (0-7)
RegName decodeReg(uint8_t bits3);  // decode 3-bit field

uint8_t encodeCr(CrName cr);    // returns 3-bit ccc encoding
CrName decodeCr(uint8_t ccc);   // decode 3-bit ccc field

bool isBytesCr(CrName cr);      // true if CR is byte-sized (CCR, BR, EP, DP, TP)

}  // namespace reg
}  // namespace h8500
}  // namespace libasm

#endif  // __LIBASM_REG_H8500_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
