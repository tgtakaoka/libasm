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

#ifndef __LIBASM_INSN_MC6809_H__
#define __LIBASM_INSN_MC6809_H__

#include "config_mc6809.h"
#include "entry_mc6809.h"
#include "insn_base.h"
#include "reg_mc6809.h"

namespace libasm {
namespace mc6809 {

struct EntryInsn : EntryInsnPrePostfix<Config, Entry> {
    AddrMode mode1() const { return flags().mode1(); }
    AddrMode mode2() const { return flags().mode2(); }
};

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName index;
    RegName base;
    bool indir;
    int8_t extra;
    uint32_t val32;
    StrScanner list;
    Operand()
        : mode(M_NONE),
          index(REG_UNDEF),
          base(REG_UNDEF),
          indir(false),
          extra(0),
          val32(0),
          list() {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    Operand op1, op2;

    void emitInsn() {
        uint8_t pos = 0;
        if (hasPrefix())
            emitByte(prefix(), pos++);
        emitByte(opCode(), pos);
    }
    void emitOperand8(uint8_t val8) { emitByte(val8, operandPos()); }
    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }
    void emitOperand32(uint32_t val32) { emitUint32(val32, operandPos()); }

private:
    uint8_t operandPos() const {
        uint8_t pos = length();
        if (pos == 0)
            pos = hasPrefix() ? 2 : 1;
        return pos;
    }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}

    auto readPostfix() {
        if (!hasPostfix())
            setPostfix(readByte());
        return postfix();
    }
};

}  // namespace mc6809
}  // namespace libasm

#endif  // __LIBASM_INSN_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
