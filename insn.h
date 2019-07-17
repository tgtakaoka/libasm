/* -*- mode: c++; -*- */
#ifndef __INSN_H__
#define __INSN_H__

#include "error_reporter.h"
#include "config_host.h"
#include "config_target.h"
#include "symbol_table.h"

class Insn : public ReportError {
public:
    target::uintptr_t address() const { return _address; }
    const target::byte_t *bytes() const { return _bytes; }
    host::uint_t insnLen() const { return _insnLen; }
    host::uint_t oprLen() const { return _oprLen; }
    target::word_t insnCode() const { return _insnCode; }
    bool hasPrefixCode() const { return insnCode() >= 0x100; }
    const char *name() const { return _name; }
    virtual AddrMode addrMode() const;

protected:
    target::uintptr_t _address;
    target::word_t    _insnCode;
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
    bool hasSymbol(const char *symbol) const {
        return _symtab && _symtab->hasSymbol(symbol);
    }
};

#endif
