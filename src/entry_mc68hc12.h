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

#ifndef __LIBASM_ENTRY_MC68HC12_H__
#define __LIBASM_ENTRY_MC68HC12_H__

#include <stdint.h>

#include "config_mc68hc12.h"
#include "entry_base.h"

namespace libasm {
namespace mc68hc12 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    // op3
    M_RL8 = 1,  // 8-bit Relative
    // op2, op3
    M_RL9 = 2,    // 9-bit Relative
    M_EXT = 3,    // Extended
    M_IDX = 4,    // Indexed
    M_IM8 = 5,    // 8-bit Immediate
    M_R8L = 6,    // 8-bit Register:  A=0, B=1, CCR=2
    M_R16L = 7,   // 16-bit Register: D=0, X=1, Y=2, SP=3
    M_PAG = 8,    // 8-bit Page number
    M_DIDX = 9,   // Indexed for bit manipulation: no indirect
    M_NIDX = 10,  // Indexed with no extra post bytes (include indirect)
    // op1, op2, op3
    M_CIDX = 11,  // Indexed as opecode
    M_RL16 = 12,  // 16-bit Relative
    M_DIR = 13,   // Direct page
    M_IM16 = 14,  // 16-bit Immediate
    M_GN8 = 15,   // Generic, M_IM8/M_DIR/M_IDX/M_EXT
    M_GN16 = 16,  // Generic, M_IM16/M_DIR/M_IDX/M_EXT
    M_GMEM = 17,  // Generic memory, M_IDX/M_EXT
    M_R8H = 18,   // 8-bit Register:  A=0, B=1, CCR=2
    M_R16H = 19,  // 16-bit Register: D=0, X=1, Y=2, SP=3
    M_TRAP = 20,  // TRAP number
    M_PAIR = 21,  // Register pair: index,base
};

enum CodeFormat : uint8_t {
    CF_00 = 0,   // 0x00
    CF_10 = 1,   // 0x10
    CF_30 = 2,   // 0x30
    CF_11 = 3,   // 0x11
    CF_13 = 4,   // 0x13
    CF_33 = 5,   // 0x33
    CF_07 = 6,   // 0x07
    CF_3F = 7,   // 0x3F
    CF_7F = 8,   // 0x7F
    CF_1C = 9,   // 0x18
    CF_FF = 10,  // 0xFF
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        static constexpr Flags create(CodeFormat cf, AddrMode opr1, AddrMode opr2, AddrMode opr3) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(opr1) << opr1_gp) |
                                               (static_cast<uint16_t>(opr2) << opr2_gp) |
                                               (static_cast<uint16_t>(opr3) << opr3_gp) |
                                               (static_cast<uint16_t>(cf) << cf_gp))};
        }

        AddrMode mode1() const { return AddrMode((_attr >> opr1_gp) & opr1_gm); }
        AddrMode mode2() const { return AddrMode((_attr >> opr2_gp) & opr2_gm); }
        AddrMode mode3() const { return AddrMode((_attr >> opr3_gp) & opr3_gm); }
        uint8_t mask() const {
            static constexpr uint8_t MASK[] PROGMEM = {
                    0x00,  // CF_00 = 0
                    0x10,  // CF_10 = 1
                    0x30,  // CF_30 = 2
                    0x11,  // CF_11 = 3
                    0x13,  // CF_13 = 4
                    0x33,  // CF_33 = 5
                    0x07,  // CF_07 = 6
                    0x3F,  // CF_3F = 7
                    0x7F,  // CF_7F = 8
                    0x1C,  // CF_1C = 9
                    0xFF,  // CF_FF = 10
            };
            return pgm_read_byte(&MASK[(_attr >> cf_gp) & cf_gm]);
        }

    private:
        static constexpr int opr1_gp = 0;
        static constexpr int opr2_gp = 5;
        static constexpr int opr3_gp = 9;
        static constexpr int cf_gp = 11;
        static constexpr uint_fast8_t opr1_gm = 0x1F;
        static constexpr uint_fast8_t opr2_gm = 0x0F;
        static constexpr uint_fast8_t opr3_gm = 0x01;
        static constexpr uint_fast8_t cf_gm = 0x0F;
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace mc68hc12
}  // namespace libasm

#endif  // __LIBASM_ENTRY_MC68HC12_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
