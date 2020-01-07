/* -*- mode: c++; -*- */
#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include <stdint.h>

class SymbolTable {
public:
    virtual const char *lookup(uint32_t address) = 0;
    virtual bool hasSymbol(const char *symbol) = 0;
    virtual uint32_t lookup(const char *symbol) = 0;
    virtual uint32_t currentAddress() = 0;
};

#endif  // __SYMBOL_TABLE_H__
