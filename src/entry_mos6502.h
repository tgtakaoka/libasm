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

#ifndef __LIBASM_ENTRY_MOS6502_H__
#define __LIBASM_ENTRY_MOS6502_H__

#include <stdint.h>

#include "config_mos6502.h"
#include "entry_base.h"

namespace libasm {
namespace mos6502 {

namespace {
static constexpr int opr1_gp = 0;
static constexpr int indir_bp = 4;  // ()
static constexpr int longi_bp = 5;  // []
static constexpr int opr2_gp = 6;
static constexpr int opr3_gp = 12;
static constexpr uint16_t mode_gm = 0xF;
static constexpr uint16_t indir_bm = (1 << indir_bp);
static constexpr uint16_t longi_bm = (1 << longi_bp);
}  // namespace

enum AddrMode : uint8_t {
    M_NONE = 0,             // none
    I_FLAG = indir_bm,      // Indirect: (_)
    L_FLAG = longi_bm,      // Indirect long: [_]
    M_REGA = 1,             // Accumulator: A
    M_REGX = 2,             // Index register: X
    I_REGX = 2 | indir_bm,  // Indirect X: (X)
    M_REGY = 3,             // Index register: Y
    M_REGS = 4,             // Stack pointer: S
    I_REGS = 4 | indir_bm,  // Stack indirect: (S)
    // Memory addressing
    M_IMA = 5,              // Immediate for A: #nn
    M_IMX = 6,              // Immediate for X: #nn
    M_IM8 = 7,              // Immediate byte: #nn
    M_ABS = 8,              // Absolute: a16
    I_ABS = 8 | indir_bm,   // Absolute indirect: (a16)
    L_ABS = 8 | longi_bm,   // Absolute long indirect: [a16]
    M_ABSL = 9,             // Absolute long: a24
    M_DPG = 10,             // Direct page: d
    I_DPG = 10 | indir_bm,  // Direct page indirect: (d)
    L_DPG = 10 | longi_bm,  // Direct page long indirect: [d]
    M_REL = 11,             // Relative: r8
    M_LREL = 12,            // Relative long: r16
    M_BANK = 13,            // Bank address: k8
    // Direct page indexed with X:           M_DPG,  M_REGX, M_NONE: d,X
    // Direct page indexed with Y:           M_DPG,  M_REGY, M_NONE: d,Y
    // Absolute indexed with X:              M_ABS,  M_REGX, M_NONE: a16,X
    // Absolute indexed with Y:              M_ABS,  M_REGY, M_NONE: a16,Y
    // Direct page indexed indirect with X:  I_DPG,  I_REGX, M_NONE: (d,X)
    // Direct page indirect indexed with Y:  I_DPG,  M_REGY, M_NONE: (d),Y
    // Absolute indirect:                    I_ABS,  M_NONE, M_NONE: (a16)
    // Absolute indexed indirect with X      I_ABS,  I_REGX, M_NONE: (a16,X)
    // Direct page indirect:                 I_DPG,  M_NONE, M_NONE: (d)
    // Direct page and relative:             M_DPG,  M_REL,  M_NONE: d,r8
    // Absolute long indexed with X:         M_ABSL, M_REGX, M_NONE: a24,X
    // Absolute long indirect:               L_ABS,  M_NONE, M_NONE: [a16]
    // Stack relative:                       M_DPG,  M_REGS, M_NONE: d,S
    // Stack relative indirect indexed:      I_DPG,  I_REGS, M_REGY: (d,S),Y
    // Direct page indirect long:            L_DPG,  M_NONE, M_NONE: [d]
    // Direct page indirect long indexed:    L_DPG,  M_REGY, M_NONE: [d],Y
    // Direct page indirect long:            L_DPG,  M_NONE, M_NONE: ___L (d)
    // Direct page indirect long indexed:    L_DPG,  M_REGY, M_NONE: ___L (d),Y
    // Block move:                           M_BANK, M_BANK, M_NONE: k8,k8
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        static constexpr Flags create(AddrMode opr1, AddrMode opr2, AddrMode opr3) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(opr1) << opr1_gp) |
                                               (static_cast<uint16_t>(opr2) << opr2_gp) |
                                               (static_cast<uint16_t>(opr3) << opr3_gp))};
        }

        AddrMode mode1() const {
            return AddrMode((_attr >> opr1_gp) & (mode_gm | indir_bm | longi_bm));
        }
        AddrMode mode2() const { return AddrMode((_attr >> opr2_gp) & (mode_gm | indir_bm)); }
        AddrMode mode3() const { return AddrMode((_attr >> opr3_gp) & mode_gm); }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opCode), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

    static AddrMode baseMode(AddrMode mode) { return AddrMode(uint8_t(mode) & mode_gm); }
    static AddrMode indirectFlags(AddrMode mode) { return AddrMode(uint8_t(mode) & ~mode_gm); }
    static bool indirect(AddrMode mode) { return (uint8_t(mode) & indir_bm) != 0; }
    static bool longIndirect(AddrMode mode) { return (uint8_t(mode) & longi_bm) != 0; }

private:
    const Flags _flags_P;
};

}  // namespace mos6502
}  // namespace libasm

#endif  // __LIBASM_ENTRY_MOS6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
