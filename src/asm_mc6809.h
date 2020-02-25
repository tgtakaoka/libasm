/* -*- mode: c++; -*- */
#ifndef __ASM_MC6809_H__
#define __ASM_MC6809_H__

#include "config_mc6809.h"

#include "insn_mc6809.h"
#include "reg_mc6809.h"
#include "table_mc6809.h"
#include "asm_interface.h"

class AsmMc6809 : public Assembler<target::uintptr_t> {
public:
    AsmOperand &getParser() override { return _parser; }
    bool setCpu(const char *cpu) override { return TableMc6809.setCpu(cpu); }
    const char *listCpu() const override { return TableMc6809::listCpu(); }

private:
    AsmMotoOperand _parser;
    RegMc6809 _regs;
    uint8_t _direct_page = 0;

    Error determineAddrMode(const char *line, Insn &insn);

    // MC6809
    Error encodeStackOp(Insn &insn);
    Error encodeRegisters(Insn &insn);
    Error encodeRelative(Insn &insn);
    Error encodeImmediate(Insn &insn);
    Error encodeDirect(Insn &insn, bool emitInsn = true);
    Error encodeExtended(Insn &insn, bool emitInsn = true);
    Error encodeIndexed(Insn &insn, bool emitInsn = true);
    // MC6809
    Error encodeBitOperation(Insn &insn);
    Error encodeImmediatePlus(Insn &insn);
    Error encodeTransferMemory(Insn &insn);
    // Pseudo instruction
    Error processPseudo(Insn &insn);
    Error encode(Insn &insn) override;
};

#endif // __ASM_MC6809_H__
