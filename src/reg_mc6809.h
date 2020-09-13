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

#include "entry_mc6809.h"
#include "reg_base.h"

namespace libasm {
namespace mc6809 {

enum RegName : char {
    REG_UNDEF = 0,
    OFFSET =  1, // Describe offset in indexed addressing
    REG_PCR = 2, // Program counter relative addressing
    REG_D =   8+0,
    REG_X =   8+1,
    REG_Y =   8+2,
    REG_U =   8+3,
    REG_S =   8+4,
    REG_PC =  8+5,
    REG_W =   8+6,
    REG_V =   8+7,
    REG_A =   8+8,
    REG_B =   8+9,
    REG_CC =  8+10,
    REG_DP =  8+11,
    REG_Z =   8+12, // "z" zero register
    REG_0 =   8+13, // "0" zero register
    REG_E =   8+14,
    REG_F =   8+15,
};

enum RegSize : uint8_t {
    SZ_BYTE = 0,
    SZ_WORD = 1,
    SZ_NONE = 2,
};

class RegMc6809 : public RegBase {
public:
    RegName parseRegName(const char *line) const;
    uint8_t regNameLen(RegName name) const;
    RegSize regSize(RegName name) const;
    char *outRegName(char *out, const RegName name) const;

    RegName decodeDataReg(uint8_t num) const;
    int8_t encodeDataReg(RegName ame) const;

    RegName decodeBaseReg(uint8_t num) const;
    int8_t encodeBaseReg(RegName name) const;
    bool isBaseReg(RegName name) const;

    int8_t encodeIndexReg(RegName name) const;
    bool isIndexReg(RegName name) const;
    bool isIndexedBase(RegName name) const;

    RegName decodeStackReg(uint8_t bitPos, bool onUserStack) const;
    uint8_t encodeStackReg(RegName name, bool onUserStack) const;

    char *outCCRBits(char *out, uint8_t val) const;

    RegName decodeBitOpReg(uint8_t num) const;
    bool isBitOpReg(RegName name) const;
    int8_t encodeBitOpReg(RegName name) const;

    RegName decodeTfmBaseReg(uint8_t num) const;
    bool isTfmBaseReg(RegName name) const;
    char tfmSrcModeChar(uint8_t mode) const;
    char tfmDstModeChar(uint8_t mode) const;
    int8_t encodeTfmBaseReg(RegName name) const;
};

} // namespace mc6809
} // namespace libasm

#endif // __REG_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
