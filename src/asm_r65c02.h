/* -*- mode: c++; -*- */
#ifndef __ASM_R65C02_H__
#define __ASM_R65C02_H__

#include "config_r65c02.h"

#include "error_reporter.h"
#include "symbol_table.h"
#include "table_r65c02.h"

template<McuType mcuType = R65C02>
class Assembler : public ErrorReporter {
public:
    Error encode(const char *line, Insn &insn,
                 target::uintptr_t addr, SymbolTable *symtab);

private:
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

    void emitInsnCode(Insn &insn) const {
        insn.emitByte(insn.insnCode());
    }

    Error getOperand16(const char *&line, target::uint16_t &val) const;
    Error parseOperand(const char *line, Insn &insn, target::uint16_t &val);

    Error encodeRelative(const char *line, Insn &insn, bool emitInsn);
    Error encodeZeroPageRelative(const char *line, Insn &insn);
};

#include "asm_r65c02_impl.h"

#endif // __ASM_R65C02_H__
