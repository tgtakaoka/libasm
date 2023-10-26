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
        const /* PROGMEM */ char *list_P = dir->listCpu_P();
        char cpuList[strlen_P(list_P) + 1];
        strcpy_P(cpuList, list_P);
        filter(cpuList, list);
    }
    return list;
}

bool AsmDriver::setCpu(const char *cpu) {
    for (auto dir : _directives) {
        if (dir->setCpu(cpu)) {
            const auto *prev = _current;
            _current = dir;
            dir->setListRadix(prev->listRadix());
            return true;
        }
    }
    return false;
}

bool AsmDriver::restrictCpu(const char *cpu) {
    const /* PROGMEM */ char *cpu_P = _current->cpu_P();
    if (setCpu(cpu)) {
        _directives.clear();
        _directives.push_back(_current);
        if (AsmDirective::is8080(cpu_P)) {
            if (setCpu("Z80"))
                _directives.push_back(_current);
        }
        return true;
    }
    return false;
}

AsmDriver::AsmDriver(AsmDirective **begin, AsmDirective **end)
    : _directives(begin, end), _current(nullptr), _symbols(), _origin(0) {
    for (auto dir : _directives)
        dir->reset(*this);
    _current = _directives.front();
    setUpperHex(true);
    setLineNumber(false);
}

void AsmDriver::setUpperHex(bool upperHex) {
    _upperHex = upperHex;
}

void AsmDriver::setLineNumber(bool lineNumber) {
    _lineNumber = lineNumber;
}

void AsmDriver::setOption(const char *name, const char *value) {
    for (auto dir : _directives) {
        dir->setOption(name, value);
    }
}

int AsmDriver::assemble(AsmSources &sources, BinMemory &memory, TextPrinter &listout,
        TextPrinter &errorout, bool reportError) {
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
        AsmDirective::Context context{sources, _symbols, reportError};
        auto scan = *line;
        auto error = directive.encode(scan, insn, context);

        const auto &config = directive.config();
        const auto unit = config.addressUnit();  // assembler may be swiched
        const auto base = insn.address() * unit;
        for (auto offset = 0; offset < insn.length(); offset++) {
            memory.writeByte(base + offset, insn.bytes()[offset]);
        }
        _origin = insn.address() + insn.length() / config.addressUnit();
        if (insn.length() == 0)
            _origin = directive.currentLocation();

        formatter.set(*line, directive, config, &context.value);
        formatter.setListRadix(current()->listRadix());
        if (formatter.hasError())
            ++errors;
        while (formatter.hasNextLine()) {
            listout.println(formatter.getLine(out).str());
            if (formatter.hasError())
                errorout.println(out.str());
        }
        if (error == END_ASSEMBLE)
            break;
    }
    while (sources.nest())
        sources.closeCurrent();
    return errors;
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
