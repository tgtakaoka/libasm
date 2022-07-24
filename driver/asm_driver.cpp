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

#include "asm_driver.h"

#include "asm_directive.h"
#include "asm_sources.h"

#include <string.h>
#include <algorithm>

namespace libasm {
namespace driver {

static void appendTo(const std::string &cpu, std::list<std::string> &list) {
    if (std::find(list.begin(), list.end(), cpu) == list.end())
        list.push_back(cpu);
}

static void filter(const char *text, std::list<std::string> &list) {
    while (*text) {
        const char *del = strchr(text, ',');
        if (del == nullptr) {
            appendTo(std::string(text), list);
            return;
        }
        const std::string cpu(text, del);
        appendTo(cpu, list);
        for (text = del + 1; *text == ' '; text++)
            ;
    }
}

std::list<std::string> AsmDriver::listCpu() const {
    std::list<std::string> list;
    for (auto dir : _directives) {
        const /* PROGMEM */ char *list_P = dir->assembler().listCpu_P();
        char cpuList[strlen_P(list_P) + 1];
        strcpy_P(cpuList, list_P);
        filter(cpuList, list);
    }
    return list;
}

AsmDirective *AsmDriver::setCpu(const char *cpu) {
    for (auto dir : _directives) {
        if (dir->assembler().setCpu(cpu))
            return switchDirective(dir);
    }
    return nullptr;
}

AsmDirective *AsmDriver::restrictCpu(const char *cpu) {
    const /* PROGMEM */ char *cpu_P = current()->assembler().cpu_P();
    AsmDirective *z80 = AsmDirective::is8080(cpu_P) ? setCpu("Z80") : nullptr;
    AsmDirective *dir = setCpu(cpu);
    if (dir) {
        _directives.clear();
        _directives.push_back(dir);
        if (z80)
            _directives.push_back(z80);
    }
    return dir;
}

void AsmDriver::setFunctionStore(FunctionStore *functionStore) {
    _functionStore = functionStore;
    _savedFuncParser = current()->assembler().parser().setFuncParser(
            reinterpret_cast<ValueParser::FuncParser *>(functionStore));
    _functionStore->setParent(_savedFuncParser);
}

AsmDirective *AsmDriver::switchDirective(AsmDirective *dir) {
    if (current())
        current()->assembler().parser().setFuncParser(_savedFuncParser);
    _current = dir;
    if (_functionStore == nullptr) {
        ;  // defer setting FunctionStore until |_functionStore| is set.
    } else {
        setFunctionStore(_functionStore);
    }
    current()->assembler().reset();
    return dir;
}

AsmDriver::AsmDriver(AsmDirective **begin, AsmDirective **end, AsmSources &sources)
    : _directives(begin, end),
      _current(nullptr),
      _sources(sources),
      _functionStore(nullptr),
      _savedFuncParser(nullptr) {
    switchDirective(_directives.front());
    setFunctionStore(&_functions);
    _origin = 0;
    _symbolMode = REPORT_UNDEFINED;
}

int AsmDriver::assemble(AsmSources &sources, BinMemory &memory, AsmFormatter &formatter,
        TextPrinter &listout, TextPrinter &errorout, bool reportError) {
    for (auto dir : _directives) {
        dir->assembler().reset();
    }
    _functions.reset();
    setOrigin(0);
    _symbolMode = reportError ? REPORT_UNDEFINED : REPORT_DUPLICATE;

    int errors = 0;
    StrScanner *scan;
    while ((scan = sources.readLine()) != nullptr) {
        // const auto error = _driver.assemble(*scan, listing);
        const auto error = formatter.assemble(*scan, reportError);
        while (formatter.hasNextLine()) {
            const char *line = formatter.getLine();
            if (formatter.isError())
                errorout.println(line);
            listout.println(line);
        }
        if (error == END_ASSEMBLE)
            break;
        errors++;
    }
    while (sources.nest())
        sources.closeCurrent();
    return errors;
}

Error AsmDriver::openSource(const StrScanner &filename) {
    return _sources.open(filename);
}

const char *AsmDriver::lookupValue(uint32_t address) const {
    return nullptr;
}

bool AsmDriver::hasSymbol(const StrScanner &symbol) const {
    return symbolExists(std::string(symbol.str(), symbol.size()));
}

uint32_t AsmDriver::lookupSymbol(const StrScanner &symbol) const {
    return symbolLookup(std::string(symbol.str(), symbol.size()));
}

Error AsmDriver::internSymbol(uint32_t value, const StrScanner &symbol) {
    return symbolIntern(value, std::string(symbol.str(), symbol.size()));
}

bool AsmDriver::symbolExists(const std::string &key) const {
    auto it = _symbols.find(key);
    return it != _symbols.end();
}

uint32_t AsmDriver::symbolLookup(const std::string &key) const {
    auto it = _symbols.find(key);
    return it == _symbols.end() ? 0 : it->second;
}

Error AsmDriver::symbolIntern(uint32_t value, const std::string &key) {
    if (symbolExists(key) && _symbolMode == REPORT_DUPLICATE)
        return DUPLICATE_LABEL;
    _symbols.erase(key);
    _symbols.emplace(key, value);
    return OK;
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
