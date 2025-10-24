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
    // General dword registers.
    REG_EAX = 0 + 16,
    REG_ECX = 1 + 16,
    REG_EDX = 2 + 16,
    REG_EBX = 3 + 16,
    REG_ESP = 4 + 16,
    REG_EBP = 5 + 16,
    REG_ESI = 6 + 16,
    REG_EDI = 7 + 16,
    // Segment registers.
    REG_ES = 0 + 24,
    REG_CS = 1 + 24,
    REG_SS = 2 + 24,
    REG_DS = 3 + 24,
    REG_FS = 4 + 24,
    REG_GS = 5 + 24,
    // Other registers.
    REG_ST = 30,  // ST
};

enum PrefixName : uint8_t {
    PRE_UNDEF = 0,
    PRE_ADDR16 = 1,
    PRE_ADDR32 = 2,
    PRE_PTR = 3,
    PRE_BYTE = Size::SZ_BYTE + PRE_PTR,
    PRE_WORD = Size::SZ_WORD + PRE_PTR,
    PRE_DWORD = Size::SZ_QUAD + PRE_PTR,
    PRE_QWORD = Size::SZ_OCTA + PRE_PTR,
    PRE_TBYTE = Size::SZ_DATA + PRE_PTR,
};

namespace reg {

RegName parseRegName(StrScanner &scan, const CpuSpec &cpuSpec, const ValueParser &parser);
StrBuffer &outRegName(StrBuffer &out, RegName name);

RegName decodeByteReg(uint8_t num);
RegName decodeWordReg(uint8_t num);
RegName decodeSegReg(uint8_t num);
uint_fast8_t encodeRegNum(RegName name);
bool isGeneralReg(RegName name, const CpuSpec &cpuSpec);
bool isSegmentReg(RegName name, const CpuSpec &cpuSpec);
OprSize generalRegSize(RegName name);

PrefixName parsePrefixName(StrScanner &scan, const ValueParser &parser);
StrBuffer &outPrefixName(StrBuffer &out, PrefixName name);

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
