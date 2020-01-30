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
        AddrMode mode;
    };

    Error parseOperand(Operand &opr);

    Error encodeImmediate(Insn &insn, RegName leftReg, uint16_t rightOpr);
    Error encodeInherent(
        Insn &insn, RegName leftReg, RegName rightReg, uint16_t leftOpr);
    Error encodeDirect(
        Insn &insn, RegName leftReg, RegName rightReg,
        target::uintptr_t leftOpr, target::uintptr_t rightOpr);
    Error encodeIoaddr(
        Insn &insn, uint16_t leftOpr, uint16_t rightOpr);
    Error encodeRelative(
        Insn &insn, target::uintptr_t leftOpr, target::uintptr_t rightOpr);
    Error encodeIndexed(
        Insn &insn, RegName leftReg, RegName rightReg,
        uint16_t leftOpr, uint16_t rightOpr);
    Error encodeIndexedImmediate8(
        Insn &insn, RegName leftReg, RegName rightReg,
        uint16_t leftOpr, uint16_t rightOpr);

    Error encode(Insn &insn) override;
};

#endif // __ASM_Z80_H__
