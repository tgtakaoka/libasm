/* -*- mode: c++; -*- */
#ifndef __ASM_M6502_H__
#define __ASM_M6502_H__

#include "config_m6502.h"

#include "insn_m6502.h"
#include "reg_m6502.h"
#include "table_m6502.h"
#include "asm_interface.h"

class AsmM6502 : public Assembler<target::uintptr_t> {
public:
    AsmOperand &getParser() override { return _parser; }
    bool setCpu(const char *cpu) override { return TableM6502.setCpu(cpu); }
    const char *listCpu() const override { return TableM6502::listCpu(); }

private:
    AsmMotoOperand _parser;
    RegM6502 _regs;

    struct Operand : public ErrorReporter {
        AddrMode mode;
        uint16_t val16;
    };

    Error selectMode(char modifier, Operand &op, AddrMode abs, AddrMode zp);
    Error parseOperand(Operand &op);

    Error encodeRelative(Insn &insn, bool emitInsn);
    Error encodeZeroPageRelative(Insn &insn);

    Error encode(Insn &insn) override;
};

#endif // __ASM_M6502_H__
