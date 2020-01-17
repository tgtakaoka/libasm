/* -*- mode: c++; -*- */
#ifndef __DIS_M6502_H__
#define __DIS_M6502_H__

#include "config_m6502.h"

#include "insn_m6502.h"
#include "dis_memory.h"
#include "symbol_table.h"
#include "dis_interface.h"

class DisM6502 : public Disassembler<target::uintptr_t> {
public:
    DisOperand &getFormatter() override { return _formatter; }
    bool acceptCpu(const char *cpu) override;

private:
    DisMotoOperand _formatter;
    McuType _mcuType = M6502;

    Error decodeImmediate(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeAbsolute(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeZeroPage(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeRelative(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decode(
        DisMemory<target::uintptr_t> &memory, Insn& insn) override;
};

#endif // __DIS_M6502_H__
