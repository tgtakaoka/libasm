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

#ifndef __TABLE_MOS6502_H__
#define __TABLE_MOS6502_H__

#include "config_mos6502.h"
#include "insn_mos6502.h"
#include "table_base.h"

namespace libasm {
namespace mos6502 {

class TableMos6502 : public TableBase {
public:
    TableMos6502();

    Error searchName(InsnMos6502 &insn) const;
    Error searchOpCode(InsnMos6502 &insn) const;

    const char *listCpu() override { return "6502, 65SC02, 65C02, W65C02S"; }
    bool setCpu(const char *cpu) override;
    const char *getCpu() override;
    bool is6502() const { return _cpuType == MOS6502; }

    struct EntryPage;

protected:
    CpuType _cpuType;
    const EntryPage *_table;
    const EntryPage *_end;

    bool setCpu(CpuType cpuType);

    static Error searchName(
        InsnMos6502 &insn, const EntryPage *pages, const EntryPage *end);
    static Error searchOpCode(
        InsnMos6502 &insn, const EntryPage *pages, const EntryPage *end);
};

extern TableMos6502 TableMos6502;

} // namespace mos6502
} // namespace libasm

#endif // __TABLE_MOS6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
