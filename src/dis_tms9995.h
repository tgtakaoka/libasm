/* -*- mode: c++; -*- */
#ifndef __DIS_TMS9995_H__
#define __DIS_TMS9995_H__

#include "config_tms9995.h"

#include "error_reporter.h"
#include "insn_tms9995.h"
#include "memory.h"
#include "symbol_table.h"

class Disassembler : public ErrorReporter {
public:
    Error decode(
        Memory &memory, Insn& insn,
        char *operands, char *comments, SymbolTable *symtab);

private:
    SymbolTable *_symtab;

    void reset(SymbolTable *symtab) {
        _symtab = symtab;
        resetError();
    }

    const char *lookup(uint16_t addr) const {
        return _symtab ? _symtab->lookup(addr) : nullptr;
    }

    Error readUint16(Memory &memory, Insn &insn, target::uint16_t &val);

    Error decodeOperand(
        Memory &memory, Insn& insn, char *&operands, char *&comments,
        host::uint_t opr);
    Error decodeImmediate(
        Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeRelative(Insn &insn, char *operands, char *comments);
};

#endif // __DIS_TMS9995_H__
