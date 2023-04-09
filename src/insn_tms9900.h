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

#ifndef __INSN_TMS9900_H__
#define __INSN_TMS9900_H__

#include "config_tms9900.h"
#include "entry_tms9900.h"
#include "insn_base.h"

namespace libasm {
namespace tms9900 {

struct InsnTms9900 final : InsnImpl<Config, Entry> {
    InsnTms9900(Insn &insn) : InsnImpl(insn) {}

    AddrMode src() const { return flags().src(); }
    AddrMode dst() const { return flags().dst(); }
    void setAddrMode(AddrMode src, AddrMode dst) { setFlags(Entry::Flags::create(src, dst)); }

    void readPost(DisMemory &memory) {
        if (src() == M_SRC2)
            setPost(readUint16(memory));
    }

    void emitInsn() {
        emitUint16(opCode(), 0);
        if (src() == M_SRC2)
            emitUint16(post(), 2);
    }

    void emitOperand16(uint16_t val) { emitUint16(val, operandPos()); }

private:
    uint8_t operandPos() const {
        uint8_t pos = length();
        if (pos == 0)
            pos = 2;
        if (src() == M_SRC2 && pos < 4)
            pos = 4;
        return pos;
    }
};

}  // namespace tms9900
}  // namespace libasm

#endif  // __INSN_TMS9900_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
