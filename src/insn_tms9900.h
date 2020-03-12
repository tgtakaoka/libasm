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

class Insn
    : public InsnBase<ENDIAN_BIG, Entry::code_max, Entry::name_max> {
public:
    AddrMode addrMode() const { return Entry::_addrMode(_flags); }
    bool is9995() const { return Entry::_cpuType(_flags) == TMS9995; }

    void setFlags(host::uint_t flags) { _flags = flags; }

    void embed(target::opcode_t data) {
        _insnCode |= data;
    }

    void emitInsn() {
        emitUint16(_insnCode, 0);
        if (_insnLen == 0) _insnLen = 2;
    }

    void emitOperand(uint16_t val) {
        if (_insnLen == 0) _insnLen = 2;
        appendUint16(val);
    }

    void appendUint16(uint16_t val) {
        emitUint16(val, _insnLen);
        _insnLen += 2;
    }

private:
    host::uint_t _flags;

    void emitUint16(uint16_t val, host::uint_t pos) {
        _bytes[pos++] = static_cast<uint8_t>(val >> 8);
        _bytes[pos] = static_cast<uint8_t>(val);
    }
};

#endif // __INSN_TMS9900_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
