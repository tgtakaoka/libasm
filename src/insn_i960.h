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

#ifndef __LIBASM_INSN_I960_H__
#define __LIBASM_INSN_I960_H__

#include "config_i960.h"
#include "entry_i960.h"
#include "insn_base.h"
#include "reg_i960.h"
#include "value.h"

namespace libasm {
namespace i960 {

// The mode bits of the REG format.  A set bit makes the matching field a
// five-bit literal instead of a register, and a field the instruction does not
// use carries a set bit too.
static constexpr Config::opcode_t BIT_M1 = UINT32_C(0x00000800);
static constexpr Config::opcode_t BIT_M2 = UINT32_C(0x00001000);
static constexpr Config::opcode_t BIT_M3 = UINT32_C(0x00002000);
static constexpr Config::opcode_t MODE_BITS = BIT_M1 | BIT_M2 | BIT_M3;

// COBR keeps the mode bit of its src1 field where REG keeps m3.
static constexpr Config::opcode_t BIT_COBR_M1 = BIT_M3;

// The two real literals a floating-point source field may name (Table B-1).
static constexpr uint8_t FLOAT_ZERO = 16;
static constexpr uint8_t FLOAT_ONE = 22;

// The MEM format splits at bit 12: MEMA holds a twelve-bit offset and MEMB a
// four-bit mode in bits 13-10.  Within MEMB, bit 13 says a second word holds a
// 32-bit displacement, bit 11 that an index register is present and bit 10 an
// address base register; mode 5 is the exception, being IP-relative with a
// displacement word.
static constexpr Config::opcode_t MEMB_BIT = UINT32_C(0x00001000);
static constexpr Config::opcode_t MEMA_ABASE = UINT32_C(0x00002000);

// The scale of an index register is encoded as the power of two, and Table
// B-4 defines only the five values up to sixteen; 101 to 111 are reserved.
static constexpr uint8_t MAX_SCALE = 4;

enum MemMode : uint8_t {
    MEM_MEMA = 0,            // not a MEMB mode; the operand is MEMA
    MEM_ABASE = 4,           // (abase)
    MEM_IP = 5,              // displacement+8(ip)
    MEM_RESERVED = 6,        // not defined
    MEM_ABASE_IX = 7,        // (abase)[index*scale]
    MEM_DISP = 12,           // displacement
    MEM_DISP_ABASE = 13,     // displacement(abase)
    MEM_DISP_IX = 14,        // displacement[index*scale]
    MEM_DISP_ABASE_IX = 15,  // displacement(abase)[index*scale]
};

struct EntryInsn : EntryInsnBase<Config, Entry> {
    EntryInsn() : _modeBits(MODE_BITS_SET), _fpu(false) {}

    AddrMode mode(uint_fast8_t pos) const { return flags().mode(pos); }
    bool hasMem() const { return flags().hasMem(); }

    // Whether the assembly may use the 80960KB extensions.  Held here rather
    // than on the assembler or disassembler so those stay free of mutable
    // state.
    void setFpu(bool enable) { _fpu = enable; }
    bool allows(const Entry::Flags &flags) const { return _fpu || !flags.fpu(); }

    // Which encoding of an unused field's mode bit the assembly uses.  Held
    // here rather than on the disassembler so that stays free of mutable
    // state.
    void setModeBits(ModeBits modeBits) { _modeBits = modeBits; }
    bool hasModeBits() const { return _modeBits == MODE_BITS_SET; }

private:
    ModeBits _modeBits;
    bool _fpu;
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory, const StrBuffer &out) : DisInsnImpl(insn, memory, out) {}

    // The MEMB displacement word, read once the addressing mode says there is
    // one, so that the instruction length is right before an operand is
    // written.
    uint32_t displacement = 0;

    // The four-bit MEMB mode, or MEM_MEMA when the operand is MEMA.
    MemMode memMode() const {
        const auto opc = opCode();
        return (opc & MEMB_BIT) ? MemMode((opc >> 10) & 0xF) : MEM_MEMA;
    }
};

}  // namespace i960
}  // namespace libasm

#endif  // __LIBASM_INSN_I960_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
