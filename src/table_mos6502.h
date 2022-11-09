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
    TableMos6502() { reset(); }
    void reset();

    static TableMos6502 TABLE;

    Error searchName(InsnMos6502 &insn) const;
    Error searchOpCode(InsnMos6502 &insn) const;
    bool useIndirectLong(bool enable);
    bool longAccumulator() const { return _longAccumulator; }
    bool setLongAccumulator(bool on);
    bool longIndex() const { return _longIndex; }
    bool setLongIndex(bool on);
    bool longImmediate(AddrMode mode) const;

    const /* PROGMEM */ char *listCpu_P() const override;
    const /* PROGMEM */ char *cpu_P() const override { return _cpu->name_P(); }
    CpuType cpuType() const;
    bool setCpu(const char *cpu) override;
    AddressWidth addressWidth() const;

    static constexpr Config::opcode_t WDM = 0x42;
    static constexpr Config::opcode_t JSL = 0x22;

    typedef EntryPageBase<Entry> EntryPage;
    typedef CpuBase<CpuType, EntryPage> Cpu;

private:
    const Cpu *_cpu;
    bool _useIndirectLong;
    bool _longAccumulator;
    bool _longIndex;

    bool setCpu(CpuType cpuType);
};

}  // namespace mos6502
}  // namespace libasm

#endif  // __TABLE_MOS6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
