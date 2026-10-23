/*
 * Copyright 2026 Tadashi G. Takaoka
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

#ifndef __LIBASM_REG_C166_H__
#define __LIBASM_REG_C166_H__

#include <stdint.h>
#include "config_c166.h"
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {

struct ValueParser;

namespace c166 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    // Word GPRs.
    REG_R0 = 0,
    REG_R15 = 15,
    // Byte GPRs; RLn and RHn are the low and high bytes of Rn.
    REG_RL0 = 16 + 0,
    REG_RH7 = 16 + 15,
};

enum CcName : int8_t {
    CC_UNDEF = -1,
    CC_UC = 0,
    CC_NET = 1,
    CC_Z = 2,
    CC_NZ = 3,
    CC_V = 4,
    CC_NV = 5,
    CC_N = 6,
    CC_NN = 7,
    CC_C = 8,
    CC_NC = 9,
    CC_SGT = 10,
    CC_SLE = 11,
    CC_SLT = 12,
    CC_SGE = 13,
    CC_UGT = 14,
    CC_ULE = 15,
    // Aliases, encoded as the condition code they share.
    CC_EQ = 16 + CC_Z,
    CC_NE = 16 + CC_NZ,
    CC_ULT = 16 + CC_C,
    CC_UGE = 16 + CC_NC,
};

namespace reg {

RegName parseRegName(StrScanner &scan, const ValueParser &parser);
CcName parseCcName(StrScanner &scan, const ValueParser &parser);
bool isWordReg(RegName name);
bool isByteReg(RegName name);
uint8_t encodeRegNum(RegName name);
// The address of the core SFR |scan| names, or 0.
uint16_t parseSfrName(StrScanner &scan, const ValueParser &parser, CpuType cpuType);

StrBuffer &outRegName(StrBuffer &out, RegName name);
StrBuffer &outCcName(StrBuffer &out, CcName name, bool prefix);

RegName decodeWordReg(uint8_t num);
RegName decodeByteReg(uint8_t num);
// The name of the core SFR at |addr|, or nullptr.
const /*PROGMEM*/ char *sfrName_P(uint16_t addr, CpuType cpuType);
uint8_t encodeCcNum(CcName name);
CcName decodeCcNum(uint8_t num);

}  // namespace reg
}  // namespace c166
}  // namespace libasm

#endif  // __LIBASM_REG_C166_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
