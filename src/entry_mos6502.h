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

#ifndef __ENTRY_MOS6502_H__
#define __ENTRY_MOS6502_H__

#include "config_mos6502.h"
#include "entry_base.h"

namespace libasm {
namespace mos6502 {

enum CpuType : uint8_t {
    MOS6502,
    G65SC02,
    R65C02,
    W65C02S,
    W65C816,
};

enum AddrMode : uint8_t {
    // MOS6502
    IMPL = 0,           // Implied
    ACCM = 1,           // Accumulator A
    IMMA = 2,           // Immediate Accumulator: #nn
    IMMX = 3,           // Immediate Index register: #nn
    ABS = 4,            // Absolute: abs
    ZPG = 5,            // Zero Page: zp
    ZPG_IDX = 6,        // Zero Page Indexed: zp,X
    ZPG_IDY = 7,        // Zero Page Indexed: zp,Y
    ABS_IDX = 8,        // Absolute Indexed: abs,X
    ABS_IDY = 9,        // Absolute Indexed: abs,Y
    REL = 10,           // Relative: abs
    ZPG_IDX_IDIR = 11,  // Indexed Indirect: (zp,X)
    ZPG_IDIR_IDY = 12,  // Indirect Indexed: (zp),Y
    ABS_IDIR = 13,      // Absolute Indirect: (abs)

    // G65SC02
    ABS_IDX_IDIR = 14,  // Indexed Absolute Indirect: (abs,X)
    ZPG_IDIR = 15,      // Zero Page Indirect: (zp)
    ZPG_REL = 16,       // Zero Page Relative: zp,abs

    // W65C816
    ABS_LONG = 17,           // Absolute Long: al
    ABS_LONG_IDX = 18,       // Absolute Long Indexed: al,x
    ABS_IDIR_LONG = 19,      // Absolute Indirect Long: [abs]
    REL_LONG = 20,           // Relative Long: rl
    SP_REL = 21,             // Stack Relative: off,s
    SP_REL_IDIR_IDY = 22,    // Stack Relative Indirect Indexed: (off,s),y
    ZPG_IDIR_LONG = 23,      // Zero Page Indirect Long: [zp]
    ZPG_IDIR_LONG_IDY = 24,  // Zero Page Indirect Long Indexed: [zp],y
    BLOCK_MOVE = 25,         // Block Move: ss,dd
    IMM8 = 26,               // Immediate Byte.
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint8_t _attr;

        static constexpr Flags create(AddrMode mode) {
            return Flags{static_cast<uint8_t>(mode)};
        }
        Flags read() const { return Flags{pgm_read_byte(&_attr)}; }

        AddrMode mode() const { return AddrMode(_attr); }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;
};

}  // namespace mos6502
}  // namespace libasm

#endif  // __ENTRY_MOS6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
