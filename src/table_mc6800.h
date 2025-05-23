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

#ifndef __LIBASM_TABLE_MC6800_H__
#define __LIBASM_TABLE_MC6800_H__

#include "config_mc6800.h"
#include "insn_mc6800.h"

namespace libasm {
namespace mc6800 {

struct TableMc6800 final : InsnTable<CpuType> {
    const /*PROGMEM*/ char *listCpu_P() const override;
    const /*PROGMEM*/ char *cpuName_P(CpuType cpuType) const override;
    Error searchCpuName(StrScanner &name, CpuType &cpuType) const override;
};

extern const TableMc6800 TABLE;

bool hasOperand(CpuType, AsmInsn &insn);
Error searchName(CpuType, AsmInsn &insn);
Error searchOpCode(CpuType, DisInsn &insn, StrBuffer &out);
Error searchOpCodeAlias(CpuType, DisInsn &insn, StrBuffer &out);
bool isPrefix(CpuType, Config::opcode_t code);

}  // namespace mc6800
}  // namespace libasm

#endif  // __LIBASM_TABLE_MC6800_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
