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

#ifndef __LIBASM_ENTRY_MC6800_H__
#define __LIBASM_ENTRY_MC6800_H__

#include <stdint.h>

#include "config_mc6800.h"
#include "entry_base.h"

namespace libasm {
namespace mc6800 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_DIR = 1,   // Direct page
    M_EXT = 2,   // Extended
    M_IDX = 3,   // Indexed
    M_REL = 4,   // Relative
    M_IM8 = 5,   // 8-bit Immediate
    M_IM16 = 6,  // 16-bit Immediate
    M_GN8 = 7,   // Generic, M_IM8/M_DIR/M_IDX/M_EXT
    M_GN16 = 8,  // Generic, M_IM16/M_DIR/M_IDX/M_EXT
    M_GMEM = 9,  // Generic memory, M_IDX/M_EXT
    // HD6301
    M_BMM = 10,  // Bit number or Immediate (for disassembler)
    M_BIT = 11,  // Bit number (for assembler)
    // MC68HC11
    M_IDY = 12,  // Indexed Y
};

enum CodeFormat : uint8_t {
    CF_00 = 0,  // 0x00
    CF_10 = 1,  // 0x10
    CF_30 = 2,  // 0x30
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

        AddrMode mode1() const { return AddrMode((_attr >> opr1_gp) & mode_gm); }
        AddrMode mode2() const { return AddrMode((_attr >> opr2_gp) & mode_gm); }
        AddrMode mode3() const { return AddrMode((_attr >> opr3_gp) & mode_gm); }
        uint8_t mask() const {
            static constexpr uint8_t MASK[] PROGMEM = {
                    0x00,  // CF_00 = 0
                    0x10,  // CF_10 = 1
                    0x30,  // CF_30 = 2
            };
            return pgm_read_byte(&MASK[(_attr >> cf_gp) & cf_gm]);
        }

    private:
        static constexpr int opr1_gp = 0;
        static constexpr int opr2_gp = 4;
        static constexpr int opr3_gp = 8;
        static constexpr int cf_gp = 12;
        static constexpr uint_fast8_t mode_gm = 0x0F;
        static constexpr uint_fast8_t cf_gm = 0x03;
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace mc6800
}  // namespace libasm

#endif  // __LIBASM_ENTRY_MC6800_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
