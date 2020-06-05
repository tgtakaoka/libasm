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

#ifndef __REG_W65C816_H__
#define __REG_W65C816_H__

#include "reg_base.h"

namespace libasm {
namespace w65c816 {

enum RegName : char {
    REG_UNDEF = 0,
    REG_A = 'A',
    REG_X = 'X',
    REG_Y = 'Y',
    REG_S = 'S',
};

class RegW65C816 : public RegBase {
public:
    uint8_t regNameLen(RegName regName) const;
    bool compareRegName(const char *line, RegName regName) const;
    RegName parseIndexReg(const char *line) const;
    char *outRegName(char *out, const RegName regName) const;
};

} // namespace w65c816
} // namespace libasm

#endif // __REG_W65C816_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4: