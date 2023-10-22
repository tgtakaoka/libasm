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
#include "asm_formatter.h"
#include "asm_sources.h"

#include <algorithm>
#include <cstring>

namespace libasm {
namespace driver {

static void appendTo(const std::string &cpu, std::list<std::string> &list) {
    if (std::find(list.begin(), list.end(), cpu) == list.end())
        list.push_back(cpu);
}

static void filter(const char *text, std::list<std::string> &list) {
    while (*text) {
        const auto del = strchr(text, ',');
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
    auto *z80 = AsmDirective::is8080(cpu_P) ? setCpu("Z80") : nullptr;
    auto *dir = setCpu(cpu);
    if (dir) {
        _directives.clear();
        _directives.push_back(dir);
        if (z80)
            _directives.push_back(z80);
    }
    return dir;
}

AsmDirective *AsmDriver::switchDirective(AsmDirective *dir) {
    _current = dir;
    return dir;
}

AsmDriver::AsmDriver(AsmDirective **begin, AsmDirective **end, SymbolMode symbolMode)
    : _directives(begin, end),
      _current(nullptr),
      _symbolMode(symbolMode),
      _functions(),
      _origin(0) {
    switchDirective(_directives.front());
    setUpperHex(true);
    setLineNumber(false);
}

void AsmDriver::reset() {
    for (auto dir : _directives) {
        auto &assembler = dir->assembler();
        assembler.reset();
    }
}

void AsmDriver::setUpperHex(bool upperHex) {
    _upperHex = upperHex;
}

void AsmDriver::setLineNumber(bool lineNumber) {
    _lineNumber = lineNumber;
}

void AsmDriver::setOption(const char *name, const char *value) {
    for (auto dir : _directives) {
        auto &assembler = dir->assembler();
        assembler.setOption(name, value);
    }
}

int AsmDriver::assemble(AsmSources &sources, BinMemory &memory, TextPrinter &listout,
        TextPrinter &errorout, bool reportError) {
    _functions.reset();
    setOrigin(0);
    _symbolMode = reportError ? REPORT_UNDEFINED : REPORT_DUPLICATE;

    char buffer[256];
    StrBuffer out{buffer, sizeof(buffer)};
    AsmFormatter formatter{*this, sources, memory};

    int errors = 0;
    StrScanner *scan;
    while ((scan = sources.readLine()) != nullptr) {
        const auto error = formatter.assemble(*scan, reportError);
        while (formatter.hasNextLine()) {
            listout.println(formatter.getLine(out).str());
            if (reportError && formatter.hasError())
                errorout.println(out.str());
        }
        if (error == END_ASSEMBLE)
            break;
        errors++;
    }
    while (sources.nest())
        sources.closeCurrent();
    return errors;
}

const char *AsmDriver::lookupValue(uint32_t address) const {
    return nullptr;
}

void AsmDriver::setLineSymbol(const StrScanner &symbol) {
    _lineSymbol = symbol.size() ? &symbol : nullptr;
}

Error AsmDriver::internLineSymbol(uint32_t value) {
    auto err = OK;
    if (_lineSymbol) {
        err = internSymbol(value, *_lineSymbol);
        _lineSymbol = nullptr;
    }
    return err;
}

bool AsmDriver::hasSymbol(const StrScanner &symbol) const {
    if (_lineSymbol && _lineSymbol->iequals(symbol))
        return true;
    return symbolInTable(symbol);
}

bool AsmDriver::symbolInTable(const StrScanner &symbol) const {
    const auto key = std::string(symbol.str(), symbol.size());
    return _symbols.find(key) != _symbols.end() || _variables.find(key) != _variables.end();
}

uint32_t AsmDriver::lookupSymbol(const StrScanner &symbol) const {
    if (_lineSymbol && _lineSymbol->iequals(symbol))
        return _origin;
    const auto key = std::string(symbol.str(), symbol.size());
    const auto s = _symbols.find(key);
    if (s != _symbols.end())
        return s->second;
    const auto v = _variables.find(key);
    if (v != _variables.end())
        return v->second;
    return 0;
}

Error AsmDriver::internSymbol(uint32_t value, const StrScanner &symbol, bool variable) {
    const auto key = std::string(symbol.str(), symbol.size());

    if (variable) {
        const auto s = _symbols.find(key);
        if (s != _symbols.end())
            return DUPLICATE_LABEL;
        auto v = _variables.find(key);
        if (v != _variables.end()) {
            v->second = value;
        } else {
            _variables.emplace(key, value);
        }
        return OK;
    }

    const auto v = _variables.find(key);
    if (v != _variables.end())
        return DUPLICATE_LABEL;
    auto s = _symbols.find(key);
    if (s != _symbols.end() && s->second != value && _symbolMode == REPORT_DUPLICATE)
        return DUPLICATE_LABEL;
    if (s != _symbols.end()) {
        s->second = value;
    } else {
        _symbols.emplace(key, value);
    }
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
