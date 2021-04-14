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

class InsnZ8000 : public InsnImpl<Config> {
public:
    InsnZ8000(Insn &insn) : InsnImpl(insn) {}

    OprSize oprSize() const { return _flags.oprSize(); }
    AddrMode dstMode() const { return _flags.dstMode(); }
    AddrMode srcMode() const { return _flags.srcMode(); }
    AddrMode ex1Mode() const { return _flags.ex1Mode(); }
    AddrMode ex2Mode() const { return _flags.ex2Mode(); }
    PostMode postMode() const { return _flags.postMode(); }
    bool hasPost() const { return postMode() != P_NO; }
    uint8_t postMask() const { return _flags.postMask(); }
    uint8_t postVal() const { return _flags.postVal(); }
    ModeField dstField() const { return _flags.dstField(); }
    ModeField srcField() const { return _flags.srcField(); }

    void setFlags(Entry::Flags flags) { _flags = flags; }
    Entry::Flags flags() const { return _flags; }

    void setAddrMode(AddrMode dst, AddrMode src, AddrMode ex1, AddrMode ex2) {
        _flags = Entry::Flags::create(dst, MF_NO, src, MF_NO, ex1, ex2, P_NO, CM_0x0000, SZ_NONE);
    }

    void setOpCode(Config::opcode_t opCode) {
        _opCode = opCode;
        _post = 0;
    }

    void readPost(DisMemory &memory) { _post = readUint16(memory); }

    void embed(Config::opcode_t data) { _opCode |= data; }

    void embedPost(Config::opcode_t data) { _post |= data; }

    Config::opcode_t opCode() const { return _opCode; }
    uint16_t post() const { return _post; }

    void emitInsn() {
        emitUint16(_opCode, 0);
        const PostMode mode = postMode();
        if (mode == P_0XX8)
            _post |= 8;
        if (mode == P_0XXE)
            _post |= 0xE;
        if (mode != P_NO)
            emitUint16(_post, 2);
    }

    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }

    void emitOperand32(uint32_t val32) { emitUint32(val32, operandPos()); }

    bool isThreeRegsInsn() const {
        const uint8_t opc = _opCode >> 8;
        return opc == 0xB8 || opc == 0xBA || opc == 0xBB;
    }
    bool isTranslateInsn() const {
        const uint8_t opc = _opCode >> 8;
        return opc == 0xB8;
    }
    bool isLoadMultiInsn() const {
        const uint8_t opc = _opCode >> 8;
        return opc == 0x1C || opc == 0x5C;
    }

private:
    Config::opcode_t _opCode;
    Config::opcode_t _post;
    Entry::Flags _flags;

    uint8_t operandPos() {
        uint8_t pos = length();
        if (pos == 0)
            pos = hasPost() ? 4 : 2;
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
