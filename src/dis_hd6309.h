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
        Memory &memory, Insn& insn,
        char *operands, char *comments, SymbolTable *symtab);
#if 0
    Error formConstantBytes(
        Memory &memory, Insn &insn, uint8_t len,
        char *operands, char *comments, SymbolTable *symtab);
#endif

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
    Error decodeDirectPage(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeIndexed(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeExtended(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeRelative(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeImmediate(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeStackOp(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeRegisters(Memory &memory, Insn &insn, char *operands, char *comments);
    // HD6309
    Error decodeImmediatePlus(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeBitOperation(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeTransferMemory(Memory &memory, Insn &insn, char *operands, char *comments);
};

#include "dis_hd6309_impl.h"

#endif // __DIS_HD6309_H__
