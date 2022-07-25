/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include "function_store.h"

#include <ctype.h>
#include <string.h>

namespace libasm {
namespace driver {

void FunctionStore::reset() {
    _functions.clear();
}

Error FunctionStore::internFunction(
        const StrScanner &_name, std::list<StrScanner> &_params, const StrScanner &body) {
    Function func;
    const std::string name(_name.str(), _name.size());
    if (_functions.find(name) != _functions.end())
        return setError(DUPLICATE_FUNCTION);
    std::list<std::string> params;
    for (auto &param : _params)
        params.emplace_back(param.str(), param.size());
    _functions.emplace(name, Function{name, std::string(body.str(), body.size()), std::move(params)});
    return OK;
}

Error FunctionStore::parseFunc(ValueParser &parser, const StrScanner &name, StrScanner &scan,
        Value &val, const SymbolTable *symtab) {
    const auto it = _functions.find(std::string(name.str(), name.size()));
    if (it == _functions.end()) {
        if (_parent) {
            _parent->parseFunc(parser, name, scan, val, symtab);
            return setError(*_parent);
        }
        return setError(UNKNOWN_FUNCTION);
    }
    const auto &func = it->second;

    struct Binding : public SymbolTable {
        void intern(const std::string &symbol, uint32_t value) {
            _params.emplace(std::make_pair(symbol, value));
        }
        bool hasSymbol(const StrScanner &symbol) const override {
            return _params.find(std::string(symbol.str(), symbol.size())) != _params.end();
        }
        uint32_t lookupSymbol(const StrScanner &symbol) const override {
            return _params.find(std::string(symbol.str(), symbol.size()))->second;
        }
        const char *lookupValue(uint32_t address) const override { return nullptr; }
        std::map<std::string, uint32_t> _params;
    } binding;

    for (auto it = func.params.cbegin(); it != func.params.cend(); ++it) {
        const std::string &param = *it;
        if (it != func.params.cbegin() && !scan.skipSpaces().expect(','))
            return setError(MISSING_FUNC_ARGUMENT);
        const Value arg = parser.eval(scan, symtab);
        if (parser.getError())
            return setError(parser);
        binding.intern(param, arg.getUnsigned());
    }
    StrScanner body_scan(func.body.c_str());
    val = parser.eval(body_scan, &binding);
    return setError(parser);
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
