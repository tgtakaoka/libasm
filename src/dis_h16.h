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

#ifndef __LIBASM_DIS_H16_H__
#define __LIBASM_DIS_H16_H__

#include "config_h16.h"
#include "dis_base.h"
#include "insn_h16.h"

namespace libasm {
namespace h16 {

// What an EA operand is allowed to be, by role:
//   EA_ANY  - any addressing mode (source operand)
//   EA_DEST - a writable destination: rejects immediate
//   EA_MEM  - a memory location (JMP/JSR target, STM/LDM operand): rejects
//             immediate AND register-direct (manual forbids Rn and #imm)
//   EA_PERI - a peripheral operand (MOVTP/MOVFP family): like EA_MEM but also
//             rejects auto-increment / auto-decrement
//   EA_BITF - a bit-field operand (BFEXT/BFINS/BFSCH): rejects immediate and
//             auto-inc/dec, but allows register-direct
enum EaKind : uint8_t { EA_ANY, EA_DEST, EA_MEM, EA_PERI, EA_BITF };

struct DisH16 final : Disassembler, Config {
    DisH16(const ValueFormatter::Plugins &plugins = defaultPlugins());

private:
    // Decode an EA byte plus any extension bytes implied by the EA code.
    // |bank| selects the register file for Rn fields: REG_R0 = global bank
    // (default), REG_CR0 / REG_PR0 when the EA was preceded by a <CRn> /
    // <PRn> prefix byte.  |kind| restricts which EAs are legal for the
    // operand's role (EA_DEST rejects immediate; EA_MEM also rejects Rn).
    void decodeEa(DisInsn &insn, StrBuffer &out, OprSize sz,
            reg::RegName bank = reg::REG_R0, EaKind kind = EA_ANY) const;

    // Output an index register reference "Xm[.L/.W][*Sf]" from an index
    // extension byte ([*|Sf|Xm], bit 7 = 0).  Xm is always the global bank
    // (Manual 4.5.2), regardless of any <CRn>/<PRn> prefix on the base.
    void outIndex(DisInsn &insn, StrBuffer &out, uint8_t ext, bool longIdx) const;

    // Decode a branch displacement at the given size.
    void decodeBranch(DisInsn &insn, StrBuffer &out, OprSize sz) const;

    // Decode one operand by AddrMode dispatch.
    void decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) const;

    // Append the .B/.W/.L size suffix to the mnemonic.
    void outSize(DisInsn &insn) const;

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) const override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
    static const ValueFormatter::Plugins &defaultPlugins();
};

}  // namespace h16
}  // namespace libasm

#endif  // __LIBASM_DIS_H16_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
