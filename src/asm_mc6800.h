/* -*- mode: c++; -*- */
#ifndef __ASM_MC6800_H__
#define __ASM_MC6800_H__

#include "config_mc6800.h"

#include "insn_mc6800.h"
#include "reg_mc6800.h"
#include "table_mc6800.h"
#include "asm_interface.h"

class AsmMc6800 : public Assembler<target::uintptr_t> {
public:
    AsmOperand &getParser() override { return _parser; }
    bool setCpu(const char *cpu) override { return TableMc6800.setCpu(cpu); }
    const char *listCpu() const override { return TableMc6800::listCpu(); }

private:
    AsmMotoOperand _parser;
    RegMc6800 _regs;

    bool adjustAccumulator(Insn &insn);
    Error determineAddrMode(const char *line, Insn &insn);

    Error encodeInherent(Insn &insn);
    Error encodeDirect(Insn &insn);
    Error encodeExtended(Insn &insn);
    Error encodeIndexed(Insn &insn);
    Error encodeRelative(Insn &insn);
    Error encodeImmediate(Insn &insn);

    Error encode(Insn &insn) override;
};

#endif // __ASM_MC6800_H__
