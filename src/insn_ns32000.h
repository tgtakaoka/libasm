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

#ifndef __INSN_NS32000_H__
#define __INSN_NS32000_H__

#include "config_ns32000.h"
#include "entry_ns32000.h"
#include "insn_base.h"
#include "reg_ns32000.h"

namespace libasm {
namespace ns32000 {

class InsnNs32000 : public InsnBase<Config> {
public:
    InsnNs32000(Insn &insn) : InsnBase(insn) {}

    AddrMode srcMode() const { return Entry::_mode(_src); }
    AddrMode dstMode() const { return Entry::_mode(_dst); }
    AddrMode ex1Mode() const { return Entry::_mode(_ex1); }
    AddrMode ex2Mode() const { return Entry::_ex2Mode(_ex2); }
    OprPos srcPos() const { return Entry::_pos(_src); }
    OprPos dstPos() const { return Entry::_pos(_dst); }
    OprPos ex1Pos() const { return Entry::_pos(_ex1); }
    OprPos ex2Pos() const { return Entry::_ex2Pos(_ex2); }
    OprSize oprSize() const { return Entry::_oprSize(_ex2); }

    void setFlags(uint32_t flags) {
        _src = Entry::_src(flags);
        _dst = Entry::_dst(flags);
        _ex1 = Entry::_ex1(flags);
        _ex2 = Entry::_ex2(flags);
        _hasPost = false;
    }

    void setAddrMode(AddrMode src, AddrMode dst, AddrMode ex1, AddrMode ex2) {
        _src = Entry::_opr(src, P_NONE);
        _dst = Entry::_opr(dst, P_NONE);
        _ex1 = Entry::_opr(ex1, P_NONE);
        _ex2 = Entry::_ex2(ex2, P_NONE, SZ_NONE);
    }

    void setOpCode(Config::opcode_t opCode, Config::opcode_t prefix = 0) {
        _opCode = opCode;
        _prefix = prefix;
        _post = 0;
    }

    void embed(Config::opcode_t data) {
        _opCode |= data;
    }

    void embedPost(Config::opcode_t data) {
        _post |= data;
    }

    void setIndexByte(uint8_t data, OprPos pos) {
        if (pos == P_GEN1) _indexByte1 = data;
        if (pos == P_GEN2) _indexByte2 = data;
    }

    void readPost(DisMemory &memory) {
        _post = readByte(memory);
    }

    void setHasPost() { _hasPost = true; }

    Config::opcode_t opCode() const { return _opCode; }
    bool hasPrefix() const { return _prefix != 0; }
    Config::opcode_t prefix() const { return _prefix; }
    bool hasPost() const { return _hasPost; }
    uint8_t post() const { return _post; }
    uint8_t indexByte(OprPos pos) const {
        return pos == P_GEN1 ? _indexByte1 : _indexByte2;
    }

    void emitInsn() {
        uint8_t pos = 0;
        if (hasPrefix()) emitByte(_prefix, pos++);
        emitByte(_opCode, pos++);
        if (hasPost()) emitByte(_post, pos);
    }

    void emitOperand8(uint8_t val8) {
        emitByte(val8, operandPos());
    }

    void emitOperand16(uint16_t val16) {
        emitUint16(val16, operandPos());
    }

    void emitOperand32(uint32_t val32) {
        emitUint32(val32, operandPos());
    }

private:
    Config::opcode_t _opCode;
    Config::opcode_t _prefix;
    Config::opcode_t _post;
    uint8_t _src;
    uint8_t _dst;
    uint8_t _ex1;
    uint8_t _ex2;
    bool _hasPost;
    uint8_t _indexByte1;
    uint8_t _indexByte2;

    uint8_t operandPos() {
        uint8_t pos = length();
        if (pos == 0) {
            if (hasPrefix()) pos++;
            pos++;
            if (hasPost()) pos++;
        }
        return pos;
    }
};

} // namespace ns32000
} // namespace libasm

#endif // __INSN_NS32000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
