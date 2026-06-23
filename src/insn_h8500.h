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

    // Assembler operating mode + the maxMode-only flag of the page being
    // searched, used to gate page-jump instructions at assembly time.
    bool maxMode = false;
    bool currentPageMaxOnly = false;

private:
    PrefixMode _prefixMode = PM_SPC;
};

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    CrName creg;
    Value val;
    // Explicit absolute size suffix (8/16/24), or 0 when none was written. Only
    // a no-suffix (0) absolute may be re-formed to the shortest reachable
    // @aa:8/@aa:16 using the assumed page registers.
    uint8_t absSize = 0;
    Operand() : mode(M_NONE), reg(REG_UNDEF), creg(CR_UNDEF), val() {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    Operand op1, op2;
    // Operand size parsed from a .B/.W mnemonic suffix (SZ_NONE if absent).
    OprSize insnSz = SZ_NONE;
    // Instruction class parsed from a ":x" mnemonic suffix (IC_N if absent,
    // i.e. a bare stem whose optimal form acceptOperands chooses).
    InsnClass insnCls = IC_N;
    // EA-byte extension byte count (0/1/2). These sit between the EA prefix byte
    // and the OP byte, so the OP byte's position floats with the EA size.
    uint8_t eaExt = 0;

    using AsmInsnImpl::emitByte;

    // Byte layout by prefix mode (the EA byte is carried in prefix()):
    //   PM_SPC  [OP][operand bytes]
    //   PM_SEC  [11][OP][operand bytes]
    //   PM_GEN  [EA][ext 0-2][OP][operand bytes]
    //   PM_EXT  [EA][ext 0-2][00][OP][operand bytes]
    uint8_t opCodePos() const {
        switch (prefixMode()) {
        case PM_SEC:
            return 1;
        case PM_GEN:
            return 1 + eaExt;
        case PM_EXT:
            return 1 + eaExt + 1;
        default:
            return 0;  // PM_SPC
        }
    }
    uint8_t operandPos() const {
        const uint8_t pos = opCodePos() + 1;
        return length() < pos ? pos : length();
    }
    // Emit the EA byte / prefix + OP byte at their computed positions; the EA
    // extension (emitEaExt) and trailing operands (emitOperand*) are placed
    // around them by position, so call order does not matter.
    void emitInsn() {
        if (prefixMode() == PM_SEC) {
            emitByte(0x11, 0);
        } else if (prefixMode() == PM_GEN || prefixMode() == PM_EXT) {
            emitByte(static_cast<uint8_t>(prefix()), 0);
            if (prefixMode() == PM_EXT)
                emitByte(0x00, 1 + eaExt);
        }
        emitByte(opCode(), opCodePos());
    }
    Error emitOperandUint8(uint8_t v) { return emitByte(v, operandPos()); }
    Error emitOperandUint16(uint16_t v) { return emitUint16Be(v, operandPos()); }
    Error emitOperandUint24(uint32_t v) {
        const auto pos = operandPos();
        emitByte(static_cast<uint8_t>(v >> 16), pos);
        return emitUint16Be(static_cast<uint16_t>(v), pos + 1);
    }
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
