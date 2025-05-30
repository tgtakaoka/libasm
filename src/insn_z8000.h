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

#ifndef __LIBASM_INSN_Z8000_H__
#define __LIBASM_INSN_Z8000_H__

#include "config_z8000.h"
#include "entry_z8000.h"
#include "insn_base.h"
#include "reg_z8000.h"
#include "str_scanner.h"
#include "value.h"

namespace libasm {
namespace z8000 {

struct EntryInsn : EntryInsnPostfix<Config, Entry> {
    OprSize size() const { return flags().size(); }
    AddrMode dst() const { return flags().dst(); }
    AddrMode src() const { return flags().src(); }
    AddrMode ex1() const { return flags().ex1(); }
    AddrMode ex2() const { return flags().ex2(); }
    PostFormat postFormat() const { return flags().postFormat(); }
    OprPos dstPos() const { return flags().dstPos(); }
    OprPos srcPos() const { return flags().srcPos(); }

    bool isThreeRegsInsn() const {
        const auto opc = opCode() >> 8;
        return opc == 0xB8 || opc == 0xBA || opc == 0xBB;
    }
    bool isTranslateInsn() const {
        const auto opc = opCode() >> 8;
        return opc == 0xB8;
    }
    bool isLoadMultiInsn() const {
        const auto opc = opCode() & 0x3F01;
        return opc == 0x1C01 || opc == 0x5C01;
    }
    bool isPushPopInsn() const {
        const auto opc = (opCode() >> 8) & ~0xC0;
        return opc == 0x11 || opc == 0x13 || opc == 0x15 || opc == 0x17;
    }
};

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;   // M_R/M_IR/M_X/M_BX/M_CTL
    RegName base;  // M_BA/M_BX
    CcName cc;     // M_CC/M_DA/M_X
    Value val;     // M_IM/M_DA/M_X/M_BA/M_INTT/M_FLAG
    StrScanner baseAt;
    Operand() : mode(M_NONE), reg(REG_UNDEF), base(REG_UNDEF), cc(CC_UNDEF), val(), baseAt() {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    Operand dstOp, srcOp, ex1Op, ex2Op;

    void emitInsn() {
        emitUint16(opCode(), 0);
        const auto format = postFormat();
        if (format == PF_0XX8)
            embedPostfix(0x8);
        if (format == PF_0XXE)
            embedPostfix(0xE);
        if (format != PF_NONE)
            emitUint16(postfix(), 2);
    }
    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }
    void emitOperand32(uint32_t val32) { emitUint32(val32, operandPos()); }

private:
    uint_fast8_t operandPos() {
        auto pos = length();
        if (pos == 0)
            pos = postFormat() != PF_NONE ? 4 : 2;
        return pos;
    }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out)
        : DisInsnImpl(insn, memory, out), _checkOverlap(0) {}

    void readPostfix() {
        if (!hasPostfix())
            setPostfix(readUint16());
    }

    void setCheckRegisterOverlap(uint8_t pass) { _checkOverlap = pass; }
    uint8_t isCheckRegisterOverlap() const { return _checkOverlap; }

private:
    uint8_t _checkOverlap;
};

}  // namespace z8000
}  // namespace libasm

#endif  // __LIBASM_INSN_Z8000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
