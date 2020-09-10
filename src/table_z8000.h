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

#ifndef __TABLE_Z8000_H__
#define __TABLE_Z8000_H__

#include "config_z8000.h"
#include "dis_memory.h"
#include "insn_z8000.h"
#include "table_base.h"

namespace libasm {
namespace z8000 {

class TableZ8000 : public TableBase {
public:
    TableZ8000();

    Error searchName(InsnZ8000 &insn) const;
    Error searchOpCode(InsnZ8000 &insn, DisMemory &memory) const;
    Error searchOpCodeAlias(InsnZ8000 &insn, DisMemory &memory) const;

    const char *listCpu() const override;
    bool setCpu(const char *cpu) override;
    const char *getCpu() const override;
    bool segmentedModel() const;
    AddressWidth addressWidth() const;

private:
    CpuType _cpuType;

    const Entry *searchOpCode(
        InsnZ8000 &insn, DisMemory &memory,
        const Entry *table, const Entry *end) const;
};

extern TableZ8000 TableZ8000;

} // namespace z8000
} // namespace libasm

#endif // __TABLE_Z8000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
