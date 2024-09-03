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

#ifndef __LIBASM_INSN_MN1610_H__
#define __LIBASM_INSN_MN1610_H__

#include "config_mn1610.h"
#include "entry_mn1610.h"
#include "insn_base.h"
#include "reg_mn1610.h"
#include "value.h"

namespace libasm {
namespace mn1610 {

struct EntryInsn : EntryInsnBase<Config, Entry> {
    AddrMode mode1() const { return flags().mode1(); }
    AddrMode mode2() const { return flags().mode2(); }
    AddrMode mode3() const { return flags().mode3(); }
    AddrMode mode4() const { return flags().mode4(); }
};

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    CcName cc;
    Value val;
    Operand() : mode(M_NONE), reg(REG_UNDEF), cc(CC_UNDEF), val() {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    Operand op1, op2, op3, op4;

    void emitInsn() { emitUint16(opCode(), 0); }
    void emitOperand16(uint16_t val) { emitUint16(val, sizeof(Config::opcode_t)); }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}
};

}  // namespace mn1610
}  // namespace libasm

#endif  // __LIBASM_INSN_MN1610_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
