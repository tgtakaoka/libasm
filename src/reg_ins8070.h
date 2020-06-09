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

#ifndef __REG_INS8070_H__
#define __REG_INS8070_H__

namespace libasm {
namespace ins8070 {

enum RegName : char {
    REG_UNDEF = 0,
    REG_PC = 'P',
    REG_SP = 'S',
    REG_P2 = '2',
    REG_P3 = '3',
    REG_A  = 'A',
    REG_E  = 'E',
    REG_EA = 'e',
    REG_T  = 'T',
    REG_S  = 's',
};

class RegIns8070 : public RegBase {
public:
    RegName parseRegister(const char *line) const;
    RegName parsePointerReg(const char *line) const;

    static int8_t encodePointerReg(RegName regName);

    static RegName decodePointerReg(uint8_t regNum);

    bool compareRegName(const char *line, RegName regName) const;
    uint8_t regNameLen(RegName regName) const;

    char *outRegName(char *out, const RegName regName) const;

private:
    RegName parseRegName(
        const char *line, const RegName *tabel, const RegName *end) const;
    char regName1stChar(RegName regName) const;
    char regName2ndChar(RegName regName) const;
};

} // namespace ins8070
} // namespace libasm

#endif // __REG_INS8070_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
