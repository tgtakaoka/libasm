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

#ifndef __INSN_Z80_H__
#define __INSN_Z80_H__

#include "config_z80.h"
#include "entry_z80.h"
#include "insn_base.h"

namespace libasm {
namespace z80 {

class InsnZ80 : public InsnBase<Config> {
public:
    InsnZ80(Insn &insn) : InsnBase(insn) {}

    AddrMode dstMode() const { return Entry::_dstMode(_flags); }
    AddrMode srcMode() const { return Entry::_srcMode(_flags); }
    bool indexBit() const { return Entry::_indexBit(_flags); }

    void setFlags(uint16_t flags) {
        _flags = flags;
    }

    void setAddrMode(AddrMode dst, AddrMode src) {
        _flags = Entry::_flags(dst, src);
    }

    void setOpCode(Config::opcode_t opCode, Config::opcode_t prefix = 0) {
        _opCode = opCode;
        _prefix = prefix;
    }

    void embed(Config::opcode_t data) {
        _opCode |= data;
    }

    bool hasPrefix() const { return prefix() != 0; }
    Config::opcode_t prefix() const { return _prefix; }
    Config::opcode_t opCode() const { return _opCode; }

    void emitInsn() {
        uint8_t pos = 0;
        if (hasPrefix())
            emitByte(prefix(), pos++);
        emitByte(opCode(), pos);
    }

    void emitOperand8(uint8_t val8) {
        emitByte(val8, operandPos());
    }

    void emitOperand16(uint16_t val16) {
        emitUint16(val16, operandPos());
    }

private:
    uint16_t _flags;
    Config::opcode_t _opCode;
    Config::opcode_t _prefix;

    uint8_t operandPos() const {
        uint8_t pos = length();
        if (hasPrefix() && pos < 2) {
            return 2;
        } else if (pos == 0) {
            return 1;
        }
        return pos;
    }
};

} // namespace z80
} // namespace libasm

#endif // __INSN_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
