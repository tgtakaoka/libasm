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

#include "insn_z80.h"
#include "reg_z80.h"
#include "table_z80.h"
#include "dis_interface.h"

namespace libasm {
namespace z80 {

class DisZ80 : public Disassembler<Config::uintptr_t> {
public:
    DisOperand &getFormatter() override { return _formatter; }
    bool setCpu(const char *cpu) override { return TableZ80.setCpu(cpu); }
    const char *listCpu() const override { return TableZ80::listCpu(); }
    Endian endian() const override { return ENDIAN_LITTLE; }
    host::uint_t maxBytes() const override { return Config::code_max; }
    host::uint_t maxName() const override { return Config::name_max; }

private:
    DisIntelOperand _formatter;
    RegZ80 _regs;

    RegBase &getRegister() override { return _regs; }
    template<typename U>
    void outAddress(U addr, bool indir = true);
    void outIndexOffset(const InsnZ80 &insn, int8_t offset);
    void outRegister(RegName regName);
    void outPointer(RegName regName);
    void outDataRegister(RegName regName);
    void outConditionName(Config::opcode_t cc, bool cc8 = true);

    Error decodeOperand(
        DisMemory<Config::uintptr_t> &memory, Insn<Config::uintptr_t>& insn);

    Error decodeInherent(InsnZ80 &insn);
    Error decodeImmediate8(InsnZ80 &insn, uint8_t val);
    Error decodeImmediate16(InsnZ80 &insn, uint16_t val);
    Error decodeDirect(InsnZ80 &insn, Config::uintptr_t addr);
    Error decodeIoaddr(InsnZ80 &insn, uint8_t ioaddr);
    Error decodeRelative(InsnZ80 &insn, int8_t delta);
    Error decodeIndexed(InsnZ80 &insn, int8_t offset);
    Error decodeIndexedImmediate8(
        InsnZ80 &insn, int8_t offset, uint8_t val);
    Error decodeIndexedBitOp(
        InsnZ80 &insn, int8_t offset, Config::opcode_t opCode);

    Error decode(
        DisMemory<Config::uintptr_t> &memory,
        Insn<Config::uintptr_t> &insn) override;
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
