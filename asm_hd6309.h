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
    static TableHd6309 _tableHd6309;

    Error getOperand32(const char *&line, target::uint32_t &val);
    Error encodeImmediate(const char *line, Insn &insn) override;
    Error encodeIndexed(const char *line, Insn &insn) override;
    Error encodeBitOperation(const char *line, Insn &insn);
    Error encodeImmediatePlus(const char *line, Insn &insn);
    Error encodeTransferMemory(const char *line, Insn &insn);
};

#endif // __ASM_HD6309_H__
