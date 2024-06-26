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

#include "dis_driver.h"

#include "dis_formatter.h"
#include "text_printer.h"

#include <algorithm>
#include <cstring>
#include <list>
#include <string>

namespace libasm {
namespace driver {

DisDriver::DisDriver(std::initializer_list<Disassembler *> disassemblers)
    : _disassemblers(), _current(nullptr) {
    for (auto dis : disassemblers)
        _disassemblers.push_back(dis);
    _current = _disassemblers.front();
    setUpperHex(true);
    setUppercase(false);
}

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

Disassembler *DisDriver::restrictCpu(const char *cpu) {
    auto dis = setCpu(cpu);
    if (dis) {
        _disassemblers.clear();
        _disassemblers.push_back(dis);
    }
    return dis;
}

Disassembler *DisDriver::setCpu(const char *cpu) {
    for (auto dis : _disassemblers) {
        if (dis->setCpu(cpu))
            return _current = dis;
    }
    return nullptr;
}

std::list<std::string> DisDriver::listCpu() const {
    std::list<std::string> list;
    for (auto dis : _disassemblers) {
        const /* PROGMEM */ char *list_P = dis->config().listCpu_P();
        char listCpu[strlen_P(list_P) + 1];
        strcpy_P(listCpu, list_P);
        filter(listCpu, list);
    }
    return list;
}

void DisDriver::setUpperHex(bool upperHex) {
    _upperHex = upperHex;
}

void DisDriver::setUppercase(bool uppercase) {
    _uppercase = uppercase;
}

bool DisDriver::setOption(const char *name, const char *value) {
    return current()->setOption(name, value);
}

void DisDriver::disassemble(BinReader &memory, const char *inputName, TextPrinter &output,
        TextPrinter &listout, TextPrinter &errorout) {
    char buffer[256];
    StrBuffer out{buffer, sizeof(buffer)};

    auto &disassembler = *current();
    DisFormatter formatter(disassembler, inputName);
    formatter.setUpperHex(_upperHex);
    formatter.setUppercase(_uppercase);

    formatter.setCpu(disassembler.config().cpu_P());
    output.println(formatter.getContent(out).str());
    listout.println(formatter.getLine(out).str());

    const auto unit = disassembler.config().addressUnit();
    for (auto block = memory.begin(); block != nullptr; block = block->next()) {
        auto reader = block->reader();
        const auto start = reader.address() / unit;
        const auto mem_size = reader.size();

        formatter.setOrigin(start);
        output.println(formatter.getContent(out).str());
        listout.println(formatter.getLine(out).str());

        for (uint32_t mem_offset = 0; mem_offset < mem_size;) {
            formatter.reset();
            auto &operands = formatter.operands();
            auto &insn = formatter.insn();
            insn.reset(start + mem_offset / unit);
            disassembler.decode(reader, insn, operands.mark(), operands.capacity());
            formatter.set(insn);
            while (formatter.hasNextContent())
                output.println(formatter.getContent(out).str());
            while (formatter.hasNextLine()) {
                listout.println(formatter.getLine(out).str());
                if (!insn.isOK())
                    errorout.println(out.str());
            }
            mem_offset += insn.length();
        }
    }
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
