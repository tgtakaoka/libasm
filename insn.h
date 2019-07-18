/* -*- mode: c++; -*- */
#ifndef __INSN_H__
#define __INSN_H__

#include "error_reporter.h"
#include "config_host.h"
#include "config_target.h"
#include "symbol_table.h"

class Insn : public ErrorReporter {
public:
    target::uintptr_t address() const { return _address; }
    const target::byte_t *bytes() const { return _bytes; }
    host::uint_t insnLen() const { return _insnLen; }
    host::uint_t oprLen() const { return _oprLen; }
    target::insn_t insnCode() const { return _insnCode; }
    const char *name() const { return _name; }

protected:
    target::uintptr_t _address;
    target::insn_t    _insnCode;
    host::uint_t      _insnLen;
    host::uint_t      _oprLen;
    target::byte_t    _bytes[8];
    char              _name[8];

    SymbolTable  *_symtab;

    void reset(target::uintptr_t addr, SymbolTable *symtab) {
        _address = addr;
        _insnLen = 0;
        _symtab = symtab;
        resetError();
    }
    const char *lookup(target::uintptr_t addr) const {
        return _symtab ? _symtab->lookup(addr) : nullptr;
    }
    bool hasSymbol(const char *symbol) const {
        return _symtab && _symtab->hasSymbol(symbol);
    }
    target::uintptr_t lookup(const char *symbol) const {
        return _symtab ? _symtab->lookup(symbol) : 0;
    }
};

#endif
