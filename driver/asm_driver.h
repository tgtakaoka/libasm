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

#include "asm_formatter.h"
#include "error_reporter.h"
#include "function_store.h"
#include "str_scanner.h"
#include "symbol_table.h"
#include "text_reader.h"
#include "text_printer.h"
#include "value_parser.h"

#include <list>
#include <map>

namespace libasm {
namespace driver {

class AsmDirective;

class AsmSourceFactory {
public:
    virtual Error open(const StrScanner &name) = 0;
    virtual const TextReader *current() const = 0;
    virtual Error closeCurrent() = 0;
    virtual size_t size() const = 0;
    virtual StrScanner *readLine() = 0;
    virtual TextPrinter &errors() = 0;
};

enum SymbolMode {
    REPORT_UNDEFINED = 0,
    REPORT_DUPLICATE = 1,
};

class AsmDriver : public ErrorAt, public SymbolTable {
public:
    AsmDriver(AsmDirective **begin, AsmDirective **end, AsmSourceFactory &sources);

    AsmDirective *restrictCpu(const char *cpu);
    AsmDirective *setCpu(const char *cpu);
    std::list<std::string> listCpu() const;
    AsmDirective *current() const { return _current; }

    Error assemble(const StrScanner &line, AsmFormatter &list);
    void reset();
    uint32_t origin() const { return _origin; }
    uint32_t setOrigin(uint32_t origin) { return _origin = origin; }
    void setSymbolMode(SymbolMode mode) { _symbolMode = mode; }
    SymbolMode symbolMode() const { return _symbolMode; }

    // SymbolTable
    const char *lookupValue(uint32_t address) const override;
    bool hasSymbol(const StrScanner &symbol) const override;
    uint32_t lookupSymbol(const StrScanner &symbol) const override;
    Error internSymbol(uint32_t value, const StrScanner &symbol);

    Error internFunction(
            const StrScanner &name, std::list<StrScanner> &params, const StrScanner &body) {
        return _functions.internFunction(name, params, body);
    }
    Error openSource(const StrScanner &filename) { return _sources.open(filename); }

private:
    std::list<AsmDirective *> _directives;
    AsmDirective *_current;
    AsmSourceFactory &_sources;
    FunctionStore *_functionStore;
    ValueParser::FuncParser *_savedFuncParser;

    uint32_t _origin;
    SymbolMode _symbolMode;

    FunctionStore _functions;
    void setFunctionStore(FunctionStore *functionStore);
    AsmDirective *switchDirective(AsmDirective *dir);

    std::map<std::string, uint32_t, std::less<>> _symbols;
    bool symbolExists(const std::string &key) const;
    uint32_t symbolLookup(const std::string &key) const;
    Error symbolIntern(uint32_t value, const std::string &key);
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
