/* -*- mode: c++; -*- */
#ifndef __DIS_HD6309_H__
#define __DIS_HD6309_H__

#include "config_hd6309.h"

#include "dis_mc6809.h"

class DisHd6309 : public DisMc6809 {
public:
    Error decode(
        Memory &memory, Insn& insn,
        char *operands, char *comments, SymbolTable *symtab) override;

protected:
    Error readUint32(Memory &memory, Insn &insn, target::uint32_t &val);

    RegName decodeIndexReg(target::byte_t regNum) const override;
    RegName decodeRegName(target::byte_t regNum) const override;

    Error decodeImmediatePlus(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeBitPosition(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeTransferMemory(Memory &memory, Insn &insn, char *operands, char *comments);

    Error decodeImmediateExtra(
        Memory &memory, Insn &insn, char *operands, char *comments) override;
    Error decodeIndexedExtra(
        Memory &memory, Insn &insn, target::byte_t post,
        RegName &index, target::uintptr_t &addr, host::int_t &offSize,
        RegName &base, host::int_t &incr) override;
};

#endif // __DIS_HD6309_H__
