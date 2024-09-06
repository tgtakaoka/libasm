/*
 * Copyright 2020 Tadashi G. Takaoka
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

#ifndef __LIBASM_INSN_NS32000_H__
#define __LIBASM_INSN_NS32000_H__

#include "config_ns32000.h"
#include "entry_ns32000.h"
#include "insn_base.h"
#include "reg_ns32000.h"
#include "value.h"

namespace libasm {
namespace ns32000 {

struct EntryInsn : EntryInsnPrePostfix<Config, Entry> {
    AddrMode src() const { return flags().src(); }
    AddrMode dst() const { return flags().dst(); }
    AddrMode ex1() const { return flags().ex1(); }
    AddrMode ex2() const { return flags().ex2(); }
    OprPos srcPos() const { return flags().srcPos(); }
    OprPos dstPos() const { return flags().dstPos(); }
    OprPos ex1Pos() const { return flags().ex1Pos(); }
    OprPos ex2Pos() const { return flags().ex2Pos(); }
    OprSize size() const { return flags().size(); }
    OprSize srcSize() const { return srcDstSize(src(), ex1() == M_NONE && ex1Pos()); }
    OprSize dstSize() const { return srcDstSize(dst(), ex2() == M_NONE && ex2Pos()); }

private:
    OprSize srcDstSize(AddrMode mode, bool octaSize) const {
        const auto sz = size();
        if (octaSize)
            return SZ_OCTA;
        if (mode == M_FENR || mode == M_FENW)
            return sz;
        return sz == SZ_OCTA ? SZ_QUAD : sz;
    }
};

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    Value value;
    uint32_t val32;
    uint32_t disp2;
    RegName index;
    OprSize size;
    Operand()
        : mode(M_NONE),
          reg(REG_UNDEF),
          value(),
          val32(0),
          disp2(0),
          index(REG_UNDEF),
          size(SZ_NONE) {}
    uint32_t getUint32() const { return value.getUnsigned(); }
#ifndef LIBASM_ASM_NOFLOAT
    double getFloat() const { return value.getFloat(); }
#endif
};

struct AsmInsn final : public AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    Operand srcOp, dstOp, ex1Op, ex2Op;

    void emitInsn() {
        uint8_t pos = 0;
        if (hasPrefix())
            emitByte(prefix(), pos++);
        emitByte(opCode(), pos++);
        if (hasPostfix())
            emitByte(postfix(), pos);
    }

    void emitOperand8(uint8_t val8) { emitByte(val8, operandPos()); }
    void emitOperand16(uint16_t val16) { emitUint16Be(val16, operandPos()); }
    void emitOperand32(uint32_t val32) { emitUint32Be(val32, operandPos()); }
#ifndef LIBASM_ASM_NOFLOAT
    void emitOpFloat32(double float64) { emitFloat32Be(float64, operandPos()); }
    void emitOpFloat64(double float64) { emitFloat64Be(float64, operandPos()); }
#endif

private:
    uint8_t operandPos() {
        uint8_t pos = length();
        if (pos == 0) {
            if (hasPrefix())
                pos++;
            pos++;
            if (hasPostfix())
                pos++;
        }
        return pos;
    }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}

    uint8_t indexByte(OprPos pos) const { return pos == P_GEN1 ? _indexByte1 : _indexByte2; }
    void setIndexByte(uint8_t data, OprPos pos) {
        if (pos == P_GEN1)
            _indexByte1 = data;
        if (pos == P_GEN2)
            _indexByte2 = data;
    }

private:
    uint8_t _indexByte1;
    uint8_t _indexByte2;
};

}  // namespace ns32000
}  // namespace libasm

#endif  // __LIBASM_INSN_NS32000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
