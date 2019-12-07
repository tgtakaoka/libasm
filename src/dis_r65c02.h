/* -*- mode: c++; -*- */
#ifndef __DIS_R65C02_H__
#define __DIS_R65C02_H__

#include "config_r65c02.h"

#include "error_reporter.h"
#include "insn_r65c02.h"
#include "memory.h"
#include "symbol_table.h"

template<McuType mcuType = R65C02>
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

    Error decodeImmediate(Memory &memory, Insn &insn);
    Error decodeAbsolute(Memory &memory, Insn &insn);
    Error decodeZeroPage(Memory &memory, Insn &insn);
    Error decodeRelative(Memory &memory, Insn &insn);
};

#include "dis_r65c02_impl.h"

#endif // __DIS_R65C02_H__
