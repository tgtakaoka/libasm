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
    OprSize oprSize() const { return Entry::_oprSize(_flags); }
    bool is6309() const { return Entry::_cpuType(_flags) == HD6309; }

    void setFlags(host::uint_t flags) {
        _flags = flags;
    }

    void setAddrMode(AddrMode addrMode) {
        _flags = Entry::_flags(
            Entry::_cpuType(_flags),
            Entry::_oprSize(_flags),
            addrMode);
    }

    Config::insn_t insnCode() const { return _insnCode; }
    void setInsnCode(Config::insn_t insnCode) {
        _insnCode = insnCode;
    }
    void setInsnCode(
        Config::opcode_t prefixCode, Config::opcode_t opCode) {
        _insnCode =
            (static_cast<Config::insn_t>(prefixCode) << 8)
            | opCode;
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
    host::uint_t _flags;
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
