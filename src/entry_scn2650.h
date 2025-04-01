/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __LIBASM_ENTRY_SCN2650_H__
#define __LIBASM_ENTRY_SCN2650_H__

#include <stdint.h>

#include "config_scn2650.h"
#include "entry_base.h"

namespace libasm {
namespace scn2650 {

enum AddrMode : uint8_t {
    // mode2
    M_NONE = 0,
    M_IMM8 = 1,  // v: Immediate, 8 bit
    M_REL7 = 2,  // (*)a: Relative, 7 bit, optionally indirect
    M_AB13 = 3,  // (*)a: Absolute, 13 bit, optionally indirect
    M_AB15 = 4,  // (*)a: Absolute jump, 15 bit, optionally indirect
    M_IX13 = 5,  // (*)a,rx(,[+-]): Indexed, 13bit (indirect and auto increment/decrement)
    // mode1, mode2
    M_REGN = 6,   // r0~r3
    M_REG0 = 7,   // r0
    M_R123 = 8,   // r1~R3
    M_CCVN = 9,   // eq/gt/lt/un: Condition code
    M_C012 = 10,  // eq/gt/lt: Condition code
    M_ABS7 = 11,  // (*)a: First page relative. 7 bit. optionally indirect.
    M_IX15 = 12,  // (*)a,r3: Indexed with r3, 15bit, optionally indirect
};

enum CodeFormat : uint8_t {
    CF_00 = 0,  // 0x00
    CF_03 = 1,  // 0x03
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint8_t _attr;

        static constexpr Flags create(CodeFormat cf, AddrMode opr1, AddrMode opr2) {
            return Flags{static_cast<uint8_t>((static_cast<uint8_t>(opr1) << mode1_gp) |
                                              (static_cast<uint8_t>(opr2) << mode2_gp) |
                                              (static_cast<uint8_t>(cf) << cf_gp))};
        }

        AddrMode mode1() const { return AddrMode((_attr >> mode1_gp) & mode1_gm); }
        AddrMode mode2() const { return AddrMode((_attr >> mode2_gp) & mode2_gm); }
        uint8_t mask() const {
            static constexpr uint8_t MASK[] PROGMEM = {
                    0x00,  // CF_00 = 0
                    0x03,  // CF_03 = 1
            };
            return pgm_read_byte(&MASK[(_attr >> cf_gp) & cf_gm]);
        }

    private:
        static constexpr int mode1_gp = 0;
        static constexpr int mode2_gp = 4;
        static constexpr int cf_gp = 7;
        static constexpr uint8_t mode1_gm = 0xF;
        static constexpr uint8_t mode2_gm = 0x7;
        static constexpr uint8_t cf_gm = 0x1;
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opCode), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_byte(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace scn2650
}  // namespace libasm

#endif  // __LIBASM_ENTRY_SCN2650_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
