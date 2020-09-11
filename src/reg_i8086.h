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

#ifndef __REG_I8086_H__
#define __REG_I8086_H__

#include "entry_i8086.h"
#include "reg_base.h"

namespace libasm {
namespace i8086 {

enum RegName : uint8_t {
    REG_UNDEF = 0,
    REG_BYTE  = 1,
    REG_WORD  = 2,
    REG_PTR   = 3,
    REG_AL = 0 + 8,
    REG_CL = 1 + 8,
    REG_DL = 2 + 8,
    REG_BL = 3 + 8,
    REG_AH = 4 + 8,
    REG_CH = 5 + 8,
    REG_DH = 6 + 8,
    REG_BH = 7 + 8,
    REG_AX = 0 + 16,
    REG_CX = 1 + 16,
    REG_DX = 2 + 16,
    REG_BX = 3 + 16,
    REG_SP = 4 + 16,
    REG_BP = 5 + 16,
    REG_SI = 6 + 16,
    REG_DI = 7 + 16,
    REG_ES = 0 + 24,
    REG_CS = 1 + 24,
    REG_SS = 2 + 24,
    REG_DS = 3 + 24,
};

class RegI8086 : public RegBase {
public:
    RegName parseRegName(const char *line) const;
    uint8_t regNameLen(RegName name) const;
    RegName decodeByteReg(uint8_t num) const;
    RegName decodeWordReg(uint8_t num) const;
    RegName decodeSegReg(uint8_t num) const;
    char *outRegName(char *out, const RegName name) const;
    uint8_t encodeRegNum(RegName name) const;
    bool isGeneralReg(RegName name) const;
    bool isSegmentReg(RegName name) const;
    OprSize regSize(RegName name) const;
};

} // namespace i8086
} // namespace libasm

#endif // __REG_I8086_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
