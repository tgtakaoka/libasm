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

#ifndef __INSN_MC68000_H__
#define __INSN_MC68000_H__

#include "config_mc68000.h"
#include "entry_mc68000.h"
#include "insn_base.h"
#include "reg_mc68000.h"

namespace libasm {
namespace mc68000 {

class InsnMc68000 : public InsnBase<Config> {
public:
    InsnMc68000(Insn &insn) : InsnBase(insn) {}

    AddrMode srcMode() const { return Entry::_mode(_src); }
    AddrMode dstMode() const { return Entry::_mode(_dst); }
    OprPos srcPos() const { return Entry::_srcPos(_pos); }
    OprPos dstPos() const { return Entry::_dstPos(_pos); }
    bool alias() const { return Entry::_alias(_pos); }
    OprSize  oprSize() const { return Entry::_oprSize(_size); }
    InsnSize insnSize() const { return Entry::_insnSize(_size); }
    void setFlags(uint32_t flags) {
        _src = Entry::_src(flags);
        _dst = Entry::_dst(flags);
        _pos = Entry::_pos(flags);
        _size = Entry::_size(flags);
    }

    Config::opcode_t opCode() const { return _opCode; }
    void setOpCode(Config::opcode_t opCode) {
        _opCode = opCode;
    }
    void embed(Config::opcode_t data, uint8_t gp = 0) {
        _opCode |= (data << gp);
    }

    void setInsnSize(InsnSize isize) {
        _size = Entry::_size(Entry::_oprSize(_size), isize);
    }
    void setOprSize(OprSize osize) {
        _size = Entry::_size(osize, Entry::_insnSize(_size));
    }
    void appendOprSize(OprSize size, RegMc68000 regs) {
        char suffix[4];
        regs.outOprSize(suffix, size);
        _insn.appendName(suffix);
    }
    void setAddrMode(AddrMode src, AddrMode dst) {
        _src = Entry::_opr(src);
        _dst = Entry::_opr(dst);
    }

    void emitInsn() {
        emitUint16(_opCode, 0);
    }

    void emitOperand16(uint16_t val16) {
        uint8_t pos = _insn.length();
        if (pos == 0) pos = 2;
        emitUint16(val16, pos);
    }

    void emitOperand32(uint32_t val32) {
        emitOperand16(static_cast<uint16_t>(val32 >> 16));
        emitOperand16(static_cast<uint16_t>(val32));
    }

private:
    Config::opcode_t _opCode;
    uint8_t _src;
    uint8_t _dst;
    uint8_t _pos;
    uint8_t _size;

    void emitUint16(uint16_t val, uint8_t pos) {
        _insn.emitByte(static_cast<uint8_t>(val >> 8), pos + 0);
        _insn.emitByte(static_cast<uint8_t>(val >> 0), pos + 1);
    }
};

} // namespace mc68000
} // namespace libasm

#endif // __INSN_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
