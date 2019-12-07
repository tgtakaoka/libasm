/* -*- mode: c++; -*- */
#ifndef __DIS_HD6309_H__
#define __DIS_HD6309_H__

#include "config_hd6309.h"

#include "error_reporter.h"
#include "insn_hd6309.h"
#include "memory.h"
#include "registers_hd6309.h"
#include "symbol_table.h"

template<McuType mcuType = HD6309>
class Disassembler : public ErrorReporter {
public:
    Error decode(
        Memory &memory, Insn& insn, char *operands, SymbolTable *symtab);

private:
    Registers<mcuType> _regs;
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
    void outOpr32Hex(uint32_t val);
    void outOpr16Int(int16_t val);
    void outRegister(RegName regName);

    // MC6809
    Error decodeDirectPage(Memory &memory, Insn &insn);
    Error decodeIndexed(Memory &memory, Insn &insn);
    Error decodeExtended(Memory &memory, Insn &insn);
    Error decodeRelative(Memory &memory, Insn &insn);
    Error decodeImmediate(Memory &memory, Insn &insn);
    Error decodeStackOp(Memory &memory, Insn &insn);
    Error decodeRegisters(Memory &memory, Insn &insn);
    // HD6309
    Error decodeImmediatePlus(Memory &memory, Insn &insn);
    Error decodeBitOperation(Memory &memory, Insn &insn);
    Error decodeTransferMemory(Memory &memory, Insn &insn);
};

#include "dis_hd6309_impl.h"

#endif // __DIS_HD6309_H__
