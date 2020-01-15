/* -*- mode: c++; -*- */
#ifndef __TEST_SYMTAB_H__
#define __TEST_SYMTAB_H__

#include "symbol_table.h"

#include <map>
#include <string>
#include <string.h>

class TestSymtab : public SymbolTable {
public:
    const char *lookup(uint32_t addr) override {
        auto it = _value_to_symbol.find(addr);
        return it == _value_to_symbol.end() ? nullptr : it->second.c_str();
    }
    bool hasSymbol(
        const char *symbol, const char *end = nullptr) override {
        return end ? hasSymbol(std::string(symbol, end)) : hasSymbol(std::string(symbol));
    }
    uint32_t lookup(
        const char *symbol, const char *end = nullptr) override {
        return end ? lookup(std::string(symbol, end)) : lookup(std::string(symbol));
    }
    void intern(
        uint32_t value, const char *symbol, const char *end = nullptr) override {
        intern(value, end ? std::string(symbol, end) : std::string(symbol));
    }

    uint32_t currentOrigin() override {
        return _current_origin;
    }

    void reset() {
        _symbol_to_value.clear();
        _value_to_symbol.clear();
        _current_origin = 0;
    }

    void setCurrentOrigin(uint32_t value) {
        _current_origin = value;
    }

private:
    std::map<std::string, uint32_t, std::less<>> _symbol_to_value;
    std::map<uint32_t, std::string> _value_to_symbol;
    uint32_t _current_origin;

    bool hasSymbol(const std::string &key) const {
        return _symbol_to_value.find(key) != _symbol_to_value.end();
    }

    uint32_t lookup(const std::string &key) const {
        auto it = _symbol_to_value.find(key);
        return it == _symbol_to_value.end() ? 0 : it->second;
    }

    void intern(uint32_t value, const std::string &key) {
        _symbol_to_value[key] = value;
        _value_to_symbol[value] = key;
    }
};

#endif
