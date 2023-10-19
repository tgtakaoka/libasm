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

#ifndef __INSN_MOS6502_H__
#define __INSN_MOS6502_H__

#include "config_mos6502.h"
#include "entry_mos6502.h"
#include "insn_base.h"

namespace libasm {
namespace mos6502 {

struct EntryInsn : EntryInsnBase<Config, Entry> {
    AddrMode mode1() const { return flags().mode1(); }
    AddrMode mode2() const { return flags().mode2(); }
    AddrMode mode3() const { return flags().mode3(); }
    void setAddrMode(AddrMode opr1, AddrMode opr2, AddrMode opr3) {
        setFlags(Entry::Flags::create(opr1, opr2, opr3));
    }

    static AddrMode baseMode(AddrMode mode) { return Entry::baseMode(mode); }
    static AddrMode indirectFlags(AddrMode mode) { return Entry::indirectFlags(mode); }
    static bool indirect(AddrMode mode) { return Entry::indirect(mode); }
    static bool longIndirect(AddrMode mode) { return Entry::longIndirect(mode); }
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    void emitInsn() { emitByte(opCode(), 0); }
    void emitOperand8(uint8_t val8) { emitByte(val8, operandPos()); }
    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }

private:
    uint8_t operandPos() const {
        uint8_t pos = length();
        return pos == 0 ? 1 : pos;
    }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}

    void setAllowIndirectLong(bool allow) { _allowIndirectLong = allow; }
    bool allowIndirectLong() const { return _allowIndirectLong; }

private:
    bool _allowIndirectLong;  // allow [] to represent indirect long
};

}  // namespace mos6502
}  // namespace libasm

#endif  // __INSN_MOS6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
