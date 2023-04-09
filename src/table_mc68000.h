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

#ifndef __TABLE_MC68000_H__
#define __TABLE_MC68000_H__

#include "config_mc68000.h"
#include "entry_table.h"
#include "insn_mc68000.h"

namespace libasm {
namespace mc68000 {

struct TableMc68000 : entry::Table {
public:
    TableMc68000();

    static TableMc68000 TABLE;

    Error searchName(InsnMc68000 &insn) const;
    Error searchOpCode(InsnMc68000 &insn, StrBuffer &out) const;

    const /* PROGMEM */ char *cpu_P() const override { return _cpu->name_P(); }
    bool setCpu(const char *cpu) override;

    void setAlias(bool enable);

    typedef entry::TableBase<Entry> EntryPage;
    typedef entry::CpuBase<CpuType, EntryPage> Cpu;

private:
    const Cpu *_cpu;
};

}  // namespace mc68000
}  // namespace libasm

#endif  // __TABLE_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
