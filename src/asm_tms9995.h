/* -*- mode: c++; -*- */
#ifndef __ASM_TMS9995_H__
#define __ASM_TMS9995_H__

#include "config_tms9995.h"

#include "error_reporter.h"
#include "symbol_table.h"
#include "table_tms9995.h"

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
    const char *lookup(target::uintptr_t addr) const {
        return _symtab ? _symtab->lookup(addr) : nullptr;
    }
    bool hasSymbol(const char *symbol) const {
        return _symtab && _symtab->hasSymbol(symbol);
    }
    target::uintptr_t lookup(const char *symbol) const {
        return _symtab ? _symtab->lookup(symbol) : 0;
    }

    Error checkComma(const char *&line);
    Error getOperand16(const char *&line, target::uint16_t &val);
    Error encodeImm(const char *&line, Insn &insn, bool emitInsn);
    Error encodeReg(const char *&line, Insn &insn, bool emitInsn);
    Error encodeCnt(const char *&line, Insn &insn, bool acceptR0, bool accept16);
    Error encodeOpr(const char *&line, Insn &insn, bool emitInsn, bool destinationa = false);
    Error encodeRel(const char *&line, Insn &insn);
    Error encodeCruOff(const char *&line, Insn &insn);
    Error encodeIoaddr(const char *line, Insn &insn);
};

#endif // __ASM_TMS9995_H__
