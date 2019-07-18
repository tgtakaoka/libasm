/* -*- mode: c++; -*- */
#ifndef __ASM_6309_H__
#define __ASM_6309_H__

#include "config_6309.h"
#include "asm_6809.h"
#include "table_6309.h"

class Asm6309 : public Asm6809 {
public:
    Error encode(target::uintptr_t addr, const char *line, SymbolTable *symtab) override;
protected:
    static Table6309 _table6309;

    void addDword(target::dword_t val) {
        addWord(target::word_t(val >> 16));
        addWord(target::word_t(val & 0xffff));
    }

    Error getOperand32(const char *&line, target::dword_t &val);
    Error determineAddrMode(const char *line, AddrMode &mode) override;
    Error encodeImmediate(const char *line) override;
    Error encodeIndexed(const char *line) override;
    Error encodeBitOperation(const char *line);
    Error encodeImmediatePlus(const char *line);
    Error encodeTransferMemory(const char *line);
};

#endif // __ASM_6309_H__
