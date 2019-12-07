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
    const char *_scan;
    SymbolTable  *_symtab;

    void reset(const char *line, SymbolTable *symtab) {
        _scan = line;
        _symtab = symtab;
        resetError();
    }
    bool hasSymbol(const char *symbol) const {
        return _symtab && _symtab->hasSymbol(symbol);
    }
    target::uintptr_t lookup(const char *symbol) const {
        return _symtab ? _symtab->lookup(symbol) : 0;
    }

    Error checkComma();
    Error getHex16(uint16_t &val);
    Error getInt16(uint16_t &val);
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
