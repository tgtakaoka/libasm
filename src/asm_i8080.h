/* -*- mode: c++; -*- */
#ifndef __ASM_I8080_H__
#define __ASM_I8080_H__

#include "config_i8080.h"

#include "symbol_table.h"
#include "table_i8080.h"
#include "asm_interface.h"

class AsmI8080 : public AsmCommon<target::uintptr_t> {
public:
    Error encode(
        const char *line,
        Insn &insn,
        target::uintptr_t addr,
        SymbolTable<target::uintptr_t> *symtab), override;

protected:
    Error checkLineEnd();
    Error getInt16(uint16_t &val);
    Error getOperand16(uint16_t &val);
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
