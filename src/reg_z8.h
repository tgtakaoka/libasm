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

#ifndef __REGISTER_Z8_H__
#define __REGISTER_Z8_H__

#include "insn_z8.h"
#include "reg_base.h"

namespace libasm {
namespace z8 {

enum RegName : host::int_t {
    REG_UNDEF = -1,
    REG_R0  = 0,
    REG_R1  = 1,
    REG_R2  = 2,
    REG_R3  = 3,
    REG_R4  = 4,
    REG_R5  = 5,
    REG_R6  = 6,
    REG_R7  = 7,
    REG_R8  = 8,
    REG_R9  = 9,
    REG_R10 = 10,
    REG_R11 = 11,
    REG_R12 = 12,
    REG_R13 = 13,
    REG_R14 = 14,
    REG_R15 = 15,
    // Register pair
    REG_RR0  = 16 + 0,
    REG_RR2  = 16 + 2,
    REG_RR4  = 16 + 4,
    REG_RR6  = 16 + 6,
    REG_RR8  = 16 + 8,
    REG_RR10 = 16 + 10,
    REG_RR12 = 16 + 12,
    REG_RR14 = 16 + 14,
};

enum CcName : host::int_t {
    CC_UNDEF = -1,
    CC_F   = 0,
    CC_LT  = 1,
    CC_LE  = 2,
    CC_ULE = 3,
    CC_OV  = 4,
    CC_MI  = 5,
    CC_Z   = 6,
    CC_C   = 7,
    CC_T   = 8,
    CC_GE  = 9,
    CC_GT  = 10,
    CC_UGT = 11,
    CC_NOV = 12,
    CC_PL  = 13,
    CC_NZ  = 14,
    CC_NC  = 15,
    // Aliases
    CC_EQ  = 16,  // == Z(6)
    CC_ULT = 17,  // == C(7)
    CC_NE  = 18,  // == NZ(14)
    CC_UGE = 19,  // == NC(15)
};

class RegZ8 : public RegBase {
public:
    RegName parseRegName(const char *line) const;
    static host::uint_t regNameLen(RegName regName);
    static uint8_t encodeRegName(RegName regName);
    static RegName decodeRegNum(uint8_t regNum, bool pair = false);
    static bool isRegPair(RegName);
    char *outRegName(char *out, RegName regName) const;

    CcName parseCcName(const char *line) const;
    static host::uint_t ccNameLen(const CcName ccName);
    static host::int_t encodeCcName(CcName ccName);
    static CcName decodeCcNum(uint8_t ccNum);
    char *outCcName(char *out, CcName ccName) const;

private:
    RegName parseRegName(
        const char *line, const RegName *table, const RegName *end) const;
    char regName1stChar(const RegName regName) const;
    char regName2ndChar(const RegName regName) const;
    char regName3rdChar(const RegName regName) const;
    char ccName1stChar(const CcName ccName) const;
    char ccName2ndChar(const CcName ccName) const;
    CcName parseCcName(const char *line, host::int_t max) const;
};

} // namespace z8
} // namespace libasm

#endif // __REGISTER_Z8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
