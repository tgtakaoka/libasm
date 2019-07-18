/* -*- mode: c++; -*- */
#ifndef __ASM_HD6309_H__
#define __ASM_HD6309_H__

#include "config_hd6309.h"
#include "asm_mc6809.h"
#include "table_hd6309.h"

class AsmHd6309 : public AsmMc6809 {
public:
    Error encode(target::uintptr_t addr, const char *line, SymbolTable *symtab) override;
protected:
    static TableHd6309 _tableHd6309;

    void addDword(target::dword_t val) {
        addWord(target::word_t(val >> 16));
        addWord(target::word_t(val & 0xffff));
    }

    Error getOperand32(const char *&line, target::dword_t &val);
    Error encodeImmediate(const char *line) override;
    Error encodeIndexed(const char *line) override;
    Error encodeBitOperation(const char *line);
    Error encodeImmediatePlus(const char *line);
    Error encodeTransferMemory(const char *line);
};

#endif // __ASM_HD6309_H__
