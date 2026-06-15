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

#ifndef __LIBASM_ENTRY_SUPERH_H__
#define __LIBASM_ENTRY_SUPERH_H__

#include <stdint.h>

#include "config_superh.h"
#include "entry_base.h"

namespace libasm {
namespace superh {

// SH-1/SH-2 addressing modes with their instruction-word bit positions.
// Each mode determines which bits in the 16-bit instruction word are variable;
// opcodeMask() ORs these together so the table search can mask them out.
enum AddrMode : uint8_t {
    M_NONE = 0,    // no operand                         bits: none
    M_RN = 1,      // Rn, bits[11:8]                     bits: 0x0F00
    M_RM = 2,      // Rm, bits[7:4]                      bits: 0x00F0
    M_R0 = 3,      // R0 implied                         bits: none
    M_IRN = 4,     // @Rn, bits[11:8]                    bits: 0x0F00
    M_IRM = 5,     // @Rm, bits[7:4]                     bits: 0x00F0
    M_INCN = 6,    // @Rn+, bits[11:8]                   bits: 0x0F00
    M_INCM = 7,    // @Rm+, bits[7:4]                    bits: 0x00F0
    M_DECN = 8,    // @-Rn, bits[11:8]                   bits: 0x0F00
    M_IDXN = 9,    // @(R0,Rn), bits[11:8]               bits: 0x0F00
    M_IDXM = 10,   // @(R0,Rm), bits[7:4]                bits: 0x00F0
    M_D4N = 11,    // @(d4,Rn), n=bits[11:8],d=bits[3:0] bits: 0x0F0F
    M_D4M = 12,    // @(d4,Rm), m=bits[7:4], d=bits[3:0] bits: 0x00FF
    M_D8B = 13,    // @(d8*1,GBR), d=bits[7:0]           bits: 0x00FF
    M_D8W = 14,    // @(d8*2,GBR), d=bits[7:0]           bits: 0x00FF
    M_D8L = 15,    // @(d8*4,GBR), d=bits[7:0]           bits: 0x00FF
    M_PCW = 16,    // @(d8*2,PC),  d=bits[7:0] (MOV.W)  bits: 0x00FF
    M_PCL = 17,    // @(d8*4,PC),  d=bits[7:0] (MOV.L/MOVA) bits: 0x00FF
    M_IMM8 = 18,   // #imm8 sign-extended, bits[7:0]     bits: 0x00FF
    M_REL8 = 19,   // 8-bit signed PC-rel branch, bits[7:0] bits: 0x00FF
    M_REL12 = 20,  // 12-bit signed PC-rel branch, bits[11:0] bits: 0x0FFF
    M_TNUM = 21,   // TRAPA vector, bits[7:0]             bits: 0x00FF
    M_SR = 22,     // SR control register                 bits: none
    M_GBR = 23,    // GBR control register                bits: none
    M_VBR = 24,    // VBR control register                bits: none
    M_MACH = 25,   // MACH system register                bits: none
    M_MACL = 26,   // MACL system register                bits: none
    M_PR = 27,     // PR system register                  bits: none
    M_IGBR = 28,   // @(R0,GBR) implied (bitwise .B forms) bits: none
    M_DSR = 29,    // DSR DSP status register             bits: none
    M_A0 = 30,     // A0 DSP accumulator low              bits: none
    M_X0 = 31,     // X0 DSP data register                bits: none
    M_X1 = 32,     // X1 DSP data register                bits: none
    M_Y0 = 33,     // Y0 DSP data register                bits: none
    M_Y1 = 34,     // Y1 DSP data register                bits: none
    M_MOD = 35,    // MOD DSP modulo register             bits: none
    M_RS = 36,     // RS DSP repeat-start register        bits: none
    M_RE = 37,     // RE DSP repeat-end register          bits: none
    M_REL8P = 38,  // 8-bit signed PC-rel as "@(*+N,PC)"  bits: 0x00FF
    M_FRN = 39,    // FRn, bits[11:8]                     bits: 0x0F00
    M_FRM = 40,    // FRm, bits[7:4]                      bits: 0x00F0
    M_FPUL = 41,   // FPUL FPU communication register     bits: none
    M_FPSCR = 42,  // FPSCR FPU status/control register   bits: none
    // SH-2A additions
    M_D12N = 43,    // @(disp12,Rn), n=bits[11:8], disp in 2nd word bits: 0x0F00
    M_D12M = 44,    // @(disp12,Rm), m=bits[7:4],  disp in 2nd word bits: 0x00F0
    M_IMM20 = 45,   // 20-bit signed imm, hi nibble in bits[7:4], lo16 in 2nd word
    M_IMM20S = 46,  // 20-bit signed imm shifted-left 8 (MOVI20S)  bits: 0x00F0
    M_IMM3 = 47,    // 3-bit unsigned imm, bits[6:4]               bits: 0x0070
    M_DRN = 48,     // DRn (even FRn pair), bits[11:9]             bits: 0x0E00
    M_DRM = 49,     // DRm (even FRm pair), bits[7:5]              bits: 0x00E0
    M_BANK = 50,    // implicit R0 destination for STBANK/LDBANK  bits: none
    M_R15 = 51,     // implicit R15 for MOVML/MOVMU stack         bits: none
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        // [13:8]=src(6), [5:0]=dst(6); bit 15 = longForm (32-bit insn), bits 14, 7:6 are spare
        static constexpr uint16_t LONG_FORM = 1u << 15;

        static constexpr Flags create(AddrMode src, AddrMode dst) {
            return Flags{static_cast<uint16_t>(
                    (static_cast<uint16_t>(src) << 8) | static_cast<uint16_t>(dst))};
        }
        static constexpr Flags createLong(AddrMode src, AddrMode dst) {
            return Flags{static_cast<uint16_t>(
                    LONG_FORM | (static_cast<uint16_t>(src) << 8) | static_cast<uint16_t>(dst))};
        }

        AddrMode src() const { return AddrMode((_attr >> 8) & 0x3F); }
        AddrMode dst() const { return AddrMode(_attr & 0x3F); }
        bool longForm() const { return (_attr & LONG_FORM) != 0; }

        // Returns the OR of all variable bits in the 16-bit instruction word.
        Config::opcode_t opcodeMask() const { return modeMask(src()) | modeMask(dst()); }

    private:
        static Config::opcode_t modeMask(AddrMode mode) {
            switch (mode) {
            case M_RN:
            case M_IRN:
            case M_INCN:
            case M_DECN:
            case M_IDXN:
            case M_FRN:
            case M_D12N:
                return 0x0F00;
            case M_RM:
            case M_IRM:
            case M_INCM:
            case M_IDXM:
            case M_FRM:
            case M_D12M:
            case M_IMM20S:
                return 0x00F0;
            case M_IMM20:
                return 0x00F0;  // hi nibble of immediate in word 1's bits[7:4]
            case M_IMM3:
                return 0x0070;
            case M_DRN:
                return 0x0E00;
            case M_DRM:
                return 0x00E0;
            case M_D4N:
                return 0x0F0F;
            case M_D4M:
            case M_D8B:
            case M_D8W:
            case M_D8L:
            case M_PCW:
            case M_PCL:
            case M_IMM8:
            case M_REL8:
            case M_REL8P:
            case M_TNUM:
                return 0x00FF;
            case M_REL12:
                return 0x0FFF;
            default:
                return 0;
            }
        }
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace superh
}  // namespace libasm

#endif  // __LIBASM_ENTRY_SUPERH_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
