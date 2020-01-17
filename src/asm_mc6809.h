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
    Error encode(
        const char *line,
        Insn &insn,
        target::uintptr_t addr,
        SymbolTable *symtab) override;
    AsmOperand &getParser() override { return _parser; }
    bool acceptCpu(const char *cpu) override;

private:
    AsmMotoOperand _parser;
    RegMc6809 _regs;
    uint8_t _direct_page = 0;

    void emitInsnCode(Insn &insn) const {
        const target::opcode_t prefix = TableMc6809::prefixCode(insn.insnCode());
        if (TableMc6809::isPrefixCode(prefix))
            insn.emitByte(prefix);
        insn.emitByte(TableMc6809::opCode(insn.insnCode()));
    }

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
};

#endif // __ASM_MC6809_H__
