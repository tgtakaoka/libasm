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

#ifndef __LIBASM_REG_I8080_H__
#define __LIBASM_REG_I8080_H__

#include <stdint.h>
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {

struct ValueParser;

namespace i8080 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    // 8-bit registers
    REG_B = 0,
    REG_C = 1,
    REG_D = 2,
    REG_E = 3,
    REG_H = 4,
    REG_L = 5,
    REG_M = 6,
    REG_A = 7,
    // 16-bit registers
    REG_BC = 0 + 8,
    REG_DE = 1 + 8,
    REG_HL = 2 + 8,
    REG_SP = 3 + 8,
    // Other registers
    REG_PSW = 4 + 8,
    REG_AF = 5 + 8,
    REG_IM = 6 + 8,
};

enum CcName : int8_t {
    CC_UNDEF = -1,
    CC_NZ = 0,
    CC_Z = 1,
    CC_NC = 2,
    CC_C = 3,
    CC_PO = 4,
    CC_PE = 5,
    CC_P = 6,
    CC_M = 7,
};

namespace reg {

RegName parseRegName(StrScanner &scan, bool zilog, const ValueParser &parser);
StrBuffer &outRegName(StrBuffer &out, RegName name, bool indirect = false);

bool isPointerReg(RegName name);
uint8_t encodePointerReg(RegName name);
bool isStackReg(RegName name);
uint8_t encodeStackReg(RegName name);
bool isIndexReg(RegName name);
uint8_t encodeIndexReg(RegName name);
bool isDataReg(RegName name);
uint8_t encodeDataReg(RegName name);

RegName decodePointerReg(uint8_t num, bool zilog);
RegName decodeStackReg(uint8_t num, bool zilog);
RegName decodeIndexReg(uint8_t num, bool zilog);
RegName decodeDataReg(uint8_t num);

CcName parseCcName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outCcName(StrBuffer &out, CcName cc);
uint8_t encodeCcName(CcName name);
CcName decodeCcName(uint8_t num);

}  // namespace reg
}  // namespace i8080
}  // namespace libasm

#endif  // __LIBASM_REG_I8080_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
