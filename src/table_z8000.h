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

#ifndef __TABLE_Z8000_H__
#define __TABLE_Z8000_H__

#include "config_z8000.h"
#include "insn_z8000.h"

namespace libasm {
namespace z8000 {

struct TableZ8000 final : InsnTable<CpuType> {
    const /*PROGMEM*/ char *listCpu_P() const override;
    const /*PROGMEM*/ char *cpuName_P(CpuType cpuType) const override;
    Error searchCpuName(StrScanner &name, CpuType &cpuType) const override;

    Error searchName(CpuType, AsmInsn &insn) const;
    Error searchOpCode(CpuType, DisInsn &insn, StrBuffer &out) const;
    Error searchOpCodeAlias(CpuType, DisInsn &insn, StrBuffer &out) const;
};

extern const TableZ8000 TABLE;

}  // namespace z8000
}  // namespace libasm

#endif  // __TABLE_Z8000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
