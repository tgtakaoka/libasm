/* -*- mode: c++; -*- */
#ifndef __DIS_I8080_H__
#define __DIS_I8080_H__

#include "config_i8080.h"

#include "error_reporter.h"
#include "insn_i8080.h"
#include "memory.h"
#include "symbol_table.h"

class DisI8080 : public ErrorReporter {
public:
    Error decode(
        Memory &memory, Insn& insn,
        char *operands, char *comments, SymbolTable *symtab);

private:
    SymbolTable *_symtab;

    void reset(SymbolTable *symtab) {
        _symtab = symtab;
        resetError();
    }

    Error readByte(Memory &memory, Insn &insn, target::byte_t &val);
    Error readUint16(Memory &memory, Insn &insn, target::uint16_t &val);

    const char *lookup(uint16_t addr) const {
        return _symtab ? _symtab->lookup(addr) : nullptr;
    }

    char *outOpr8Hex(char *out, target::byte_t val) const;
    char *outOpr16Hex(char *out, target::uint16_t val) const;

    Error decodeImmediate8(
        Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeImmediate16(
        Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeDirect(
        Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeIoaddr(
        Memory &memory, Insn &insn, char *operands, char *comments);
};

#endif // __DIS_I8080_H__
