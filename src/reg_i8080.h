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

#ifndef __REGISTER_I8080_H__
#define __REGISTER_I8080_H__

namespace libasm {
namespace i8080 {

enum RegName : char {
    REG_UNDEF = 0,
    REG_B = 'B',
    REG_C = 'C',
    REG_D = 'D',
    REG_E = 'E',
    REG_H = 'H',
    REG_L = 'L',
    REG_M = 'M',
    REG_A = 'A',
    REG_SP = 'S',
    REG_PSW = 'P',
};

class RegI8080 : public RegBase {
public:
    RegName parseRegister(const char *line) const;
    RegName parsePointerReg(const char *line) const;
    RegName parseStackReg(const char *line) const;
    RegName parseIndexReg(const char *line) const;
    RegName parseDataReg(const char *line) const;

    static int8_t encodePointerReg(RegName regName);
    static int8_t encodeStackReg(RegName regName);
    static int8_t encodeIndexReg(RegName regName);
    static int8_t encodeDataReg(RegName regName);

    static RegName decodePointerReg(uint8_t regNum);
    static RegName decodeStackReg(uint8_t regNum);
    static RegName decodeIndexReg(uint8_t regNum);
    static RegName decodeDataReg(uint8_t regNum);

    bool compareRegName(const char *line, RegName regName) const;
    uint8_t regNameLen(RegName regName) const;

    char *outRegName(char *out, const RegName regName) const;

private:
    RegName parseRegName(
        const char *line, const RegName *tabel, const RegName *end) const;
    char regName1stChar(RegName regName) const;
    char regName2ndChar(RegName regName) const;
    char regName3rdChar(RegName regName) const;
};

} // namespace i8080
} // namespace libasm

#endif // __REGISTER_I8080_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
