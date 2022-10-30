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

#include <stdint.h>

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
    M_NONE = 0,           // Implied
    M_ACCM = 1,           // Accumulator A
    M_IMMA = 2,           // Immediate Accumulator: #nn
    M_IMMX = 3,           // Immediate Index register: #nn
    M_ABS = 4,            // Absolute: abs
    M_ZPG = 5,            // Zero Page: zp
    M_ZPG_IDX = 6,        // Zero Page Indexed: zp,X
    M_ZPG_IDY = 7,        // Zero Page Indexed: zp,Y
    M_ABS_IDX = 8,        // Absolute Indexed: abs,X
    M_ABS_IDY = 9,        // Absolute Indexed: abs,Y
    M_REL = 10,           // Relative: abs
    M_ZPG_IDX_IDIR = 11,  // Indexed Indirect: (zp,X)
    M_ZPG_IDIR_IDY = 12,  // Indirect Indexed: (zp),Y
    M_ABS_IDIR = 13,      // Absolute Indirect: (abs)

    // G65SC02
    M_ABS_IDX_IDIR = 14,  // Indexed Absolute Indirect: (abs,X)
    M_ZPG_IDIR = 15,      // Zero Page Indirect: (zp)
    M_ZPG_REL = 16,       // Zero Page Relative: zp,abs

    // W65C816
    M_ABS_LONG = 17,           // Absolute Long: al
    M_ABS_LONG_IDX = 18,       // Absolute Long Indexed: al,x
    M_ABS_IDIR_LONG = 19,      // Absolute Indirect Long: [abs]
    M_REL_LONG = 20,           // Relative Long: rl
    M_SP_REL = 21,             // Stack Relative: off,s
    M_SP_REL_IDIR_IDY = 22,    // Stack Relative Indirect Indexed: (off,s),y
    M_ZPG_IDIR_LONG = 23,      // Zero Page Indirect Long: [zp]
    M_ZPG_IDIR_LONG_IDY = 24,  // Zero Page Indirect Long Indexed: [zp],y
    M_BLOCK_MOVE = 25,         // Block Move: ss,dd
    M_IMM8 = 26,               // Immediate Byte.
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint8_t _attr;

        static constexpr Flags create(AddrMode mode) { return Flags{static_cast<uint8_t>(mode)}; }
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
