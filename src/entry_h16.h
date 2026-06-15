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

// Addressing modes.  The EA-encodable range M_DREG..M_PBANK is contiguous
// so isEaMode() can use a single range check.
enum AddrMode : uint8_t {
    M_NONE = 0,
    // ---- EA-encodable range (must stay contiguous) ----------------------
    M_DREG = 1,   // Rn: register direct
    M_INDIR = 2,  // @Rn or @(disp,Rn): register indirect
    M_PINC = 3,   // @Rn+: register indirect auto-increment
    M_PDEC = 4,   // @-Rn: register indirect auto-decrement
    M_IMM = 5,    // #imm: immediate (Si field selects byte/word/long)
    M_ABS = 6,    // @addr: absolute address (Sa field selects byte/word/long)
    M_SCALE = 7,  // @Rn*Sf or @(disp,Rn*Sf): register indirect with scale
    M_INDEX = 8,  // @([disp,]Xm[.Sz][*Sf],Rn): register indirect with index
    M_PCIDX = 9,  // @([disp,]Xm[.Sz][*Sf],PC): PC relative with index
    M_PCREL = 10, // @PC or @(disp,PC): PC relative
    M_DIND = 11,  // @@Rn or @(ds2,@(ds1,Rn)): register double indirect
    M_CBANK = 12, // <CRn>: current bank register prefix
    M_PBANK = 13, // <PRn>: previous bank register prefix
    // ---- non-EA modes --------------------------------------------------
    M_EA = 14,    // any EA mode (source operand for 2-op insn)
    M_EAD = 15,   // any EA mode as destination (handles accumulator form)
    M_IQ = 16,    // 8-bit signed immediate for :Q format
    M_IRQ = 17,   // 4-bit unsigned 0..7 immediate for :RQ format
    M_RNNO = 18,  // 8-bit register number byte (CGBN, LINK Rn, UNLK Rn)
    M_RR = 19,    // register-register byte [Rns(4)|Rnd(4)] (ADD:R etc.)
    M_DISP = 20,  // branch displacement (Sd field in opcode selects size)
    M_CR = 21,    // control register (ANDC, ORC, XORC, LDC, STC)
    M_ITRPN = 22, // TRAPA trap number 0..15
    M_FRMSZ = 23, // LINK frame size, RTD dealloc size (Sd-coded)
    // ---- Multi-byte family operand modes (in prefix pages) -------------
    M_SCNTR = 24, // SFT dynamic count: Rn in low 4 bits of opcode byte
    M_SCNTI = 25, // SFT static count: Imm5 in low 5 bits of opcode byte
    M_BNUMR = 26, // BIT dynamic bit-no: Rn in low 4 bits of opcode byte
    M_BNUMI = 27, // BIT static bit-no: Imm5 in low 5 bits of opcode byte
};

static inline bool isEaMode(AddrMode mode) {
    return mode >= M_DREG && mode <= M_PBANK;
}

// Instruction size qualifier for the mnemonic suffix.
enum InsnSize : uint8_t {
    ISZ_NONE = 0,  // no size suffix (fixed or size-less instruction)
    ISZ_DATA = 1,  // .B/.W/.L from Sz field in low 2 bits of opcode
    ISZ_WORD = 2,  // always .W (e.g., MOVTP, MOVFP, SWAP)
    ISZ_FIXB = 3,  // always .B (e.g., DNEG.B)
    ISZ_EXTU = 4,  // EXTU/EXTS remapped Sz: 00=W, 01=L, 10=B
    ISZ_PRSZ = 5,  // .B/.W/.L from Sz field in low 2 bits of prefix
    ISZ_OBIT4 = 6, // .B/.W from bit 4 of opcode (MUL/DIV mode byte)
    ISZ_FIXL = 7,  // always .L (e.g., XCH)
    ISZ_AUTO = 8,  // .B/.W/.L from explicit suffix OR auto-picked from the
                   // operand's value size (e.g., RTD/LINK frame size)
};

// Per-entry attribute flags packed into 16 bits.
//   [15:14]=isz(2)  [13:12]=mask_kind(2)
//   [11:6]=src(6)   [5:0]=dst(6)
// mask_kind: 0=no mask (fixed opcode), 1=Sz mask (0x03), 2=Sd mask (0x03),
//            3=Reserved
struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        // [15:12]=isz(4) [11:6]=dst(6) [5:0]=src(6)
        static constexpr Flags create(InsnSize isz, AddrMode src, AddrMode dst) {
            return Flags{static_cast<uint16_t>(
                    (static_cast<uint16_t>(isz) << 12) |
                    (static_cast<uint16_t>(dst) << 6) |
                    (static_cast<uint16_t>(src)))};
        }

        InsnSize insnSize() const { return InsnSize((_attr >> 12) & 0xF); }
        AddrMode dst() const { return AddrMode((_attr >> 6) & 0x3F); }
        AddrMode src() const { return AddrMode(_attr & 0x3F); }

        // Mask of variable bits in the opcode byte (Sz/Sd field for sized
        // instructions, 0 for fixed-opcode instructions).
        uint8_t opCodeMask() const {
            switch (insnSize()) {
            case ISZ_DATA:
            case ISZ_EXTU:
            case ISZ_AUTO:
                return 0x03;
            default:
                return 0x00;
            }
        }
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
            : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

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
