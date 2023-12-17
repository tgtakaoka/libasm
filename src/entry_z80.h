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

#ifndef __LIBASM_ENTRY_Z80_H__
#define __LIBASM_ENTRY_Z80_H__

#include <stdint.h>

#include "config_z80.h"
#include "entry_base.h"
#include "reg_z80.h"

namespace libasm {
namespace z80 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_IM8 = 1,    // Immediate 8-bit: nn
    M_IM16 = 2,   // Immediate 16-bit: nnnn
    M_ABS = 3,    // Direct address 16-bit: (nnnn)
    M_IOA = 4,    // I/O address 8-bit: (nn)
    M_REL = 5,    // PC-Relative: nnnn
    M_IDX = 6,    // Indexed: (IX/IY+nn)
    M_CC4 = 7,    // |...|cc|...|: NZ/Z/NC/C
    M_CC8 = 8,    // |..|ccc|...|: NZ/Z/NC/C/PO/PE/P/M
    M_R16 = 9,    // |..|pp|....|: BC/DE/HL/SP
    M_NOHL = 10,  // |..|pp|....|: BC/DE/__/SP
    M_R16X = 11,  // |..|pp|....|: BC/DE/Ix/SP
    M_STK = 12,   // |..|pp|....|: BC/DE/HL/AF
    M_SRC = 13,   // |......|rrr|: B/C/D/E/H/L/(HL)/A
    M_DST = 14,   // |..|rrr|...|: B/C/D/E/H/L/(HL)/A
    M_SR8 = 15,   // |......|rrr|: B/C/D/E/H/L/___/A
    M_DR8 = 16,   // |..|rrr|...|: B/C/D/E/H/L/___/A
    M_VEC = 17,   // |..|vvv|...|: vector 0~7
    M_BIT = 18,   // |..|bbb|...|: bit 0-7
    M_IMMD = 19,  // |...|mm|...|: interrupt mode 0-2
    T_IXB = 20,   // DD/FD CB nn xx
    I_PTR = 21,   // |...|i|....|: (BC)/(DE)
    I_IDX = 22,   // (IX)/(IY)
    R_IDX = 26,   // IX/IY
    R_BASE = 27,
    R_BC = R_BASE + REG_BC,
    R_DE = R_BASE + REG_DE,
    R_HL = R_BASE + REG_HL,
    R_SP = R_BASE + REG_SP,
    R_AF = R_BASE + REG_AF,
    R_I = R_BASE + REG_I,
    R_R = R_BASE + REG_R,
    R_AFP = R_BASE + REG_AFP,
    R_C = R_BASE + REG_C,  // or CC_C
    R_A = R_BASE + REG_A,
    R_IM = R_BASE + REG_IM,  // i8085
    I_BASE = R_BASE + 18,
    I_HL = I_BASE + REG_HL,  // (HL)
    I_SP = I_BASE + REG_SP,  // (SP)
    I_C = I_BASE + REG_C,    // (C)
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint8_t _dst;
        uint8_t _src;

        static constexpr Flags create(AddrMode dst, AddrMode src) {
            return Flags{Entry::dst(dst, false), Entry::src(src)};
        }

        static constexpr Flags ixbit(AddrMode dst, AddrMode src) {
            return Flags{Entry::dst(dst, true), Entry::src(src)};
        }

        Flags read() const { return Flags{pgm_read_byte(&_dst), pgm_read_byte(&_src)}; }
        AddrMode dst() const { return AddrMode(_dst & mode_gm); }
        AddrMode src() const { return AddrMode(_src & mode_gm); }
        bool indexBit() const { return _dst & ixbit_bm; }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : Base(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    const Flags _flags;

    static constexpr uint8_t dst(AddrMode mode, bool ixbit) {
        return static_cast<uint8_t>(static_cast<uint8_t>(mode) | (ixbit ? ixbit_bm : 0));
    }
    static constexpr uint8_t src(AddrMode mode) {
        return static_cast<uint8_t>(static_cast<uint8_t>(mode));
    }

    static constexpr uint8_t mode_gm = 0x7F;
    // |_dst|
    static constexpr int ixbit_bp = 7;
    static constexpr uint8_t ixbit_bm = (1 << ixbit_bp);
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
