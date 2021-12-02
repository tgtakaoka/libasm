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
#include "insn_mn1610.h"
#include "table_base.h"

namespace libasm {
namespace mn1610 {

class TableMn1610 : public TableBase {
public:
    TableMn1610();

    Error searchName(InsnMn1610 &insn) const;
    Error searchOpCode(InsnMn1610 &insn) const;

    const char *listCpu() const override;
    bool setCpu(const char *cpu) override;
    const char *getCpu() const override;

    AddressWidth addressWidth() const;
    Error checkAddressRange(Config::uintptr_t addr) const;

    struct EntryPage;

private:
    CpuType _cpuType;
    const EntryPage *_table;
    const EntryPage *_end;

    bool setCpu(CpuType cpuType);
    Error searchName(InsnMn1610 &insn, const EntryPage *table, const EntryPage *end) const;
    Error searchOpCode(InsnMn1610 &insn, const EntryPage *table, const EntryPage *end) const;
};

extern TableMn1610 TableMn1610;

}  // namespace mn1610
}  // namespace libasm

#endif  // __TABLE_MN1610_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4: