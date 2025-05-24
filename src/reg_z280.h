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

#ifndef __LIBASM_REG_Z280_H__
#define __LIBASM_REG_Z280_H__

#include "config_z280.h"
#include "reg_z80.h"

namespace libasm {

struct ValueParser;

namespace z280 {

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

}  // namespace reg
}  // namespace z280
}  // namespace libasm

#endif  // __LIBASM_REG_Z280_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
