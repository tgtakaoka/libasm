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

#ifndef __DIS_I8080_H__
#define __DIS_I8080_H__

#include "config_i8080.h"
#include "dis_base.h"
#include "insn_i8080.h"
#include "reg_i8080.h"
#include "table_i8080.h"

namespace libasm {
namespace i8080 {

class DisI8080 : public Disassembler, public Config {
public:
    DisI8080() : Disassembler(_formatter, _regs, TableI8080) {}

private:
    IntelValueFormatter _formatter;
    RegI8080 _regs;

    char *outRegister(char *out, RegName regName);

    Error decodeOperand(
            DisMemory &memory, InsnI8080 &insn, char *out, AddrMode mode);
    Error decode(DisMemory &memory, Insn &insn, char *out) override;
};

}  // namespace i8080
}  // namespace libasm

#endif  // __DIS_I8080_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
