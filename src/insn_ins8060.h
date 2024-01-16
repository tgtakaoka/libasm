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

#ifndef __LIBASM_INSN_INS8060_H__
#define __LIBASM_INSN_INS8060_H__

#include "config_ins8060.h"
#include "entry_ins8060.h"
#include "insn_base.h"
#include "reg_ins8060.h"

namespace libasm {
namespace ins8060 {

struct EntryInsn : EntryInsnBase<Config, Entry> {
    AddrMode addrMode() const { return flags().mode(); }
};

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    RegName index;
    uint16_t val16;
    Operand() : mode(M_NONE), reg(REG_UNDEF), index(REG_UNDEF), val16(0) {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    Operand op;

    void emitInsn() { emitByte(opCode()); }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}
};

}  // namespace ins8060
}  // namespace libasm

#endif  // __LIBASM_INSN_INS8060_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
