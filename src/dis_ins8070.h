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

#ifndef __DIS_INS8070_H__
#define __DIS_INS8070_H__

#include "config_ins8070.h"
#include "dis_base.h"
#include "insn_ins8070.h"
#include "reg_ins8070.h"
#include "table_ins8070.h"

namespace libasm {
namespace ins8070 {

class DisIns8070
    : public Disassembler,
      public Config {
public:
    DisIns8070() : Disassembler(_formatter, _regs, TableIns8070) {}

    void reset() override { setImmediateSymbol(false); }
    void setImmediateSymbol(bool usualSharp) { _immSym = usualSharp; }

private:
    ValueFormatter _formatter{'$'};
    RegIns8070 _regs;
    bool _immSym = false;

    char *outRegister(char *out, RegName regName);
    char *outOperand(char *out, OprFormat opr, uint8_t value = 0);

    Error decodeImplied(InsnIns8070 &insn, char *out);
    Error decodeImmediate(DisMemory &memory, InsnIns8070 &insn, char *out);
    Error decodeAbsolute(DisMemory &memory, InsnIns8070 &insn, char *out);
    Error decodeDirect(DisMemory &memory, InsnIns8070 &insn, char *out);
    Error decodeRelative(DisMemory &memory, InsnIns8070 &insn, char *out);
    Error decodeGeneric(DisMemory &memory, InsnIns8070 &insn, char *out);
    Error decode(DisMemory &memory, Insn &insn, char *out) override;
};

} // namespace ins8070
} // namespace libasm

#endif // __DIS_INS8070_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
