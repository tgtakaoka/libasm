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

#ifndef __TABLE_MN1610_H__
#define __TABLE_MN1610_H__

#include "config_mn1610.h"
#include "entry_table.h"
#include "insn_mn1610.h"

namespace libasm {
namespace mn1610 {

struct TableMn1610 : entry::Table {
public:
    TableMn1610();

    static TableMn1610 TABLE;

    Error searchName(InsnMn1610 &insn) const;
    Error searchOpCode(InsnMn1610 &insn) const;

    const /* PROGMEM */ char *listCpu_P() const override;
    const /* PROGMEM */ char *cpu_P() const override { return _cpu->name_P(); }
    bool setCpu(const char *cpu) override;

    AddressWidth addressWidth() const;

    typedef entry::TableBase<Entry> EntryPage;
    typedef entry::CpuBase<CpuType, EntryPage> Cpu;

private:
    const Cpu *_cpu;

    bool setCpu(CpuType cpuType);
};

}  // namespace mn1610
}  // namespace libasm

#endif  // __TABLE_MN1610_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
