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

#ifndef __LIBASM_REG_MC68000_H__
#define __LIBASM_REG_MC68000_H__

#include <stdint.h>
#include "entry_mc68000.h"

namespace libasm {

struct StrBuffer;
struct StrScanner;
struct ValueParser;

namespace mc68000 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    // Data registers.
    REG_D0 = 0 + 0,
    REG_D1 = 1 + 0,
    REG_D2 = 2 + 0,
    REG_D3 = 3 + 0,
    REG_D4 = 4 + 0,
    REG_D5 = 5 + 0,
    REG_D6 = 6 + 0,
    REG_D7 = 7 + 0,
    // Address registers.
    REG_A0 = 0 + 8,
    REG_A1 = 1 + 8,
    REG_A2 = 2 + 8,
    REG_A3 = 3 + 8,
    REG_A4 = 4 + 8,
    REG_A5 = 5 + 8,
    REG_A6 = 6 + 8,
    REG_A7 = 7 + 8,
    // Floating point data register
    REG_FP0 = 0 + 16,
    REG_FP1 = 1 + 16,
    REG_FP2 = 2 + 16,
    REG_FP3 = 3 + 16,
    REG_FP4 = 4 + 16,
    REG_FP5 = 5 + 16,
    REG_FP6 = 6 + 16,
    REG_FP7 = 7 + 16,
    // Other registers.
    REG_PC = 0 + 24,
    REG_CCR = 1 + 24,
    REG_SR = 2 + 24,
    // Floating point status/control registers
    REG_FPCR = 4 + 24,
    REG_FPSR = 5 + 24,
    REG_FPIAR = 6 + 24,
    // Control register
    REG_SFC = 32 + 0,
    REG_DFC = 32 + 1,
    REG_USP = 32 + 2,
    REG_VBR = 32 + 3,
};

namespace reg {

RegName parseRegName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outRegName(StrBuffer &out, RegName name);
bool isDataReg(RegName name);
bool isAddrReg(RegName name);
bool isGeneralReg(RegName name);
bool isControlReg(RegName name);
bool isFloatReg(RegName name);
bool isFloatControlReg(RegName name);
Config::opcode_t encodeGeneralRegNo(RegName name);
Config::opcode_t encodeControlRegNo(RegName name);
uint_fast8_t encodeGeneralRegPos(RegName name);
uint_fast8_t encodeFloatRegPos(RegName name);
uint_fast8_t encodeFloatControlRegPos(RegName name);
RegName decodeGeneralReg(uint_fast8_t regno);
RegName decodeDataReg(uint_fast8_t regno);
RegName decodeAddrReg(uint_fast8_t regno);
RegName decodeControlReg(Config::opcode_t regno);

uint_fast8_t encodeAddrMode(AddrMode mode);
uint_fast8_t encodeRegNo(AddrMode mode, RegName reg);
AddrMode decodeAddrMode(uint_fast8_t mode, uint_fast8_t regno);
RegName decodeRegNo(uint_fast8_t mode, uint_fast8_t regno);

InsnSize parseSize(StrScanner &scan);
InsnSize parseIndexSize(StrScanner &scan);
uint_fast8_t sizeNameLen(OprSize size);
char sizeSuffix(OprSize size);

}  // namespace reg
}  // namespace mc68000
}  // namespace libasm

#endif  // __LIBASM_REG_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
