/* -*- mode: c++; -*- */
#ifndef __ASM_INTERFACE_H__
#define __ASM_INTERFACE_H__

#include "error_reporter.h"
#include "symbol_table.h"
#include "asm_operand.h"

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
    virtual AsmOperand *getParser() = 0;
    const char *errorAt() const { return _scan; }

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

    Error getOperand32(uint32_t &val32) {
        _scan = getParser()->eval(_scan, val32, _symtab);
        return ErrorReporter::setError(getParser()->getError());
    }
    Error getOperand16(uint16_t &val16) {
        _scan = getParser()->eval(_scan, val16, _symtab);
        return ErrorReporter::setError(getParser()->getError());
    }
    Error getOperand8(uint8_t &val8) {
        _scan = getParser()->eval(_scan, val8, _symtab);
        return ErrorReporter::setError(getParser()->getError());
    }

    struct strcasecmp {
        bool operator()(const char *a, const char *b) const {
            return strcasecmp(a, b) < 0;
        }
    };
};

#endif // __ASM_INTERFACE_H__
