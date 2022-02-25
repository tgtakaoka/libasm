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

#include <stdint.h>
#include <string.h>

#include <map>
#include <string>

#include "symbol_table.h"

namespace libasm {
namespace test {

class TestSymtab : public SymbolTable {
public:
    const char *lookupValue(uint32_t addr) const override {
        auto it = _value_to_symbol.find(addr);
        return it == _value_to_symbol.end() ? nullptr : it->second.c_str();
    }
    bool hasSymbol(const char *symbol, const char *end = nullptr) const override {
        return end ? hasSymbol(std::string(symbol, end)) : hasSymbol(std::string(symbol));
    }
    uint32_t lookupSymbol(const char *symbol, const char *end = nullptr) const override {
        return end ? lookup(std::string(symbol, end)) : lookup(std::string(symbol));
    }

    void intern(uint32_t value, const char *symbol, const char *end = nullptr) {
        intern(value, end ? std::string(symbol, end) : std::string(symbol));
    }

    void reset() {
        _symbol_to_value.clear();
        _value_to_symbol.clear();
    }

private:
    std::map<std::string, uint32_t, std::less<>> _symbol_to_value;
    std::map<uint32_t, std::string> _value_to_symbol;

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

}  // namespace test
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
