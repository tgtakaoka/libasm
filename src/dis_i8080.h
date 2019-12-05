/* -*- mode: c++; -*- */
#ifndef __DIS_I8080_H__
#define __DIS_I8080_H__

#include "config_i8080.h"

#include "error_reporter.h"
#include "insn_i8080.h"
#include "memory.h"
#include "symbol_table.h"

class Disassembler : public ErrorReporter {
public:
    Error decode(
        Memory &memory, Insn& insn, char *operands, SymbolTable *symtab);

private:
    SymbolTable *_symtab;

    void reset(SymbolTable *symtab) {
        _symtab = symtab;
        resetError();
    }

    const char *lookup(target::uintptr_t addr) const {
        return _symtab ? _symtab->lookup(addr) : nullptr;
    }

    Error decodeImmediate8(Memory &memory, Insn &insn, char *operands);
    Error decodeImmediate16(Memory &memory, Insn &insn, char *operands);
    Error decodeDirect(Memory &memory, Insn &insn, char *operands);
    Error decodeIoaddr(Memory &memory, Insn &insn, char *operands);
};

#endif // __DIS_I8080_H__
