/*
 * Copyright 2025 Tadashi G. Takaoka
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

#ifndef __LIBASM_REG_Z380_H__
#define __LIBASM_REG_Z380_H__

#include "config_z380.h"
#include "reg_z80.h"

namespace libasm {

struct ValueParser;

namespace z380 {

enum DdName : int8_t {
    DD_UNDEF = -1,
    DD_W = 0,
    DD_IB = 1,
    DD_IW = 2,
    DD_LW = 4,
};

enum CtlName : uint8_t {
    CTL_UNDEF = 0,
    CTL_LW = 1,
    CTL_LCK = 2,
    CTL_XM = 3,
};

struct DisInsn;

namespace reg {

using namespace libasm::z80::common;

RegName parseRegName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outRegName(StrBuffer &out, RegName name);
uint8_t encodeDataReg(RegName name);
RegName decodeDataReg(uint8_t num);
uint8_t encodePointerReg(RegName name);
uint8_t encodePointerRegIx(RegName name, RegName ix);
uint8_t encodeIndirectBase(RegName name);
RegName decodeIndirectBase(uint8_t num);
bool isIndexReg(RegName name);
bool isAlternateReg(RegName name);
RegName alt2BaseReg(RegName alt);

CcName parseCcName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outCcName(StrBuffer &out, CcName cc);
bool isCc4Name(CcName name);
bool isCcAlias(CcName name);
uint8_t encodeCcName(CcName name);
CcName decodeCcName(uint8_t num);

DdName parseDdName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outDdName(StrBuffer &out, DdName dd);
int8_t encodeDdNames(DdName word, DdName imm);
StrBuffer &outDdNames(StrBuffer &out, const DisInsn &insn);

CtlName parseCtlName(StrScanner &scan, const ValueParser &parser);
uint8_t encodeCtlName(CtlName name);
StrBuffer &outCtlName(StrBuffer &out, const DisInsn &insn);

}  // namespace reg
}  // namespace z380
}  // namespace libasm

#endif  // __LIBASM_REG_Z380_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
