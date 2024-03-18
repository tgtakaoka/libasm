/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "symbol_store.h"

namespace libasm {
namespace driver {

SymbolStoreImpl::SymbolStoreImpl() : _functions() {}

bool SymbolStoreImpl::operator==(const SymbolStoreImpl &other) const {
    return _symbols == other._symbols && _variables == other._variables &&
           _functions == other._functions;
}

void SymbolStoreImpl::copy(const SymbolStoreImpl &other) {
    clear();
    for (auto &it : other._symbols)
        _symbols.emplace(it.first, it.second);
    for (auto &it : other._variables)
        _variables.emplace(it.first, it.second);
    _functions.copy(other._functions);
}

void SymbolStoreImpl::clear() {
    _symbols.clear();
    _variables.clear();
    clearFunctions();
}

void SymbolStoreImpl::clearFunctions() {
    _functions.clear();
}

bool SymbolStoreImpl::hasSymbol(const StrScanner &symbol) const {
    return hasValue(symbol, false) || hasValue(symbol, true);
}

uint32_t SymbolStoreImpl::lookupSymbol(const StrScanner &symbol) const {
    const auto key = std::string(symbol.str(), symbol.size());
    const auto s = _symbols.find(key);
    if (s != _symbols.end())
        return s->second;
    const auto v = _variables.find(key);
    if (v != _variables.end())
        return v->second;
    return 0;
}

bool SymbolStoreImpl::hasValue(const StrScanner &symbol, bool variable) const {
    const auto &map = variable ? _variables : _symbols;
    const auto key = std::string(symbol.str(), symbol.size());
    return map.find(key) != map.end();
}

Error SymbolStoreImpl::internSymbol(uint32_t value, const StrScanner &symbol, bool variable) {
    if (hasValue(symbol, !variable))
        return DUPLICATE_LABEL;

    const auto key = std::string(symbol.str(), symbol.size());
    auto &map = variable ? _variables : _symbols;
    auto it = map.find(key);
    if (it == map.end()) {
        map.emplace(key, value);
    } else {
        it->second = value;
    }
    return OK;
}

bool SymbolStoreImpl::hasFunction(const StrScanner &name) const {
    return _functions.hasFunction(name);
}

const void *SymbolStoreImpl::lookupFunction(const StrScanner &name) const {
    return _functions.lookupFunction(name);
}

Error SymbolStoreImpl::internFunction(const StrScanner &name, const std::list<StrScanner> &params,
        const StrScanner &body, const ValueParser &parser) {
    return _functions.internFunction(name, params, body, parser, this);
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
