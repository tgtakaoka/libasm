/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __LIBASM_ENTRY_MC6805_H__
#define __LIBASM_ENTRY_MC6805_H__

#include <stdint.h>

#include "config_mc6805.h"
#include "entry_base.h"

namespace libasm {
namespace mc6805 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    // op1, op2, op3
    M_REL = 1,  // Relative
    // op1, op2
    M_DIR = 2,   // Direct page
    M_IX0P = 3,  // Indexed X with post increment
    // op1
    M_EXT = 4,    // Extended
    M_IX1 = 5,    // Indexed X with 8-bit offset
    M_IMM = 6,    // Immediate 8-bit
    M_IX0 = 7,    // Indexed X with no offset
    M_IX2 = 8,    // Indexed X with 16-bit offset
    M_BNO = 9,    // Bit number in opcode
    M_GEN1 = 10,  // Generic: M_IMM/M_DIR
    M_GEN2 = 11,  // Generic: M_EXT/M_IX1/M_IX2/M_IX0
    M_IX10 = 12,  // Generic memory, M_IX1/M_IX0
    M_IX1P = 13,  // Indexed X with 8-bit offset with post increment
    M_SP1 = 14,   // Indexed SP with 8-bit offset
    M_SP2 = 15,   // Indexed SP with 16-bit offset
    M_IM16 = 16,  // Immediate 16-bit
    M_SIM8 = 17,  // Signed immediate 8-bit
    M_REGX = 18,  // Indexed X with no offset (without preceeding ',')
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

        AddrMode mode1() const { return AddrMode((_attr >> opr1_gp) & opr1_gm); }
        AddrMode mode2() const { return AddrMode((_attr >> opr2_gp) & opr2_gm); }
        AddrMode mode3() const { return AddrMode((_attr >> opr3_gp) & opr3_gm); }
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
        static constexpr int opr2_gp = 5;
        static constexpr int opr3_gp = 7;
        static constexpr int cf_gp = 8;
        static constexpr uint_fast8_t opr1_gm = 0x1F;
        static constexpr uint_fast8_t opr2_gm = 0x03;  // <= M_IX0P
        static constexpr uint_fast8_t opr3_gm = 0x01;  // <= M_REL
        static constexpr uint_fast8_t cf_gm = 0x03;
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opCode), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace mc6805
}  // namespace libasm

#endif  // __LIBASM_ENTRY_MC6805_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
