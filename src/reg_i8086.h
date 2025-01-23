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

#ifndef __LIBASM_REG_I8086_H__
#define __LIBASM_REG_I8086_H__

#include <stdint.h>
#include "entry_i8086.h"
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {

struct ValueParser;

namespace i8086 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    // General word registers.
    REG_AX = 0 + 0,
    REG_CX = 1 + 0,
    REG_DX = 2 + 0,
    REG_BX = 3 + 0,
    REG_SP = 4 + 0,
    REG_BP = 5 + 0,
    REG_SI = 6 + 0,
    REG_DI = 7 + 0,
    // General byte registers.
    REG_AL = 0 + 8,
    REG_CL = 1 + 8,
    REG_DL = 2 + 8,
    REG_BL = 3 + 8,
    REG_AH = 4 + 8,
    REG_CH = 5 + 8,
    REG_DH = 6 + 8,
    REG_BH = 7 + 8,
    // Segment registers.
    REG_ES = 0 + 16,
    REG_CS = 1 + 16,
    REG_SS = 2 + 16,
    REG_DS = 3 + 16,
    // Other registers.
    REG_ST = 20,           // ST
    REG_ST0 = 0 + REG_ST,  // ST(0)
    REG_ST1 = 1 + REG_ST,  // ST(1)
    REG_ST2 = 2 + REG_ST,  // ST(2)
    REG_ST3 = 3 + REG_ST,  // ST(3)
    REG_ST4 = 4 + REG_ST,  // ST(4)
    REG_ST5 = 5 + REG_ST,  // ST(5)
    REG_ST6 = 6 + REG_ST,  // ST(6)
    REG_PT7 = 7 + REG_ST,  // ST(7)
    REG_PTR = 28,
    REG_BYTE = Size::SZ_BYTE + REG_PTR,
    REG_WORD = Size::SZ_WORD + REG_PTR,
    REG_DWORD = Size::SZ_QUAD + REG_PTR,
    REG_QWORD = Size::SZ_OCTA + REG_PTR,
    REG_TBYTE = Size::SZ_DATA + REG_PTR,
};

namespace reg {

RegName parseRegName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outRegName(StrBuffer &out, RegName name);

RegName decodeByteReg(uint8_t num);
RegName decodeWordReg(uint8_t num);
RegName decodeSegReg(uint8_t num);
uint8_t encodeRegNum(RegName name);
bool isGeneralReg(RegName name);
bool isSegmentReg(RegName name);
OprSize generalRegSize(RegName name);

}  // namespace reg
}  // namespace i8086
}  // namespace libasm

#endif  // __LIBASM_REG_I8086_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
