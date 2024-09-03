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

#ifndef __LIBASM_SYMBOL_STORE_H__
#define __LIBASM_SYMBOL_STORE_H__

#include <map>
#include <string>

#include "error_reporter.h"
#include "function_store.h"
#include "str_scanner.h"
#include "symbol_table.h"
#include "value_parser.h"

namespace libasm {
namespace driver {

struct SymbolStore : SymbolTable {
    virtual bool hasValue(const StrScanner &symbol, bool variable) const = 0;
    virtual Error internSymbol(
            const Value &value, const StrScanner &symbol, bool variable = false) = 0;
    virtual bool hasFunction(const StrScanner &name) const = 0;
    virtual Error internFunction(const StrScanner &name, const FunctionStore::Parameters &params,
            const StrScanner &body, const ValueParser &parser) = 0;
};

struct SymbolStoreImpl final : SymbolStore {
    explicit SymbolStoreImpl();

    bool operator==(const SymbolStoreImpl &other) const;
    bool operator!=(const SymbolStoreImpl &other) const { return !(*this == other); }
    void copy(const SymbolStoreImpl &other);
    void clear();
    void clearFunctions();

    // SymbolTable
    const char *lookupValue(symval_t) const override { return nullptr; }
    bool hasSymbol(const StrScanner &name) const override;
    symval_t lookupSymbol(const StrScanner &symbol) const override;
    const void *lookupFunction(const StrScanner &name) const override;

    bool hasValue(const StrScanner &symbol, bool variable) const override;
    Error internSymbol(
            const Value &value, const StrScanner &symbol, bool variable = false) override;

    bool hasFunction(const StrScanner &name) const override;
    Error internFunction(const StrScanner &name, const FunctionStore::Parameters &params,
            const StrScanner &body, const ValueParser &parser) override;

private:
    std::map<std::string, symval_t, std::less<>> _symbols;
    std::map<std::string, symval_t, std::less<>> _variables;
    FunctionStore _functions;
};

}  // namespace driver
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
