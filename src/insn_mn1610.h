/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __INSN_MN1610_H__
#define __INSN_MN1610_H__

#include "config_mn1610.h"
#include "entry_mn1610.h"
#include "insn_base.h"

namespace libasm {
namespace mn1610 {

class InsnMn1610 : public InsnImpl<Config, Entry> {
public:
    InsnMn1610(Insn &insn) : InsnImpl(insn) {}

    AddrMode op1() const { return flags().op1(); }
    AddrMode op2() const { return flags().op2(); }
    AddrMode op3() const { return flags().op3(); }
    AddrMode op4() const { return flags().op4(); }
    void setAddrMode(AddrMode op1, AddrMode op2, AddrMode op3, AddrMode op4) {
        setFlags(Entry::Flags::create(op1, op2, op3, op4));
    }

    void emitInsn() { emitUint16(opCode(), 0); }
    void emitOperand16(uint16_t val) { emitUint16(val, sizeof(Config::opcode_t)); }
};

}  // namespace mn1610
}  // namespace libasm

#endif  // __INSN_MN1610_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4: