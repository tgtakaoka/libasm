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

#ifndef __INSN_M6502_H__
#define __INSN_M6502_H__

#include "insn_base.h"
#include "entry_m6502.h"

class Insn : public InsnBase {
public:
    AddrMode addrMode() const { return Entry::_addrMode(_flags); }
    bool supported(CpuType cpuType) const {
        const CpuType insnType = Entry::_cpuType(_flags);
        if (host::uint_t(cpuType) < host::uint_t(insnType))
            return false;
        if (cpuType == W65C816 && insnType == R65C02BIT)
            return false;
        return true;
    }

    void setFlags(host::uint_t flags) {
        _flags = flags;
    }

    void setAddrMode(AddrMode addrMode) {
        _flags = Entry::_flags(Entry::_cpuType(_flags), addrMode);
    }

    void emitInsn() {
        emitByte(_insnCode);
    }

protected:
    Endian endian() override { return ENDIAN_LITTLE; }

private:
    host::uint_t _flags;
};

#endif // __INSN_M6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
