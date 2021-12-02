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

#ifndef __TABLE_MC6805_H__
#define __TABLE_MC6805_H__

#include "config_mc6805.h"
#include "entry_mc6805.h"
#include "insn_mc6805.h"
#include "table_base.h"

namespace libasm {
namespace mc6805 {

class TableMc6805 : public TableBase {
public:
    TableMc6805();

    Error searchName(InsnMc6805 &insn) const;
    Error searchOpCode(InsnMc6805 &insn) const;

    const char *listCpu() const override;
    bool setCpu(const char *cpu) override;
    const char *getCpu() const override;
    CpuType cpuType() const { return _cpuType; }

    struct EntryPage;

private:
    CpuType _cpuType;
    const EntryPage *_table;
    const EntryPage *_end;

    bool setCpu(CpuType cpuType);
    Error searchName(InsnMc6805 &insn, const EntryPage *pages, const EntryPage *end) const;
    const Entry *searchOpCode(InsnMc6805 &insn, const EntryPage *pages, const EntryPage *end) const;
};

extern TableMc6805 TableMc6805;

}  // namespace mc6805
}  // namespace libasm

#endif  // __TABLE_MC6805_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4: