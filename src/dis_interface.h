/* -*- mode: c++; -*- */
#ifndef __DIS_INTERFACE_H__
#define __DIS_INTERFACE_H__

#include "error_reporter.h"
#include "symbol_table.h"

template<typename Addr>
class Disassembler : public ErrorReporter {
public:
    virtual Error decode(
        DisMemory<target::uintptr_t> &memory,
        Insn& insn,
        char *operands,
        SymbolTable<Addr> *symtab) = 0;
};

template<typename Addr>
class DisCommon : public Disassembler<Addr> {
protected:
    char *_operands;
    SymbolTable<Addr> *_symtab;

    void reset(char *operands, SymbolTable<Addr> *symtab) {
        *(_operands = operands) = 0;
        _symtab = symtab;
        ErrorReporter::resetError();
    }
    const char *lookup(Addr addr) const {
        return _symtab ? _symtab->lookup(addr) : nullptr;
    }
    void outText(const char *text) {
        char *p = _operands;
        while ((*p = *text++) != 0)
            p++;
        _operands = p;
    }
};

#endif // __DIS_INTERFACE_H__
