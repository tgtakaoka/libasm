/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __REG_SCN2650_H__
#define __REG_SCN2650_H__

#include "config_scn2650.h"
#include "reg_base.h"

namespace libasm {
namespace scn2650 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    REG_R0 = 0,
    REG_R1 = 1,
    REG_R2 = 2,
    REG_R3 = 3,
};

enum CcName : int8_t {
    CC_UNDEF = -1,
    CC_EQ = 0,  // Equal, Zero
    CC_GT = 1,  // Greater Than, Positive
    CC_LT = 2,  // Less Than, Negavtive
    CC_UN = 3,  // Unconditional, Always
};

class RegScn2650 : public RegBase {
public:
    static RegName parseRegName(StrScanner &scan);
    static uint8_t encodeRegName(RegName name);
    static RegName decodeRegName(Config::opcode_t opc);
    StrBuffer &outRegName(StrBuffer &out, const RegName name) const;

    static CcName parseCcName(StrScanner &scan);
    static uint8_t encodeCcName(CcName name);
    static CcName decodeCcName(Config::opcode_t opc);
    StrBuffer &outCcName(StrBuffer &out, const CcName name) const;
};

}  // namespace scn2650
}  // namespace libasm

#endif  // __REG_SCN2650_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
