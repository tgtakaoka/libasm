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

#ifndef __TABLE_CDP1802_H__
#define __TABLE_CDP1802_H__

#include "config_cdp1802.h"
#include "entry_table.h"
#include "insn_cdp1802.h"

namespace libasm {
namespace cdp1802 {

class TableCdp1802 : public EntryTable {
public:
    TableCdp1802();

    static TableCdp1802 TABLE;

    Error searchName(InsnCdp1802 &insn) const;
    Error searchOpCode(InsnCdp1802 &insn) const;
    bool isPrefix(uint8_t code) const { return _cpu->isPrefix(code); }

    const /* PROGMEM */ char *listCpu_P() const override;
    const /* PROGMEM */ char *cpu_P() const override { return _cpu->name_P(); }
    bool setCpu(const char *cpu) override;

    typedef EntryTableBase<Entry> EntryPage;
    typedef CpuBase<CpuType, EntryPage> Cpu;

private:
    const Cpu *_cpu;

    bool setCpu(CpuType cpuType);
};

}  // namespace cdp1802
}  // namespace libasm

#endif  // __TABLE_CDP1802_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
