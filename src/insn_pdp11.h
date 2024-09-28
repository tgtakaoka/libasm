/*
 * Copyright 2024 Tadashi G. Takaoka
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

#ifndef __LIBASM_INSN_PDP11_H__
#define __LIBASM_INSN_PDP11_H__

#include "config_pdp11.h"
#include "entry_pdp11.h"
#include "insn_base.h"
#include "reg_pdp11.h"
#include "value.h"

namespace libasm {
namespace pdp11 {

struct EntryInsn : EntryInsnPostfix<Config, Entry> {
    AddrMode src() const { return flags().src(); }
    AddrMode dst() const { return flags().dst(); }
    OprPos srcPos() const { return flags().srcPos(); }
    OprPos dstPos() const { return flags().dstPos(); }
    OprSize size() const { return flags().size(); }
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

    void embed(Config::opcode_t data, OprPos pos);
    void emitInsn() { emitUint16(opCode(), 0); }

    uint_fast8_t operandPos() const;
    Error emitOperand16(uint16_t u16) { return emitUint16(u16, operandPos()); }
    Error emitOperand32(uint32_t u32, uint8_t pos);
    Error emitOperand64(uint64_t u64, uint8_t pos);
#if !defined(LIBASM_ASM_NOFLOAT) && !defined(LIBASM_MC68000_NOFPU)
    Error emitDecFloat32(const float80_t &val80) { return emitDecFloat32(val80, operandPos()); }
    Error emitDecFloat64(const float80_t &val80) { return emitDecFloat64(val80, operandPos()); }
    Error emitDecFloat32(const float80_t &val80, uint8_t pos);
    Error emitDecFloat64(const float80_t &val80, uint8_t pos);
#endif
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}

    uint32_t readUint32Mix();
    uint64_t readUint64Mix();
#if !defined(LIBASM_DIS_NOFLOAT)
    float80_t readDecFloat32();
    float80_t readDecFloat64();
#endif
};

}  // namespace pdp11
}  // namespace libasm

#endif  // __LIBASM_INSN_PDP11_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
