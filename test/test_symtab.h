/*
 * Copyright 2020 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TEST_SYMTAB_H__
#define __TEST_SYMTAB_H__

#include <cstdint>
#include <cstring>
#include <map>
#include <string>

#include "symbol_table.h"

namespace libasm {
namespace test {

struct TestSymtab final : SymbolTable {
    const char *lookupValue(uint32_t addr) const override {
        auto it = _values.find(addr);
        return it == _values.end() ? nullptr : it->second.c_str();
    }

    bool hasSymbol(const StrScanner &symbol) const override {
        const std::string key(symbol.str(), symbol.size());
        return _symbols.find(key) != _symbols.end();
    }

    uint32_t lookupSymbol(const StrScanner &symbol) const override {
        const std::string key(symbol.str(), symbol.size());
        auto it = _symbols.find(key);
        return it == _symbols.end() ? 0 : it->second;
    }

    const void *lookupFunction(const StrScanner &symbol) const override {
        const std::string key(symbol.str(), symbol.size());
        auto it = _functions.find(key);
        return it == _functions.end() ? nullptr : it->second;
    }

    void intern(uint32_t value, const std::string &key) {
        _symbols[key] = value;
        _values[value] = key;
    }

    void internFunction(const void *value, const std::string &key) { _functions[key] = value; }

    void reset() {
        _symbols.clear();
        _values.clear();
        _functions.clear();
    }

private:
    std::map<std::string, uint32_t, std::less<>> _symbols;
    std::map<uint32_t, std::string> _values;
    std::map<std::string, const void *, std::less<>> _functions;
};

}  // namespace test
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
