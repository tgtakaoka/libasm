/* -*- mode: c++; -*- */
#ifndef __DIS_TMS9995_H__
#define __DIS_TMS9995_H__

#include "config_tms9995.h"

#include "error_reporter.h"
#include "insn_tms9995.h"
#include "dis_memory.h"
#include "symbol_table.h"
#include "dis_interface.h"

class DisTms9995 : public Disassembler<target::uintptr_t>, public ErrorReporter {
public:
    Error decode(
        DisMemory<target::uintptr_t> &memory,
        Insn& insn,
        char *operands,
        SymbolTable<target::uintptr_t> *symtab) override;

private:
    char *_operands;
    SymbolTable<target::uintptr_t> *_symtab;

    void reset(char *operands, SymbolTable<target::uintptr_t> *symtab) {
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

    Error decodeOperand(
        DisMemory<target::uintptr_t> &memory, Insn& insn, host::uint_t opr);
    Error decodeImmediate(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeRelative(Insn &insn);
};

#endif // __DIS_TMS9995_H__
