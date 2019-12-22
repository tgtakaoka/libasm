/* -*- mode: c++; -*- */
#ifndef __ASM_INTERFACE_H__
#define __ASM_INTERFACE_H__

#include "error_reporter.h"
#include "symbol_table.h"

#include <string.h>

template<typename Addr>
class Assembler : public ErrorReporter {
public:
    typedef Addr addr_t;

    virtual Error encode(
        const char *line,
        Insn &insn,
        Addr addr,
        SymbolTable *symtab) = 0;
    virtual const char *errorAt() const = 0;
    virtual bool isRegister(const char *text) const = 0;
};

template<typename Addr>
class AsmCommon : public Assembler<Addr> {
public:
    const char *errorAt() const override { return _scan; }

protected:
    const char *_scan;
    SymbolTable  *_symtab;

    void reset(const char *line, SymbolTable *symtab) {
        _scan = line;
        _symtab = symtab;
        ErrorReporter::resetError();
    }
    bool hasSymbol(const char *symbol) const {
        return _symtab && _symtab->hasSymbol(symbol);
    }
    Addr lookup(const char *symbol) const {
        return _symtab ? _symtab->lookup(symbol) : 0;
    }

    struct strcasecmp {
        bool operator()(const char *a, const char *b) const {
            return strcasecmp(a, b) < 0;
        }
    };
};

#endif // __ASM_INTERFACE_H__
