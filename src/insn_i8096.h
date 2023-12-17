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

#ifndef __LIBASM_INSN_I8096_H__
#define __LIBASM_INSN_I8096_H__

#include "config_i8096.h"
#include "entry_i8096.h"
#include "insn_base.h"

namespace libasm {
namespace i8096 {

struct EntryInsn : EntryInsnBase<Config, Entry> {
    AaMode aa() const { return AaMode(opCode() & 3); }
    AddrMode dst() const { return flags().dst(); }
    AddrMode src1() const { return flags().src1(); }
    AddrMode src2() const { return flags().src2(); }
    void embedAa(AaMode aa) { embed(uint8_t(aa)); }
};

struct Operand final : ErrorAt {
    AddrMode mode;
    uint8_t regno;
    Error regerr;
    uint16_t val16;
    Operand() : mode(M_NONE), regno(0), regerr(OK), val16(0) {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    Operand dstOp, src1Op, src2Op;

    void emitInsn() {
        uint8_t pos = 0;
        if (hasPrefix())
            emitByte(prefix(), pos++);
        emitByte(opCode(), pos);
    }
    void emitOperand8(uint8_t val8) { emitByte(val8, operandPos()); }
    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }

private:
    uint8_t operandPos() {
        uint8_t pos = length();
        if (pos == 0)
            pos = hasPrefix() ? 2 : 1;
        return pos;
    }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}
};

}  // namespace i8096
}  // namespace libasm

#endif  // __INSN_I8096_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
