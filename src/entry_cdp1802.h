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
    M_REGN = 1,       // Register number, 0~15
    M_REG1 = 2,       // Register number, 1~15
    M_IMM8 = 3,       // Immediate, 8 bit
    M_PAGE = 4,       // Page offset, 8 bit
    M_ADDR = 5,       // Absolute address, 16 bit
    M_SHRT = 6,       // Page offser, has M_LONG counterpart
    M_LONG = 7,       // Absolute address, has M_SHRT counterpar
    M_IOAD = 8,       // IO address, 1~7
    M_REGN_ADDR = 9,  // M_REGN, M_ADDR
};

enum CodeFormat : uint8_t {
    CF_00 = 0,  // 0x00
    CF_07 = 1,  // 0x07
    CF_0F = 2,  // 0x0F
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint8_t _attr;

        static constexpr Flags create(CodeFormat cf, AddrMode mode) {
            return Flags{static_cast<uint8_t>(mode | (cf << cf_gp))};
        }

        AddrMode mode1() const { return mode() == M_REGN_ADDR ? M_REGN : mode(); }
        AddrMode mode2() const { return mode() == M_REGN_ADDR ? M_ADDR : M_NONE; }
        uint8_t mask() const {
            static constexpr uint8_t MASK[] PROGMEM = {
                    0x00,  // CF_00
                    0x07,  // CF_07
                    0x0F,  // CF_0F
            };
            return pgm_read_byte(&MASK[(_attr >> cf_gp) & cf_gm]);
        }

    private:
        AddrMode mode() const { return AddrMode((_attr >> mode_gp) & mode_gm); }

        static constexpr int mode_gp = 0;
        static constexpr int cf_gp = 4;
        static constexpr uint8_t mode_gm = 0xF;
        static constexpr uint8_t cf_gm = 0x3;
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

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
