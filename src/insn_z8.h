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

class InsnZ8 : public InsnBase<Config> {
public:
    InsnZ8(Insn &insn) : InsnBase(insn) {}

    AddrMode dstMode() const { return Entry::_mode(_dst); }
    AddrMode srcMode() const { return Entry::_mode(_src); }
    AddrMode extMode() const { return Entry::_mode(_ext); }
    PostFormat postFormat() const { return Entry::_postFmt(_fmt); }
    bool dstSrc() const { return Entry::_dstSrc(_fmt); }

    void setFlags(uint32_t flags) {
        _dst = Entry::_dst(flags);
        _src = Entry::_src(flags);
        _ext = Entry::_ext(flags);
        _fmt = Entry::_fmt(flags);
    }

    void setAddrMode(AddrMode dst, AddrMode src, AddrMode ext) {
        _dst = Entry::_opr(dst);
        _src = Entry::_opr(src);
        _ext = Entry::_opr(ext);
    }

    void setOpCode(Config::opcode_t opCode) {
        _opCode = opCode;
    }

    void embed(Config::opcode_t data) {
        _opCode |= data;
    }

    void readPost(DisMemory &memory) {
        readByte(memory);
    }

    Config::opcode_t opCode() const { return _opCode; }

    uint8_t post() const { return bytes()[1]; }

    static bool operandInOpCode(Config::opcode_t opCode) {
        const Config::opcode_t low4 = opCode & 0xF;
        return low4 >= 0x8 && low4 < 0xF;
    }

    bool singleByteOpCode() const {
        const Config::opcode_t low4 = _opCode & 0xF;
        return low4 == 0x0E || low4 == 0xF;
    }

    void emitInsn() {
        emitByte(_opCode);
    }

    void emitUint16Le(uint16_t val) {
        emitByte(val >> 0);
        emitByte(val >> 8);
    }

private:
    Config::opcode_t _opCode;
    uint8_t _dst;
    uint8_t _src;
    uint8_t _ext;
    uint8_t _fmt;
};

} // namespace z8
} // namespace libasm

#endif // __INSN_Z8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
