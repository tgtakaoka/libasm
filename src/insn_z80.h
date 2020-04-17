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
    InsnZ80(InsnZ80 &other) : InsnBase(other._insn) {}

    AddrMode addrMode() const { return Entry::_addrMode(_flags); }
    InsnFormat insnFormat() const { return Entry::_insnFormat(_flags); }
    OprFormat leftFormat() const { return Entry::_leftFormat(_flags); }
    OprFormat rightFormat() const { return Entry::_rightFormat(_flags); }

    void setFlags(uint16_t flags) {
        _flags = flags;
    }
    void setFlags(const InsnZ80 &other) {
        _flags = other._flags;
    }

    Config::insn_t insnCode() const { return _insnCode; }
    void setInsnCode(Config::insn_t insnCode) {
        _insnCode = insnCode;
    }
    void embed(Config::opcode_t data) {
        _insnCode |= data;
    }
    void setInsnCode(Config::opcode_t prefixCode, Config::opcode_t opCode) {
        _insnCode = (static_cast<Config::insn_t>(prefixCode) << 8) | opCode;
    }
    bool hasPrefix() const { return prefixCode() != 0; }
    Config::opcode_t prefixCode() const {
        return static_cast<Config::opcode_t>(_insnCode >> 8);
    }
    Config::opcode_t opCode() const {
        return static_cast<Config::opcode_t>(_insnCode);
    }

    void emitInsn() {
        if (hasPrefix())
            emitByte(prefixCode());
        emitByte(opCode());
    }

private:
    Config::insn_t _insnCode;
    uint16_t _flags;
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
