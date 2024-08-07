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

#ifndef __LIBASM_DIS_DRIVER_H__
#define __LIBASM_DIS_DRIVER_H__

#include "bin_reader.h"
#include "dis_base.h"

#include <initializer_list>
#include <list>
#include <string>

namespace libasm {
namespace driver {

struct TextPrinter;
struct BinMemory;
struct DisFormatter;

struct DisDriver final {
    DisDriver(std::initializer_list<Disassembler *> disassemblers);

    Disassembler *restrictCpu(const char *cpu);
    Disassembler *setCpu(const char *cpu);
    std::list<std::string> listCpu() const;
    Disassembler *current() const { return _current; }

    void setUpperHex(bool upperHex);
    void setUppercase(bool uppercase);
    bool setOption(const char *name, const char *value);

    void disassemble(BinReader &memory, const char *inputName, TextPrinter &output,
            TextPrinter &listout, TextPrinter &errorout);

    auto begin() const { return _disassemblers.cbegin(); }
    auto end() const { return _disassemblers.cend(); }

private:
    std::list<Disassembler *> _disassemblers;
    Disassembler *_current;
    bool _upperHex;
    bool _uppercase;
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
