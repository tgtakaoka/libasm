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

#ifndef __LIBASM_INSN_I8048_H__
#define __LIBASM_INSN_I8048_H__

#include "config_i8048.h"
#include "entry_i8048.h"
#include "insn_base.h"
#include "reg_i8048.h"

namespace libasm {
namespace i8048 {

struct EntryInsn : EntryInsnBase<Config, Entry> {
    AddrMode dst() const { return flags().dst(); }
    AddrMode src() const { return flags().src(); }
};

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    uint16_t val16;
    Operand() : mode(M_NONE), reg(REG_UNDEF), val16(0) {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    Operand dstOp, srcOp;

    void emitOperand8(uint8_t val) { emitByte(val, 1); }
    void emitInsn() { emitByte(opCode(), 0); }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}
};

}  // namespace i8048
}  // namespace libasm

#endif  // __INSN_I8048_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
