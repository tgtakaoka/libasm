/* -*- mode: c++; -*- */
#ifndef __ASM_HD6309_H__
#define __ASM_HD6309_H__

#include "config_hd6309.h"

#include "error_reporter.h"
#include "reg_hd6309.h"
#include "symbol_table.h"
#include "table_hd6309.h"

template<McuType mcuType>
class Asm09 : public ErrorReporter {
public:
    Error encode(const char *line, Insn &insn,
                 target::uintptr_t addr, SymbolTable *symtab);

private:
    RegHd6309<mcuType> _regs;
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
        const target::opcode_t prefix = TableHd6309Base::prefixCode(insn.insnCode());
        if (TableHd6309Base::isPrefixCode(prefix))
            insn.emitByte(prefix);
        insn.emitByte(TableHd6309Base::opCode(insn.insnCode()));
    }

    Error checkLineEnd();
    Error getHex16(uint16_t &val);
    Error getHex32(uint32_t &val);
    Error getInt16(uint16_t &val);
    Error getInt32(uint32_t &val);
    Error getOperand16(uint16_t &val);
    Error getOperand32(uint32_t &val);
    Error determineAddrMode(const char *line, Insn &insn);

    // MC6809
    Error encodeStackOp(Insn &insn);
    Error encodeRegisters(Insn &insn);
    Error encodeRelative(Insn &insn);
    Error encodeImmediate(Insn &insn);
    Error encodeDirect(Insn &insn, bool emitInsn = true);
    Error encodeExtended(Insn &insn, bool emitInsn = true);
    Error encodeIndexed(Insn &insn, bool emitInsn = true);
    // HD6309
    Error encodeBitOperation(Insn &insn);
    Error encodeImmediatePlus(Insn &insn);
    Error encodeTransferMemory(Insn &insn);
};

#include "asm_hd6309_impl.h"

typedef Asm09<MC6809> AsmMc6809;
typedef Asm09<HD6309> AsmHd6309;

#endif // __ASM_HD6309_H__
