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

#ifndef __TABLE_I8080_H__
#define __TABLE_I8080_H__

#include "config_i8080.h"
#include "entry_table.h"
#include "insn_i8080.h"

namespace libasm {
namespace i8080 {

struct TableI8080 : entry::Table {
public:
    TableI8080();

    static TableI8080 TABLE;

    Error searchName(InsnI8080 &insn) const;
    Error searchOpCode(InsnI8080 &insn) const;
    bool isPrefix(uint8_t code) const { return _cpu->isPrefix(code); }

    const /* PROGMEM */ char *listCpu_P() const override;
    const /* PROGMEM */ char *cpu_P() const override;
    bool setCpu(const char *cpu) override;

    typedef entry::TableBase<Entry> EntryPage;
    typedef entry::CpuBase<CpuType, EntryPage> Cpu;

private:
    const Cpu *_cpu;

    bool setCpu(CpuType cpuType);
};

}  // namespace i8080
}  // namespace libasm

#endif  // __TABLE_I8080_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
