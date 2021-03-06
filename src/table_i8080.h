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

#ifndef __TABLE_I8080_H__
#define __TABLE_I8080_H__

#include "config_i8080.h"
#include "insn_i8080.h"
#include "table_base.h"

namespace libasm {
namespace i8080 {

class TableI8080 : public TableBase {
public:
    TableI8080();

    Error searchName(InsnI8080 &insn) const;
    Error searchOpCode(InsnI8080 &insn) const;

    const char *listCpu() const override;
    bool setCpu(const char *cpu) override;
    const char *getCpu() const override;

    class EntryPage;

private:
    CpuType _cpuType;
    const EntryPage *_table;
    const EntryPage *_end;

    bool setCpu(CpuType cpuType);
    Error searchName(InsnI8080 &insn, const EntryPage *pages,
            const EntryPage *end) const;
    Error searchOpCode(InsnI8080 &insn, const EntryPage *pages,
            const EntryPage *end) const;
};

extern TableI8080 TableI8080;

}  // namespace i8080
}  // namespace libasm

#endif  // __TABLE_I8080_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
