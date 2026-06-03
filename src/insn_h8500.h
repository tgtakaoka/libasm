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

#ifndef __LIBASM_INSN_H8500_H__
#define __LIBASM_INSN_H8500_H__

#include "config_h8500.h"
#include "entry_h8500.h"
#include "insn_base.h"
#include "reg_h8500.h"
#include "value.h"

namespace libasm {
namespace h8500 {

struct EntryInsn : EntryInsnBase<Config, Entry> {
    AddrMode src() const { return flags().src(); }
    AddrMode dst() const { return flags().dst(); }
    InsnFmt fmt() const { return flags().fmt(); }
    EaSz eaSz() const { return flags().eaSz(); }
};

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    CrName creg;
    CcName cc;
    Value val;
    Operand() : mode(M_NONE), reg(REG_UNDEF), creg(CR_UNDEF), cc(CC_UNDEF), val() {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    Operand op1, op2;

    using AsmInsnImpl::emitByte;
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out)
        : DisInsnImpl(insn, memory, out) {}

    uint8_t eaByte = 0;
    uint8_t opByte = 0;
    uint8_t eaExt1 = 0;        // first EA extension byte (buffered before OP byte)
    uint8_t eaExt2 = 0;        // second EA extension byte
    InsnFmt insnFmt = FMT_GEN; // set in decodeImpl before searchOpCode
};

}  // namespace h8500
}  // namespace libasm

#endif  // __LIBASM_INSN_H8500_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
