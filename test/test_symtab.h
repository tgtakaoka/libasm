/* -*- mode: c++; -*- */
#ifndef __TEST_SYMTAB_H__
#define __TEST_SYMTAB_H__

#include "symbol_table.h"

#include <map>
#include <cstring>

class TestSymtab : public SymbolTable {
public:
    bool hasSymbol(const char *symbol) const override {
        return _sym_to_addrs.find(symbol) != _sym_to_addrs.end();
    }
    uint32_t lookup(const char *symbol) const override {
        auto it = _sym_to_addrs.find(symbol);
        return it == _sym_to_addrs.end() ? 0 : it->second;
    }
    const char *lookup(uint32_t addr) const override {
        auto it = _addr_to_syms.find(addr);
        return it == _addr_to_syms.end() ? nullptr : it->second;
    }

    void reset() {
        _sym_to_addrs.clear();
        _addr_to_syms.clear();
    }

    void put(uint32_t addr, const char *symbol) {
        _sym_to_addrs.insert_or_assign(symbol, addr);
        _addr_to_syms.insert_or_assign(addr, symbol);
    }

private:
    struct compare_text {
        bool operator()(const char *a, const char *b) const {
            return strcmp(a, b) < 0;
        }
    };
    std::map<const char *, uint32_t, compare_text> _sym_to_addrs;
    std::map<uint32_t, const char *> _addr_to_syms;
};

#endif
