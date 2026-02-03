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

#ifndef __LIBASM_TABLE_I8086_H__
#define __LIBASM_TABLE_I8086_H__

#include "config_i8086.h"
#include "insn_i8086.h"
#include "reg_i8086.h"

namespace libasm {
namespace i8086 {

struct TableI8086 final : InsnTable<CpuType> {
    const /*PROGMEM*/ char *listCpu_P() const override;
    const /*PROGMEM*/ char *cpuName_P(CpuType cpuType) const override;
    Error searchCpuName(StrScanner &name, CpuType &cpuType) const override;
};

extern const TableI8086 TABLE;

Error searchName(const CpuSpec &, AsmInsn &insn);
Error searchOpCode(const CpuSpec &, DisInsn &insn, StrBuffer &out);
bool isPrefix(const CpuSpec &, uint16_t code);
bool isRepeatPrefix(const CpuSpec &, Config::opcode_t opcode);
bool isSegmentPrefix(Config::opcode_t opcode);
RegName overrideSeg(Config::opcode_t opcode, RegName defSeg = REG_UNDEF);
Config::opcode_t segOverridePrefix(RegName name);

}  // namespace i8086
}  // namespace libasm

#endif  // __LIBASM_TABLE_I8086_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
