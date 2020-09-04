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

#ifndef __INSN_I8086_H__
#define __INSN_I8086_H__

#include "config_i8086.h"
#include "entry_i8086.h"
#include "insn_base.h"

namespace libasm {
namespace i8086 {

class InsnI8086 : public InsnBase<Config> {
public:
    InsnI8086(Insn &insn)
        : InsnBase(insn),
          _segment(0),
          _modReg(0) {}

    AddrMode dstMode() const { return Entry::_mode(_dst); }
    AddrMode srcMode() const { return Entry::_mode(_src); }
    OprPos dstPos() const { return Entry::_dstPos(_pos); }
    OprPos srcPos() const { return Entry::_srcPos(_pos); }
    OprSize oprSize() const { return Entry::_size(_size); }
    bool stringInst() const { return Entry::_strInst(_size); }

    void setFlags(uint32_t flags) {
        _dst = Entry::_dst(flags);
        _src = Entry::_src(flags);
        _pos = Entry::_pos(flags);
        _size = Entry::_size(flags);
    }

    Config::opcode_t segment() const { return _segment; }
    Config::opcode_t first() const { return _first; }
    Config::opcode_t opCode() const { return _opCode; }
    void setSegment(Config::opcode_t segment) {
        _segment = segment;
    }
    void setOpCode(Config::opcode_t opCode, Config::opcode_t first) {
        _first = first;
        _opCode = opCode;
    }
    Config::opcode_t modReg() const { return _modReg; }
    Error readModReg(DisMemory &memory) {
        if (_first) {
            _modReg = _opCode;
            return OK;
        }
        const OprPos dst = dstPos();
        const OprPos src = srcPos();
        if (dst == P_MOD || dst == P_REG || src == P_MOD || src == P_REG)
            return readByte(memory, _modReg);
        return OK;
    }

private:
    Config::opcode_t _segment;
    Config::opcode_t _first;
    Config::opcode_t _opCode;
    Config::opcode_t _modReg;
    uint8_t _dst;
    uint8_t _src;
    uint8_t _pos;
    uint8_t _size;
};

} // namespace i8086
} // namespace libasm

#endif // __INSN_I8086_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
