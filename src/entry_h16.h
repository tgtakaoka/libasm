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

#ifndef __LIBASM_ENTRY_H16_H__
#define __LIBASM_ENTRY_H16_H__

#include <stdint.h>

#include "config_h16.h"
#include "entry_base.h"

namespace libasm {
namespace h16 {

// Operand size: Sz field in instruction byte bits[1:0].
enum OprSize : uint8_t {
    SZ_BYTE = 0,  // Sz=00: byte (8 bits)
    SZ_WORD = 1,  // Sz=01: word (16 bits)
    SZ_LONG = 2,  // Sz=10: long word (32 bits)
    SZ_NONE = 3,  // no Sz field (fixed-size or no-size instruction)
};

// Addressing modes.  Ordered so the packed mode fields nest (mode3 <= M_EADST,
// mode2 <= M_EAPERI, mode1 <= M_EAMEM) and the EA-encodable range M_DREG..M_DIND
// stays contiguous for isEaMode().  The <CRn>/<PRn> bank prefix is not a mode --
// it rides on the CR/PR register name of the inner EA.
enum AddrMode : uint8_t {
    M_NONE = 0,
    // ---- mode3 range: the only non-NONE third operand is a memory EAd.
    M_EADST = 1,  // EA as destination (handles accumulator form)
    // ---- rest of the mode2 range: destination-position modes.
    M_EASRC = 2,  // EA source operand
    M_RLIST = 3,  // STM/LDM/PGBN list; also BFMOV/string/bit-field register run
    M_DISP = 4,   // branch displacement (Sd field selects size)
    M_CR = 5,     // control register (ANDC, ORC, XORC, LDC, STC)
    M_EAPERI = 6, // peripheral EA (MOVTP/MOVFP): memory EA also excluding
                  // auto-inc/dec -- see isPeriEaMode()
    // ---- rest of the mode1 range: source-only stored modes.
    M_FRMSZ = 7,  // LINK/RTD frame size (Sd-coded)
    M_IQ = 8,     // 8-bit signed immediate for :Q
    M_IRQ = 9,    // 4-bit unsigned 0..15 immediate for :RQ (ADD:RQ, SUB:RQ)
    M_SIRQ = 10,  // 4-bit signed -8..7 immediate for :RQ (CMP:RQ, MOV:RQ)
    M_TRAPV = 11, // TRAPA trap number 0..15
    M_RNNO = 12,  // 8-bit register number byte (CGBN, LINK Rn, UNLK Rn)
    M_RR = 13,    // register-register byte [Rns(4)|Rnd(4)]
    M_RCC = 14,   // SCB: [Rn(4)|cc(4)] byte
    M_STROP = 15, // string op register tuple (SMOV/SSTR/SCMP/SSCH)
    M_BITF = 16,  // bit-field register tuple (BFEXT/BFINS/BFSCH): Rnb,Rnf;
                  // mode2/mode3 carry the EAs/EAd when memory EAs.
    M_BFMOV = 17, // BFMOV six-register move; distinct mode avoids an opcode check
    M_SCNTR = 18, // SFT dynamic count: Rn in low 4 bits of opcode byte
    M_SCNTI = 19, // SFT static count: Imm5 in low 5 bits of opcode byte
    M_BNUMR = 20, // BIT dynamic bit-no: Rn in low 4 bits of opcode byte
    M_BNUMI = 21, // BIT static bit-no: Imm5 in low 5 bits of opcode byte
    M_EAMEM = 22, // JMP/JSR target: memory EA (STM/LDM test their EA via the
                  // register-list path instead)
    // ---- parser-only EA-detail modes (carried in Operand.mode, never stored in
    //      an Entry).  Kept contiguous with the stored EA modes so isEaMode()
    //      stays a single range check. -----------------------------------
    M_DREG = 23,  // Rn: register direct
    M_INDIR = 24, // @Rn or @(disp,Rn): register indirect
    M_PINC = 25,  // @Rn+: auto-increment
    M_PDEC = 26,  // @-Rn: auto-decrement
    M_IMM = 27,   // #imm (Si field selects byte/word/long)
    M_ABS = 28,   // @addr (Sa field selects byte/word/long)
    M_SCALE = 29, // @Rn*Sf or @(disp,Rn*Sf): indirect with scale
    M_INDEX = 30, // @([disp,]Xm[.Sz][*Sf],Rn): indirect with index
    M_PCIDX = 31, // @([disp,]Xm[.Sz][*Sf],PC): PC relative with index
    M_PCREL = 32, // @PC or @(disp,PC): PC relative
    M_DIND = 33,  // @@Rn or @(ds2,@(ds1,Rn)): double indirect
};

static inline bool isEaMode(AddrMode mode) {
    return mode >= M_DREG && mode <= M_DIND;
}

// A memory EA: any EA except register-direct and immediate.  The manual's
// "Available EA" tables forbid Rn and #imm for JMP/JSR and STM/LDM operands.
static inline bool isMemEaMode(AddrMode mode) {
    return isEaMode(mode) && mode != M_DREG && mode != M_IMM;
}

// A peripheral EA (MOVTP/MOVFP): a memory EA that also forbids auto-inc/dec,
// since the peripheral move post-modifies the address internally.  The manual's
// "Available EA" hatches Rn, #imm, @Rn+ and @-Rn for the peripheral operand.
static inline bool isPeriEaMode(AddrMode mode) {
    return isMemEaMode(mode) && mode != M_PINC && mode != M_PDEC;
}

// A bit-field operand (BFEXT/BFINS/BFSCH): any EA except immediate and
// auto-inc/dec; register-direct IS allowed.  The manual's "Available EA" hatches
// #imm, @Rn+, @-Rn for both bit-field source and destination.
static inline bool isBitFieldEaMode(AddrMode mode) {
    return isEaMode(mode) && mode != M_IMM && mode != M_PINC && mode != M_PDEC;
}

// Instruction class: the ":X" qualifier in the mnemonic (e.g. ADD:G, MOV:RQ).
// Stored so the table name stays stem-only and a class-less mnemonic can pick
// the optimal class.
enum InsnClass : uint8_t {
    IC_N = 0,   // no ":X" qualifier
    IC_G = 1,   // :G  general (EA forms, general branch)
    IC_Q = 2,   // :Q  quick immediate (imm8 signed)
    IC_R = 3,   // :R  register-register
    IC_RQ = 4,  // :RQ register + quick imm4 (0..15)
    IC_CC = 5,  // /cc condition-code suffix emitting a [*|cc] byte (TRAP, SET)
};

// String-operation direction, from the /F (forward) or /B (backward) suffix.
enum StrDir : uint8_t {
    DIR_NONE = 0,
    DIR_F = 1,
    DIR_B = 2,
};

// Instruction size qualifier for the mnemonic suffix.  A missing suffix on a
// sized instruction defaults to .W (see encodeImpl).
enum InsnSize : uint8_t {
    ISZ_NONE = 0,   // no size suffix; XCH and MOVA accept an optional ".L"
    ISZ_DATA = 1,   // .B/.W/.L from 2-bit Sz in opcode low bits (Sz=11 reserved);
                    // LINK/RTD frame size (M_FRMSZ) auto-sizes when unsuffixed
    ISZ_OBIT0 = 2,  // .B/.W from opcode bit 0 (SWAP)
    ISZ_OBIT4 = 3,  // .B/.W from mode-byte bit 4 (MUL/DIV)
    ISZ_EXTU = 4,   // EXTU/EXTS remapped Sz: 00=W, 01=L, 10=B
    ISZ_PRSZ = 5,   // .B/.W/.L from 2-bit Sz in prefix byte (SFT/BIT/Bcc:G)
    ISZ_FIXED = 6,  // fixed size (no opcode size field), carried in Flags oprSize
                    // and accepted as optional suffix.  Byte: PGBN/DNEG/DADD/
                    // DSUB/TAS/SET.  Long: XCH/MOVA/ICBN/DCBN.
    ISZ_OWL = 7,    // .W/.L from opcode bit 0: 0=word, 1=long (MOVTP/MOVFP)
};

// Per-entry attribute flags packed into 32 bits:
//   [17:15]=class(3) [14:12]=isz(3) [11:10]=oprSize(2)
//   [9:8]=mode3(2)   [7:5]=mode2(3) [4:0]=mode1(5)
// The mode fields widen left to right: mode3 holds M_NONE/M_EADST, mode2 adds
// destination-position modes, mode1 covers all stored modes.  oprSize holds the
// fixed operand size for ISZ_FIXED ops (else SZ_NONE).
struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint32_t _attr;

        static constexpr Flags create(InsnSize isz, InsnClass cls, AddrMode mode1,
                AddrMode mode2, AddrMode mode3 = M_NONE, OprSize osz = SZ_NONE) {
            return Flags{(static_cast<uint32_t>(cls) << bp_class) |
                         (static_cast<uint32_t>(isz) << bp_isz) |
                         (static_cast<uint32_t>(osz) << bp_oprSize) |
                         (static_cast<uint32_t>(mode3) << bp_mode3) |
                         (static_cast<uint32_t>(mode2) << bp_mode2) |
                         (static_cast<uint32_t>(mode1) << bp_mode1)};
        }

        InsnClass insnClass() const { return InsnClass((_attr >> bp_class) & gm_class); }
        InsnSize insnSize() const { return InsnSize((_attr >> bp_isz) & gm_isz); }
        OprSize oprSize() const { return OprSize((_attr >> bp_oprSize) & gm_oprSize); }
        AddrMode mode3() const { return AddrMode((_attr >> bp_mode3) & gm_mode3); }
        AddrMode mode2() const { return AddrMode((_attr >> bp_mode2) & gm_mode2); }
        AddrMode mode1() const { return AddrMode((_attr >> bp_mode1) & gm_mode1); }

        // Mask of variable size bits in the opcode byte: 0x03 for a 2-bit Sz
        // field, 0x01 for a 1-bit bit-0 field, else 0 (prefix-page size bits are
        // masked at the page level instead).
        uint8_t opCodeMask() const {
            switch (insnSize()) {
            case ISZ_DATA:
            case ISZ_EXTU:
                return 0x03;
            case ISZ_OBIT0:
            case ISZ_OWL:
                return 0x01;
            default:
                return 0x00;
            }
        }

    private:
        // bp_ = field bit position, gm_ = field mask.
        static constexpr int bp_mode1 = 0;
        static constexpr int bp_mode2 = 5;
        static constexpr int bp_mode3 = 8;
        static constexpr int bp_oprSize = 10;
        static constexpr int bp_isz = 12;
        static constexpr int bp_class = 15;
        static constexpr uint32_t gm_mode1 = 0x1F;
        static constexpr uint32_t gm_mode2 = 0x07;
        static constexpr uint32_t gm_mode3 = 0x03;
        static constexpr uint32_t gm_oprSize = 0x03;
        static constexpr uint32_t gm_isz = 0x07;
        static constexpr uint32_t gm_class = 0x07;
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
            : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_dword(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace h16
}  // namespace libasm

#endif  // __LIBASM_ENTRY_H16_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
