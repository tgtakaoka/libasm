/*
 * Copyright 2026 Tadashi G. Takaoka
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

#ifndef __LIBASM_INSN_AM29000_H__
#define __LIBASM_INSN_AM29000_H__

#include "config_am29000.h"
#include "entry_am29000.h"
#include "insn_base.h"
#include "reg_am29000.h"
#include "value.h"

namespace libasm {
namespace am29000 {

// The M bit of an operation code selects a register or a constant operand, and
// the A bit selects a Program-Counter relative or an absolute target.  They
// share bit 24; which one an entry means follows from its operands.
static constexpr Config::opcode_t BIT_M = UINT32_C(0x01000000);
static constexpr Config::opcode_t BIT_A = UINT32_C(0x01000000);

// Coprocessor enable of a load or store.
static constexpr Config::opcode_t BIT_CE = UINT32_C(0x00800000);

struct EntryInsn : EntryInsnBase<Config, Entry> {
    EntryInsn() : _fpu(false), _intMul(false), _cpuType(AM29200) {}

    AddrMode mode(uint_fast8_t pos) const { return flags().mode(pos); }
    Extension extension() const { return flags().extension(); }

    // Which arithmetic the assembly may use, and which CPU decides the rest.
    // Held here rather than on the assembler or disassembler so those stay free
    // of mutable state.
    void setExtensions(bool fpu, bool intMul, CpuType cpuType) {
        _fpu = fpu;
        _intMul = intMul;
        _cpuType = cpuType;
    }
    CpuType cpuType() const { return _cpuType; }
    bool hasCopro() const { return Config::cpuHasCopro(_cpuType); }
    bool allows(Extension ext) const {
        if (ext == EXT_FPU)
            return _fpu;
        if (ext == EXT_INTMUL)
            return _intMul;
        return true;
    }

private:
    bool _fpu;
    bool _intMul;
    CpuType _cpuType;
};

struct Operand final : ErrorAt {
    AddrMode mode;
    RegName sreg;
    uint8_t regno;
    Value val;
    Operand() : mode(M_NONE), sreg(REG_UNDEF), regno(0), val() {}
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    Operand operands[MAX_OPERANDS];

    void emitInsn() { emitUint32(opCode(), 0); }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}
};

}  // namespace am29000
}  // namespace libasm

#endif  // __LIBASM_INSN_AM29000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
