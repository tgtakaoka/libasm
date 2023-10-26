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

AsmDriver::AsmDriver(AsmDirective **begin, AsmDirective **end)
    : _directives(begin, end), _current(nullptr), _functions(), _origin(0) {
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
    _origin = 0;

    char buffer[256];
    StrBuffer out{buffer, sizeof(buffer)};
    AsmFormatter formatter{sources};
    formatter.setUpperHex(_upperHex);
    formatter.setLineNumber(_lineNumber);

    int errors = 0;
    StrScanner *line;
    while ((line = sources.readLine()) != nullptr) {
        auto &directive = *current();
        auto &insn = formatter.insn();
        insn.reset(_origin);
        AsmDirective::Context context{sources, reportError};
        auto scan = *line;
        auto error = directive.encode(scan, insn, context, *this);

        const auto &config = directive.assembler().config();
        const auto unit = config.addressUnit();  // assembler may be swiched
        const auto base = insn.address() * unit;
        for (auto offset = 0; offset < insn.length(); offset++) {
            memory.writeByte(base + offset, insn.bytes()[offset]);
        }
        _origin = insn.address() + insn.length() / config.addressUnit();
        if (insn.length() == 0)
            _origin = directive.assembler().currentLocation();

        formatter.set(*line, directive, config, &context.value);
        formatter.setListRadix(current()->assembler().listRadix());
        while (formatter.hasNextLine()) {
            listout.println(formatter.getLine(out).str());
            if (formatter.hasError())
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

bool AsmDriver::hasSymbol(const StrScanner &symbol) const {
    return symbolInTable(symbol, false) || symbolInTable(symbol, true);
}

uint32_t AsmDriver::lookupSymbol(const StrScanner &symbol) const {
    const auto key = std::string(symbol.str(), symbol.size());
    const auto s = _symbols.find(key);
    if (s != _symbols.end())
        return s->second;
    const auto v = _variables.find(key);
    if (v != _variables.end())
        return v->second;
    return 0;
}

bool AsmDriver::symbolInTable(const StrScanner &symbol, bool variable) const {
    const auto &map = variable ? _variables : _symbols;
    const auto key = std::string(symbol.str(), symbol.size());
    return map.find(key) != map.end();
}

Error AsmDriver::internSymbol(uint32_t value, const StrScanner &symbol, bool variable) {
    if (symbolInTable(symbol, !variable))
        return DUPLICATE_LABEL;

    const auto key = std::string(symbol.str(), symbol.size());
    auto &map = variable ? _variables : _symbols;
    auto it = map.find(key);
    if (it == map.end()) {
        map.emplace(key, value);
    } else {
        it->second = value;
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
