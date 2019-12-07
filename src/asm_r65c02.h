/* -*- mode: c++; -*- */
#ifndef __ASM_R65C02_H__
#define __ASM_R65C02_H__

#include "config_r65c02.h"

#include "error_reporter.h"
#include "symbol_table.h"
#include "table_r65c02.h"

template<McuType mcuType>
class Asm6502 : public ErrorReporter {
public:
    Error encode(const char *line, Insn &insn,
                 target::uintptr_t addr, SymbolTable *symtab);

private:
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

    void emitInsnCode(Insn &insn) const {
        insn.emitByte(insn.insnCode());
    }

    Error checkLineEnd();
    Error getHex16(uint16_t &val);
    Error getInt16(uint16_t &val);
    Error getOperand16(uint16_t &val);
    Error parseOperand(Insn &insn, uint16_t &val);

    Error encodeRelative(Insn &insn, bool emitInsn);
    Error encodeZeroPageRelative(Insn &insn);
};

#include "asm_r65c02_impl.h"

typedef Asm6502<R6502> AsmR6502;
typedef Asm6502<R65C02> AsmR65c02;

#endif // __ASM_R65C02_H__
