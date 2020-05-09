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

#ifndef __REGISTER_INS8060_H__
#define __REGISTER_INS8060_H__

namespace libasm {
namespace ins8060 {

enum RegName : char {
    REG_UNDEF = 0,
    REG_P0 = '0',
    REG_P1 = '1',
    REG_P2 = '2',
    REG_P3 = '3',
    REG_E  = 'E',
    REG_PC = 'P',
};

class RegIns8060 : public RegBase {
public:
    RegName parseRegister(const char *line) const;
    RegName parsePointerReg(const char *line) const;

    static host::int_t encodePointerReg(RegName regName);

    static RegName decodePointerReg(uint8_t regNum);

    bool compareRegName(const char *line, RegName regName) const;
    host::uint_t regNameLen(RegName regName) const;

    char *outRegName(char *out, const RegName regName) const;

private:
    RegName parseRegName(
        const char *line, const RegName *tabel, const RegName *end) const;
    char regName1stChar(RegName regName) const;
    char regName2ndChar(RegName regName) const;
};

} // namespace ins8060
} // namespace libasm

#endif // __REGISTER_INS8060_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
