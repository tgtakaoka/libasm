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

struct FunctionStore : FunctionParser {
    FunctionStore() : _parent(nullptr) {}

    void reset();
    void setParent(const FunctionParser *parent) { _parent = parent; }

    bool hasFunction(const StrScanner &name) const;
    Error internFunction(const StrScanner &name, const std::list<StrScanner> &params,
            const StrScanner &body, const ValueParser &parser);

    const Functor *parseFunction(StrScanner &scan, ErrorAt &error) const override;

private:
    struct Function : Functor {
        Function(const StrScanner &body, const std::list<StrScanner> &params,
                const ValueParser &parser);
        // Functor
        int8_t nargs() const override { return params.size(); }
        Error eval(ValueStack &stack, uint8_t argc) const override;

    private:
        std::string body;
        const ValueParser &parser;
        std::list<std::string> params;
    };

    const FunctionParser *_parent;
    std::map<std::string, Function, std::less<>> _functions;
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
