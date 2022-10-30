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

#ifndef __ENTRY_Z80_H__
#define __ENTRY_Z80_H__

#include <stdint.h>

#include "config_z80.h"
#include "entry_base.h"

namespace libasm {
namespace z80 {

enum CpuType : uint8_t {
    Z80,
    I8080,
    I8085,
    V30EMU,
};

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_IM8 = 1,       // Immediate 8-bit: nn
    M_IM16 = 2,      // Immediate 16-bit: nnnn
    M_ABS = 3,       // Direct address 16-bit: (nnnn)
    M_IOA = 4,       // I/O address 8-bit: (nn)
    M_REL = 5,       // PC-Relative: nnnn
    M_INDX = 6,      // Indexed: (IX/IY+nn)
    M_CC4 = 7,       // |...|cc|...|: NZ/Z/NC/C
    M_CC8 = 8,       // |..|ccc|...|: NZ/Z/NC/C/PO/PE/P/M
    M_PTR = 9,       // |..|pp|....|: BC/DE/HL/SP
    M_PIX = 10,      // |..|pp|....|: BC/DE/Ix/SP
    M_STK = 11,      // |..|pp|....|: BC/DE/HL/AF
    I_BCDE = 12,     // |...|i|....|: (BC)/(DE)
    M_REG = 13,      // |......|rrr|: B/C/D/E/H/L/(HL)/A
    M_DST = 14,      // |..|rrr|...|: B/C/D/E/H/L/(HL)/A
    M_VEC = 15,      // |..|vvv|...|: vector 0~7
    M_BIT = 17,      // |..|bbb|...|: bit 0-7
    M_IMMD = 18,     // |...|mm|...|: interrupt mode 0-2
    R_IR = 19,       // |....|r|...|: I/R
    I_HL = 20,       // (HL)
    I_SP = 21,       // (SP)
    I_IXIY = 22,     // (IX)/(IY)
    I_C = 23,        // (C)
    T_IXB = 24,      // DD/FD CB nn xx
    R_IXIY = 25,     // IX/IY
    R_BC = 26 + 0,   // REG_BC
    R_DE = 26 + 1,   // REG_DE
    R_HL = 26 + 2,   // REG_HL
    R_SP = 26 + 3,   // REG_SP
    R_AF = 26 + 6,   // REG_AF
    R_AFP = 26 + 7,  // REG_AFP
    R_C = 26 + 8,    // REG_C  or Carry-condition
    R_A = 26 + 15,   // REG_A
    R_IM = 26 + 18,  // REG_IM (8085)
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint8_t _dst;
        uint8_t _src;

        static constexpr Flags create(AddrMode dst, AddrMode src) {
            return Flags{Entry::dst(dst, false), Entry::src(src, false)};
        }

        static constexpr Flags ixbit(AddrMode dst, AddrMode src) {
            return Flags{Entry::dst(dst, true), Entry::src(src, false)};
        }

        static constexpr Flags undef(AddrMode dst, AddrMode src) {
            return Flags{Entry::dst(dst, false), Entry::src(src, true)};
        }

        Flags read() const { return Flags{pgm_read_byte(&_dst), pgm_read_byte(&_src)}; }
        AddrMode dst() const { return AddrMode(_dst & mode_gm); }
        AddrMode src() const { return AddrMode(_src & mode_gm); }
        bool indexBit() const { return _dst & ixbit_bm; }
        bool undefined() const { return _src & undef_bm; }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr uint8_t dst(AddrMode mode, bool ixbit) {
        return static_cast<uint8_t>(static_cast<uint8_t>(mode) | (ixbit ? ixbit_bm : 0));
    }
    static constexpr uint8_t src(AddrMode mode, bool undef) {
        return static_cast<uint8_t>(static_cast<uint8_t>(mode) | (undef ? undef_bm : 0));
    }

    static constexpr uint8_t mode_gm = 0x7F;
    // |_dst|
    static constexpr int ixbit_bp = 7;
    static constexpr uint8_t ixbit_bm = (1 << ixbit_bp);
    // |_src|
    static constexpr int undef_bp = 7;
    static constexpr uint8_t undef_bm = (1 << undef_bp);
};

}  // namespace z80
}  // namespace libasm

#endif  // __ENTRY_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
