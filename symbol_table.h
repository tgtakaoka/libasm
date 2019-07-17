/* -*- mode: c++; -*- */
#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "config_host.h"
#include "config_target.h"

class SymbolTable {
public:
    virtual const char *lookup(target::uintptr_t address) const { return nullptr; }
    virtual bool hasSymbol(const char *symbol) const { return false; }
    virtual target::uintptr_t lookup(const char *symbol) const { return 0; }

    virtual ~SymbolTable();
};

#endif  // __SYMBOL_TABLE_H__
