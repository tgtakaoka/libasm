/* -*- mode: c++; -*- */
#ifndef __DIS_M6502_H__
#define __DIS_M6502_H__

#include "config_m6502.h"

#include "insn_m6502.h"
#include "dis_memory.h"
#include "symbol_table.h"
#include "dis_interface.h"

class DisM6502 : public DisCommon<target::uintptr_t> {
public:
    Error decode(
        DisMemory<target::uintptr_t> &memory,
        Insn& insn,
        char *operands,
        SymbolTable *symtab) override;
    DisOperand &getFormatter() override { return _formatter; }

    void acceptCpu(McuType mcuType) { _mcuType = mcuType; }

private:
    DisMotoOperand _formatter;
    McuType _mcuType = M6502;

    Error decodeImmediate(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeAbsolute(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeZeroPage(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeRelative(DisMemory<target::uintptr_t> &memory, Insn &insn);
};

#endif // __DIS_M6502_H__
