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

#include <cctype>
#include <cstring>

namespace libasm {
namespace driver {

void FunctionStore::reset() {
    _functions.clear();
}

bool FunctionStore::hasFunction(const StrScanner &_name) const {
    const std::string name(_name.str(), _name.size());
    return _functions.find(name) != _functions.end();
}

FunctionStore::Function::Function(
        const StrScanner &b, const std::list<StrScanner> &plist, const ValueParser &_parser)
    : body(b.str(), b.size()), parser(_parser) {
    for (const auto &p : plist)
        params.emplace_back(p.str(), p.size());
}

Error FunctionStore::internFunction(const StrScanner &_name, const std::list<StrScanner> &params,
        const StrScanner &body, const ValueParser &parser) {
    const std::string name(_name.str(), _name.size());
    if (_functions.find(name) != _functions.end())
        return DUPLICATE_FUNCTION;
    _functions.emplace(name, Function{body, params, parser});
    return OK;
}

const Functor *FunctionStore::parseFunction(StrScanner &scan, ErrorAt &error) const {
    auto p = scan;
    p.trimStart([](char c) { return c != '(' && !isspace(c); });
    const std::string name(scan.str(), p.str() - scan.str());
    const auto it = _functions.find(name);
    if (it == _functions.end())
        return _parent ? _parent->parseFunction(scan, error) : &Functor::FN_NONE;
    scan = p;
    return &it->second;
}

Error FunctionStore::Function::eval(const Arguments &args, Value &val) const {
    struct Binding : SymbolTable {
        Binding(const std::list<std::string> &params, const Functor::Arguments &args)
            : _args(args) {
            int index = 1;
            for (const std::string &param : params)
                _params.emplace(param, index++);
        }
        bool hasSymbol(const StrScanner &symbol) const override {
            return _params.find(std::string(symbol.str(), symbol.size())) != _params.end();
        }
        uint32_t lookupSymbol(const StrScanner &symbol) const override {
            const auto it = _params.find(std::string(symbol.str(), symbol.size()));
            return _args.at(it->second).getUnsigned();
        }
        const char *lookupValue(uint32_t address) const override { return nullptr; }

    private:
        const Functor::Arguments &_args;
        std::map<std::string, int, std::less<>> _params;
    } binding{params, args};
    StrScanner body_scan(body.c_str());
    ErrorAt error;
    val = parser.eval(body_scan, error, &binding);
    return error.getError();
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
