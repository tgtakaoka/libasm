/* -*- mode: c++; -*- */
#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "config_host.h"

template<typename Addr>
class SymbolTable {
public:
    virtual const char *lookup(Addr address) const = 0;
    virtual bool hasSymbol(const char *symbol) const = 0;
    virtual Addr lookup(const char *symbol) const = 0;
};

#endif  // __SYMBOL_TABLE_H__
