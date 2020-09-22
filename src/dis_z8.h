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

#ifndef __DIS_Z8_H__
#define __DIS_Z8_H__

#include "config_z8.h"
#include "dis_base.h"
#include "insn_z8.h"
#include "reg_z8.h"
#include "table_z8.h"

namespace libasm {
namespace z8 {

class DisZ8
    : public Disassembler,
      public Config {
public:
    DisZ8() : Disassembler(_formatter, _regs, TableZ8) {}

    void reset() override { preferWorkRegister(true); }
    void preferWorkRegister(bool enabled) {
        _preferWorkRegister = enabled;
    }

private:
    IntelValueFormatter _formatter;
    RegZ8 _regs;
    bool _preferWorkRegister = true;

    char *outCcName(char *out, Config::opcode_t opCode);
    char *outIndexed(char *out, uint16_t base, RegName idx, AddrMode mode);
    char *outWorkReg(char *out, uint8_t regNum, bool indir = false);
    char *outPairReg(char *out, uint8_t regNum, bool indir = false);
    char *outRegAddr(char *out, uint8_t regAddr, bool indir = false);
    char *outPairAddr(char *out, uint8_t regAddr, bool indir = false);
    char *outBitPos(char *out, uint8_t bitPos);

    Error decodeOperand(
        DisMemory &memory, InsnZ8 &insn, char *out, AddrMode mode);
    Error decodeAbsolute(
        DisMemory &memory, InsnZ8 &insn, char *out, Endian endian = ENDIAN_BIG);
    Error decodeRelative(DisMemory &memory, InsnZ8 &insn, char *out);
    Error decodeIndexed(
        DisMemory &memory, InsnZ8 &insn, char *out, uint8_t opr1);
    Error decodeIndirectRegPair(DisMemory &memory, InsnZ8 &insn, char *out);
    Error decodeInOpCode(DisMemory &memory, InsnZ8 &insn, char *out);
    Error decodeTwoOperands(DisMemory &memory, InsnZ8 &insn, char *out);
    Error decodePostByte(DisMemory &memory, InsnZ8 &insn, char *out);
    Error decode(DisMemory &memory, Insn &insn, char *out) override;
};

} // namespace z8
} // namespace libasm

#endif // __DIS_Z8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
