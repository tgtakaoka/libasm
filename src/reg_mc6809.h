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
    REG_Z =   'Z', // zero register
    REG_0 =   '0', // "0" zero register
    REG_00 =  '1', // "00" zero register
};

#include "reg_base.h"

class RegMc6809 : public RegBase {
public:
    RegName parseIndexReg(const char *line) const;
    RegName parseBaseReg(const char *line) const;
    RegName parseDataReg(const char *line) const;
    host::int_t encodeIndexReg(RegName regName) const;
    host::int_t encodeBaseReg(RegName regName) const;
    host::int_t encodeDataReg(RegName regName) const;
    RegName decodeIndexReg(uint8_t regNum) const;
    RegName decodeBaseReg(uint8_t regNum) const;
    RegName decodeRegName(uint8_t regNum) const;

    char *outRegName(char *out, const RegName regName) const;
    char *outCCRBits(char *out, uint8_t val) const;

    static RegName getStackReg(host::uint_t bit, target::insn_t insnCode);
    bool compareRegName(const char *line, RegName regName) const;
    host::uint_t regNameLen(RegName regName) const;

    RegName parseBitOpReg(const char *line) const;
    static host::int_t encodeBitOpReg(RegName regName);

    RegName parseTfmBaseReg(const char *line) const;
    static host::int_t encodeTfmBaseReg(RegName regName);
    static RegName decodeBitOpReg(uint8_t regNum);
    static RegName decodeTfmBaseReg(uint8_t regNum);
    static char tfmSrcModeChar(host::uint_t mode);
    static char tfmDstModeChar(host::uint_t mode);

    static OprSize regSize(RegName regName);

private:
    RegName parseRegName(
        const char *line, const RegName *table, const RegName *end) const;

    char regName1stChar(const RegName regName) const;
    char regName2ndChar(const RegName regName) const;
    char regName3rdChar(const RegName regName) const;
};

#endif // __REG_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
