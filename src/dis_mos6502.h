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

#ifndef __DIS_MOS6502_H__
#define __DIS_MOS6502_H__

#include "config_mos6502.h"
#include "dis_base.h"
#include "insn_mos6502.h"

namespace libasm {
namespace mos6502 {

class DisMos6502 : public Disassembler, public Config {
public:
    DisMos6502();

    const ConfigBase &config() const override { return *this; }
    AddressWidth addressWidth() const override;
    void reset() override;

    Error setLongAccumulator(bool enable);
    Error setLongIndex(bool enable);
    Error setUseIndirectLong(bool enable);

private:
    MotorolaValueFormatter _formatter;
    const BoolOption<DisMos6502> _opt_longa;
    const BoolOption<DisMos6502> _opt_longi;
    const BoolOption<DisMos6502> _opt_indirectLong;

    Error decodeImmediate(DisMemory &memory, InsnMos6502 &insn, StrBuffer &out, AddrMode mode);
    Error decodeAbsoluteLong(DisMemory &memory, InsnMos6502 &insn, StrBuffer &out);
    Error decodeAbsolute(DisMemory &memory, InsnMos6502 &insn, StrBuffer &out);
    Error decodeDirectPage(DisMemory &memory, InsnMos6502 &insn, StrBuffer &out);
    Error decodeRelative(DisMemory &memory, InsnMos6502 &insn, StrBuffer &out, AddrMode mode);
    Error decodeBlockMove(DisMemory &memory, InsnMos6502 &insn, StrBuffer &out);
    Error decodeOperand(DisMemory &memory, InsnMos6502 &insn, StrBuffer &out, AddrMode mode);
    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;
};

}  // namespace mos6502
}  // namespace libasm

#endif  // __DIS_MOS6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
