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
    AddrMode ex1() const { return flags().ex1(); }
    AddrMode ex2() const { return flags().ex2(); }
    OprPos srcPos() const { return flags().srcPos(); }
    OprPos dstPos() const { return flags().dstPos(); }
    OprPos ex1Pos() const { return flags().ex1Pos(); }
    OprPos ex2Pos() const { return flags().ex2Pos(); }
    OprSize oprSize() const { return flags().oprSize(); }
    bool hasPostVal() const { return flags().hasPostVal(); }
    Config::opcode_t postVal() const { return flags().postVal(); }

    Config::opcode_t post2;  // for CAS2
};

enum IndexScale : int8_t {
    SCALE_NONE = -1,
    SCALE_1 = 0,
    SCALE_2 = 1,
    SCALE_4 = 2,
    SCALE_8 = 3,
};

struct Indexing {
    StrScanner at;
    RegName reg;
    InsnSize size;  // index register size
    StrScanner scaleAt;
    IndexScale scale;
    bool post;
    explicit Indexing(RegName index = REG_UNDEF)
        : reg(index), size(ISZ_NONE), scale(SCALE_NONE), post(false) {}
    const char *str() const;
};

struct Displacement {
    bool none;
    StrScanner at;
    Value value;
    Displacement() : none(true) {}
    const char *str() const;
};

struct Addressing {
    StrScanner indirectAt;
    bool indirect;
    RegName base;
    Indexing index;
    Displacement disp;
    Displacement outer;
    InsnSize absSize;
    Addressing() : indirect(false), base(REG_UNDEF), absSize(ISZ_NONE) {}
    const char *str() const;
};

struct Operand final : ErrorAt {
    AddrMode mode;
    Value val;
    union {
        RegName reg;
        CntlReg creg;
#if !defined(LIBASM_MC68000_NOPMMU)
        PmmuReg preg;
#endif
    };
    RegName reg2;
    union {
        Addressing addr;
        Value val2;
    };
    StrScanner list;
    Operand() : mode(M_NONE), val(), reg(REG_UNDEF), reg2(REG_UNDEF), addr(), list() {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn), _isize(ISZ_NONE) {}

    Operand srcOp, dstOp, ex1Op;
#if !defined(LIBASM_MC68000_NOPMMU)
    Operand ex2Op;
#endif

    InsnSize parseInsnSize();
    InsnSize insnSize() const { return _isize; }
    void emitInsn() {
        emitUint16(opCode(), 0);
        if (hasPostVal())
            emitUint16(postfix(), 2);
    }
    Error emitOperand16(uint16_t val16) { return emitUint16(val16, operandPos()); }
    Error emitOperand32(uint32_t val32) { return emitUint32(val32, operandPos()); }
    Error emitOperand64(uint64_t val64) { return emitUint64(val64, operandPos()); }
#if !defined(LIBASM_ASM_NOFLOAT) && !defined(LIBASM_MC68000_NOFPU)
    Error emitFloat32(const float80_t &val80, uint_fast8_t pos) {
        return emitFloat32Be(val80, pos);
    }
    Error emitFloat64(const float80_t &val80, uint_fast8_t pos) {
        return emitFloat64Be(val80, pos);
    }
    Error emitExtendedReal(const float80_t &val80, uint_fast8_t pos);
    Error emitDecimalString(const float80_t &val80, uint_fast8_t pos);
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
};

#if !defined(LIBASM_MC68000_NOFPU)
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
#endif

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}
    InsnSize insnSize() const { return flags().insnSize(); }
    void appendName(StrBuffer &out, char c);

    void readPostfix() {
        if (!hasPostfix())
            setPostfix(readUint16());
    }

#if !defined(LIBASM_MC68000_NOFPU)
    ExtendedReal readExtendedReal();
    DecimalString readDecimalString();
#endif
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
