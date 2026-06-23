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

namespace libasm {
namespace h8500 {

struct EntryInsn : EntryInsnPrefix<Config, Entry> {
    AddrMode src() const { return flags().src(); }
    AddrMode dst() const { return flags().dst(); }
    OprSize oprSize() const { return flags().oprSize(); }
    InsnSize insnSize() const { return flags().insnSize(); }
    InsnClass insnClass() const { return flags().insnClass(); }

    // The routing class of this instruction's leading byte(s): set during
    // decode (from the lead byte) and during table search (from the page).
    void setPrefixMode(PrefixMode pm) { _prefixMode = pm; }
    PrefixMode prefixMode() const { return _prefixMode; }

    // Operating mode, used to gate page-jump (maxMode-only) instructions: the
    // disassembler hides them in minimum mode.
    bool maxMode = false;

private:
    PrefixMode _prefixMode = PM_SPC;
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}

    uint8_t eaByte = 0;  // the EA prefix byte (0 if none)
    uint8_t opByte = 0;  // the opcode byte

    // EA operand resolved once from the EA byte + extension bytes, then emitted
    // by the M_EASRC/M_EADST arm of decodeOperand.
    AddrMode eaMode = M_NONE;
    RegName eaReg = REG_UNDEF;
    uint16_t eaVal = 0;
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
