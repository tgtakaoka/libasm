/* -*- mode: c++; -*- */
#ifndef __DIS_MC68000_H__
#define __DIS_MC68000_H__

#include "config_mc68000.h"

#include "insn_mc68000.h"
#include "reg_mc68000.h"
#include "dis_interface.h"

class DisMc68000 : public Disassembler<target::uintptr_t> {
public:
    DisOperand &getFormatter() override { return _formatter; }
    bool acceptCpu(const char *cpu) override;

private:
    DisMotoOperand _formatter;
    RegMc68000 _regs;

    RegBase &getRegister() override { return _regs; }

    void outRegName(RegName regName);
    void outEaSize(EaSize size);

    Error decodeImmediateData(
        DisMemory<target::uintptr_t> &memory, Insn &insn, EaSize eaSize);
    Error decodeEffectiveAddr(
        DisMemory<target::uintptr_t> &memory, Insn &insn, const EaMc68000 &ea);

    Error decodeImplied(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeDestSiz(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeAddrReg(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeDataReg(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeMoveUsp(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeTrapVec(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeDataDst(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeDestOpr(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeSignExt(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeRelative(DisMemory<target::uintptr_t> &memory, Insn &insn);
    void  decodeMoveMltRegList(
        uint16_t list, bool pop,
        void (DisMc68000::*outRegs)(RegName, RegName, char));
    void  outMoveMltRegs(RegName start, RegName last, char suffix);
    Error decodeMoveMlt(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeMoveSr(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeMoveQic(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeMovePer(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeAregSiz(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeDregDst(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeDataQic(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeDmemSiz(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeDregRot(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeDmemOpr(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeRegsExg(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeMoveOpr(DisMemory<target::uintptr_t> &memory, Insn &insn);

    Error decode(
        DisMemory<target::uintptr_t> &memory, Insn& insn) override;
};

#endif // __DIS_MC68000_H__
