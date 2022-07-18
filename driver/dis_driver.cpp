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

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
