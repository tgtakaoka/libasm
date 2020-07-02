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

#ifndef __INSN_I8051_H__
#define __INSN_I8051_H__

#include "config_i8051.h"
#include "entry_i8051.h"
#include "insn_base.h"

namespace libasm {
namespace i8051 {

class InsnI8051 : public InsnBase<Config> {
public:
    InsnI8051(Insn &insn) : InsnBase(insn) {}

    AddrMode dstMode() const { return Entry::_dstMode(_flags); }
    AddrMode srcMode() const { return Entry::_srcMode(_flags); }
    AddrMode extMode() const { return Entry::_extMode(_flags); }
    void setAddrMode(AddrMode dst, AddrMode src, AddrMode ext) {
        _flags = Entry::_flags(dst, src, ext);
    }

    void setFlags(uint16_t flags) { _flags = flags; }

    Config::opcode_t opCode() const { return _opCode; }
    void setOpCode(Config::opcode_t opCode) {
        _opCode = opCode;
    }
    void embed(Config::opcode_t data) {
        _opCode |= data;
    }

    void emitInsn() {
        emitByte(_opCode);
    }

private:
    Config::opcode_t _opCode;
    uint16_t _flags;
};

} // namespace i8051
} // namespace libasm

#endif // __INSN_I8051_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
