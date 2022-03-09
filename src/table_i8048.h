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

#ifndef __TABLE_I8048_H__
#define __TABLE_I8048_H__

#include "config_i8048.h"
#include "entry_i8048.h"
#include "insn_i8048.h"
#include "table_base.h"

namespace libasm {
namespace i8048 {

class TableI8048 : public TableBase {
public:
    TableI8048();

    Error searchName(InsnI8048 &insn);
    Error searchOpCode(InsnI8048 &insn);

    const /* PROGMEM */ char *listCpu_P() const override;
    const /* PROGMEM */ char *cpu_P() const override;
    bool setCpu(const char *cpu) override;
    CpuType cpuType() const { return _cpuType; }

    struct EntryPage;

private:
    CpuType _cpuType;
    const EntryPage *_table;
    const EntryPage *_end;

    bool setCpu(CpuType cpuType);
    Error searchName(InsnI8048 &insn, const EntryPage *pages, const EntryPage *end) const;
    const Entry *searchOpCode(InsnI8048 &insn, const EntryPage *pages, const EntryPage *end) const;
};

extern TableI8048 TableI8048;

}  // namespace i8048
}  // namespace libasm

#endif  // __TABLE_I8048_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
