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

#ifndef __INSN_INS8070_H__
#define __INSN_INS8070_H__

#include "config_ins8070.h"
#include "entry_ins8070.h"
#include "insn_base.h"

namespace libasm {
namespace ins8070 {

class InsnIns8070 : public InsnImpl<Config> {
public:
    InsnIns8070(Insn &insn) : InsnImpl(insn) {}

    AddrMode addrMode() const { return _flags.mode(); }
    OprFormat dstOpr() const { return _flags.dstOpr(); }
    OprFormat srcOpr() const { return _flags.srcOpr(); }
    OprSize oprSize() const { return _flags.size(); }

    void setFlags(Entry::Flags flags) { _flags = flags; }
    Entry::Flags flags() const { return _flags; }

    void setOprFormats(OprFormat dst, OprFormat src) {
        _flags = Entry::Flags::create(UNDEF, dst, src, SZ_NONE);
    }

    void setOpCode(Config::opcode_t opCode) { _opCode = opCode; }

    Config::opcode_t opCode() const { return _opCode; }

    void embed(Config::opcode_t data) { _opCode |= data; }

    void emitInsn() { emitByte(_opCode, 0); }

    void emitOperand8(uint8_t val8) { emitByte(val8, operandPos()); }

    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }

private:
    Config::opcode_t _opCode;
    Entry::Flags _flags;

    uint8_t operandPos() const {
        uint8_t pos = length();
        if (pos == 0)
            pos = 1;
        return pos;
    }
};

}  // namespace ins8070
}  // namespace libasm

#endif  // __INSN_INS8070_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
