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

#ifndef __LIBASM_INSN_H16_H__
#define __LIBASM_INSN_H16_H__

#include "config_h16.h"
#include "entry_h16.h"
#include "insn_base.h"
#include "reg_h16.h"
#include "value.h"

namespace libasm {
namespace h16 {

struct EntryInsn : EntryInsnPrefix<Config, Entry> {
    AddrMode src() const { return flags().src(); }
    AddrMode dst() const { return flags().dst(); }
    InsnSize insnSize() const { return flags().insnSize(); }

    // Set by encode/decode from the Sz field in the opcode byte.
    OprSize oprSize = SZ_NONE;
};

struct Operand final : ErrorAt {
    AddrMode mode;
    reg::RegName reg;
    reg::CrName creg;
    reg::CcName cc;
    Value val;
    Operand() : mode(M_NONE), reg(reg::REG_UNDEF), creg(reg::CR_UNDEF), cc(reg::CC_UNDEF), val() {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    Operand op1, op2;

    using AsmInsnImpl::emitByte;
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out)
        : DisInsnImpl(insn, memory, out) {}

    // Set by decodeEa when the source EA byte has bit 7 set (accumulator
    // form): destination is implicitly R0 and no EAd byte follows.
    bool accumulatorForm = false;

    // Pre-decoded CR register for ANDC/ORC/XORC/LDC/STC.  These ops have
    // byte order [opcode][CR-code][EA] but mnemonic order varies.  We
    // pre-read the CR byte before operand dispatch so M_CR just outputs it.
    reg::CrName crReg = reg::CR_UNDEF;
};

}  // namespace h16
}  // namespace libasm

#endif  // __LIBASM_INSN_H16_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
