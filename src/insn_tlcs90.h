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

#ifndef __INSN_TLCS90_H__
#define __INSN_TLCS90_H__

#include "config_tlcs90.h"
#include "entry_tlcs90.h"
#include "insn_base.h"

namespace libasm {
namespace tlcs90 {

class InsnTlcs90 : public InsnBase<Config> {
public:
    InsnTlcs90(Insn &insn) : InsnBase(insn) {}

    AddrMode dstMode() const { return _flags.dstMode(); }
    AddrMode srcMode() const { return _flags.srcMode(); }
    AddrMode preMode() const { return _preMode; }

    void setFlags(Entry::Flags flags) { _flags = flags; }
    Entry::Flags flags() const { return _flags; }

    void setAddrMode(AddrMode dst, AddrMode src) {
        _flags = Entry::Flags::create(dst, src);
    }
    void setPreMode(AddrMode pre) {
        _preMode = pre;
    }

    void setOpCode(Config::opcode_t opCode, Config::opcode_t prefix = 0) {
        _opCode = opCode;
        _prefix = prefix;
    }

    void embed(Config::opcode_t data) { _opCode |= data; }

    bool hasPrefix() const { return prefix() != 0; }
    Config::opcode_t prefix() const { return _prefix; }
    Config::opcode_t opCode() const { return _opCode; }

private:
    Entry::Flags _flags;
    Config::opcode_t _opCode;
    Config::opcode_t _prefix;
    AddrMode _preMode;
};

}  // namespace tlcs90
}  // namespace libasm

#endif  // __INSN_TLCS90_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
