/* -*- mode: c++; -*- */
#ifndef __ASM_M6502_H__
#define __ASM_M6502_H__

#include "config_m6502.h"

#include "symbol_table.h"
#include "table_m6502.h"
#include "asm_interface.h"

class AsmM6502 : public Assembler<target::uintptr_t> {
public:
    Error encode(
        const char *line,
        Insn &insn,
        target::uintptr_t addr,
        SymbolTable *symtab) override;
    AsmOperand &getParser() override { return _parser; }
    bool acceptCpu(const char *cpu) override;

private:
    AsmMotoOperand _parser;
    McuType _mcuType = M6502;

    void emitInsnCode(Insn &insn) const { insn.emitByte(insn.insnCode()); }

    Error parseOperand(Insn &insn, uint16_t &val);

    Error encodeRelative(Insn &insn, bool emitInsn);
    Error encodeZeroPageRelative(Insn &insn);
};

#endif // __ASM_M6502_H__
