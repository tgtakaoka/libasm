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

#ifndef __FUNCTION_STORE_H__
#define __FUNCTION_STORE_H__

#include "error_reporter.h"
#include "str_scanner.h"
#include "value_parser.h"

#include <list>
#include <map>
#include <string>

namespace libasm {
namespace driver {

struct FunctionStore : FunCallParser {
    FunctionStore() : _parent(nullptr) {}

    void reset();
    void setParent(FunCallParser *parent) { _parent = parent; }

    Error internFunction(
            const StrScanner &name, std::list<StrScanner> &params, const StrScanner &body);

    // FunCallParser
    Error parseFunCall(const StrScanner &name, StrScanner &scan, Value &val, ErrorAt &error,
            const ValueParser &parser, const SymbolTable *symtab) const override;

private:
    struct icasecmp {
        bool operator()(const std::string &lhs, const std::string &rhs) const {
            return strcasecmp(lhs.c_str(), rhs.c_str()) < 0;
        }
    };

    FunCallParser *_parent;
    struct Function {
        std::string name;
        std::string body;
        std::list<std::string> params;
    };
    std::map<std::string, Function, icasecmp> _functions;
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
