/*
 * Copyright 2025 Tadashi G. Takaoka
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

#ifndef __LIBASM_INSN_TMS370_H__
#define __LIBASM_INSN_TMS370_H__

#include "config_tms370.h"
#include "entry_tms370.h"
#include "insn_base.h"
#include "reg_tms370.h"
#include "value.h"

namespace libasm {
namespace tms370 {

struct EntryInsn : EntryInsnPrefix<Config, Entry> {
    AddrMode src() const { return flags().src(); }
    AddrMode dst() const { return flags().dst(); }
    AddrMode ext() const { return flags().ext(); }
};

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    Value val;
    Operand() : mode(M_NONE), val() {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    Operand srcOp, dstOp, extOp;

    void emitInsn() {
        uint_fast8_t pos = 0;
        if (hasPrefix())
            emitByte(prefix(), pos++);
        emitByte(opCode(), pos);
    }
    void emitOperand8(uint8_t val8) { emitByte(val8, operandPos()); }
    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }

    uint_fast8_t operandPos() const {
        uint_fast8_t pos = length();
        if (pos == 0)
            pos = hasPrefix() ? 2 : 1;
        return pos;
    }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}
};

}  // namespace tms370
}  // namespace libasm

#endif  // __LIBASM_INSN_TMS370_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
