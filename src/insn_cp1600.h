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

#ifndef __LIBASM_INSN_CP1600_H__
#define __LIBASM_INSN_CP1600_H__

#include "config_cp1600.h"
#include "entry_cp1600.h"
#include "insn_base.h"
#include "reg_cp1600.h"
#include "value.h"

namespace libasm {
namespace cp1600 {

struct EntryInsn : EntryInsnPrefix<Config, Entry> {
    static constexpr Config::opcode_t SDBD = 0x0001;

    AddrMode src() const { return flags().src(); }
    AddrMode dst() const { return flags().dst(); }
    bool isSdbd() const { return !hasPrefix() && opCode() == SDBD; }
    bool is_sdbdAware() const {
        constexpr auto MVI = 01200;
        constexpr auto XOR = 01777;
        return opCode() >= MVI && opCode() <= XOR && (src() == M_INDIR || src() == M_IMM16);
    }
};

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    Value val;
    Operand() : mode(M_NONE), reg(REG_UNDEF), val() {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    Operand srcOp, dstOp;

    // Position of the first operand word.  For prefix-table entries (Jump
    // family) the prefix occupies position 0..1 and the opcode 2..3, so
    // operands start at 4; otherwise the opcode occupies 0..1 and operands
    // start at 2.  Lets emitOperand16 be called before emitInsn.
    uint8_t operandPos() const { return hasPrefix() ? 4 : 2; }

    // Writes prefix (when present) and opcode at the head of the buffer.
    void emitInsn() {
        if (hasPrefix()) {
            emitUint16(prefix(), 0);
            emitUint16(opCode(), 2);
        } else {
            emitUint16(opCode(), 0);
        }
    }
    // Appends an operand word at operandPos(), or at length() if some operands
    // have already been emitted.  Order of emitInsn vs emitOperand16 does not
    // matter -- the header slot is always preserved for the prefix/opcode.
    void emitOperand16(uint16_t val) {
        const auto pos = length() < operandPos() ? operandPos() : length();
        emitUint16(val, pos);
    }
    void emitSdbd() { emitUint16(0x0001, 0); }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out)
        : DisInsnImpl(insn, memory, out), sdbd_prefix(false) {}

    // True if the previous decode emitted SDBD and this is the continuation;
    // makes M_IMM16 read two words (low byte / high byte) instead of one.
    // Set by DisCp1600::decodeImpl from its own _sdbdPrefix state.
    bool sdbd_prefix;
};

}  // namespace cp1600
}  // namespace libasm

#endif  // __LIBASM_INSN_CP1600_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:

        
