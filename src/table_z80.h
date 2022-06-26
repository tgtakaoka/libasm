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

#ifndef __TABLE_Z80_H__
#define __TABLE_Z80_H__

#include "config_z80.h"
#include "insn_z80.h"
#include "table_base.h"

namespace libasm {
namespace z80 {

class TableZ80 : public TableBase {
public:
    TableZ80();

    Error searchName(InsnZ80 &insn);
    Error searchOpCode(InsnZ80 &insn);

    const /* PROGMEM */ char *listCpu_P() const override;
    const /* PROGMEM */ char *cpu_P() const override;
    bool setCpu(const char *cpu) override;

    bool isPrefix(Config::opcode_t opCode) const;

    static constexpr Config::opcode_t PREFIX_IX = 0xDD;
    static constexpr Config::opcode_t PREFIX_IY = 0xFD;

    typedef PrefixedEntryPage<Entry> EntryPage;
    typedef CpuBase<CpuType, EntryPage> Cpu;

private:
    const Cpu *_cpu;

    bool setCpu(CpuType cpuType);
};

extern TableZ80 TableZ80;

}  // namespace z80
}  // namespace libasm

#endif  // __TABLE_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
