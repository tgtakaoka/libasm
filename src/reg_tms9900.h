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

#ifndef __REG_TMS9900_H__
#define __REG_TMS9900_H__

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
    REG_R8 = '8',
    REG_R9 = '9',
    REG_R10 = 'A',
    REG_R11 = 'B',
    REG_R12 = 'C',
    REG_R13 = 'D',
    REG_R14 = 'E',
    REG_R15 = 'F',
};

#include "reg_base.h"

class RegTms9900 : public RegBase {
public:
    host::uint_t regNameLen(RegName regName) const;
    RegName parseRegName(const char *line) const;
    char *outRegName(char *out, host::uint_t regno) const;
    uint16_t encodeRegNumber(RegName regName) const;
};

#endif // __REG_TMS9900_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
