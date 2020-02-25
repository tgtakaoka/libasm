/* -*- mode: c++; -*- */
#ifndef __ASM_TMS9900_H__
#define __ASM_TMS9900_H__

#include "config_tms9900.h"

#include "insn_tms9900.h"
#include "reg_tms9900.h"
#include "table_tms9900.h"
#include "asm_interface.h"

class AsmTms9900 : public Assembler<target::uintptr_t> {
public:
    AsmOperand &getParser() override { return _parser; }
    bool setCpu(const char *cpu) override { return TableTms9900.setCpu(cpu); }
    const char *listCpu() const override { return TableTms9900::listCpu(); }

private:
    AsmIntelOperand _parser;
    RegTms9900 _regs;

    Error checkComma();

    Error encodeImm(Insn &insn, bool emitInsn);
    Error encodeReg(Insn &insn, bool emitInsn);
    Error encodeCnt(Insn &insn, bool acceptR0, bool accept16);
    Error encodeOpr(Insn &insn, bool emitInsn, bool destinationa = false);
    Error encodeRel(Insn &insn);
    Error encodeCruOff(Insn &insn);
    Error encodeIoaddr(Insn &insn);

    Error encode(Insn &insn) override;
};

#endif // __ASM_TMS9900_H__
