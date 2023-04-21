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

#ifndef __ENTRY_SCN2650_H__
#define __ENTRY_SCN2650_H__

#include <stdint.h>

#include "config_scn2650.h"
#include "entry_base.h"

namespace libasm {
namespace scn2650 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_REGN = 1,   // r0~r3; means Undefined when in mode2
    M_REG0 = 2,   // r0
    M_R123 = 3,   // r1~R3
    M_IMM8 = 4,   // v: Immediate, 8 bit
    M_CCVN = 5,   // eq/gt/lt/un: Condition code
    M_C012 = 6,   // eq/gt/lt: Condition code
    M_REL7 = 7,   // (*)a: Relative, 7 bit, optionally indirect
    M_ABS7 = 8,   // (*)a: First page relative. 7 bit. optionally indirect.
    M_AB13 = 9,   // (*)a: Absolute, 13 bit, optionally indirect
    M_IX13 = 10,  // (*)a,rx(,[+-]): Indexed, 13bit (indirect and auto increment/decrement)
    M_IX15 = 11,  // (*)a,r3: Indexed with r3, 15bit, optionally indirect
    M_AB15 = 12,  // (*)a: Absolute jump, 15 bit, optionally indirect
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint8_t _attr;

        static constexpr Flags create(AddrMode opr1, AddrMode opr2) {
            return Flags{static_cast<uint8_t>((static_cast<uint8_t>(opr1) << opr1_gp) |
                                              (static_cast<uint8_t>(opr2) << opr2_gp))};
        }

        static constexpr Flags undef() {
            return Flags{static_cast<uint8_t>((static_cast<uint8_t>(M_NONE) << opr1_gp) |
                                              (static_cast<uint8_t>(M_REGN) << opr2_gp))};
        }

        Flags read() const { return Flags{pgm_read_byte(&_attr)}; }
        AddrMode mode1() const { return AddrMode((_attr >> opr1_gp) & mode_gm); }
        AddrMode mode2() const { return AddrMode((_attr >> opr2_gp) & mode_gm); }
        bool undefined() const { return mode2() == M_REGN; }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : Base(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr int opr1_gp = 0;
    static constexpr int opr2_gp = 4;
    static constexpr uint8_t mode_gm = 0x0F;
};

}  // namespace scn2650
}  // namespace libasm

#endif  // __ENTRY_SCN2650_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
