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

#ifndef __TABLE_MC6809_H__
#define __TABLE_MC6809_H__

#include "config_mc6809.h"
#include "insn_mc6809.h"
#include "table_base.h"

namespace libasm {
namespace mc6809 {

class TableMc6809 : private TableBase {
public:
    TableMc6809();

    Error searchName(InsnMc6809 &insn) const;
    Error searchNameAndAddrMode(InsnMc6809 &insn) const;
    Error searchOpCode(InsnMc6809 &insn) const;

    const char *listCpu() override { return "6809, 6309"; }
    bool setCpu(const char *cpu) override;
    const char *getCpu() override;
    bool is6309() const { return _cpuType == HD6309; }

    static bool isPrefixCode(Config::opcode_t opCode);

    struct EntryPage;

private:
    CpuType _cpuType;
    const EntryPage *_table;
    const EntryPage *_end;

    void setCpu(CpuType cpuType);

    static Error searchName(
        InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end);
    static Error searchNameAndAddrMode(
        InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end);
    static Error searchOpCode(
        InsnMc6809 &insn, const EntryPage *pages, const EntryPage *end);
};

extern TableMc6809 TableMc6809;

} // namespace mc6809
} // namespace libasm

#endif // __TABLE_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
