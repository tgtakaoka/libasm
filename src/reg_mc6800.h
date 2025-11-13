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

#ifndef __LIBASM_REG_MC6800_H__
#define __LIBASM_REG_MC6800_H__

#include <stdint.h>

#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {
namespace mc6800 {

enum RegName : int8_t {
    REG_UNDEF = 0,
    REG_X = 'X',
    // MC68HC11
    REG_Y = 'Y',
};

enum CcName : int8_t {
    CC_UNDEF = -1,
    CC_RA = 0x00,  // Always
    CC_HI = 0x02,  // Higher
    CC_LS = 0x03,  // Lower or same
    CC_HS = 0x04,  // Higher or same
    CC_LO = 0x05,  // Lower
    CC_NE = 0x06,  // Not equal
    CC_EQ = 0x07,  // Equal
    CC_VC = 0x08,  // Overflow clear
    CC_VS = 0x09,  // Overflow set
    CC_PL = 0x0A,  // Plus
    CC_MI = 0x0B,  // Minus
    CC_GE = 0x0C,  // Greater or equal
    CC_LT = 0x0D,  // Less than
    CC_GT = 0x0E,  // Greater than
    CC_LE = 0x0F,  // Less or equal
    // Aliases
    CC_CC = 0x14,  // Carry clear
    CC_CS = 0x15,  // Carry set
};

namespace reg {

RegName parseRegName(StrScanner &scan);
StrBuffer &outRegName(StrBuffer &out, RegName name);

CcName parseCcName(StrScanner &scan);
StrBuffer &outCcName(StrBuffer &out, CcName cc);
uint_fast8_t encodeCcName(CcName name);
CcName decodeCcName(uint_fast8_t num);

}  // namespace reg
}  // namespace mc6800
}  // namespace libasm

#endif  // __LIBASM_REG_MC6800_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
