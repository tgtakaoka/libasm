/* -*- mode: c++; -*- */
#ifndef __ASM_Z80_H__
#define __ASM_Z80_H__

#include "config_z80.h"

#include "error_reporter.h"
#include "symbol_table.h"
#include "table_z80.h"

class AsmZ80 : public ErrorReporter {
public:
    Error encode(const char *line, Insn &insn,
                 target::uintptr_t addr, SymbolTable *symtab);

protected:
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
        const target::opcode_t prefix = InsnTable::prefixCode(insn.insnCode());
        if (InsnTable::isPrefixCode(prefix))
            insn.emitByte(prefix);
        insn.emitByte(InsnTable::opCode(insn.insnCode()));
    }

    const char *parseOperand(
        const char *line, OprFormat &oprFormat, RegName &regName,
        target::uint16_t &operand);
    Error getOperand16(const char *&line, target::uint16_t &val);

    Error encodeImmediate(Insn &insn, RegName leftReg, target::uint16_t rightOpr);
    Error encodeInherent(
        Insn &insn, RegName leftReg, RegName rightReg, target::uint16_t leftOpr);
    Error encodeDirect(
        Insn &insn, RegName leftReg, RegName rightReg,
        target::uintptr_t leftOpr, target::uintptr_t rightOpr);
    Error encodeIoaddr(
        Insn &insn, target::uint16_t leftOpr, target::uint16_t rightOpr);
    Error encodeRelative(
        Insn &insn, target::uintptr_t leftOpr, target::uintptr_t rightOpr);
    Error encodeIndexed(
        Insn &insn, RegName leftReg, RegName rightReg,
        target::uint16_t leftOpr, target::uint16_t rightOpr);
    Error encodeIndexedImmediate8(
        Insn &insn, RegName leftReg, RegName rightReg,
        target::uint16_t leftOpr, target::uint16_t rightOpr);
};

#endif // __ASM_Z80_H__