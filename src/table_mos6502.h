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
    void useIndirectLong(bool enable);
    void longAccumlator(bool on);
    void longIndex(bool on);
    bool longImmediate(AddrMode addrMode) const;

    const char *listCpu() const override;
    bool setCpu(const char *cpu) override;
    const char *getCpu() const override;
    AddressWidth addressWidth() const;

    static constexpr Config::opcode_t WDM = 0x42;

    struct EntryPage;

private:
    CpuType _cpuType;
    const EntryPage *_table;
    const EntryPage *_end;
    bool _useIndirectLong;
    bool _longAccumulator;
    bool _longIndex;

    bool setCpu(CpuType cpuType);

    Error searchName(
        InsnMos6502 &insn, const EntryPage *pages, const EntryPage *end) const;
    Error searchOpCode(
        InsnMos6502 &insn, bool useIndirectLong,
        const EntryPage *pages, const EntryPage *end) const;
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
