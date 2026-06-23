/*
 * Copyright 2025 Tadashi G. Takaoka
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

#ifndef __LIBASM_TABLE_H8500_H__
#define __LIBASM_TABLE_H8500_H__

#include "config_h8500.h"
#include "insn_h8500.h"

namespace libasm {
namespace h8500 {

struct TableH8500 final : InsnTable<CpuType> {
    const /*PROGMEM*/ char *listCpu_P() const override;
    const /*PROGMEM*/ char *cpuName_P(CpuType cpuType) const override;
    Error searchCpuName(StrScanner &name, CpuType &cpuType) const override;
};

extern const TableH8500 TABLE;

// If |b| is an EA prefix byte for |cpuType|, set |mode| to the resolved EA
// AddrMode and return the extension-byte count (0..2); otherwise return -1.
int classifyLead(CpuType cpuType, uint8_t b, AddrMode &mode);

// True if |code| is a secondary-format prefix byte (0x11).
bool isPrefix(CpuType cpuType, Config::opcode_t code);

// Search all CPU pages by OP byte for disassembly.
// insn.prefixMode(), insn.opCode() (and insn.eaByte/eaMode for GEN/EXT, or
// insn.prefix() for SEC) must be set before calling.
Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out);

}  // namespace h8500
}  // namespace libasm

#endif  // __LIBASM_TABLE_H8500_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
