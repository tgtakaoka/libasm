/* -*- mode: c++; -*- */
#ifndef __DIS_MC6809_H__
#define __DIS_MC6809_H__

#include "config_mc6809.h"

#include "dis.h"
#include "error_reporter.h"
#include "insn.h"
#include "memory.h"
#include "symbol_table.h"
#include "table_mc6809.h"

class DisMc6809 : public Dis, public ErrorReporter {
public:
    virtual Error decode(
        Memory &memory, Insn& insn,
        char *operands, char *comments, SymbolTable *symtab);
#if 0
    Error formConstantBytes(
        Memory &memory, Insn &insn, uint8_t len,
        char *operands, char *comments, SymbolTable *symtab);
#endif

protected:
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
    char *outRegName(char *out, const RegName regName) const;
    char *outCCRBits(char *out, target::byte_t val) const;

    virtual RegName decodeIndexReg(target::byte_t regNum) const;
    RegName decodeBaseReg(target::byte_t regNum) const;
    virtual RegName decodeRegName(target::byte_t regNum) const;

    Error decodeDirectPage(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeIndexed(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeExtended(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeRelative(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeImmediate(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeStackOp(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeRegisters(Memory &memory, Insn &insn, char *operands, char *comments);

    virtual Error decodeImmediateExtra(
        Memory &memory, Insn &insn, char *operands, char *comments) {
        return UNKNOWN_INSTRUCTION;
    }
    virtual Error decodeIndexedExtra(
        Memory &memory, Insn &insn, target::byte_t post,
        RegName &index, target::uintptr_t &addr, host::int_t &offSize,
        RegName &base, host::int_t &incr) {
        return UNKNOWN_INSTRUCTION;
    }
};

#endif // __DIS_MC6809_H__
