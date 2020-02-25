/* -*- mode: c++; -*- */
#ifndef __DIS_I8080_H__
#define __DIS_I8080_H__

#include "config_i8080.h"

#include "insn_i8080.h"
#include "reg_i8080.h"
#include "table_i8080.h"
#include "dis_interface.h"

class DisI8080 : public Disassembler<target::uintptr_t> {
public:
    DisOperand &getFormatter() override { return _formatter; }
    bool setCpu(const char *cpu) override { return TableI8080.setCpu(cpu); }
    const char *listCpu() const override { return TableI8080::listCpu(); }

private:
    DisIntelOperand _formatter;
    RegI8080 _regs;

    RegBase &getRegister() override { return _regs; }
    void outRegister(RegName regName);

    Error decodeImmediate8(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeImmediate16(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeDirect(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeIoaddr(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decode(
        DisMemory<target::uintptr_t> &memory, Insn& insn) override;
};

#endif // __DIS_I8080_H__
