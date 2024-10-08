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

#ifndef __LIBASM_ENTRY_CDP1802_H__
#define __LIBASM_ENTRY_CDP1802_H__

#include <stdint.h>

#include "config_cdp1802.h"
#include "entry_base.h"

namespace libasm {
namespace cdp1802 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_REGN = 1,  // Register number, 0~15
    M_REG1 = 2,  // Register number, 1~15
    M_IMM8 = 3,  // Immediate, 8 bit
    M_PAGE = 4,  // Page offset, 8 bit
    M_ADDR = 5,  // Absolute address, 16 bit
    M_SHRT = 6,  // Page offser, has M_LONG counterpart
    M_LONG = 7,  // Absolute address, has M_SHRT counterpar
    M_IOAD = 8,  // IO address, 1~7
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint8_t _attr;

        static constexpr Flags create(AddrMode opr1, AddrMode opr2) {
            return Flags{static_cast<uint8_t>((static_cast<uint8_t>(opr1) << opr1_gp) |
                                              (static_cast<uint8_t>(opr2) << opr2_gp))};
        }

        AddrMode mode1() const { return AddrMode((_attr >> opr1_gp) & mode_gm); }
        AddrMode mode2() const { return AddrMode((_attr >> opr2_gp) & mode_gm); }

    private:
        static constexpr int opr1_gp = 0;
        static constexpr int opr2_gp = 4;
        static constexpr uint8_t mode_gm = 0xF;
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opCode), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_byte(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace cdp1802
}  // namespace libasm

#endif  // __LIBASM_ENTRY_CDP1802_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
