/* -*- mode: c++; -*- */
#ifndef __ASM_I8080_H__
#define __ASM_I8080_H__

#include "config_i8080.h"

#include "symbol_table.h"
#include "table_i8080.h"
#include "asm_interface.h"
#include "asm_operand.h"

class AsmI8080 : public AsmCommon<target::uintptr_t> {
public:
    Error encode(
        const char *line,
        Insn &insn,
        target::uintptr_t addr,
        SymbolTable *symtab), override;
    bool isRegister(const char *text) const override {
        return RegI8080::parseRegister(text) != REG_UNDEF;
    }

private:
    AsmIntelOperand _parser;

    Error checkLineEnd();
    Error getOperand16(uint16_t &val16);
    Error getOperand8(uint8_t &val8);
    Error encodePointerReg(Insn &insn);
    Error encodeStackReg(Insn &insn);
    Error encodeIndexReg(Insn &insn);
    Error encodeDataReg(Insn &insn);
    Error encodeDataDataReg(Insn &insn);
    Error encodeVectorNo(Insn &insn);

    Error encodeImmediate(Insn &insn);
    Error encodeDirect(Insn &insn);
    Error encodeIoaddr(Insn &insn);
};

#endif // __ASM_I8080_H__
