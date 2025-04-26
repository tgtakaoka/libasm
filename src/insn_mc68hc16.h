/*
 * Copyright 2025 Tadashi G. Takaoka
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

#ifndef __LIBASM_INSN_MC68HC16_H__
#define __LIBASM_INSN_MC68HC16_H__

#include "config_mc68hc16.h"
#include "entry_mc68hc16.h"
#include "insn_base.h"
#include "reg_mc68hc16.h"
#include "value.h"

namespace libasm {
namespace mc68hc16 {

struct EntryInsn : EntryInsnPrefix<Config, Entry> {
    AddrMode mode1() const { return flags().mode1(); }
    AddrMode mode2() const { return flags().mode2(); }
    AddrMode mode3() const { return flags().mode3(); }
};

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName base;
    StrScanner baseAt;
    int8_t size;
    union {
        Value val;
        StrScanner list;
    };
    Operand() : mode(M_NONE), base(REG_UNDEF), size(0), val() {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    Operand op1, op2, op3;
    Config::opcode_t opr;

    void emitInsn();
    void emitOperand8(uint8_t val8) { emitByte(val8, operandPos()); }
    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }
    bool hasList() const { return prefix() == 0 && (opCode() == 0x34 || opCode() == 0x35); }

private:
    uint_fast8_t operandPos() const;
    bool hasPost() const { return flags().hasPost(); }
    uint8_t post() const { return flags().post(); }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}

    uint16_t opr;
};

}  // namespace mc68hc16
}  // namespace libasm

#endif  // __LIBASM_INSN_MC68HC16_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
