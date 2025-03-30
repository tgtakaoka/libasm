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

#ifndef __LIBASM_ENTRY_I8051_H__
#define __LIBASM_ENTRY_I8051_H__

#include <stdint.h>

#include "config_i8051.h"
#include "entry_base.h"

namespace libasm {
namespace i8051 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_ADR8 = 1,    // Internal 8-bit RAM address
    M_IMM8 = 2,    // 8-bit Immediate data
    M_REL = 3,     // Relative
    M_AREG = 4,    // A register
    M_RREG = 5,    // Rn register
    M_CREG = 6,    // C register
    M_DREG = 7,    // DPTR register
    M_ABREG = 8,   // AB register
    M_ADR11 = 9,   // 11-bit program code address
    M_ADR16 = 10,  // 16-bot program code address
    M_BITAD = 11,  // 8-bit bit address
    M_NOTAD = 12,  // not(/) 8-bit address
    M_IMM16 = 13,  // 16-bit Immediate data
    M_IDIRR = 14,  // @Rn indirect
    M_IDIRD = 15,  // @DPTR indirect
    M_INDXD = 16,  // @A+DPTR
    M_INDXP = 17,  // @A+PC
    M3_BASE = 18,
    M_ADR8_REL = M3_BASE + M_ADR8,  // M_ADR8, M_REL
    M_IMM8_REL = M3_BASE + M_IMM8,  // M_IMM8, M_REL
};

enum CodeFormat : uint8_t {
    CF_00 = 0,  // 0x00
    CF_01 = 1,  // 0x01
    CF_07 = 2,  // 0x07
    CF_E0 = 3,  // 0xE0
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        static constexpr Flags create(CodeFormat cf, AddrMode dst, AddrMode src) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(dst) << dst_gp) |
                                               (static_cast<uint16_t>(src) << src_gp) |
                                               (static_cast<uint16_t>(cf) << cf_gp))};
        }

        AddrMode dst() const { return AddrMode((_attr >> dst_gp) & mode_gm); }
        AddrMode src() const { return mode2() >= M3_BASE ? AddrMode(mode2() - M3_BASE) : mode2(); }
        AddrMode ext() const { return mode2() >= M3_BASE ? M_REL : M_NONE; }
        uint8_t mask() const {
            static constexpr uint8_t MASK[] PROGMEM = {
                    0x00,  // CF_00 = 0
                    0x01,  // CF_01 = 1
                    0x07,  // CF_07 = 2
                    0xE0,  // CF_E0 = 4
            };
            return pgm_read_byte(&MASK[(_attr >> cf_gp) & cf_gm]);
        }

    private:
        AddrMode mode2() const { return AddrMode((_attr >> src_gp) & mode_gm); }

        static constexpr int dst_gp = 0;
        static constexpr int src_gp = 5;
        static constexpr int cf_gp = 10;
        static constexpr uint_fast8_t mode_gm = 0x1F;
        static constexpr uint_fast8_t cf_gm = 0x3;
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace i8051
}  // namespace libasm

#endif  // __LIBASM_ENTRY_I8051_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
