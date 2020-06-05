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

#ifndef __TABLE_MC6800_H__
#define __TABLE_MC6800_H__

#include "config_mc6800.h"
#include "entry_mc6800.h"
#include "insn_mc6800.h"
#include "table_base.h"

namespace libasm {
namespace mc6800 {

class TableMc6800 : public TableBase {
public:
    TableMc6800();

    Error searchName(InsnMc6800 &insn) const;
    Error searchOpCode(InsnMc6800 &insn) const;
    Error searchOpCodeAlias(InsnMc6800 &insn) const;
    bool isPrefixCode(Config::opcode_t opCode);

    const char *listCpu() override { return "6800, 6801, 6301, 6811"; }
    bool setCpu(const char *cpu) override;
    const char *getCpu() override;
    bool is6811() const { return _cpuType == MC68HC11; }

    struct EntryPage;

private:
    CpuType _cpuType;
    const EntryPage *_table;
    const EntryPage *_end;

    bool setCpu(CpuType cpuType);
    static Error searchName(
        InsnMc6800 &insn, const EntryPage *pages, const EntryPage *end);
    static const Entry *searchOpCode(
        InsnMc6800 &insn, const EntryPage *pages, const EntryPage *end);
};

extern TableMc6800 TableMc6800;

} // namespace mc6800
} // namespace libasm

#endif // __TABLE_MC6800_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
