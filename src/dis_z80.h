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

#ifndef __DIS_Z80_H__
#define __DIS_Z80_H__

#include "config_z80.h"
#include "dis_base.h"
#include "insn_z80.h"
#include "reg_z80.h"
#include "table_z80.h"

namespace libasm {
namespace z80 {

class DisZ80 : public Disassembler, public Config {
public:
    DisZ80() : Disassembler(_formatter, _regs, TableZ80, '$'), _formatter(), _regs() {}

    const ConfigBase &config() const override { return *this; }

private:
    IntelValueFormatter _formatter;
    RegZ80 _regs;

    StrBuffer &outIndirectAddr(StrBuffer &out, uint16_t addr, uint8_t bits);
    StrBuffer &outRegister(StrBuffer &out, RegName reg);
    StrBuffer &outIndirectReg(StrBuffer &out, RegName reg);
    StrBuffer &outIndexOffset(StrBuffer &out, RegName reg, int8_t offset);
    StrBuffer &outDataReg(StrBuffer &out, RegName reg);

    Error decodeIndexedBitOp(DisMemory &memory, InsnZ80 &insn, StrBuffer &out);
    Error decodeRelative(DisMemory &memory, InsnZ80 &insn, StrBuffer &out);
    Error decodeOperand(DisMemory &memory, InsnZ80 &insn, StrBuffer &out, AddrMode mode);
    Error decode(DisMemory &memory, Insn &insn, StrBuffer &out) override;
};

}  // namespace z80
}  // namespace libasm

#endif  // __DIS_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
