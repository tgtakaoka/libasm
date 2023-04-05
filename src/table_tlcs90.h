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

class TableTlcs90 : public EntryTable {
public:
    TableTlcs90();

    static TableTlcs90 TABLE;

    Error searchName(InsnTlcs90 &insn) const;
    Error searchOpCode(InsnTlcs90 &insn) const;
    Error readInsn(DisMemory &memory, InsnTlcs90 &insn, Operand &op) const;

    const /* PROGMEM */ char *listCpu_P() const override;
    const /* PROGMEM */ char *cpu_P() const override { return _cpu->name_P(); }
    bool setCpu(const char *cpu) override;

    struct EntryPage : EntryTableBase<Entry> {
        constexpr EntryPage(Config::opcode_t prefix, AddrMode mode, const Entry *table,
                const Entry *end, const uint8_t *index, const uint8_t *iend)
            : EntryTableBase(prefix, table, end, index, iend), _mode(uint8_t(mode)) {}

        AddrMode mode() const;
        bool prefixMatch(Config::opcode_t code) const;

    private:
        uint8_t _mode;
    };

    typedef CpuBase<CpuType, EntryPage> Cpu;

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
