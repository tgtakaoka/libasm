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

class InsnZ8000 : public InsnBase<Config> {
public:
    InsnZ8000(Insn &insn) : InsnBase(insn) {}

    OprSize  oprSize() const { return Entry::_oprSize(_size); }
    AddrMode dstMode() const { return Entry::_mode(_dst); }
    AddrMode srcMode() const { return Entry::_mode(_src); }
    AddrMode ex1Mode() const { return Entry::_ex1(_ext); }
    AddrMode ex2Mode() const { return Entry::_ex2(_ext); }
    PostMode postMode() const { return Entry::_post(_ext); }
    bool hasPost() const { return postMode() != P_NO; }
    uint8_t postMask() const { return Entry::_postMask(postMode()); }
    uint8_t postVal() const { return Entry::_postVal(postMode()); }
    ModeField dstField() const { return Entry::_field(_dst); }
    ModeField srcField() const { return Entry::_field(_src); }

    void setFlags(uint32_t flags) {
        _dst = Entry::_dst(flags);
        _src = Entry::_src(flags);
        _ext = Entry::_ext(flags);
        _size = Entry::_size(flags);
    }

    void setAddrMode(
            AddrMode dst, AddrMode src, AddrMode ex1, AddrMode ex2) {
        _dst = Entry::_opr(dst, MF_NO);
        _src = Entry::_opr(src, MF_NO);
        _ext = Entry::_ext(ex1, ex2, P_NO);
    }

    void setOpCode(Config::opcode_t opCode) {
        _opCode = opCode;
        _post = 0;
    }

    void readPost(DisMemory &memory) {
        _post = readUint16(memory);
    }

    void embed(Config::opcode_t data) {
        _opCode |= data;
    }

    void embedPost(Config::opcode_t data) {
        _post |= data;
    }

    Config::opcode_t opCode() const { return _opCode; }
    uint16_t post() const { return _post; }

    void emitInsn() {
        emitUint16(_opCode, 0);
        const PostMode mode = postMode();
        if (mode == P_0XX8) _post |= 8;
        if (mode == P_0XXE) _post |= 0xE;
        if (mode != P_NO) emitUint16(_post, 2);
    }

    void emitOperand16(uint16_t val16) {
        emitUint16(val16, operandPos());
    }

    void emitOperand32(uint32_t val32) {
        emitUint32(val32, operandPos());
    }

private:
    Config::opcode_t _opCode;
    Config::opcode_t _post;
    uint8_t _dst;
    uint8_t _src;
    uint8_t _ext;
    uint8_t _size;

    uint8_t operandPos() {
        uint8_t pos = length();
        if (pos == 0) pos = hasPost() ? 4 : 2;
        return pos;
    }
};

} // namespace z8000
} // namespace libasm

#endif // __INSN_Z8000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
