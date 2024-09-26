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

#ifndef __LIBASM_ENTRY_MC6809_H__
#define __LIBASM_ENTRY_MC6809_H__

#include <stdint.h>

#include "config_mc6809.h"
#include "entry_base.h"

namespace libasm {
namespace mc6809 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_DIR = 1,     // Direct Page
    M_EXT = 2,     // Extended
    M_IDX = 3,     // Indexed
    M_REL = 4,     // Relative
    M_LREL = 5,    // Long Relative
    M_IM8 = 6,     // Immediate 8-bit
    M_IM32 = 7,    // Immediate 32-bit
    M_PAIR = 8,    // Register pair
    M_LIST = 9,    // Register list; means Undefined when in mode2
    M_RBIT = 10,   // Register bit
    M_DBIT = 11,   // Direct Page bit
    M_RTFM = 12,   // Transfer Memory Register
    M_GEN8 = 13,   // Generic: M_IM8/M_DIR/M_IDX/M_EXT
    M_GEN16 = 14,  // Generic: M_IM16/M_DIR/M_IDX/M_EXT
    M_GMEM = 15,   // Generic memory: M_DIR/M_IDX/M_EXT
};

enum OprSize : uint8_t {
    SZ_NONE = Size::SZ_NONE,
    SZ_BYTE = Size::SZ_BYTE,
    SZ_WORD = Size::SZ_WORD,
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint8_t _attr;

        static constexpr Flags create(AddrMode opr1, AddrMode opr2) {
            return Flags{static_cast<uint8_t>((static_cast<uint8_t>(opr1) << opr1_gp) |
                                              (static_cast<uint8_t>(opr2) << opr2_gp))};
        }

        static constexpr Flags undef(AddrMode opr1) {
            return Flags{static_cast<uint8_t>((static_cast<uint8_t>(opr1) << opr1_gp) |
                                              (static_cast<uint8_t>(M_LIST) << opr2_gp))};
        }

        AddrMode mode1() const { return AddrMode((_attr >> opr1_gp) & mode_gm); }
        AddrMode mode2() const { return AddrMode((_attr >> opr2_gp) & mode_gm); }
        bool undefined() const { return mode2() == M_LIST; }

    private:
        static constexpr int opr1_gp = 0;
        static constexpr int opr2_gp = 4;
        static constexpr uint8_t mode_gm = 0xf;
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opCode), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_byte(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace mc6809
}  // namespace libasm

#endif  // __LIBASM_ENTRY_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
