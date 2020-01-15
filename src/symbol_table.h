/* -*- mode: c++; -*- */
#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include <stdint.h>

class SymbolTable {
public:
    virtual const char *lookup(uint32_t value) = 0;
    virtual bool hasSymbol(
        const char *symbol, const char *end = nullptr) = 0;
    virtual uint32_t lookup(
        const char *symbol, const char *end = nullptr) = 0;
    virtual void intern(
        uint32_t value, const char *symbol, const char *end = nullptr) = 0;
    virtual uint32_t currentOrigin() = 0;
};

#endif  // __SYMBOL_TABLE_H__
