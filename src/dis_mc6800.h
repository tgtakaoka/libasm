/* -*- mode: c++; -*- */
#ifndef __DIS_MC6800_H__
#define __DIS_MC6800_H__

#include "config_mc6800.h"

#include "insn_mc6800.h"
#include "reg_mc6800.h"
#include "table_mc6800.h"
#include "dis_interface.h"

class DisMc6800 : public Disassembler<target::uintptr_t> {
public:
    DisOperand &getFormatter() override { return _formatter; }
    bool setCpu(const char *cpu) override { return TableMc6800.setCpu(cpu); }
    const char *listCpu() const override { return TableMc6800::listCpu(); }

private:
    DisMotoOperand _formatter;
    RegMc6800 _regs;

    RegBase &getRegister() override { return _regs; }
    void outRegister(RegName regName);
    bool outAccumulator(const Insn &insn);

    // MC6800
    Error decodeInherent(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeDirectPage(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeExtended(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeIndexed(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeRelative(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeImmediate(DisMemory<target::uintptr_t> &memory, Insn &insn);

    Error decode(
        DisMemory<target::uintptr_t> &memory, Insn& insn) override;
};

#endif // __DIS_MC6800_H__
