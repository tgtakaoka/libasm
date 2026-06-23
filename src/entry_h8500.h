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

#ifndef __LIBASM_ENTRY_H8500_H__
#define __LIBASM_ENTRY_H8500_H__

#include <stdint.h>

#include "config_h8500.h"
#include "entry_base.h"

namespace libasm {
namespace h8500 {

// AddrMode values: the eleven EA-resolvable modes occupy a contiguous range
// [M_REG, M_IMM16] so isEaMode() is a single range comparison. M_EASRC/M_EADST
// are placeholders that the EA prefix byte resolves into one of those modes.
enum AddrMode : uint8_t {
    M_NONE = 0,      // no operand
    // ---- EA-resolvable range (must stay contiguous; see isEaMode) ----------
    M_REG = 1,       // Rn register (0-7)
    M_IND = 2,       // @Rn register indirect
    M_PDEC = 3,      // @-Rn pre-decrement
    M_PINC = 4,      // @Rn+ post-increment
    M_IDX8 = 5,      // @(d:8,Rn) signed 8-bit displacement
    M_FPX8 = 6,      // @(d:8,FP) for MOV:F (treated as M_IDX8 with reg=FP)
    M_IDX16 = 7,     // @(d:16,Rn) signed 16-bit displacement
    M_ABS8 = 8,      // @aa:8 short absolute (via BR)
    M_ABS16 = 9,     // @aa:16 absolute
    M_IMM8 = 10,     // #xx:8 8-bit immediate
    M_IMM16 = 11,    // #xx:16 16-bit immediate
    // ---- non-EA modes ------------------------------------------------------
    M_CR = 12,       // control register (SR/CCR/BR/EP/DP/TP)
    M_BIT = 13,      // bit number 0-15 in OP[3:0]
    M_REL8 = 14,     // PC-relative 8-bit (branch d:8)
    M_REL16 = 15,    // PC-relative 16-bit (branch d:16)
    M_REGLIST = 16,  // register list (LDM/STM)
    M_ABS24 = 17,    // page + 16-bit addr (PJMP/PJSR)
    M_TRAPV = 18,    // trap vector 0-15 (TRAPA)
    M_SCB = 19,      // SCB count register (0xB8|Rn second byte)
    M_IMM2 = 20,     // #+/-1 or #+/-2 for ADD:Q short format (assembly only)
    M_FP = 21,       // FP register (R6) for LINK/UNLK
    M_SP = 22,       // implicit stack operand: @SP+ (LDM src) / @-SP (STM dst)
    M_REGP = 23,     // Rn forming a pair Rn:Rn+1; must be even when word (MULXU/DIVXU.W dst)
    M_INDP = 24,     // @Rn forming a pair Rn:Rn+1; must be even (PJMP/PJSR @Rn src)
    // ---- EA placeholders: resolved from the EA prefix byte at decode time ---
    M_EAREG = 25,    // EA operand restricted to a register (XCH; narrower than MOV:G)
    M_EASRC = 26,    // EA operand, source slot (accepts all 11 EA modes)
    M_EADST = 27,    // EA operand, destination slot (no immediate)
    M_EAMEM = 28,    // EA operand restricted to a memory mode (MOVFPE/MOVTPE)
};

// Routing class of an instruction's leading byte(s). Recorded on the Insn and
// matched against each page; replaces the per-entry InsnFmt.
enum PrefixMode : uint8_t {
    PM_SPC = 0,  // [OP]                    special format, no prefix
    PM_GEN = 1,  // [EA][ext 0-2][OP]       general format
    PM_EXT = 2,  // [EA][ext 0-2][00][OP]   extended format
    PM_SEC = 3,  // [11][OP][...]           secondary (0x11) prefix
};

// Operand size of an instruction (H8/300-style). SZ_DATA means the size is
// carried by the live EA-byte Sz bit (bit 3), so one entry covers both .B/.W.
enum OprSize : uint8_t {
    SZ_NONE = 0,  // no operand size
    SZ_BYTE = 1,  // always byte (Sz=0)
    SZ_WORD = 2,  // always word (Sz=1)
    SZ_DATA = 3,  // size from the EA-byte (or OP-byte) Sz bit
};

// Whether the mnemonic carries a .B/.W size suffix.
enum InsnSize : uint8_t {
    ISZ_NONE = 0,  // no suffix
    ISZ_DATA = 1,  // .B (Sz=0) / .W (Sz=1) suffix from the Sz bit
};

// Instruction class: the ":x" format suffix of MOV/ADD/CMP. The mnemonic name
// stores only the stem (MOV/ADD/CMP); the class is an entry attribute, parsed
// from a written ":x" and emitted by the disassembler. IC_N means the mnemonic
// has no class suffix (every instruction other than MOV/ADD/CMP).
enum InsnClass : uint8_t {
    IC_N = 0,    // no class suffix
    IC_G = 1,  // :G general
    IC_E = 2,  // :E
    IC_F = 3,  // :F
    IC_I = 4,  // :I
    IC_L = 5,  // :L
    IC_S = 6,  // :S
    IC_Q = 7,  // :Q (ADD:Q)
};

// Variable-bit mask of an opcode (or EA classifier) byte. CF_08 is used only by
// the EA-prefix page classifiers (the @aa Sz bit); instruction entries use
// CF_00 (exact), CF_07 (register field), CF_0F (bit/condition field).
enum CodeFormat : uint8_t {
    CF_00 = 0,  // mask 0x00
    CF_07 = 1,  // mask 0x07
    CF_08 = 2,  // mask 0x08
    CF_0F = 3,  // mask 0x0F
};

namespace {
constexpr uint8_t CODE_MASK[] = {0x00, 0x07, 0x08, 0x0F};
}

struct Entry final : entry::Base<uint8_t> {
    struct Flags final {
        // Three bytes (read with pgm_read_byte, no 32-bit PROGMEM math) hold the
        // 18 attribute bits. The 29-value AddrMode forces src/dst to 5 bits each,
        // so the total cannot fit a single 16-bit word; 3x uint8 beats a uint32.
        //   _a: [7]=isz(1) [6:5]=cf(2) [4:0]=dst(5)
        //   _b:           [6:5]=osz(2) [4:0]=src(5)
        //   _c:                        [2:0]=cls(3)
        uint8_t _a;
        uint8_t _b;
        uint8_t _c;

        static constexpr Flags create(CodeFormat cf, OprSize osz, InsnSize isz, InsnClass cls,
                AddrMode src, AddrMode dst) {
            return Flags{static_cast<uint8_t>((static_cast<uint8_t>(isz) << isz_gp) |
                                              (static_cast<uint8_t>(cf) << cf_gp) |
                                              (static_cast<uint8_t>(dst) << mode_gp)),
                    static_cast<uint8_t>((static_cast<uint8_t>(osz) << osz_gp) |
                                         (static_cast<uint8_t>(src) << mode_gp)),
                    static_cast<uint8_t>(static_cast<uint8_t>(cls) << cls_gp)};
        }

        CodeFormat cf() const { return CodeFormat((_a >> cf_gp) & cf_gm); }
        OprSize oprSize() const { return OprSize((_b >> osz_gp) & osz_gm); }
        InsnSize insnSize() const { return InsnSize((_a >> isz_gp) & isz_gm); }
        InsnClass insnClass() const { return InsnClass((_c >> cls_gp) & cls_gm); }
        AddrMode src() const { return AddrMode((_b >> mode_gp) & mode_gm); }
        AddrMode dst() const { return AddrMode((_a >> mode_gp) & mode_gm); }
        uint8_t mask() const { return CODE_MASK[cf()]; }

    private:
        static constexpr int mode_gp = 0;
        static constexpr int cf_gp = 5;
        static constexpr int isz_gp = 7;
        static constexpr int osz_gp = 5;
        static constexpr int cls_gp = 0;
        static constexpr uint_fast8_t mode_gm = 0x1F;
        static constexpr uint_fast8_t cf_gm = 0x3;
        static constexpr uint_fast8_t isz_gm = 0x1;
        static constexpr uint_fast8_t osz_gm = 0x3;
        static constexpr uint_fast8_t cls_gm = 0x7;
    };

    constexpr Entry(uint8_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const {
        return Flags{pgm_read_byte(&_flags_P._a), pgm_read_byte(&_flags_P._b),
                pgm_read_byte(&_flags_P._c)};
    }

private:
    const Flags _flags_P;
};

}  // namespace h8500
}  // namespace libasm

#endif  // __LIBASM_ENTRY_H8500_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
