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

#ifndef __LIBASM_FUNCTION_STORE_H__
#define __LIBASM_FUNCTION_STORE_H__

#include <list>
#include <map>
#include <string>

#include "error_reporter.h"
#include "str_scanner.h"
#include "value_parser.h"

namespace libasm {
namespace driver {

struct FunctionStore final {
    explicit FunctionStore();

    bool operator==(const FunctionStore &other) const;
    bool operator!=(const FunctionStore &other) const { return !(*this == other); }
    void copy(const FunctionStore &other);
    void clear();

    bool hasFunction(const StrScanner &name) const;
    const Functor *lookupFunction(const StrScanner &symbol) const;

    using Parameters = std::list<StrScanner>;
    Error internFunction(const StrScanner &name, const Parameters &params, const StrScanner &body,
            const ValueParser &parser, const SymbolTable *symtab);

private:
    using ParametersAt = std::map<const std::string, int>;

    struct Function final : Functor {
        Function(const std::string &body_, const ParametersAt &paramsAt_,
                const ValueParser &parser_, const SymbolTable *symtab_)
            : body(body_), paramsAt(paramsAt_), parser(parser_), symtab(symtab_) {}
        Function(const Function &other);

        bool operator==(const Function &other) const;
        bool operator!=(const Function &other) const { return !(*this == other); }
        int_fast8_t nargs() const override { return paramsAt.size(); }
        Error eval(ValueStack &stack, ParserContext &context, uint_fast8_t argc) const override;

    private:
        const std::string body;
        const ParametersAt paramsAt;
        const ValueParser &parser;
        const SymbolTable *const symtab;
    };

    /**
     * Intermediate symbol table to bind function parameters to actual arguments in |stack|.
     */
    struct Binding final : SymbolTable {
        Binding(const ParametersAt &paramsAt_, const ValueStack &stack_, const SymbolTable *parent_)
            : paramsAt(paramsAt_), stack(stack_), parent(parent_) {}
        bool hasSymbol(const StrScanner &symbol) const override;
        const Value *lookupSymbol(const StrScanner &symbol) const override;
        const Functor *lookupFunction(const StrScanner &symbol) const override;

    private:
        const ParametersAt &paramsAt;
        const ValueStack &stack;
        const SymbolTable *const parent;
    };

    std::map<const std::string, const Function, std::less<>> _functions;
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
