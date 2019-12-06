/* -*- mode: c++; -*- */
#ifndef __ASM_Z80_H__
#define __ASM_Z80_H__

#include "config_z80.h"

#include "error_reporter.h"
#include "symbol_table.h"
#include "table_z80.h"

class Assembler : public ErrorReporter {
public:
    Error encode(const char *line, Insn &insn,
                 target::uintptr_t addr, SymbolTable *symtab);

protected:
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
        const target::opcode_t prefix = InsnTable::prefixCode(insn.insnCode());
        if (InsnTable::isPrefixCode(prefix))
            insn.emitByte(prefix);
        insn.emitByte(InsnTable::opCode(insn.insnCode()));
    }

    Error parseOperand(
        OprFormat &oprFormat, RegName &regName, target::uint16_t &operand);
    Error getInt16(target::uint16_t &val);
    Error getOperand16(target::uint16_t &val);

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
