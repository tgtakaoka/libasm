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

#ifndef __REG_Z80_H__
#define __REG_Z80_H__

#include "insn_z80.h"
#include "reg_base.h"

namespace libasm {
namespace z80 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    // 16-bit registers
    // Pointer registers
    REG_BC  = 0 + 0,
    REG_DE  = 1 + 0,
    REG_HL  = 2 + 0,
    REG_SP  = 3 + 0,
    REG_IX  = 4 + 0,
    REG_IY  = 5 + 0,
    REG_AF  = 6 + 0,
    REG_AFP = 7 + 0,
    // 8-bit registers
    // Data registers
    REG_B = 0 + 8,
    REG_C = 1 + 8,
    REG_D = 2 + 8,
    REG_E = 3 + 8,
    REG_H = 4 + 8,
    REG_L = 5 + 8,
    REG_A = 7 + 8,
    // Other registers
    REG_I  = 0 + 16,
    REG_R  = 1 + 16,
    // 8085
    REG_IM = 0 + 18,
};

enum CcName : int8_t {
    CC_UNDEF = -1,
    CC_NZ = 0,
    CC_Z  = 1,
    CC_NC = 2,
    CC_C  = 3,
    CC_PO = 4,
    CC_PE = 5,
    CC_P  = 6,
    CC_M  = 7,
};

class RegZ80 : public RegBase {
public:
    static RegName parseRegName(const char *line);
    static uint8_t regNameLen(const RegName name);
    char *outRegName(char *out, const RegName name) const;
    static OprSize regSize(const RegName name);

    static uint8_t encodeDataReg(RegName name);
    static RegName decodeDataReg(uint8_t num);

    static uint8_t encodePointerReg(RegName name);
    static uint8_t encodePointerRegIx(RegName name, RegName ix);
    static RegName decodePointerReg(
        uint8_t num, const InsnZ80 *insn = nullptr);

    static uint8_t encodeStackReg(RegName name);
    static RegName decodeStackReg(uint8_t num);

    static uint8_t encodeIndirectBase(RegName name);
    static RegName decodeIndirectBase(uint8_t num);

    static void encodeIndexReg(InsnZ80 &insn, RegName ixReg);
    static RegName decodeIndexReg(const InsnZ80 &insn);

    static uint8_t encodeIrReg(RegName name);
    static RegName decodeIrReg(uint8_t num);

    static CcName parseCcName(const char *line);
    static uint8_t ccNameLen(const CcName name);
    char *outCcName(char *out, CcName cc) const;
    static bool isCc4Name(CcName name);
    static uint8_t encodeCcName(CcName name);
    static CcName decodeCcName(uint8_t num);
};

} // namespace z80
} // namespace libasm

#endif // __REG_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
