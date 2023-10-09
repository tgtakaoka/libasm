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

namespace libasm {
namespace z8 {

struct EntryInsn : EntryInsnBase<Config, Entry> {
    AddrMode dst() const { return flags().dst(); }
    AddrMode src() const { return flags().src(); }
    AddrMode ext() const { return flags().ext(); }
    PostFormat postFormat() const { return flags().postFormat(); }
    bool dstFirst() const { return flags().dstFirst(); }
    void setAddrMode(AddrMode dst, AddrMode src, AddrMode ext) {
        setFlags(Entry::Flags::create(dst, src, ext, ORDER_NONE, PF_NONE));
    }

    static bool operandInOpCode(Config::opcode_t opCode) {
        const Config::opcode_t low4 = opCode & 0xF;
        return low4 >= 0x8 && low4 < 0xF;
    }
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    uint8_t emitLength() const { return operandPos() + 1; }
    void emitInsn() { emitByte(opCode(), 0); }
    void emitOperand8(uint8_t val) { emitByte(val, operandPos()); }
    void emitOperand16(uint16_t val) { emitUint16(val, operandPos()); }
    void emitOperand16Le(uint16_t val) { emitUint16Le(val, operandPos()); }

private:
    uint8_t operandPos() const {
        uint8_t pos = length();
        return pos == 0 ? 1 : pos;
    }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory) : DisInsnImpl(insn, memory) {}

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
