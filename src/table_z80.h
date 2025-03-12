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

#ifndef __LIBASM_TABLE_Z80_H__
#define __LIBASM_TABLE_Z80_H__

#include "config_z80.h"
#include "insn_z80.h"

namespace libasm {
namespace z80 {

struct TableZ80 final : InsnTable<CpuType> {
    const /*PROGMEM*/ char *listCpu_P() const override;
    const /*PROGMEM*/ char *cpuName_P(CpuType cpuType) const override;
    Error searchCpuName(StrScanner &name, CpuType &cpuType) const override;

    static constexpr uint16_t EXT = 0xED;
    static constexpr uint16_t IX = 0xDD;
    static constexpr uint16_t IY = 0xFD;
    static constexpr uint16_t BIT = 0xCB;
    static constexpr uint16_t IXEXT = 0xDDED;
    static constexpr uint16_t IYEXT = 0xFDED;
    static constexpr uint16_t IXBIT = 0xDDCB;
    static constexpr uint16_t IYBIT = 0xFDCB;
};

extern const TableZ80 TABLE;

Error searchName(CpuType, AsmInsn &insn);
Error searchOpCode(CpuType, DisInsn &insn, StrBuffer &out);
bool isPrefix(CpuType, uint16_t code);

}  // namespace z80
}  // namespace libasm

#endif  // __LIBASM_TABLE_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
