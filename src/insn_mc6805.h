/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __INSN_MC6805_H__
#define __INSN_MC6805_H__

#include "config_mc6805.h"
#include "entry_mc6805.h"
#include "insn_base.h"

namespace libasm {
namespace mc6805 {

class InsnMc6805 : public InsnImpl<Config, Entry> {
public:
    InsnMc6805(Insn &insn) : InsnImpl(insn) {}

    AddrMode mode1() const { return flags().mode1(); }
    AddrMode mode2() const { return flags().mode2(); }
    AddrMode mode3() const { return flags().mode3(); }
    bool undefined() const { return flags().undefined(); }
    void setAddrMode(const AddrMode op1, const AddrMode op2, const AddrMode op3) {
        setFlags(Entry::Flags::create(op1, op2, op3));
    }

    void emitInsn() { emitByte(opCode(), 0); }
    void emitOperand8(uint8_t val8) { emitByte(val8, operandPos()); }
    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }

private:
    uint8_t operandPos() const {
        uint8_t pos = length();
        if (pos == 0)
            pos = 1;
        return pos;
    }
};

}  // namespace mc6805
}  // namespace libasm

#endif  // __INSN_MC6805_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
