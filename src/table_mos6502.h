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

#ifndef __LIBASM_TABLE_MOS6502_H__
#define __LIBASM_TABLE_MOS6502_H__

#include "config_mos6502.h"
#include "insn_mos6502.h"

namespace libasm {
namespace mos6502 {

struct TableMos6502 final : InsnTable<CpuType> {
    const /*PROGMEM*/ char *listCpu_P() const override;
    const /*PROGMEM*/ char *cpuName_P(CpuType cpuType) const override;
    Error searchCpuName(StrScanner &name, CpuType &cpuType) const override;

    static constexpr Config::opcode_t WDM = 0x42;
    static constexpr Config::opcode_t JSL = 0x22;
};

extern const TableMos6502 TABLE;

bool hasOperand(CpuType, AsmInsn &insn);
Error searchName(CpuType, AsmInsn &insn);
Error searchOpCode(CpuType, DisInsn &insn, StrBuffer &out);

}  // namespace mos6502
}  // namespace libasm

#endif  // __LIBASM_TABLE_MOS6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
