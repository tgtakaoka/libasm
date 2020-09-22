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

#ifndef __DIS_Z80_H__
#define __DIS_Z80_H__

#include "config_z80.h"
#include "dis_base.h"
#include "insn_z80.h"
#include "reg_z80.h"
#include "table_z80.h"

namespace libasm {
namespace z80 {

class DisZ80
    : public Disassembler,
      public Config {
public:
    DisZ80() : Disassembler(_formatter, _regs, TableZ80) {}

private:
    IntelValueFormatter _formatter;
    RegZ80 _regs;

    char *outRegister(char *out, RegName regName);
    char *outPointer(char *out, RegName regName);
    char *outDataRegister(char *out, RegName regName);
    template<typename U>
    char *outAbsolute(char *out, U addr);
    char *outConditionName(char *out, Config::opcode_t cc, bool cc8 = true);
    char *outIndexOffset(const InsnZ80 &insn, char *out, int8_t offset);

    Error decodeOperand(DisMemory &memory, Insn& insn);

    Error decodeInherent(InsnZ80 &insn, char *out);
    Error decodeImmediate8(InsnZ80 &insn, char *out, uint8_t val);
    Error decodeImmediate16(InsnZ80 &insn, char *out, uint16_t val);
    Error decodeDirect(InsnZ80 &insn, char *out, Config::uintptr_t addr);
    Error decodeIoaddr(InsnZ80 &insn, char *out, uint8_t ioaddr);
    Error decodeRelative(InsnZ80 &insn, char *out, int8_t delta);
    Error decodeIndexed(InsnZ80 &insn, char *out, int8_t offset);
    Error decodeIndexedImmediate8(
        InsnZ80 &insn, char *out, int8_t offset, uint8_t val);
    Error decodeIndexedBitOp(
        InsnZ80 &insn, char *out, int8_t offset, Config::opcode_t opCode);
    Error decode(DisMemory &memory, Insn &insn, char *out) override;
};

} // namespace z80
} // namespace libasm

#endif // __DIS_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
