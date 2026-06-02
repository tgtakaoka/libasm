/*
 * Copyright 2026 Tadashi G. Takaoka
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

#ifndef __LIBASM_INSN_H8300_H__
#define __LIBASM_INSN_H8300_H__

#include "config_h8300.h"
#include "entry_h8300.h"
#include "insn_base.h"
#include "reg_h8300.h"
#include "value.h"

namespace libasm {
namespace h8300 {

struct EntryInsn : EntryInsnPrefix<Config, Entry> {
    AddrMode src() const { return flags().src(); }
    AddrMode dst() const { return flags().dst(); }
    OprPos srcPos() const { return flags().srcPos(); }
    OprPos dstPos() const { return flags().dstPos(); }
    InsnSize insnSize() const { return flags().insnSize(); }
    OprSize oprSize() const { return flags().oprSize(); }
    uint16_t codeMask() const { return flags().opCodeMask(); }

    AddrMode prefixMode;
    OprPos prefixPos;
};

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    Value val;
    int_fast8_t bitSuffix;
    Operand() : mode(M_NONE), reg(REG_UNDEF), val(), bitSuffix(0) {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn), sizeSuffix(SZ_NONE) {}

    Operand srcOp, dstOp;

    AddrMode dst() const { return dstPos() == POS_PRX ? prefixMode : EntryInsn::dst(); }

    OprSize parseSizeSuffix();

    void emitInsn();
    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }

    OprSize sizeSuffix;
    ;

private:
    uint_fast8_t operandPos() const;
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}

    AddrMode dst() const { return prefixMode == M_NONE ? EntryInsn::dst() : prefixMode; }
};

}  // namespace h8300
}  // namespace libasm

#endif  // __LIBASM_INSN_H8300_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
