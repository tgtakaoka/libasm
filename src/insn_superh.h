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

#ifndef __LIBASM_INSN_SUPERH_H__
#define __LIBASM_INSN_SUPERH_H__

#include "config_superh.h"
#include "entry_superh.h"
#include "insn_base.h"
#include "reg_superh.h"
#include "value.h"

namespace libasm {
namespace superh {

struct EntryInsn : EntryInsnBase<Config, Entry> {
    AddrMode src() const { return flags().src(); }
    AddrMode dst() const { return flags().dst(); }
};

struct Operand final : ErrorAt {
    AddrMode mode;  // parsed addressing mode
    RegName reg;    // primary register (Rn or Rm)
    Value val;      // displacement / immediate / branch target
    Operand() : mode(M_NONE), reg(REG_UNDEF), val() {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    Operand srcOp, dstOp;

    // Offset of the first operand byte after the opcode header. SH operands
    // are normally folded into the 16-bit opcode itself (the encoder sets
    // register/displacement bits via OR), so emitOperandUint*() is rarely
    // called. Defining it anyway gives the encoder an order-free emission
    // contract: helpers may write operand data before or after emitInsn(),
    // and the header slot is always preserved.
    uint8_t operandPos() const { return 2; }

    void emitOperandUint8(uint8_t val) {
        const auto pos = length() < operandPos() ? operandPos() : length();
        emitByte(val, pos);
    }
    void emitOperandUint16Be(uint16_t val) {
        const auto pos = length() < operandPos() ? operandPos() : length();
        emitUint16Be(val, pos);
    }
    void emitOperandUint32Be(uint32_t val) {
        const auto pos = length() < operandPos() ? operandPos() : length();
        emitUint32Be(val, pos);
    }

    // Finalize the instruction. Call after all encodeOperand() invocations.
    void emitInsn() { emitUint16Be(opCode(), 0); }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}
};

}  // namespace superh
}  // namespace libasm

#endif  // __LIBASM_INSN_SUPERH_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
