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

#ifndef __DIS_TLCS90_H__
#define __DIS_TLCS90_H__

#include "config_tlcs90.h"
#include "dis_base.h"
#include "insn_tlcs90.h"
#include "reg_tlcs90.h"
#include "table_tlcs90.h"

namespace libasm {
namespace tlcs90 {

class DisTlcs90 : public Disassembler, public Config {
public:
    DisTlcs90() : Disassembler(_formatter, _regs, TableTlcs90::TABLE, '$'), _formatter(), _regs() {}

    const ConfigBase &config() const override { return *this; }

private:
    IntelValueFormatter _formatter;
    RegTlcs90 _regs;

    Error readOperand(DisMemory &memory, InsnTlcs90 &insn, AddrMode mode, Operand &op);
    Error decodeRelative(InsnTlcs90 &insn, StrBuffer &out, AddrMode mode, const Operand &op);
    Error decodeOperand(InsnTlcs90 &insn, StrBuffer &out, AddrMode mode, const Operand &op);
    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;
};

}  // namespace tlcs90
}  // namespace libasm

#endif  // __DIS_TLCS90_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
