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

#ifndef __REG_I8080_H__
#define __REG_I8080_H__

#include "reg_base.h"

namespace libasm {
namespace i8080 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    // Data registers.
    REG_B = 0,
    REG_C = 1,
    REG_D = 2,
    REG_E = 3,
    REG_H = 4,
    REG_L = 5,
    REG_M = 6,
    REG_A = 7,
    // Other registers
    REG_SP = 0 + 8,
    REG_PSW = 1 + 8,
};

class RegI8080 : public RegBase {
public:
    static RegName parseRegName(StrScanner &scan);
    StrBuffer &outRegName(StrBuffer &out, const RegName name) const;

    static bool isPointerReg(RegName name);
    static uint8_t encodePointerReg(RegName name);
    static bool isStackReg(RegName name);
    static uint8_t encodeStackReg(RegName name);
    static bool isIndexReg(RegName name);
    static uint8_t encodeIndexReg(RegName name);
    static bool isDataReg(RegName name);
    static uint8_t encodeDataReg(RegName name);

    static RegName decodePointerReg(uint8_t num);
    static RegName decodeStackReg(uint8_t num);
    static RegName decodeIndexReg(uint8_t num);
    static RegName decodeDataReg(uint8_t num);
};

}  // namespace i8080
}  // namespace libasm

#endif  // __REG_I8080_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
