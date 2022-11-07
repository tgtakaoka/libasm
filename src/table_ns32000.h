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

#ifndef __TABLE_NS32000_H__
#define __TABLE_NS32000_H__

#include "config_ns32000.h"
#include "dis_memory.h"
#include "insn_ns32000.h"
#include "str_scanner.h"
#include "table_base.h"

namespace libasm {
namespace ns32000 {

class TableNs32000 : public TableBase {
public:
    TableNs32000();

    static TableNs32000 TABLE;

    void reset();
    Error searchName(InsnNs32000 &insn);
    Error searchOpCode(InsnNs32000 &insn, DisMemory &memory);
    bool isPrefixCode(uint8_t opCode) const;

    const /* PROGMEM */ char *listCpu_P() const override;
    const /* PROGMEM */ char *cpu_P() const override;
    bool setCpu(const char *cpu) override;
    bool setFpu(StrScanner fpu);
    bool setMmu(StrScanner mmu);
    FpuType getFpu() const { return _fpu->cpuType(); }
    MmuType getMmu() const { return _mmu->cpuType(); }

    struct EntryPage : EntryPageBase<Entry> {
        constexpr EntryPage(Config::opcode_t prefix, Config::opcode_t mask, uint8_t post,
                const Entry *table, const Entry *end, const uint8_t *index, const uint8_t *iend)
            : EntryPageBase(prefix, table, end, index, iend), _mask(mask), _post(post) {}

        Config::opcode_t mask() const;
        Config::opcode_t post() const;

    private:
        Config::opcode_t _mask;
        uint8_t _post;
    };
    typedef CpuBase<CpuType, EntryPage> Cpu;
    typedef CpuBase<FpuType, EntryPage> Fpu;
    typedef CpuBase<MmuType, EntryPage> Mmu;

private:
    const Cpu *const _cpu;
    const Fpu *_fpu;
    const Mmu *_mmu;

    bool setFpu(FpuType fpuType);
    bool setMmu(MmuType mmuType);
    Error searchName(InsnNs32000 &insn, const EntryPage *pages, const EntryPage *end) const;
    Error searchOpCode(InsnNs32000 &insn, DisMemory &memory, const EntryPage *pages,
            const EntryPage *end) const;
};

}  // namespace ns32000
}  // namespace libasm

#endif  // __TABLE_NS32000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
