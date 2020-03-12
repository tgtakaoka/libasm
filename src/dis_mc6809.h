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

#ifndef __DIS_MC6809_H__
#define __DIS_MC6809_H__

#include "config_mc6809.h"

#include "insn_mc6809.h"
#include "reg_mc6809.h"
#include "table_mc6809.h"
#include "dis_interface.h"

class DisMc6809 : public Disassembler<target::uintptr_t> {
public:
    DisOperand &getFormatter() override { return _formatter; }
    bool setCpu(const char *cpu) override { return TableMc6809.setCpu(cpu); }
    const char *listCpu() const override { return TableMc6809::listCpu(); }

private:
    DisMotoOperand _formatter;
    RegMc6809 _regs;

    RegBase &getRegister() override { return _regs; }
    void outRegister(RegName regName);

    // MC6809
    Error decodeDirectPage(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeIndexed(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeExtended(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeRelative(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeImmediate(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeStackOp(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeRegisters(DisMemory<target::uintptr_t> &memory, Insn &insn);
    // HD6309
    Error decodeImmediatePlus(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeBitOperation(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeTransferMemory(DisMemory<target::uintptr_t> &memory, Insn &insn);

    Error decode(
        DisMemory<target::uintptr_t> &memory, Insn& insn) override;
};

#endif // __DIS_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
