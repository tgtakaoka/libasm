/* -*- mode: c++; -*- */
#ifndef __ASM_INTERFACE_H__
#define __ASM_INTERFACE_H__

#include "error_reporter.h"
#include "symbol_table.h"
#include "asm_operand.h"

#include <ctype.h>
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
    virtual AsmOperand &getParser() = 0;
    const char *errorAt() const { return _scan; }
    virtual bool acceptCpu(const char *cpu) = 0;

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
        AsmOperand &parser = getParser();
        _scan = parser.eval(_scan, val32, _symtab);
        return ErrorReporter::setError(parser);
    }
    Error getOperand16(uint16_t &val16) {
        AsmOperand &parser = getParser();
        _scan = parser.eval(_scan, val16, _symtab);
        return ErrorReporter::setError(parser);
    }
    Error getOperand8(uint8_t &val8) {
        AsmOperand &parser = getParser();
        _scan = parser.eval(_scan, val8, _symtab);
        return ErrorReporter::setError(parser);
    }

    const char *skipSpaces(const char *scan) {
        while (isspace(*scan))
            scan++;
        return scan;
    }
    Error checkLineEnd(const char *scan = nullptr) {
        if (scan == nullptr) scan = _scan;
        const char *p = skipSpaces(scan);
        if (*p == 0 || *p == ';')
            return setError(OK);
        return setError(GARBAGE_AT_END);
    }
};

#endif // __ASM_INTERFACE_H__
