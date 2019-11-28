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
    SymbolTable *_symtab;

    void reset(SymbolTable *symtab) {
        _symtab = symtab;
        resetError();
    }

    const char *lookup(uint16_t addr) const {
        return _symtab ? _symtab->lookup(addr) : nullptr;
    }

    Error readByte(Memory &memory, Insn &insn, target::byte_t &val);
    Error readUint16(Memory &memory, Insn &insn, target::uint16_t &val);
    Error readUint32(Memory &memory, Insn &insn, target::uint32_t &val);

    // MC6809
    Error decodeDirectPage(Memory &memory, Insn &insn, char *operands);
    Error decodeIndexed(Memory &memory, Insn &insn, char *operands);
    Error decodeExtended(Memory &memory, Insn &insn, char *operands);
    Error decodeRelative(Memory &memory, Insn &insn, char *operands);
    Error decodeImmediate(Memory &memory, Insn &insn, char *operands);
    Error decodeStackOp(Memory &memory, Insn &insn, char *operands);
    Error decodeRegisters(Memory &memory, Insn &insn, char *operands);
    // HD6309
    Error decodeImmediatePlus(Memory &memory, Insn &insn, char *operands);
    Error decodeBitOperation(Memory &memory, Insn &insn, char *operands);
    Error decodeTransferMemory(Memory &memory, Insn &insn, char *operands);
};

#include "dis_hd6309_impl.h"

#endif // __DIS_HD6309_H__
