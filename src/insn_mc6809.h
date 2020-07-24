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

#ifndef __INSN_MC6809_H__
#define __INSN_MC6809_H__

#include "config_mc6809.h"
#include "entry_mc6809.h"
#include "insn_base.h"

namespace libasm {
namespace mc6809 {

class InsnMc6809 : public InsnBase<Config> {
public:
    InsnMc6809(Insn &insn) : InsnBase(insn) {}

    AddrMode addrMode() const { return Entry::_addrMode(_flags); }
    AddrMode extraMode() const { return Entry::_extraMode(_flags); }

    void setFlags(uint8_t flags) {
        _flags = flags;
    }

    void setAddrMode(AddrMode mode, AddrMode extra) {
        _flags = Entry::_flags(mode, extra);
    }

    void setOpCode(
        Config::opcode_t opCode, Config::opcode_t prefixCode = 0) {
        _opCode = opCode;
        _prefixCode = prefixCode;
    }
    void embed(Config::opcode_t opCode) {
        _opCode |= opCode;
    }
    bool hasPrefix() const { return prefixCode() != 0; }
    Config::opcode_t prefixCode() const { return _prefixCode; }
    Config::opcode_t opCode() const { return _opCode; }

    void emitInsn() {
        if (hasPrefix())
            emitByte(prefixCode());
        emitByte(opCode());
    }

private:
    Config::opcode_t _opCode;
    Config::opcode_t _prefixCode;
    uint8_t _flags;
};

} // namespace mc6809
} // namespace libasm

#endif // __INSN_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
