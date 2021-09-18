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

#ifndef __DIS_MC6800_H__
#define __DIS_MC6800_H__

#include "config_mc6800.h"
#include "dis_base.h"
#include "insn_mc6800.h"
#include "reg_mc6800.h"
#include "table_mc6800.h"

namespace libasm {
namespace mc6800 {

class DisMc6800 : public Disassembler, public Config {
public:
    DisMc6800() : Disassembler(_formatter, _regs, TableMc6800) {}

    AddressWidth addressWidth() const override { return TableMc6800.addressWidth(); }

private:
    MotoValueFormatter _formatter;
    RegMc6800 _regs;

    StrBuffer &outRegister(StrBuffer &out, RegName regName);

    Error decodeDirectPage(DisMemory &memory, InsnMc6800 &insn, StrBuffer &out);
    Error decodeExtended(DisMemory &memory, InsnMc6800 &insn, StrBuffer &out);
    Error decodeIndexed(DisMemory &memory, InsnMc6800 &insn, StrBuffer &out, AddrMode mode);
    Error decodeRelative(DisMemory &memory, InsnMc6800 &insn, StrBuffer &out);
    Error decodeImmediate(DisMemory &memory, InsnMc6800 &insn, StrBuffer &out);
    Error decodeBitNumber(DisMemory &memory, InsnMc6800 &insn, StrBuffer &out);
    Error decodeOperand(DisMemory &memory, InsnMc6800 &insn, StrBuffer &out, AddrMode mode);
    Error decode(DisMemory &memory, Insn &insn, StrBuffer &out) override;
};

}  // namespace mc6800
}  // namespace libasm

#endif  // __DIS_MC6800_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
