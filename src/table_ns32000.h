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
    Error searchName(InsnNs32000 &insn) const;
    Error searchOpCode(InsnNs32000 &insn, DisMemory &memory) const;
    bool isPrefixCode(uint8_t opCode) const;

    const /* PROGMEM */ char *listCpu_P() const override;
    const /* PROGMEM */ char *cpu_P() const override { return _cpu->name_P(); }
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

    template <typename CPUTYPE_T>
    struct CpuCommon : CpuBase<CPUTYPE_T, EntryPage> {
        constexpr CpuCommon(CPUTYPE_T cpuType, const /* PROGMEM */ char *name_P,
                const EntryPage *table, const EntryPage *end)
            : CpuBase<CPUTYPE_T, EntryPage>(cpuType, name_P, table, end) {}

        Error searchNameCommon(InsnNs32000 &insn, bool (*accept)(InsnNs32000 &, const Entry *),
                void (*pageSetup)(InsnNs32000 &, const EntryPage *)) const {
            this->searchName(insn, accept, pageSetup);
            return insn.getError();
        }

        Error searchOpCodeCommon(InsnNs32000 &insn, DisMemory &memory,
                bool (*matchOpCode)(InsnNs32000 &, const Entry *, const EntryPage *),
                void (*readEntryName)(InsnNs32000 &, const Entry *, const EntryPage *)) const {
            auto entry = this->searchOpCode(insn, matchOpCode, readEntryName);
            if (entry && insn.hasPost())
                insn.readPost(memory);
            return insn.getError();
        }
    };
    typedef CpuCommon<CpuType> Cpu;
    typedef CpuCommon<FpuType> Fpu;
    typedef CpuCommon<MmuType> Mmu;

private:
    const Cpu *const _cpu;
    const Fpu *_fpu;
    const Mmu *_mmu;

    bool setFpu(FpuType fpuType);
    bool setMmu(MmuType mmuType);
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
