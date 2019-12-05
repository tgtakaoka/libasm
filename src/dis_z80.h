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
        Memory &memory, Insn& insn, char *operands, SymbolTable *symtab);

private:
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
    void outOpr8Hex(target::byte_t val);
    void outOpr16Hex(target::uint16_t val);
    void outOpr16Int(target::int16_t val);
    void outOpr8Addr(target::byte_t addr);
    void outOpr16Addr(target::uintptr_t addr, bool indir = true);
    void outIndexOffset(target::insn_t insnCode, target::int8_t offset);
    void outRegister(RegName regName);
    void outPointer(RegName regName);
    void outDataRegister(RegName regName);
    void outConditionName(target::opcode_t cc, bool cc8 = true);

    Error decodeOperand(Memory &memory, Insn& insn);

    Error decodeInherent(Insn &insn);
    Error decodeImmediate8(Insn &insn, target::byte_t val);
    Error decodeImmediate16(Insn &insn, target::uint16_t val);
    Error decodeDirect(Insn &insn, target::uintptr_t addr);
    Error decodeIoaddr(Insn &insn, target::byte_t ioaddr);
    Error decodeRelative(Insn &insn, target::int8_t delta);
    Error decodeIndexed(Insn &insn, target::int8_t offset);
    Error decodeIndexedImmediate8(
        Insn &insn, target::int8_t offset, target::byte_t val);
    Error decodeIndexedBitOp(Insn &insn, target::int8_t offset,
        target::opcode_t opCode);
};

#endif // __DIS_Z80_H__
