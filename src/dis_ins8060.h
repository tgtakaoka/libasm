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

#ifndef __DIS_INS8060_H__
#define __DIS_INS8060_H__

#include "config_ins8060.h"
#include "dis_base.h"
#include "insn_ins8060.h"
#include "reg_ins8060.h"
#include "table_ins8060.h"

namespace libasm {
namespace ins8060 {

class DisIns8060
    : public Disassembler,
      public Config {
public:
    ValueFormatter &getFormatter() override { return _formatter; }

private:
    ValueFormatter _formatter;
    RegIns8060 _regs;

    TableBase &getTable() const override { return TableIns8060; }
    RegBase &getRegister() override { return _regs; }

    void outRegister(RegName regName);

    Error decodePntr(InsnIns8060 &insn);
    Error decodeImm8(DisMemory &memory, InsnIns8060 &insn);
    Error decodeIndx(DisMemory &memory, InsnIns8060 &insn, bool hasMode);
    Error decode(DisMemory &memory, Insn &insn) override;
};

} // namespace ins8060
} // namespace libasm

#endif // __DIS_INS8060_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
