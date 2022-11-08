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

#ifndef __INSN_Z8000_H__
#define __INSN_Z8000_H__

#include "config_z8000.h"
#include "entry_z8000.h"
#include "insn_base.h"

namespace libasm {
namespace z8000 {

class InsnZ8000 : public InsnImpl<Config, Entry> {
public:
    InsnZ8000(Insn &insn) : InsnImpl(insn) {}

    OprSize size() const { return flags().size(); }
    AddrMode dst() const { return flags().dst(); }
    AddrMode src() const { return flags().src(); }
    AddrMode ex1() const { return flags().ex1(); }
    AddrMode ex2() const { return flags().ex2(); }
    PostFormat postFormat() const { return flags().postFormat(); }
    uint8_t postMask() const { return flags().postMask(); }
    uint8_t postVal() const { return flags().postVal(); }
    ModeField dstField() const { return flags().dstField(); }
    ModeField srcField() const { return flags().srcField(); }
    void setAddrMode(AddrMode dst, AddrMode src, AddrMode ex1, AddrMode ex2) {
        setFlags(Entry::Flags::create(dst, src, ex1, ex2));
    }

    void setMemory(DisMemory &memory) { _memory = &memory; }
    void readPost() { setPost(readUint16(*_memory)); }
    void readPost(DisMemory &memory) { setPost(readUint16(memory)); }

    void emitInsn() {
        emitUint16(opCode(), 0);
        const PostFormat format = postFormat();
        if (format == PF_0XX8)
            embedPost(0x8);
        if (format == PF_0XXE)
            embedPost(0xE);
        if (format != PF_NONE)
            emitUint16(post(), 2);
    }
    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }
    void emitOperand32(uint32_t val32) { emitUint32(val32, operandPos()); }

    bool isThreeRegsInsn() const {
        const uint8_t opc = opCode() >> 8;
        return opc == 0xB8 || opc == 0xBA || opc == 0xBB;
    }
    bool isTranslateInsn() const {
        const uint8_t opc = opCode() >> 8;
        return opc == 0xB8;
    }
    bool isLoadMultiInsn() const {
        const uint8_t opc = opCode() >> 8;
        return opc == 0x1C || opc == 0x5C;
    }
    bool isPushPopInsn() const {
        const uint8_t opc = (opCode() >> 8) & ~0xC0;
        return opc == 0x11 || opc == 0x13 || opc == 0x15 || opc == 0x17;
    }

private:
    DisMemory *_memory;

    uint8_t operandPos() {
        uint8_t pos = length();
        if (pos == 0)
            pos = postFormat() != PF_NONE ? 4 : 2;
        return pos;
    }
};

}  // namespace z8000
}  // namespace libasm

#endif  // __INSN_Z8000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
