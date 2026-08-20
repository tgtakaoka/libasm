/*
 * Copyright 2026 Tadashi G. Takaoka
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

#ifndef __LIBASM_TABLE_AM29000_H__
#define __LIBASM_TABLE_AM29000_H__

#include "config_am29000.h"
#include "insn_am29000.h"

namespace libasm {
namespace am29000 {

struct TableAm29000 final : InsnTable<CpuType> {
    const /*PROGMEM*/ char *listCpu_P() const override;
    const /*PROGMEM*/ char *cpuName_P(CpuType cpuType) const override;
    Error searchCpuName(StrScanner &name, CpuType &cpuType) const override;
};

extern const TableAm29000 TABLE;

// |fpu| and |intMul| tell whether the floating-point and integer-multiply
// instructions may be used; they follow the CPU by default but either may be
// turned on for a system whose trap handlers provide the arithmetic.
Error searchOpCode(CpuType, bool fpu, bool intMul, DisInsn &insn, StrBuffer &out);

}  // namespace am29000
}  // namespace libasm

#endif  // __LIBASM_TABLE_AM29000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
