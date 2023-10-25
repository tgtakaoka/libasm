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

#ifndef __INSN_Z8_H__
#define __INSN_Z8_H__

#include "config_z8.h"
#include "entry_z8.h"
#include "insn_base.h"
#include "reg_z8.h"

namespace libasm {
namespace z8 {

struct EntryInsn : EntryInsnBase<Config, Entry> {
    AddrMode dst() const { return flags().dst(); }
    AddrMode src() const { return flags().src(); }
    AddrMode ext() const { return flags().ext(); }
    OprPos dstPos() const { return flags().dstPos(); }
    OprPos srcPos() const { return flags().srcPos(); }
    OprPos extPos() const { return flags().extPos(); }
    PostFormat postFormat() const { return flags().postFormat(); }
    Config::opcode_t postVal() const { return flags().postVal(); }
};

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    CcName cc;
    uint16_t val16;
    StrScanner regAt;
    Operand() : mode(M_NONE), reg(REG_UNDEF), cc(CC_UNDEF), val16(0), regAt() {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    Operand dstOp, srcOp, extOp;

    void emitInsn();
    void emitOperand(uint16_t val, OprPos pos);
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}

    void readPost() {
        if (length() < 2)
            setPost(readByte());
    }
};

}  // namespace z8
}  // namespace libasm

#endif  // __INSN_Z8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
