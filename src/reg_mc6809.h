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
    OFFSET =  'o', // Describe offset in indexed addressing
    REG_D =   'D',
    REG_X =   'X',
    REG_Y =   'Y',
    REG_U =   'U',
    REG_S =   'S',
    REG_PC =  'P',
    REG_PCR = 'p', // Program counter relative addressing
    REG_A =   'A',
    REG_B =   'B',
    REG_CC =  'c',
    REG_DP =  'd',
    REG_E =   'E',
    REG_F =   'F',
    REG_W =   'W',
    REG_V =   'V',
    REG_Z =   'Z', // "z" zero register
    REG_0 =   '0', // "0" zero register
};

enum RegSize {
    SZ_BYTE = 0,
    SZ_WORD = 1,
    SZ_NONE = 2,
};

class RegMc6809 : public RegBase {
public:
    RegName parseRegName(const char *line) const;
    RegName parseIndexReg(const char *line) const;
    RegName parseBaseReg(const char *line) const;
    RegName parseDataReg(const char *line) const;
    int8_t encodeIndexReg(RegName regName) const;
    int8_t encodeBaseReg(RegName regName) const;
    int8_t encodeDataReg(RegName regName) const;
    bool isIndexReg(RegName regName) const;
    bool isBaseReg(RegName regName) const;
    static uint8_t encodeStackReg(RegName regName, bool onUserStack);
    RegName decodeBaseReg(uint8_t regNum) const;
    RegName decodeRegName(uint8_t regNum) const;
    RegName decodeStackReg(uint8_t bitPos, bool onUserStack) const;

    char *outRegName(char *out, const RegName regName) const;
    char *outCCRBits(char *out, uint8_t val) const;

    bool compareRegName(const char *line, RegName regName) const;
    uint8_t regNameLen(RegName regName) const;

    RegName parseBitOpReg(const char *line) const;
    static int8_t encodeBitOpReg(RegName regName);

    RegName parseTfmBaseReg(const char *line) const;
    bool isTfmBaseReg(RegName regName) const;
    static int8_t encodeTfmBaseReg(RegName regName);
    static RegName decodeBitOpReg(uint8_t regNum);
    static RegName decodeTfmBaseReg(uint8_t regNum);
    static char tfmSrcModeChar(uint8_t mode);
    static char tfmDstModeChar(uint8_t mode);

    static RegSize regSize(RegName regName);

private:
    RegName parseRegName(
        const char *line, const RegName *table, const RegName *end) const;

    char regName1stChar(const RegName regName) const;
    char regName2ndChar(const RegName regName) const;
    char regName3rdChar(const RegName regName) const;
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
