/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef __LIBASM_INSN_PDP8_H__
#define __LIBASM_INSN_PDP8_H__

#include "config_pdp8.h"
#include "entry_pdp8.h"
#include "insn_base.h"

namespace libasm {
namespace pdp8 {

struct EntryInsn : EntryInsnBase<Config, Entry> {
    AddrMode mode() const { return flags().mode(); }
    Config::opcode_t bits() const { return flags().bits(); }
    Config::opcode_t selector() const { return flags().selector(); }
    bool combination() const { return flags().combination(); }
    bool multiGroup() const { return flags().multiGroup(); }
    void setAddrMode(AddrMode opr) { setFlags(Entry::Flags::create(opr)); }
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    void emitInsn() { emitUint16(opCode() & 07777); }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}
    uint16_t readUint12() { return readUint16() & 07777; }
};

}  // namespace pdp8
}  // namespace libasm

#endif  // __LIBASM_INSN_PDP8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
