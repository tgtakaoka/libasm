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

#ifndef __INSN_W65C816_H__
#define __INSN_W65C816_H__

#include "config_w65c816.h"
#include "entry_w65c816.h"
#include "insn_base.h"

namespace libasm {
namespace w65c816 {

class InsnW65C816 : public InsnBase<Config> {
public:
    InsnW65C816(Insn &insn) : InsnBase(insn) {}

    AddrMode addrMode() const {
        return AddrMode(Entry::_addrMode(_flags));
    }

    uint8_t flags() const { return _flags; }
    void setFlags(uint8_t flags) {
        _flags = flags;
    }

    void setAddrMode(AddrMode addrMode) {
        _flags = Entry::_set(_flags, addrMode);
    }

    Config::opcode_t opCode() const { return _opCode; }
    void setOpCode(Config::opcode_t opCode) {
        _opCode = opCode;
    }
    void emitInsn() {
        emitByte(_opCode);
    }

private:
    Config::opcode_t _opCode;
    uint8_t _flags;
};

} // namespace w65c816
} // namespace libasm

#endif // __INSN_W65C816_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
