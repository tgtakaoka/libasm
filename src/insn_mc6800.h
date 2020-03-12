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

#include "insn_base.h"
#include "entry_mc6800.h"

class Insn
    : public InsnBase<ENDIAN_BIG, Entry::code_max, Entry::name_max> {
public:
    AddrMode addrMode() const { return Entry::_addrMode(_flags); }
    InsnAdjust insnAdjust() const { return Entry::_insnAdjust(_flags); }
    OprSize oprSize() const { return Entry::_oprSize(_flags); }

    void setFlags(host::uint_t flags) {
        _flags = flags;
    }

    void setAddrMode(AddrMode addrMode) {
        _flags = Entry::_flags(
            addrMode, Entry::_insnAdjust(_flags), Entry::_oprSize(_flags));
    }

    void embed(target::opcode_t data) {
        _insnCode |= data;
    }
    void emitInsn() {
        emitByte(_insnCode);
    }

private:
    host::uint_t _flags;
};

#endif // __INSN_MC6800_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
