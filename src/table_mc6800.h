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

    static TableMc6800 TABLE;

    Error searchName(InsnMc6800 &insn);
    Error searchOpCode(InsnMc6800 &insn);
    Error searchOpCodeAlias(InsnMc6800 &insn);
    bool isPrefix(uint8_t code) const { return _cpu->isPrefix(code); }

    const /* PROGMEM */ char *listCpu_P() const override;
    const /* PROGMEM */ char *cpu_P() const override;
    bool setCpu(const char *cpu) override;

    typedef EntryPageBase<Entry> EntryPage;
    typedef CpuBase<CpuType, EntryPage> Cpu;

private:
    const Cpu *_cpu;

    bool setCpu(CpuType cpuType);
    const Entry *searchOpCodeImpl(InsnMc6800 &insn) const;
};

}  // namespace mc6800
}  // namespace libasm

#endif  // __TABLE_MC6800_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
