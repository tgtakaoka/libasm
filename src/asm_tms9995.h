/* -*- mode: c++; -*- */
#ifndef __ASM_TMS9995_H__
#define __ASM_TMS9995_H__

#include "config_tms9995.h"

#include "symbol_table.h"
#include "table_tms9995.h"
#include "asm_interface.h"

class AsmTms9995 : public AsmCommon<target::uintptr_t> {
public:
    Error encode(
        const char *line,
        Insn &insn,
        target::uintptr_t addr,
        SymbolTable<target::uintptr_t> *symtab) override;
    bool isRegister(const char *text) const override;

private:
    Error checkComma();
    Error getOperand16(uint16_t &val);
    Error parseRegName(uint8_t &regno);

    Error encodeImm(Insn &insn, bool emitInsn);
    Error encodeReg(Insn &insn, bool emitInsn);
    Error encodeCnt(Insn &insn, bool acceptR0, bool accept16);
    Error encodeOpr(Insn &insn, bool emitInsn, bool destinationa = false);
    Error encodeRel(Insn &insn);
    Error encodeCruOff(Insn &insn);
    Error encodeIoaddr(Insn &insn);
};

#endif // __ASM_TMS9995_H__
