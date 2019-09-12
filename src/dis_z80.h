/* -*- mode: c++; -*- */
#ifndef __DIS_Z80_H__
#define __DIS_Z80_H__

#include "config_z80.h"

#include "error_reporter.h"
#include "insn_z80.h"
#include "memory.h"
#include "registers_z80.h"
#include "symbol_table.h"

class Disassembler : public ErrorReporter {
public:
    Error decode(
        Memory &memory, Insn& insn,
        char *operands, char *comments, SymbolTable *symtab);
#if 0
    Error definedBytes(
        Memory &memory, Insn &insn, uint8_t len,
        char *operands, char *comments, SymbolTable *symtab);
#endif

private:
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

    Error decodeOperand(
        Memory &memory, Insn& insn, char *operands, char *comments);

    Error decodeInherent(
        Insn &insn, char *operands, char *comments);
    Error decodeImmediate8(
        Insn &insn, char *operands, char *comments, target::byte_t val);
    Error decodeImmediate16(
        Insn &insn, char *operands, char *comments, target::uint16_t val);
    Error decodeDirect(
        Insn &insn, char *operands, char *comments, target::uintptr_t addr);
    Error decodeIoaddr(
        Insn &insn, char *operands, char *comments, target::byte_t ioaddr);
    Error decodeRelative(
        Insn &insn, char *operands, char *comments, target::int8_t delta);
    Error decodeIndexed(
        Insn &insn, char *operands, char *comments, target::int8_t offset);
    Error decodeIndexedImmediate8(
        Insn &insn, char *operands, char *comments, target::int8_t offset,
        target::byte_t val);
    Error decodeIndexedBitOp(
        Insn &insn, char *operands, char *comments, target::int8_t offset,
        target::opcode_t opCode);
};

#endif // __DIS_Z80_H__
