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

#ifndef __LIBASM_TABLE_H16_H__
#define __LIBASM_TABLE_H16_H__

#include "config_h16.h"
#include "insn_h16.h"

namespace libasm {
namespace h16 {

struct TableH16 final : InsnTable<CpuType> {
    const /*PROGMEM*/ char *listCpu_P() const override;
    const /*PROGMEM*/ char *cpuName_P(CpuType cpuType) const override;
    Error searchCpuName(StrScanner &name, CpuType &cpuType) const override;
};

extern const TableH16 TABLE;

// True if |code| is the first byte of a multi-byte (prefixed) instruction.
bool isPrefix(CpuType cpuType, Config::opcode_t code);

// Search by opcode for disassembly. insn.opCode() must be set.
Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out);

// Search by name + operand modes for assembly.
Error searchName(CpuType cpuType, AsmInsn &insn);

}  // namespace h16
}  // namespace libasm

#endif  // __LIBASM_TABLE_H16_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
