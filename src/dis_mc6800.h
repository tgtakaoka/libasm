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

#ifndef __DIS_MC6800_H__
#define __DIS_MC6800_H__

#include "config_mc6800.h"

#include "insn_mc6800.h"
#include "reg_mc6800.h"
#include "table_mc6800.h"
#include "dis_interface.h"

namespace libasm {
namespace mc6800 {

class DisMc6800 : public Disassembler<target::uintptr_t> {
public:
    DisOperand &getFormatter() override { return _formatter; }
    bool setCpu(const char *cpu) override { return TableMc6800.setCpu(cpu); }
    const char *listCpu() const override { return TableMc6800::listCpu(); }
    Endian endian() const override { return ENDIAN_BIG; }
    host::uint_t maxBytes() const override { return Entry::code_max; }
    host::uint_t maxName() const override { return Entry::name_max; }

private:
    DisMotoOperand _formatter;
    RegMc6800 _regs;

    RegBase &getRegister() override { return _regs; }
    void outRegister(RegName regName);
    bool outAccumulator(const InsnMc6800 &insn);

    // MC6800
    Error decodeInherent(DisMemory<target::uintptr_t> &memory, InsnMc6800 &insn);
    Error decodeDirectPage(DisMemory<target::uintptr_t> &memory, InsnMc6800 &insn);
    Error decodeExtended(DisMemory<target::uintptr_t> &memory, InsnMc6800 &insn);
    Error decodeIndexed(DisMemory<target::uintptr_t> &memory, InsnMc6800 &insn);
    Error decodeRelative(DisMemory<target::uintptr_t> &memory, InsnMc6800 &insn);
    Error decodeImmediate(DisMemory<target::uintptr_t> &memory, InsnMc6800 &insn);

    Error decode(
        DisMemory<target::uintptr_t> &memory, Insn& insn) override;
};

} // namespace m6502
} // namespace libasm

#endif // __DIS_MC6800_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
