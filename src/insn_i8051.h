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

class InsnI8051 : public InsnImpl<Config> {
public:
    InsnI8051(Insn &insn) : InsnImpl(insn) {}

    AddrMode dstMode() const { return _flags.dstMode(); }
    AddrMode srcMode() const { return _flags.srcMode(); }
    AddrMode extMode() const { return _flags.extMode(); }

    void setFlags(Entry::Flags flags) { _flags = flags; }
    Entry::Flags flags() const { return _flags; }

    void setAddrMode(AddrMode dst, AddrMode src, AddrMode ext) {
        _flags = Entry::Flags::create(dst, src, ext);
    }

    void setOpCode(Config::opcode_t opCode) { _opCode = opCode; }

    void embed(Config::opcode_t data) { _opCode |= data; }

    Config::opcode_t opCode() const { return _opCode; }

    void emitInsn() { emitByte(_opCode, 0); }

    void emitOperand8(uint8_t val) { emitByte(val, operandPos()); }

    void emitOperand16(uint16_t val) { emitUint16(val, operandPos()); }

private:
    Entry::Flags _flags;
    Config::opcode_t _opCode;

    uint8_t operandPos() const {
        uint8_t pos = length();
        if (pos == 0)
            pos = 1;
        return pos;
    }
};

}  // namespace i8051
}  // namespace libasm

#endif  // __INSN_I8051_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
