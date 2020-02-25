/* -*- mode: c++; -*- */
#ifndef __ASM_INTERFACE_H__
#define __ASM_INTERFACE_H__

#include "error_reporter.h"
#include "symbol_table.h"
#include "asm_operand.h"

#include <ctype.h>

template<typename Addr>
class Assembler : public ErrorReporter {
public:
    typedef Addr addr_t;

    Error encode(
        const char *line, Insn &insn, Addr addr,SymbolTable *symtab) {
        this->resetError();
        _scan = skipSpaces(line);
        if (checkLineEnd() == OK)
            return setError(NO_INSTRUCTION);
        this->resetError();
        _symtab = symtab;
        insn.resetAddress(addr);
        return encode(insn);
    }
    virtual AsmOperand &getParser() = 0;
    const char *errorAt() const { return _scan; }
    virtual bool setCpu(const char *cpu) = 0;
    virtual const char *listCpu() const = 0;

protected:
    const char *_scan;
    SymbolTable  *_symtab;

    void reset(const char *line, SymbolTable *symtab) {
        _scan = line;
        _symtab = symtab;
        resetError();
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
        if (setError(parser) == UNDEFINED_SYMBOL)
            return OK;
        return getError();
    }

    Error getOperand16(uint16_t &val16) {
        AsmOperand &parser = getParser();
        _scan = parser.eval(_scan, val16, _symtab);
        if (setError(parser) == UNDEFINED_SYMBOL)
            return OK;
        return getError();
    }

    Error getOperand8(uint8_t &val8) {
        AsmOperand &parser = getParser();
        _scan = parser.eval(_scan, val8, _symtab);
        if (setError(parser) == UNDEFINED_SYMBOL)
            return OK;
        return getError();
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
            return setError(getError());
        return setError(GARBAGE_AT_END);
    }

private:
    virtual Error encode(Insn &insn) = 0;
};

#endif // __ASM_INTERFACE_H__
