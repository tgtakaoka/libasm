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

#ifndef __INSN_TMS9995_H__
#define __INSN_TMS9995_H__

#include "insn_base.h"
#include "entry_tms9900.h"

namespace libasm {
namespace tms9900 {

class InsnTms9900 : public InsnBase<ENDIAN_BIG> {
public:
    InsnTms9900(Insn &insn) : InsnBase(insn) {}

    AddrMode addrMode() const { return Entry::_addrMode(_flags); }
    bool is9995() const { return Entry::_cpuType(_flags) == TMS9995; }

    void setFlags(host::uint_t flags) { _flags = flags; }

    target::insn_t insnCode() const { return _insnCode; }
    void setInsnCode(target::insn_t insnCode) {
        _insnCode = insnCode;
    }
    void embed(target::opcode_t data) {
        _insnCode |= data;
    }

    void emitInsn() {
        emitUint16(_insnCode, 0);
    }

    void emitOperand(uint16_t val) {
        host::uint_t pos = _insn.length();
        if (pos == 0) pos = 2;
        emitUint16(val, pos);
    }

private:
    target::insn_t _insnCode;
    host::uint_t _flags;

    void emitUint16(uint16_t val, host::uint_t pos) {
        _insn.emitByte(static_cast<uint8_t>(val >> 8), pos + 0);
        _insn.emitByte(static_cast<uint8_t>(val >> 0), pos + 1);
    }
};

} // namespace tms9900
} // namespace libasm

#endif // __INSN_TMS9900_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
