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

#ifndef __ENTRY_MC6809_H__
#define __ENTRY_MC6809_H__

#include "config_mc6809.h"
#include "entry_base.h"

namespace libasm {
namespace mc6809 {

enum CpuType : uint8_t {
    MC6809,
    HD6309,
};

enum AddrMode : uint8_t {
    NONE = 0,
    DIR = 1,       // Direct Page
    EXT = 2,       // Extended
    IDX = 3,       // Indexed
    REL = 4,       // Relative
    LREL = 5,      // Long Relative
    IM8 = 6,       // Immediate 8-bit
    IM16 = 7,      // Immediate 16-bit
    IM32 = 8,      // Immediate 32-bit
    REG_REG = 9,   // Register pair
    REGLIST = 10,  // Register list
    REG_BIT = 11,  // Register bit
    DIR_BIT = 12,  // Direct Page bit
    REG_TFM = 13,  // Transfer Memory Register
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint8_t _attr;

        static constexpr Flags create(AddrMode op1, AddrMode op2) {
            return Flags{static_cast<uint8_t>(
                    (static_cast<uint8_t>(op1) << op1_gp) |
                    (static_cast<uint8_t>(op2) << op2_gp))};
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

    static constexpr uint8_t mode_gm = 0xf;
    static constexpr int op1_gp = 0;
    static constexpr int op2_gp = 4;
};

}  // namespace mc6809
}  // namespace libasm

#endif  // __ENTRY_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
