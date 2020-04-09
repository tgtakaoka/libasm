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

#ifndef __DIS_MC68000_H__
#define __DIS_MC68000_H__

#include "config_mc68000.h"

#include "insn_mc68000.h"
#include "reg_mc68000.h"
#include "table_mc68000.h"
#include "dis_interface.h"

namespace libasm {
namespace mc68000 {

class DisMc68000 : public Disassembler<Config> {
public:
    DisOperand &getFormatter() override { return _formatter; }
    bool setCpu(const char *cpu) override { return TableMc68000.setCpu(cpu); }
    const char *listCpu() const override { return TableMc68000::listCpu(); }

private:
    DisMotoOperand _formatter;
    RegMc68000 _regs;

    RegBase &getRegister() override { return _regs; }

    void outRegName(RegName regName);
    void outEaSize(EaSize size);

    Error decodeImmediateData(
        DisMemory<Config> &memory, InsnMc68000 &insn, EaSize eaSize);
    Error decodeEffectiveAddr(
        DisMemory<Config> &memory, InsnMc68000 &insn, const EaMc68000 &ea);

    Error decodeImplied(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeDestSiz(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeAddrReg(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeDataReg(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeMoveUsp(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeTrapVec(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeDataDst(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeDestOpr(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeSignExt(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeRelative(DisMemory<Config> &memory, InsnMc68000 &insn);
    void  decodeMoveMltRegList(
        uint16_t list, bool pop,
        void (DisMc68000::*outRegs)(RegName, RegName, char));
    void  outMoveMltRegs(RegName start, RegName last, char suffix);
    Error decodeMoveMlt(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeMoveSr(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeMoveQic(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeMovePer(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeAregSiz(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeDregDst(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeDataQic(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeDmemSiz(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeDregRot(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeDmemOpr(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeRegsExg(DisMemory<Config> &memory, InsnMc68000 &insn);
    Error decodeMoveOpr(DisMemory<Config> &memory, InsnMc68000 &insn);

    Error decode(DisMemory<Config> &memory, Insn &insn) override;
};

} // namespace mc68000
} // namespace libasm

#endif // __DIS_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
