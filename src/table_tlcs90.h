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

#include "asm_base.h"
#include "config_tlcs90.h"
#include "entry_table.h"
#include "entry_tlcs90.h"
#include "reg_tlcs90.h"

namespace libasm {
namespace tlcs90 {

class InsnTlcs90;

struct Operand : public OperandBase {
    AddrMode mode;
    RegName reg;
    CcName cc;
    uint16_t val16;
    Operand() : mode(M_NONE), reg(REG_UNDEF), cc(CC_UNDEF), val16(0) {}
};

struct TableTlcs90 : entry::Table {
public:
    TableTlcs90();

    static TableTlcs90 TABLE;

    Error searchName(InsnTlcs90 &insn) const;
    Error searchOpCode(InsnTlcs90 &insn, StrBuffer &out) const;
    Error readInsn(DisMemory &memory, InsnTlcs90 &insn, Operand &op) const;

    const /* PROGMEM */ char *listCpu_P() const override;
    const /* PROGMEM */ char *cpu_P() const override;
    bool setCpu(const char *cpu) override;

    struct Cpu;

private:
    const Cpu *const _cpu;
};

}  // namespace tlcs90
}  // namespace libasm

#endif  // __TABLE_TLCS90_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
