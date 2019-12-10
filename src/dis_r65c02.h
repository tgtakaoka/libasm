/* -*- mode: c++; -*- */
#ifndef __DIS_R65C02_H__
#define __DIS_R65C02_H__

#include "config_r65c02.h"

#include "error_reporter.h"
#include "insn_r65c02.h"
#include "dis_memory.h"
#include "symbol_table.h"

template<McuType mcuType>
class Dis6502 : public ErrorReporter {
public:
    Error decode(
        DisMemory<target::uintptr_t> &memory,
        Insn& insn,
        char *operands,
        SymbolTable<target::uintptr_t> *symtab);

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

    Error decodeImmediate(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeAbsolute(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeZeroPage(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeRelative(DisMemory<target::uintptr_t> &memory, Insn &insn);
};

#include "dis_r65c02_impl.h"

typedef Dis6502<R6502> DisR6502;
typedef Dis6502<R65C02> DisR65c02;

#endif // __DIS_R65C02_H__
