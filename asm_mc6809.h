/* -*- mode: c++; -*- */
#ifndef __ASM_MC6809_H__
#define __ASM_MC6809_H__

#include "config_mc6809.h"

#include "error_reporter.h"
#include "asm.h"
#include "insn.h"
#include "symbol_table.h"
#include "table_mc6809.h"

class AsmMc6809 : public Asm, public ErrorReporter {
public:
    virtual Error encode(const char *line, Insn &insn,
                         target::uintptr_t addr, SymbolTable *symtab);

protected:
    SymbolTable  *_symtab;
    static TableMc6809 _tableMc6809;

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
        const target::opcode_t prefix = _tableMc6809.prefixCode(insn.insnCode());
        if (_tableMc6809.isPrefixCode(prefix))
            emitByte(insn, prefix);
        emitByte(insn, _tableMc6809.opCode(insn.insnCode()));
    }
#if 0
    target::opcode_t prefixCode() const {
        return _tableMc6809.prefixCode(_insn.insnCode());
    }
    bool isPrefixCode(target::opcode_t opcode) const {
        return _tableMc6809.isPrefixCode(opcode);
    }
    void addInsnCode() {
        target::opcode_t prefix = prefixCode();
        if (hasPrefixCode(prefix)) _insn.addByte(prefix);
        _insn.addByte(_tableMc6809.opCode(_insn.insnCode()));
    }
#endif

    static bool compareRegName(const char *line, RegName regName);
    static host::uint_t regNameLen(RegName regName);
    static host::int_t encodeRegNumber(RegName regName, const RegName *table, host::uint_t len);
    static RegName parseRegName(const char *, const RegName *table, host::uint_t len);

    Error getOperand16(const char *&line, target::uint16_t &val);
    Error determineAddrMode(const char *line, Insn &insn);
    Error encodeStackOp(const char *line, Insn &insn);
    Error encodeRegisters(const char *line, Insn &insn);
    Error encodeRelative(const char *line, Insn &insn);
    virtual Error encodeImmediate(const char *line, Insn &insn);
    Error encodeDirect(const char *line, Insn &insn);
    Error encodeExtended(const char *line, Insn &insn);
    virtual Error encodeIndexed(const char *line, Insn &insn);
};

#endif // __ASM_MC6809_H__
