/* -*- mode: c++; -*- */
#ifndef __ASM_I8080_H__
#define __ASM_I8080_H__

#include "config_i8080.h"

#include "error_reporter.h"
#include "symbol_table.h"
#include "table_i8080.h"

class Assembler : public ErrorReporter {
public:
    Error encode(const char *line, Insn &insn,
                 target::uintptr_t addr, SymbolTable *symtab);

protected:
    SymbolTable  *_symtab;

    void reset(SymbolTable *symtab) {
        _symtab = symtab;
        resetError();
    }
    bool hasSymbol(const char *symbol) const {
        return _symtab && _symtab->hasSymbol(symbol);
    }
    target::uintptr_t lookup(const char *symbol) const {
        return _symtab ? _symtab->lookup(symbol) : 0;
    }

    const char *encodePointerReg(const char *line, Insn &insn);
    const char *encodeStackReg(const char *line, Insn &insn);
    const char *encodeIndexReg(const char *line, Insn &insn);
    const char *encodeDataReg(const char *line, Insn &insn);
    const char *encodeDataDataReg(const char *line, Insn &insn);
    const char *encodeVectorNo(const char *line, Insn &insn);

    Error getOperand16(const char *&line, target::uint16_t &val);
    Error encodeImmediate(const char *line, Insn &insn);
    Error encodeDirect(const char *line, Insn &insn);
    Error encodeIoaddr(const char *line, Insn &insn);
};

#endif // __ASM_I8080_H__
