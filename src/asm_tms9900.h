/* -*- mode: c++; -*- */
#ifndef __ASM_TMS9900_H__
#define __ASM_TMS9900_H__

#include "config_tms9900.h"

#include "insn_tms9900.h"
#include "asm_interface.h"

class AsmTms9900 : public Assembler<target::uintptr_t> {
public:
    AsmOperand &getParser() override { return _parser; }
    bool acceptCpu(const char *cpu) override;

private:
    AsmIntelOperand _parser;

    Error checkComma();
    bool isRegisterName(const char *scan) const;
    Error parseRegName(uint8_t &regno);

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