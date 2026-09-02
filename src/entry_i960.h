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

#ifndef __LIBASM_ENTRY_I960_H__
#define __LIBASM_ENTRY_I960_H__

#include <stdint.h>

#include "config_i960.h"
#include "entry_base.h"

namespace libasm {
namespace i960 {

// There are four instruction formats, told apart by the operation code in bits
// 31 through 24.  Each addressing mode names both the operand kind and the
// field it occupies, so an entry needs no separate position table.
//
// REG (58-7F) holds a 12-bit operation code split between bits 31-24 and bits
// 10-7, three operand fields, and a mode bit for each of them:
//
//   31      24 23   19 18   14 13 12 11 10  7 6   5 4    0
//  |  OPCODE  |src/dst| src2 |m3|m2|m1|OPCODE| 0 0| src1 |
//
// A mode bit selects a five-bit literal in place of a register.  Where an
// instruction leaves a field unused, its mode bit is set; that is what GNU as
// and the original Intel assembler emit, and it is the form this table takes
// as canonical, so the bit belongs to the operation code rather than to an
// operand.
//
// COBR (20-3F) compares and branches.  Note that its src1 field is the one REG
// calls src/dst, not the one REG calls src1:
//
//   31      24 23   19 18   14 13 12                2 1 0
//  |  OPCODE  | src1  | src2 |m1|    DISPLACEMENT    |t|s2|
//
// CTRL (08-1F) is a branch or a call with nothing but a displacement:
//
//   31      24 23                                    2 1 0
//  |  OPCODE  |            DISPLACEMENT               |t|0|
//
// MEM (80-CF) addresses memory.  MEMA has a 12-bit unsigned offset, MEMB a
// four-bit mode, a scale, and an optional second word holding a 32-bit
// displacement:
//
//   31      24 23   19 18   14 13 12         0
//  |  OPCODE  |src/dst| abase| 0|   OFFSET   |            MEMA
//  |  OPCODE  |src/dst| abase|  MODE  |SCALE|0 0|index|   MEMB
enum AddrMode : uint8_t {
    M_NONE = 0,
    // The four groups below are what acceptMode() range-checks against, so an
    // operand kind has to stay inside the group whose operands it accepts.
    //
    // Fields taking either a register or a five-bit literal, the mode bit
    // telling the two apart.  src1 is bits 4-0 with mode bit m1, src2 is bits
    // 18-14 with m2, and src/dst is bits 23-19 with m3.
    M_SRC1 = 1,   // src1
    M_SRC1L = 2,  // src1, register pair
    M_SRC1Q = 3,  // src1, register quad
    M_SRC2 = 4,   // src2
    M_SRC2L = 5,  // src2, register pair
    M_CSRC1 = 6,  // src1 of COBR, bits 23-19
    // Fields taking a register and nothing else.  Their mode bit has to read
    // zero, so it belongs to the operation code rather than to the operand.
    M_REG1 = 7,    // src1
    M_REG2 = 8,    // src2
    M_CREG1 = 9,   // src1 of COBR, bits 23-19
    M_CSRC2 = 10,  // src2 of COBR, bits 18-14
    M_DST = 11,    // src/dst
    M_DSTL = 12,   // src/dst, register pair
    M_DSTQ = 13,   // src/dst, register quad
    M_MDST = 14,   // src/dst of MEM
    M_MDSTL = 15,  // src/dst of MEM, register pair
    M_MDSTQ = 16,  // src/dst of MEM, register quad
    // Operands written as a bare expression.
    M_CDISP = 17,  // 13-bit word displacement, bits 12-2; the target address
    M_TDISP = 18,  // 24-bit word displacement, bits 23-2; the target address
    M_MEM = 19,    // the memory operand, bits 18-0 and any second word
    // Floating-point fields.  A source whose mode bit is clear names a global
    // or local register as usual; a set one names a floating-point register or
    // one of the two real literals (Table B-1).  A destination takes a
    // floating-point register when its mode bit is set, and no literal (Table
    // B-2 and the text below it).
    M_FSRC1 = 20,   // src1, real
    M_FSRC1L = 21,  // src1, long real
    M_FSRC1Q = 22,  // src1, extended real
    M_FSRC2 = 23,   // src2, real
    M_FSRC2L = 24,  // src2, long real
    M_FSRC2Q = 25,  // src2, extended real
    M_FDST = 26,    // src/dst, real
    M_FDSTL = 27,   // src/dst, long real
    M_FDSTQ = 28,   // src/dst, extended real
    // What the assembler's parser reports, since which field an operand
    // occupies, and whether a bare expression is a literal, a branch target or
    // a memory offset, follows from the entry it matches.  These never appear
    // in a table entry.  A mode which DOES must fit the five-bit field
    // Entry::Flags packs it into, so 31 is the ceiling for those.
    M_FREG = 29,   // a floating-point register or a real literal
    M_REG = 30,    // a global or local register
    M_VALUE = 31,  // a bare expression
};

// The bits an operand of |mode| occupies.  A mode bit belongs to the operand
// only where the field may hold a literal; where the operand must be a
// register the bit has to read zero, so it stays part of the operation code.
static inline constexpr Config::opcode_t operandMask(AddrMode mode) {
    return mode == M_SRC1 || mode == M_SRC1L || mode == M_SRC1Q ? UINT32_C(0x0000081F)
           : mode == M_REG1                                     ? UINT32_C(0x0000001F)
           : mode == M_SRC2 || mode == M_SRC2L                  ? UINT32_C(0x0007D000)
           : mode == M_REG2 || mode == M_CSRC2                  ? UINT32_C(0x0007C000)
           : mode == M_DST || mode == M_DSTL || mode == M_DSTQ  ? UINT32_C(0x00F80000)
           : mode == M_CSRC1                                    ? UINT32_C(0x00F82000)
           : mode == M_CREG1                                    ? UINT32_C(0x00F80000)
           : mode == M_CDISP                                    ? UINT32_C(0x00001FFC)
           : mode == M_TDISP                                    ? UINT32_C(0x00FFFFFC)
           : mode == M_MEM                                      ? UINT32_C(0x0007FFFF)
           : mode == M_MDST || mode == M_MDSTL || mode == M_MDSTQ ? UINT32_C(0x00F80000)
           : mode == M_FSRC1 || mode == M_FSRC1L || mode == M_FSRC1Q ? UINT32_C(0x0000081F)
           : mode == M_FSRC2 || mode == M_FSRC2L || mode == M_FSRC2Q ? UINT32_C(0x0007D000)
           : mode == M_FDST || mode == M_FDSTL || mode == M_FDSTQ ? UINT32_C(0x00F82000)
                                                                  : UINT32_C(0);
}

// How many registers an operand of |mode| spans.  A pair must name an even
// register and a quad one which is a multiple of four; the manual leaves an
// unaligned operand undefined, so a decoder rejects it.
static inline constexpr uint_fast8_t operandRegs(AddrMode mode) {
    return mode == M_SRC1L || mode == M_SRC2L || mode == M_DSTL || mode == M_MDSTL ||
                           mode == M_FSRC1L || mode == M_FSRC2L || mode == M_FDSTL
                   ? 2
           : mode == M_SRC1Q || mode == M_DSTQ || mode == M_MDSTQ || mode == M_FSRC1Q ||
                           mode == M_FSRC2Q || mode == M_FDSTQ
                   ? 4
                   : 1;
}

// No instruction names more than three operands.
static constexpr uint_fast8_t MAX_OPERANDS = 3;

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        static constexpr Flags create(AddrMode opr1 = M_NONE, AddrMode opr2 = M_NONE,
                AddrMode opr3 = M_NONE, bool fpu = false) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(opr1) << mode1_gp) |
                                               (static_cast<uint16_t>(opr2) << mode2_gp) |
                                               (static_cast<uint16_t>(opr3) << mode3_gp) |
                                               (static_cast<uint16_t>(fpu ? 1 : 0) << fpu_gp))};
        }

        // Whether the entry is one of the extensions the 80960KB adds to the
        // base instruction set, which the fpu option enables.
        bool fpu() const { return (_attr >> fpu_gp) & 1; }

        AddrMode mode1() const { return AddrMode((_attr >> mode1_gp) & mode_gm); }
        AddrMode mode2() const { return AddrMode((_attr >> mode2_gp) & mode_gm); }
        AddrMode mode3() const { return AddrMode((_attr >> mode3_gp) & mode_gm); }

        AddrMode mode(uint_fast8_t pos) const {
            switch (pos) {
            case 0:
                return mode1();
            case 1:
                return mode2();
            default:
                return mode3();
            }
        }

        // Whether the instruction addresses memory, hence may carry a second
        // word.
        bool hasMem() const {
            for (uint_fast8_t pos = 0; pos < MAX_OPERANDS; pos++) {
                if (mode(pos) == M_MEM)
                    return true;
            }
            return false;
        }

        // The bits which belong to operands; every other bit of the word must
        // equal the entry's operation code.
        Config::opcode_t mask() const {
            Config::opcode_t m = 0;
            for (uint_fast8_t pos = 0; pos < MAX_OPERANDS; pos++)
                m |= operandMask(mode(pos));
            return m;
        }

    private:
        static constexpr int mode1_gp = 0;
        static constexpr int mode2_gp = 5;
        static constexpr int mode3_gp = 10;
        static constexpr int fpu_gp = 15;
        static constexpr uint_fast8_t mode_gm = 0x1F;
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const /*PROGMEM*/ char *name_P)
        : Base(name_P, opCode), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace i960
}  // namespace libasm

#endif  // __LIBASM_ENTRY_I960_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
