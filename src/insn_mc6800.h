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

#ifndef __INSN_MC6800_H__
#define __INSN_MC6800_H__

#include "config_mc6800.h"
#include "entry_mc6800.h"
#include "insn_base.h"
#include "reg_mc6800.h"

namespace libasm {
namespace mc6800 {

class InsnMc6800 : public InsnImpl<Config> {
public:
    InsnMc6800(Insn &insn) : InsnImpl(insn) {}

    AddrMode mode1() const { return _flags.mode1(); }
    AddrMode mode2() const { return _flags.mode2(); }
    AddrMode mode3() const { return _flags.mode3(); }
    OprSize size() const { return _flags.size(); }

    void setFlags(Entry::Flags flags) { _flags = flags; }
    Entry::Flags flags() const { return _flags; }

    void setAddrMode(
            const AddrMode op1, const AddrMode op2, const AddrMode op3) {
        _flags = Entry::Flags::create(op1, op2, op3, SZ_NONE);
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

}  // namespace mc6800
}  // namespace libasm

#endif  // __INSN_MC6800_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
