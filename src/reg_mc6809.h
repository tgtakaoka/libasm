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

#ifndef __REG_MC6809_H__
#define __REG_MC6809_H__

#include <stdint.h>

#include "entry_mc6809.h"
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {
namespace mc6809 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    // Data registers.
    REG_D = 0,
    REG_X = 1,
    REG_Y = 2,
    REG_U = 3,
    REG_S = 4,
    REG_PC = 5,
    REG_W = 6,  // 6309
    REG_V = 7,  // 6309
    REG_A = 8,
    REG_B = 9,
    REG_CC = 10,
    REG_DP = 11,
    REG_0 = 12,  // 6309: "0" zero register
    REG_Z = 13,  // 6309: "z" zero register
    REG_E = 14,  // 6309
    REG_F = 15,  // 6309
    // Other registers.
    REG_PCR = 0 + 16,  // Program counter relative addressing
};

enum RegSize : uint8_t {
    SZ_BYTE = 0,
    SZ_WORD = 1,
    SZ_NONE = 2,
};

namespace reg {

RegName parseRegName(StrScanner &scan);
RegSize regSize(RegName name) __attribute__((noinline));
StrBuffer &outRegName(StrBuffer &out, RegName name);

RegName decodeDataReg(CpuType cpuType, uint8_t num);
bool isDataReg(CpuType cpuType, RegName name);
uint8_t encodeDataReg(RegName name);

RegName decodeBaseReg(uint8_t num);
bool isBaseReg(RegName name);
bool isIndexedBase(RegName name);
uint8_t encodeBaseReg(RegName name);

RegName decodeStackReg(uint8_t bitPos, bool userStack);
uint8_t encodeStackReg(RegName name, bool userStack) __attribute__((noinline));

RegName decodeBitOpReg(uint8_t num);
bool isBitOpReg(RegName name);
uint8_t encodeBitOpReg(RegName name);

RegName decodeTfmBaseReg(uint8_t num);
bool isTfmBaseReg(RegName name);
uint8_t encodeTfmBaseReg(RegName name);
int8_t encodeTfmMode(char src, char dst);
char tfmSrcModeChar(uint8_t mode);
char tfmDstModeChar(uint8_t mode);

}  // namespace reg
}  // namespace mc6809
}  // namespace libasm

#endif  // __REG_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
