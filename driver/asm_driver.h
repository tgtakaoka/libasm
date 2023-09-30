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

#ifndef __ASM_DRIVER_H__
#define __ASM_DRIVER_H__

#include "error_reporter.h"
#include "function_store.h"
#include "str_scanner.h"
#include "symbol_table.h"
#include "text_printer.h"
#include "value_parser.h"

#include <list>
#include <map>

namespace libasm {
namespace driver {

struct AsmDirective;
struct AsmDirective;
struct AsmFormatter;
struct AsmSources;
struct BinMemory;

enum SymbolMode {
    REPORT_UNDEFINED = 0,
    REPORT_DUPLICATE = 1,
};

struct AsmDriver : SymbolTable {
    AsmDriver(AsmDirective **begin, AsmDirective **end, AsmSources &sources,
            std::map<std::string, std::string> *options = nullptr,
            SymbolMode symbolMode = REPORT_UNDEFINED);

    AsmDirective *restrictCpu(const char *cpu);
    AsmDirective *setCpu(const char *cpu);
    std::list<std::string> listCpu() const;
    AsmDirective *current() const { return _current; }

    void applyOptions() const;
    int assemble(AsmSources &sources, BinMemory &memory, AsmFormatter &formatter,
            TextPrinter &listout, TextPrinter &errorout, bool reportError);

    uint32_t origin() const { return _origin; }
    uint32_t setOrigin(uint32_t origin) { return _origin = origin; }
    SymbolMode symbolMode() const { return _symbolMode; }

    // SymbolTable
    const char *lookupValue(uint32_t address) const override;
    bool hasSymbol(const StrScanner &symbol) const override;
    uint32_t lookupSymbol(const StrScanner &symbol) const override;
    const void *lookupFunction(const StrScanner &symbol) const override {
        return _functions.lookupFunction(symbol);
    }

    Error internSymbol(uint32_t value, const StrScanner &symbol, bool variable = false);
    bool symbolInTable(const StrScanner &symbol) const;
    void setLineSymbol(const StrScanner &symbol);
    Error internLineSymbol(uint32_t value);

    bool hasFunction(const StrScanner &name) const { return _functions.hasFunction(name); }
    Error internFunction(const StrScanner &name, const std::list<StrScanner> &params,
            const StrScanner &body, const ValueParser &parser) {
        return _functions.internFunction(name, params, body, parser, this);
    }
    Error openSource(const StrScanner &filename);

    auto begin() const { return _directives.cbegin(); }
    auto end() const { return _directives.cend(); }

private:
    std::list<AsmDirective *> _directives;
    AsmDirective *_current;
    AsmSources &_sources;
    std::map<std::string, std::string> *_options;

    SymbolMode _symbolMode;
    FunctionStore _functions;
    uint32_t _origin;

    AsmDirective *switchDirective(AsmDirective *dir);

    const StrScanner *_lineSymbol;
    std::map<std::string, uint32_t, std::less<>> _symbols;
    std::map<std::string, uint32_t, std::less<>> _variables;
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
