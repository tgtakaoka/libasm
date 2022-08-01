/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __DIS_SCN2650_H__
#define __DIS_SCN2650_H__

#include "config_scn2650.h"
#include "dis_base.h"
#include "insn_scn2650.h"
#include "reg_scn2650.h"
#include "table_scn2650.h"

namespace libasm {
namespace scn2650 {

class DisScn2650 : public Disassembler, public Config {
public:
    DisScn2650() : Disassembler(_formatter, _regs, TableScn2650::TABLE, '$'), _formatter(), _regs() {}

    const ConfigBase &config() const override { return *this; }

private:
    IntelValueFormatter _formatter;
    RegScn2650 _regs;

    Error decodeAbsolute(DisMemory &memory, InsnScn2650 &insn, StrBuffer &out, AddrMode mode);
    Error decodeIndexed(DisMemory &memory, InsnScn2650 &insn, StrBuffer &out, AddrMode mode);
    Error decodeRelative(DisMemory &memory, InsnScn2650 &insn, StrBuffer &out, AddrMode mode);
    Error decodeOperand(DisMemory &memory, InsnScn2650 &insn, StrBuffer &out, const AddrMode mode);
    Error decode(DisMemory &memory, Insn &insn, StrBuffer &out) override;
};

}  // namespace scn2650
}  // namespace libasm

#endif  // __DIS_SCN2650_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4: