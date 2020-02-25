/* -*- mode: c++; -*- */
#ifndef __ASM_I8080_H__
#define __ASM_I8080_H__

#include "config_i8080.h"

#include "insn_i8080.h"
#include "reg_i8080.h"
#include "table_i8080.h"
#include "asm_interface.h"

class AsmI8080 : public Assembler<target::uintptr_t> {
public:
    AsmOperand &getParser() override { return _parser; }
    bool setCpu(const char *cpu) override { return TableI8080.setCpu(cpu); }
    const char *listCpu() const override { return TableI8080::listCpu(); }

private:
    AsmIntelOperand _parser;
    RegI8080 _regs;

    Error encodePointerReg(Insn &insn);
    Error encodeStackReg(Insn &insn);
    Error encodeIndexReg(Insn &insn);
    Error encodeDataReg(Insn &insn);
    Error encodeDataDataReg(Insn &insn);
    Error encodeVectorNo(Insn &insn);

    Error encodeImmediate(Insn &insn);
    Error encodeDirect(Insn &insn);
    Error encodeIoaddr(Insn &insn);

    Error encode(Insn &insn) override;
};

#endif // __ASM_I8080_H__
