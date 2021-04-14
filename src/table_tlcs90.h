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

#ifndef __TABLE_TLCS90_H__
#define __TABLE_TLCS90_H__

#include "config_tlcs90.h"
#include "insn_tlcs90.h"
#include "reg_tlcs90.h"
#include "table_base.h"

namespace libasm {
namespace tlcs90 {

struct Operand : public ErrorReporter {
    AddrMode mode;
    RegName reg;
    CcName cc;
    uint16_t val16;
    Operand() : ErrorReporter(), mode(M_NO), reg(REG_UNDEF), cc(CC_UNDEF), val16(0) {}
};

class TableTlcs90 : public TableBase {
public:
    Error searchName(InsnTlcs90 &insn) const;
    Error searchOpCode(InsnTlcs90 &insn) const;

    const char *listCpu() const override;
    bool setCpu(const char *cpu) override;
    const char *getCpu() const override;

    Error readInsn(DisMemory &memory, InsnTlcs90 &insn, Operand &op) const;
    struct EntryPage;

private:
    Error searchName(InsnTlcs90 &insn, const EntryPage *pages, const EntryPage *end) const;
    Error searchOpCode(InsnTlcs90 &insn, const EntryPage *pages, const EntryPage *end) const;
};

extern TableTlcs90 TableTlcs90;

}  // namespace tlcs90
}  // namespace libasm

#endif  // __TABLE_TLCS90_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
