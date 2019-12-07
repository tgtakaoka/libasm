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
        Memory &memory, Insn& insn, char *operands, SymbolTable *symtab);

private:
    char *_operands;
    SymbolTable *_symtab;

    void reset(char *operands, SymbolTable *symtab) {
        *(_operands = operands) = 0;
        _symtab = symtab;
        resetError();
    }

    const char *lookup(target::uintptr_t addr) const {
        return _symtab ? _symtab->lookup(addr) : nullptr;
    }

    void outChar(char c) { *_operands++ = c; *_operands = 0; }
    void outText(const char *text);
    void outOpr8Hex(uint8_t val);
    void outOpr16Hex(uint16_t val);
    void outOpr16Int(uint16_t val);
    void outOpr16Addr(target::uintptr_t addr);
    void outRegister(host::uint_t regno);

    Error decodeOperand(Memory &memory, Insn& insn, host::uint_t opr);
    Error decodeImmediate(Memory &memory, Insn &insn);
    Error decodeRelative(Insn &insn);
};

#endif // __DIS_TMS9995_H__
