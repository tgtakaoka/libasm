/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef __TABLE_IM6100_H__
#define __TABLE_IM6100_H__

#include "config_im6100.h"
#include "insn_im6100.h"
#include "str_buffer.h"

namespace libasm {
namespace im6100 {

struct TableIm6100 final : InsnTable<CpuType> {
    const /*PROGMEM*/ char *listCpu_P() const override;
    const /*PROGMEM*/ char *cpuName_P(CpuType cpuType) const override;
    Error searchCpuName(StrScanner &name, CpuType &cpuType) const override;

    Error searchName(CpuType, AsmInsn &insn) const;
    Error searchMicro(CpuType, AsmInsn &micro, AddrMode mode) const;
    Error searchOpCode(CpuType, DisInsn &insn, StrBuffer &out) const;
};

extern const TableIm6100 TABLE;

}  // namespace im6100
}  // namespace libasm

#endif  // __TABLE_IM6100_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4: