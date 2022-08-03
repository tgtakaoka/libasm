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

#include "bin_memory.h"
#include "dis_formatter.h"
#include "text_printer.h"

#include <string.h>

#include <algorithm>
#include <list>
#include <string>

namespace libasm {
namespace driver {

DisDriver::DisDriver(Disassembler **begin, Disassembler **end)
    : _disassemblers(begin, end), _current(nullptr) {}

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
    Disassembler *dis = setCpu(cpu);
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
        const /* PROGMEM */ char *list_P = dis->listCpu_P();
        char listCpu[strlen_P(list_P) + 1];
        strcpy_P(listCpu, list_P);
        filter(listCpu, list);
    }
    return list;
}

void DisDriver::disassemble(const BinMemory &memory, uint32_t dis_start, uint32_t dis_end,
        DisFormatter &formatter, TextPrinter &output, TextPrinter &listout, TextPrinter &errorout) {
    const auto addrUnit = current()->config().addressUnit();
    for (const auto &it : memory) {
        auto mem_base = it.base;
        auto mem_size = it.data.size();
        auto start = mem_base / addrUnit;
        const auto end = start + (mem_size - 1) / addrUnit;
        if (start > dis_end || end < dis_start)
            continue;
        if (start < dis_start) {
            mem_base = dis_start * addrUnit;
            mem_size -= (dis_start - start) * addrUnit;
            start = dis_start;
        }
        if (end > dis_end)
            mem_size -= (end - dis_end) * addrUnit;
        formatter.setOrigin(start);
        output.println(formatter.getContent());
        listout.println(formatter.getLine());
        auto reader = memory.reader(mem_base);
        for (size_t mem_offset = 0; mem_offset < mem_size;) {
            formatter.disassemble(reader, start + mem_offset / addrUnit);
            mem_offset += formatter.byteLength();
            while (formatter.hasNextLine()) {
                const char *line = formatter.getLine();
                listout.println(line);
                if (formatter.isError())
                    errorout.println(line);
            }
            while (formatter.hasNextContent())
                output.println(formatter.getContent());
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
