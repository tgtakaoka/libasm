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

#ifndef __INSN_CDP1802_H__
#define __INSN_CDP1802_H__

#include "config_cdp1802.h"
#include "entry_cdp1802.h"
#include "insn_base.h"

namespace libasm {
namespace cdp1802 {

class InsnCdp1802 : public InsnImpl<Config> {
public:
    InsnCdp1802(Insn &insn) : InsnImpl(insn) {}

    AddrMode mode1() const { return _flags.mode1(); }
    AddrMode mode2() const { return _flags.mode2(); }

    void setFlags(Entry::Flags flags) { _flags = flags; }
    Entry::Flags flags() const { return _flags; }

    void setAddrMode(AddrMode mode1, AddrMode mode2) {
        _flags = Entry::Flags::create(mode1, mode2);
    }

    void setOpCode(Config::opcode_t opCode, Config::opcode_t prefix = 0) {
        _opCode = opCode;
        _prefix = prefix;
    }

    void embed(Config::opcode_t data) { _opCode |= data; }

    bool hasPrefix() const { return _prefix != 0; }
    Config::opcode_t opCode() const { return _opCode; }
    Config::opcode_t prefix() const { return _prefix; }

    void emitInsn() {
        if (hasPrefix())
            emitByte(prefix());
        emitByte(opCode());
    }

private:
    Entry::Flags _flags;
    Config::opcode_t _opCode;
    Config::opcode_t _prefix;
};

}  // namespace cdp1802
}  // namespace libasm

#endif  // __INSN_CDP1802_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
