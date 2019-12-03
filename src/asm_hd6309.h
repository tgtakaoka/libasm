/* -*- mode: c++; -*- */
#ifndef __ASM_HD6309_H__
#define __ASM_HD6309_H__

#include "config_hd6309.h"

#include "error_reporter.h"
#include "registers_hd6309.h"
#include "symbol_table.h"
#include "table_hd6309.h"

template<McuType mcuType = HD6309>
class Assembler : public ErrorReporter {
public:
    Error encode(const char *line, Insn &insn,
                 target::uintptr_t addr, SymbolTable *symtab);

private:
    Registers<mcuType> _regs;
    SymbolTable  *_symtab;

    void reset(SymbolTable *symtab) {
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
        const target::opcode_t prefix = InsnTableUtils::prefixCode(insn.insnCode());
        if (InsnTableUtils::isPrefixCode(prefix))
            insn.emitByte(prefix);
        insn.emitByte(InsnTableUtils::opCode(insn.insnCode()));
    }

    Error getOperand16(const char *&line, target::uint16_t &val) const;
    Error getOperand32(const char *&line, target::uint32_t &val) const;
    Error determineAddrMode(const char *line, Insn &insn);

    // MC6809
    Error encodeStackOp(const char *line, Insn &insn);
    Error encodeRegisters(const char *line, Insn &insn);
    Error encodeRelative(const char *line, Insn &insn);
    Error encodeImmediate(const char *line, Insn &insn);
    Error encodeDirect(const char *line, Insn &insn, bool emitInsn = true);
    Error encodeExtended(const char *line, Insn &insn, bool emitInsn = true);
    Error encodeIndexed(const char *line, Insn &insn, bool emitInsn = true);
    // HD6309
    Error encodeBitOperation(const char *line, Insn &insn);
    Error encodeImmediatePlus(const char *line, Insn &insn);
    Error encodeTransferMemory(const char *line, Insn &insn);
};

#include "asm_hd6309_impl.h"

#endif // __ASM_HD6309_H__
