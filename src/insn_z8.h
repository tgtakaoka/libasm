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

class InsnZ8 : public InsnImpl<Config, Entry> {
public:
    InsnZ8(Insn &insn) : InsnImpl(insn) {}

    AddrMode dst() const { return flags().dst(); }
    AddrMode src() const { return flags().src(); }
    AddrMode ext() const { return flags().ext(); }
    PostFormat postFormat() const { return flags().postFormat(); }
    bool dstFirst() const { return flags().dstFirst(); }
    void setAddrMode(AddrMode dst, AddrMode src, AddrMode ext) {
        setFlags(Entry::Flags::create(dst, src, ext, ORDER_NONE, PF_NONE));
    }

    void setMemory(DisMemory &memory) { _memory = &memory; }
    void readPost() { setPost(readByte(*_memory)); }

    static bool operandInOpCode(Config::opcode_t opCode) {
        const Config::opcode_t low4 = opCode & 0xF;
        return low4 >= 0x8 && low4 < 0xF;
    }

    bool singleByteOpCode() const {
        const Config::opcode_t low4 = opCode() & 0xF;
        return low4 == 0x0E || low4 == 0xF;
    }

    void emitInsn() { emitByte(opCode()); }
    void emitUint16Le(uint16_t val) {
        emitByte(val >> 0);
        emitByte(val >> 8);
    }

private:
    DisMemory *_memory;
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
