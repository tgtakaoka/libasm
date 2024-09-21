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

#ifndef __LIBASM_INSN_MC68000_H__
#define __LIBASM_INSN_MC68000_H__

#include "config_mc68000.h"
#include "entry_mc68000.h"
#include "insn_base.h"
#include "reg_mc68000.h"
#include "value.h"

namespace libasm {
namespace mc68000 {

struct EntryInsn : EntryInsnPostfix<Config, Entry> {
    AddrMode src() const { return flags().src(); }
    AddrMode dst() const { return flags().dst(); }
    OprPos srcPos() const { return flags().srcPos(); }
    OprPos dstPos() const { return flags().dstPos(); }
    OprSize oprSize() const { return flags().oprSize(); }
    bool hasPostVal() const { return flags().hasPostVal(); }
    Config::opcode_t postVal() const { return flags().postVal(); }
};

struct AsmInsn;
struct Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    RegName indexReg;
    InsnSize indexSize;
    Value val;
    StrScanner list;
    AddrMode kMode;
    union {
        int8_t kFact;
        RegName kDreg;
    };
    Operand()
        : mode(M_NONE),
          reg(REG_UNDEF),
          indexReg(REG_UNDEF),
          indexSize(ISZ_NONE),
          val(),
          list(),
          kMode(M_NONE) {}
    Config::uintptr_t offset(const AsmInsn &insn) const;
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn), _isize(ISZ_NONE) { parseInsnSize(); }

    Operand srcOp, dstOp;

    InsnSize insnSize() const { return _isize; }
    void emitInsn() {
        emitUint16(opCode(), 0);
        if (hasPostVal())
            emitUint16(postfix() | postVal(), 2);
    }
    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }
    void emitOperand32(uint32_t val32) { emitUint32(val32, operandPos()); }
    void emitOperand64(uint64_t val64) { emitUint64(val64, operandPos()); }
#ifndef LIBASM_ASM_NOFLOAT
    void emitExtendedReal(const float80_t &val80) { _insn.emitFloat96Be(val80, operandPos()); }
    void emitDecimalString(const float80_t &val80) { _insn.emitPackedBcd96Be(val80, operandPos()); }
#endif
    uint8_t operandPos() const {
        auto pos = length();
        if (pos == 0)
            pos = 2;
        if (hasPostVal() && pos < 4)
            pos = 4;
        return pos;
    }

private:
    InsnSize _isize;
    void parseInsnSize();
};

struct ExtendedReal {
    uint16_t tag;
    uint16_t pad;
    uint64_t sig;
    bool isValid() const;
};

struct DecimalString {
    uint16_t tag;
    uint16_t integ;
    uint64_t sig;
    bool isValid() const;
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}
    InsnSize insnSize() const { return flags().insnSize(); }

    void readPostfix() {
        if (!hasPostfix())
            setPostfix(readUint16());
    }

    ExtendedReal readExtendedReal();
    DecimalString readDecimalString();
};

}  // namespace mc68000
}  // namespace libasm

#endif  // __LIBASM_INSN_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
