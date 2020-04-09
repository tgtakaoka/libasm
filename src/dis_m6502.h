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

#ifndef __DIS_M6502_H__
#define __DIS_M6502_H__

#include "config_m6502.h"

#include "insn_m6502.h"
#include "reg_m6502.h"
#include "table_m6502.h"
#include "dis_interface.h"

namespace libasm {
namespace m6502 {

class DisM6502 : public Disassembler<Config> {
public:
    DisOperand &getFormatter() override { return _formatter; }
    bool setCpu(const char *cpu) override { return TableM6502.setCpu(cpu); }
    const char *listCpu() const override { return TableM6502::listCpu(); }

    void acceptIndirectLong(bool accept) { _acceptIndirectLong = accept; }

private:
    DisMotoOperand _formatter;
    RegM6502 _regs;
    bool _acceptIndirectLong = true;

    RegBase &getRegister() override { return _regs; }

    Error decodeImmediate(DisMemory &memory, InsnM6502 &insn);
    Error decodeAbsolute(DisMemory &memory, InsnM6502 &insn);
    Error decodeZeroPage(DisMemory &memory, InsnM6502 &insn);
    Error decodeRelative(DisMemory &memory, InsnM6502 &insn);
    Error decodeBlockMove(DisMemory &memory, InsnM6502 &insn);
    Error decode(DisMemory &memory, Insn &insn) override;
};

} // namespace m6502
} // namespace libasm

#endif // __DIS_M6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
