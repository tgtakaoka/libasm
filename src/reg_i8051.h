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

#include "reg_base.h"

#ifndef __REG_I8051_H__
#define __REG_I8051_H__

namespace libasm {
namespace i8051 {

enum RegName : char {
    REG_UNDEF = 0,
    REG_R0 = '0',
    REG_R1 = '1',
    REG_R2 = '2',
    REG_R3 = '3',
    REG_R4 = '4',
    REG_R5 = '5',
    REG_R6 = '6',
    REG_R7 = '7',
    REG_A  = 'A',
    REG_AB = 'a',
    REG_C  = 'C',
    REG_DPTR = 'd',
    REG_PC = 'p',
};

class RegI8051 : public RegBase {
public:
    RegName parseRegister(const char *line) const;

    bool isRReg(RegName regName) const;
    int8_t encodeRReg(RegName regName) const;
    RegName decodeRReg(uint8_t regNum) const;

    bool compareRegName(const char *line, RegName regName) const;
    uint8_t regNameLen(RegName regName) const;

    char *outRegName(char *out, const RegName regName) const;

private:
    RegName parseRegName(
        const char *line, const RegName *tabel, const RegName *end) const;
    char regName1stChar(RegName regName) const;
    char regName2ndChar(RegName regName) const;
    char regName3rdChar(RegName regName) const;
    char regName4thChar(RegName regName) const;
};

} // namespace i8051
} // namespace libasm

#endif // __REG_I8051_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
