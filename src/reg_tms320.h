/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __LIBASM_REG_TMS320_H__
#define __LIBASM_REG_TMS320_H__

#include <stdint.h>

namespace libasm {

struct StrBuffer;
struct StrScanner;
struct ValueParser;

namespace tms320 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    REG_AR0 = 0,    // Auxiliary Register 0
    REG_AR1 = 1,    // Auxiliary Register 1
    REG_AR2 = 2,    // Auxiliary Register 2
    REG_AR3 = 3,    // Auxiliary Register 3
    REG_AR4 = 4,    // Auxiliary Register 4
    REG_AR5 = 5,    // Auxiliary Register 5
    REG_AR6 = 6,    // Auxiliary Register 6
    REG_AR7 = 7,    // Auxiliary Register 7
    REG_PA0 = 8,    // Port address 0
    REG_PA1 = 9,    // Port address 1
    REG_PA2 = 10,   // Port address 2
    REG_PA3 = 11,   // Port address 3
    REG_PA4 = 12,   // Port address 4
    REG_PA5 = 13,   // Port address 5
    REG_PA6 = 14,   // Port address 6
    REG_PA7 = 15,   // Port address 7
    REG_PA8 = 16,   // Port address 8
    REG_PA9 = 17,   // Port address 9
    REG_PA10 = 18,  // Port address 10
    REG_PA11 = 19,  // Port address 11
    REG_PA12 = 20,  // Port address 12
    REG_PA13 = 21,  // Port address 13
    REG_PA14 = 22,  // Port address 14
    REG_PA15 = 23,  // Port address 15
    CTL_BASE = 0x40,
    REG_INTM = CTL_BASE + 0x00,  // ST0: interrupt mode
    REG_OVM = CTL_BASE + 0x02,   // ST0: overflow mode
    REG_CNF = CTL_BASE + 0x04,   // ST1: RAM configuration
    REG_SXM = CTL_BASE + 0x06,   // ST1: sign extension mode
    REG_XF = CTL_BASE + 0x0C,    // ST1: XF pin
    REG_C = CTL_BASE + 0x30,     // ST1: carry
    REG_TC = CTL_BASE + 0x32,    // ST1: test/control flag
    REG_HM = CTL_BASE + 0x38,    // ST1: hold mode
};

enum CcName : int16_t {
    CC_UNDEF = -1,
    CC_UNC = 0,   // UNCOND: B
    CC_EQ = 1,    // Z=1:    BZ
    CC_NEQ = 2,   // Z=0:    BNZ
    CC_LT = 3,    // ACC<0:  BLZ
    CC_LEQ = 4,   // ACC<=0: BLEZ
    CC_GT = 5,    // ACC>0:  BGZ
    CC_GEQ = 6,   // ACC>=0: BGEZ
    CC_NOV = 7,   // V=0:    BNV
    CC_OV = 8,    // V=1:    BV
    CC_NC = 9,    // C=0:    BNC
    CC_C = 10,    // C=1:    BC
    CC_BIO = 11,  // BIO=0:  BIOZ
    CC_NTC = 12,  // TC=0:   BBZ
    CC_TC = 13,   // TC=1:   BBNZ
};

namespace reg {

RegName parseRegName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outRegName(StrBuffer &out, RegName name);
uint_fast8_t encodeRegName(RegName name);
bool isControlName(RegName name);
uint_fast8_t encodeControlName(RegName name);

CcName parseCcName(StrScanner &scan, const ValueParser &parser);
uint16_t encodeCcName(CcName name);

}  // namespace reg
}  // namespace tms320
}  // namespace libasm

#endif  // __LIBASM_REG_TMS320_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
