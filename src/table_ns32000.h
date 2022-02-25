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
#include "table_base.h"

namespace libasm {
namespace ns32000 {

class TableNs32000 : public TableBase {
public:
    Error searchName(InsnNs32000 &insn) const;
    Error searchOpCode(InsnNs32000 &insn, DisMemory &memory) const;
    bool isPrefixCode(Config::opcode_t opCode) const;

    const /* PROGMEM */ char *listCpu_P() const override;
    const /* PROGMEM */ char *cpu_P() const override;
    bool setCpu(const char *cpu) override;
    void setFpu(FpuType fpuType) { _fpuType = fpuType; }
    FpuType getFpu() const { return _fpuType; }
    void setMmu(MmuType mmuType) { _mmuType = mmuType; }
    MmuType getMmu() const { return _mmuType; }

    struct EntryPage;

private:
    FpuType _fpuType;
    MmuType _mmuType;

    Error searchName(InsnNs32000 &insn, const EntryPage *pages, const EntryPage *end) const;
    Error searchOpCode(InsnNs32000 &insn, DisMemory &memory, const EntryPage *pages,
            const EntryPage *end) const;
};

extern TableNs32000 TableNs32000;

}  // namespace ns32000
}  // namespace libasm

#endif  // __TABLE_NS32000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
