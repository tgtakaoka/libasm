/* -*- mode: c++; -*- */
#ifndef __ASM_M6502_H__
#define __ASM_M6502_H__

#include "config_m6502.h"

#include "insn_m6502.h"
#include "reg_m6502.h"
#include "asm_interface.h"

class AsmM6502 : public Assembler<target::uintptr_t> {
public:
    AsmOperand &getParser() override { return _parser; }
    bool acceptCpu(const char *cpu) override;

private:
    AsmMotoOperand _parser;
    RegM6502 _regs;

    Error parseOperand(Insn &insn, uint16_t &val);

    Error encodeRelative(Insn &insn, bool emitInsn);
    Error encodeZeroPageRelative(Insn &insn);

    Error encode(Insn &insn) override;
};

#endif // __ASM_M6502_H__
