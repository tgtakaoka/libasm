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

#ifndef __TABLE_Z8_H__
#define __TABLE_Z8_H__

#include "config_z8.h"
#include "dis_memory.h"
#include "entry_table.h"
#include "insn_z8.h"

namespace libasm {
namespace z8 {

struct TableZ8 : entry::Table {
public:
    TableZ8();

    static TableZ8 TABLE;

    Error searchName(InsnZ8 &insn) const;
    Error searchOpCode(InsnZ8 &insn, DisMemory &memory) const;
    bool isSuper8() const;

    const /* PROGMEM */ char *listCpu_P() const override;
    const /* PROGMEM */ char *cpu_P() const override { return _cpu->name_P(); }
    bool setCpu(const char *cpu) override;

    static constexpr Config::opcode_t SRP = 0x31;

    typedef entry::TableBase<Entry> EntryPage;
    typedef entry::CpuBase<CpuType, EntryPage> Cpu;

private:
    const Cpu *_cpu;

    bool setCpu(CpuType cpuType);
};

}  // namespace z8
}  // namespace libasm

#endif  // __TABLE_Z8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
