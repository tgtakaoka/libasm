/* -*- mode: c++; -*- */
#ifndef __ASM_HD6309_H__
#define __ASM_HD6309_H__

#include "config_hd6309.h"

#include "asm_mc6809.h"
#include "table_hd6309.h"

class AsmHd6309 : public AsmMc6809 {
public:
    Error encode(const char *line, Insn &insn,
                 target::uintptr_t addr, SymbolTable *symtab) override;

protected:
    host::int_t encodeRegister(RegName regName) const override;
    RegName parseRegister(const char *line) const override;
    host::int_t encodeIndexReg(RegName regName) const override;
    RegName parseIndexReg(const char *line) const override;
    host::int_t encodeBaseReg(RegName regName) const override;
    RegName parseBaseReg(const char *line) const override;

    Error getOperand32(const char *&line, target::uint32_t &val) const;
    Error encodeBitOperation(const char *line, Insn &insn);
    Error encodeImmediatePlus(const char *line, Insn &insn);
    Error encodeTransferMemory(const char *line, Insn &insn);

    Error encodeImmediateExtra(const char *line, Insn &insn) override;
    Error encodeIndexedExtra(Insn &insn, RegName index, target::uintptr_t addr,
                             RegName base, host::int_t incr, bool indir) override;
};

#endif // __ASM_HD6309_H__
