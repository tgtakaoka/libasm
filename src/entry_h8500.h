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

// AddrMode values are arranged so the eleven EA-encodable modes occupy a
// contiguous range [M_REG, M_IMM16]; isEaMode() exploits this with a single
// range comparison rather than a switch.
enum AddrMode : uint8_t {
    M_NONE = 0,  // no operand
    // ---- EA-encodable range (must stay contiguous; see isEaMode) -----------
    M_REG = 1,      // Rn register (0-7)
    M_IND = 2,      // @Rn register indirect
    M_PREDEC = 3,   // @-Rn pre-decrement
    M_POSTINC = 4,  // @Rn+ post-increment
    M_DISP8 = 5,    // @(d:8,Rn) signed 8-bit displacement
    M_DISP8F = 6,   // @(d:8,FP) for MOV:F (treated as M_DISP8 with reg=FP)
    M_DISP16 = 7,   // @(d:16,Rn) signed 16-bit displacement
    M_ABS8 = 8,     // @aa:8 short absolute (via BR)
    M_ABS16 = 9,    // @aa:16 absolute
    M_IMM8 = 10,    // #xx:8 8-bit immediate
    M_IMM16 = 11,   // #xx:16 16-bit immediate
    // ---- non-EA modes ------------------------------------------------------
    M_CR = 12,       // control register (SR/CCR/BR/EP/DP/TP)
    M_BIT = 13,      // bit number 0-15 in OP[3:0]
    M_PCREL8 = 14,   // PC-relative 8-bit (branch d:8)
    M_PCREL16 = 15,  // PC-relative 16-bit (branch d:16)
    M_REGLIST = 16,  // register list (LDM/STM)
    M_ABS24 = 17,    // page + 16-bit addr (PJMP/PJSR)
    M_TRAPV = 18,    // trap vector 0-3 (TRAPA)
    M_SCB = 19,      // SCB count register
    M_IMM2 = 20,     // #+/-1 or #+/-2 for SUBS/ADDS (word add/subtract 1 or 2)
    M_EA = 21,       // general EA (any value in the range above)
    M_FP = 22,       // FP register (R6) for LINK/UNLK
};

// Instruction format (determines byte ordering)
enum InsnFmt : uint8_t {
    FMT_GEN = 0,  // Format A: [EA byte][EA ext 0-2 bytes][OP byte]
    FMT_SPC = 1,  // Format B: [OP byte][additional bytes]
    FMT_EXT = 2,  // Format C: [EA byte][EA ext][0x00][OP byte]
    FMT_SEC = 3,  // Secondary: [0x11][OP byte][additional bytes]
};

// Sz bit in EA byte for register-based modes (0=byte, 1=word)
enum EaSz : uint8_t {
    EA_BYTE = 0,
    EA_WORD = 1,
};

struct Entry final : entry::Base<uint8_t> {
    struct Flags final {
        uint16_t _attr;

        // [15:14]=fmt(2), [13]=eaSz(1), [12:8]=src(5), [7:3]=dst(5), [2:0]=unused
        static constexpr Flags create(InsnFmt fmt, EaSz easZ, AddrMode src, AddrMode dst) {
            return Flags{static_cast<uint16_t>(
                    (static_cast<uint16_t>(fmt) << 14) | (static_cast<uint16_t>(easZ) << 13) |
                    (static_cast<uint16_t>(src) << 8) | (static_cast<uint16_t>(dst) << 3))};
        }

        InsnFmt fmt() const { return InsnFmt((_attr >> 14) & 0x3); }
        EaSz eaSz() const { return EaSz((_attr >> 13) & 0x1); }
        AddrMode src() const { return AddrMode((_attr >> 8) & 0x1F); }
        AddrMode dst() const { return AddrMode((_attr >> 3) & 0x1F); }
    };

    constexpr Entry(uint8_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

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
