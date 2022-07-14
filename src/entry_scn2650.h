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
    NONE = 0,
    REGN = 1,  // r0~r3
    REG0 = 2,  // r0
    R123 = 3,  // r1~R3
    IMM8 = 4,  // v: Immediate, 8 bit
    CCVN = 5,  // eq/gt/lt/un: Condition code
    C012 = 6,  // eq/gt/lt: Condition code
    REL7 = 7,  // (*)a: Relative, 7 bit, optionally indirect
    ABS7 = 8,  // (*)a: First page relative. 7 bit. optionally indirect.
    AB13 = 9,   // (*)a: Absolute, 13 bit, optionnally indirect
    IX13 = 10,  // (*)a,rx(,[+-]): Indexed, 13bit, optionally indirect and auto increment/decrement
    IX15 = 11,  // (*)a,r3: Indexed with r3, 15bit, optionally indirect
    AB15 = 12,  // (*)a: Absolute jump, 15 bit, optionally indirect
    UNDF = 15,  // Undefined instruction
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint8_t _attr;

        static constexpr Flags create(AddrMode op1, AddrMode op2) {
            return Flags{static_cast<uint8_t>(
                    (static_cast<uint8_t>(op1) << op1_gp) | (static_cast<uint8_t>(op2) << op2_gp))};
        }
        Flags read() const { return Flags{pgm_read_byte(&_attr)}; }

        AddrMode mode1() const { return AddrMode((_attr >> op1_gp) & mode_gm); }
        AddrMode mode2() const { return AddrMode((_attr >> op2_gp) & mode_gm); }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr int op1_gp = 0;
    static constexpr int op2_gp = 4;
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
