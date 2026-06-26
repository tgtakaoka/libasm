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
    bool longForm() const { return flags().longForm(); }
    InsnSize insnSize() const { return flags().insnSize(); }
};

struct Operand final : ErrorAt {
    AddrMode mode;  // parsed addressing mode
    RegName reg;    // primary register (Rn or Rm)
    Value val;      // displacement / immediate / branch target
    Operand() : mode(M_NONE), reg(REG_UNDEF), val() {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn), _opCode2(0) {}

    Operand srcOp, dstOp;

    // Strip a trailing .B/.W/.L size suffix off the mnemonic (in place) and
    // record it. A non-size dot (FMOV.S) or trailing garbage is left in the
    // name for the table lookup to resolve. '/'-suffix names (CMP/EQ) have no
    // '.' and pass through untouched.
    InsnSize parseInsnSize();
    InsnSize parsedSize() const { return _parsedSize; }

    // Effective access size: ISZ_DATA entries carry it in the opcode (resolved
    // once the size field has been inserted), fixed entries carry it directly.
    InsnSize effectiveSize() const {
        return insnSize() == ISZ_DATA ? decodeDataSize(src(), dst(), opCode()) : insnSize();
    }

    // Second 16-bit word for SH-2A 32-bit instructions (MOVI20, MOV with
    // 12-bit displacement, bit-manipulation memory forms, etc.).
    Config::opcode_t opCode2() const { return _opCode2; }
    void setOpCode2(Config::opcode_t opc) { _opCode2 = opc; }

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
    // Long-form (SH-2A 32-bit) instructions emit a second 16-bit word too.
    void emitInsn() {
        emitUint16Be(opCode(), 0);
        if (longForm())
            emitUint16Be(_opCode2, 2);
    }

private:
    Config::opcode_t _opCode2;
    InsnSize _parsedSize{ISZ_NONE};
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out)
        : DisInsnImpl(insn, memory, out), _opCode2(0) {}

    Config::opcode_t opCode2() const { return _opCode2; }
    void setOpCode2(Config::opcode_t opc) { _opCode2 = opc; }

    // Effective access size: resolve ISZ_DATA from the opcode bits, otherwise
    // the entry's fixed size. Drives the disassembly suffix and disp scaling.
    InsnSize effectiveSize() const {
        return insnSize() == ISZ_DATA ? decodeDataSize(src(), dst(), opCode()) : insnSize();
    }

private:
    Config::opcode_t _opCode2;
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
