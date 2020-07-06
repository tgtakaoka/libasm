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

class InsnIns8070 : public InsnBase<Config> {
public:
    InsnIns8070(Insn &insn) : InsnBase(insn) {}

    AddrMode addrMode() const { return Entry::_addrMode(_flags); }
    OprFormat dstOpr() const { return Entry::_dstOpr(_flags); }
    OprFormat srcOpr() const { return Entry::_srcOpr(_flags); }
    OprSize oprSize() const { return Entry::_oprSize(_flags); }

    void setFlags(uint16_t flags) {
        _flags = flags;
    }

    void setOprFormats(OprFormat dst, OprFormat src) {
        _flags = Entry::_flags(addrMode(), dst, src, oprSize());
    }

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

} // namespace ins8070
} // namespace libasm

#endif // __INSN_INS8070_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
