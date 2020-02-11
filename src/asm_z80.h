/* -*- mode: c++; -*- */
#ifndef __ASM_Z80_H__
#define __ASM_Z80_H__

#include "config_z80.h"

#include "insn_z80.h"
#include "reg_z80.h"
#include "table_z80.h"
#include "asm_interface.h"

class AsmZ80 : public Assembler<target::uintptr_t> {
public:
    AsmOperand &getParser() override { return _parser; }
    bool acceptCpu(const char *cpu) override;

private:
    AsmIntelOperand _parser;
    RegZ80 _regs;

    void emitInsnCode(Insn &insn) const {
        const target::opcode_t prefix = TableZ80::prefixCode(insn.insnCode());
        if (TableZ80::isPrefixCode(prefix))
            insn.emitByte(prefix);
        insn.emitByte(TableZ80::opCode(insn.insnCode()));
    }

    struct Operand : public ErrorReporter {
        OprFormat format;
        RegName reg;
        uint16_t val;
        OprSize size;
    };

    Error parseOperand(const Insn &insn, Operand &opr);

    Error encodeImmediate(
        Insn &insn, const Operand &left, const Operand &right);
    Error encodeInherent(
        Insn &insn, const Operand &left, const Operand &right);
    Error encodeDirect(
        Insn &insn, const Operand &left, const Operand &right);
    Error encodeIoaddr(
        Insn &insn, const Operand &left, const Operand &right);
    Error encodeRelative(
        Insn &insn, const Operand &left, const Operand &right);
    Error encodeIndexed(
        Insn &insn, const Operand &left, const Operand &right);
    Error encodeIndexedImmediate8(
        Insn &insn, const Operand &left, const Operand &right);

    Error encode(Insn &insn) override;
};

#endif // __ASM_Z80_H__
