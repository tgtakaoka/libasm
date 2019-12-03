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
    SymbolTable *_symtab;

    void reset(SymbolTable *symtab) {
        _symtab = symtab;
        resetError();
    }

    const char *lookup(target::uintptr_t addr) const {
        return _symtab ? _symtab->lookup(addr) : nullptr;
    }

    Error readByte(Memory &memory, Insn &insn, target::byte_t &val);
    Error readUint16(Memory &memory, Insn &insn, target::uint16_t &val);

    Error decodeImmediate(Memory &memory, Insn &insn, char *operands);
    Error decodeAbsolute(Memory &memory, Insn &insn, char *operands);
    Error decodeZeroPage(Memory &memory, Insn &insn, char *operands);
    Error decodeRelative(Memory &memory, Insn &insn, char *operands);
};

#include "dis_r65c02_impl.h"

#endif // __DIS_R65C02_H__
