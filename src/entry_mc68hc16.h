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

#ifndef __LIBASM_ENTRY_MC68HC16_H__
#define __LIBASM_ENTRY_MC68HC16_H__

#include <stdint.h>

#include "config_mc68hc16.h"
#include "entry_base.h"

namespace libasm {
namespace mc68hc16 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_RL8 = 1,    // 8-bit Relative
    M_RL16 = 2,   // 16-bit Relative
    M_IM8 = 3,    // 8-bit Immediate
    M_IM16 = 4,   // 16-bit Immediate
    M_IM4H = 5,   // 4-bit Signed Immediate
    M_IM4L = 6,   // 4-bit Signed Immediate
    M_EX16 = 7,   // 16-bit Extended
    M_EX20 = 8,   // 20-bit Extended
    M_IX8 = 9,    // 8-bit indexed, d8,X/d8,Y/d8,X
    M_IX16 = 10,  // 16-bit indexed, d16,X/d16,Y/d16,Z
    M_IX20 = 11,  // 20-bit Indexed, d20,X/d20,Y/d20/Z
    M_IXE = 12,   // Accumulate indexed, E,X/E,Y/E,Z
    M_IXX8 = 13,  // 8-bit X-Indexed, d8,X
    M_LIST = 14,  // Register list
    // mode3
    M_POST = 15,  // has post byte
};

enum CodeFormat : uint8_t {
    CF_00 = 0,  // 0x00
    CF_30 = 1,  // 0x30
    CF_FF = 2,  // 0xFF
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

        static constexpr Flags postbyte(uint8_t post) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(post) << post_gp) |
                                               (static_cast<uint16_t>(M_POST) << opr3_gp) |
                                               (static_cast<uint16_t>(CF_00) << cf_gp))};
        }

        AddrMode mode1() const {
            return hasPost() ? M_NONE : AddrMode((_attr >> opr1_gp) & opr1_gm);
        }
        AddrMode mode2() const {
            return hasPost() ? M_NONE : AddrMode((_attr >> opr2_gp) & opr2_gm);
        }
        AddrMode mode3() const { return AddrMode((_attr >> opr3_gp) & opr3_gm); }
        bool hasPost() const { return mode3() == M_POST; }
        uint8_t post() const { return (_attr >> post_gp) & post_gm; }
        CodeFormat format() const { return CodeFormat((_attr >> cf_gp) & cf_gm); }
        uint8_t mask() const {
            static constexpr uint8_t MASK[] PROGMEM = {
                    0x00,  // CF_00 = 0
                    0x30,  // CF_30 = 1
                    0xFF,  // CF_FF = 1
            };
            return pgm_read_byte(MASK + format());
        }

    private:
        static constexpr int opr1_gp = 0;
        static constexpr int opr2_gp = 4;
        static constexpr int opr3_gp = 8;
        static constexpr int cf_gp = 12;
        static constexpr int post_gp = 0;
        static constexpr uint_fast8_t opr1_gm = 0x0F;
        static constexpr uint_fast8_t opr2_gm = 0x0F;
        static constexpr uint_fast8_t opr3_gm = 0x0F;
        static constexpr uint_fast8_t cf_gm = 0x0F;
        static constexpr uint_fast8_t post_gm = 0xFF;
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace mc68hc16
}  // namespace libasm

#endif  // __LIBASM_ENTRY_MC68HC16_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
