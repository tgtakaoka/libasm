/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __TABLE_TMS32010_H__
#define __TABLE_TMS32010_H__

#include "config_tms32010.h"
#include "entry_table.h"
#include "insn_tms32010.h"

namespace libasm {
namespace tms32010 {

struct TableTms32010 : entry::Table {
public:
    TableTms32010();

    static TableTms32010 TABLE;

    Error searchName(InsnTms32010 &insn) const;
    Error searchOpCode(InsnTms32010 &insn, StrBuffer &out) const;

    const /* PROGMEM */ char *cpu_P() const override { return _cpu->name_P(); }
    bool setCpu(const char *cpu) override;
    uint16_t dataMemoryLimit() const;

    typedef entry::TableBase<Entry> EntryPage;
    typedef entry::CpuBase<CpuType, EntryPage> Cpu;

private:
    const Cpu *_cpu;

    bool setCpu(CpuType cpuType);
};

}  // namespace tms32010
}  // namespace libasm

#endif  // __TABLE_TMS32010_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
