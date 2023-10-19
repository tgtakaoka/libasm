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

#ifndef __INSN_TLCS90_H__
#define __INSN_TLCS90_H__

#include "config_tlcs90.h"
#include "entry_tlcs90.h"
#include "insn_base.h"
#include "reg_tlcs90.h"

namespace libasm {
namespace tlcs90 {

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    CcName cc;
    uint16_t val16;
    Operand() : ErrorAt(), mode(M_NONE), reg(REG_UNDEF), cc(CC_UNDEF), val16(0) {}
};

struct EntryInsn : EntryInsnBase<Config, Entry> {
    AddrMode dst() const { return flags().dst(); }
    AddrMode src() const { return flags().src(); }
    void setAddrMode(AddrMode dst, AddrMode src) { setFlags(Entry::Flags::create(dst, src)); }
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    AddrMode pre() const { return _prefixMode; }
    void setPrefixMode(AddrMode mode) { _prefixMode = mode; }

    void setEmitInsn() { _emitInsn = true; }
    void emitInsn(Config::opcode_t opc) {
        if (_emitInsn) {
            emitByte(opc);
            _emitInsn = false;
        }
    }

private:
    AddrMode _prefixMode;
    bool _emitInsn;
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}

    Error readOpCode(Operand &op) {
        const auto prefix = opCode();
        switch (op.mode) {
        case M_EXT:
            op.val16 = readUint16();
            break;
        case M_DIR:
            op.val16 = readByte();
            break;
        case M_IND:
            op.reg = reg::decodeReg16(prefix);
            break;
        case M_IDX:
            op.reg = reg::decodeIndexReg(prefix);
            op.val16 = static_cast<int8_t>(readByte());
            break;
        case M_REG8:
            op.reg = reg::decodeReg8(prefix);
            break;
        case M_CC:
            op.cc = reg::decodeCcName(prefix);
            break;
        default:
            break;
        }
        setOpCode(readByte(), prefix);
        return getError();
    }
};

}  // namespace tlcs90
}  // namespace libasm

#endif  // __INSN_TLCS90_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
