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
    M_IM8 = 1,     // Immediate 8-bit: nn
    M_IM16 = 2,    // Immediate 16-bit: nnnn
    M_ABS = 3,     // Direct address 16-bit: (nnnn)
    M_IOA = 4,     // I/O address 8-bit: (nn)
    M_REL8 = 5,    // 8-bit PC-Relative: nnnn
    M_IDX8 = 6,    // Indexed: DD:(IX+d8) ED:(IY+d8)
    M_CC4 = 7,     // |...|cc|...|: NZ/Z/NC/C
    M_CC8 = 8,     // |..|ccc|...|: NZ/Z/NC/C/PO/PE/P/M
    M_R16 = 9,     // |..|pp|....|: BC/DE/HL/SP
    M_NOHL = 10,   // |..|pp|....|: BC/DE/__/SP
    M_R16X = 11,   // |..|pp|....|: BC/DE/Ix/SP
    M_STK = 12,    // |..|pp|....|: BC/DE/HL/AF
    M_SRC = 13,    // |......|rrr|: B/C/D/E/H/L/(HL)/A
    M_DST = 14,    // |..|rrr|...|: B/C/D/E/H/L/(HL)/A
    M_SR8 = 15,    // |......|rrr|: B/C/D/E/H/L/_/A
    M_DR8 = 16,    // |..|rrr|...|: B/C/D/E/H/L/_/A
    M_VEC = 17,    // |..|vvv|...|: vector 0~7
    M_BIT = 18,    // |..|bbb|...|: bit 0-7
    M_IMMD = 19,   // |...|mm|...|: interrupt mode 0-3
    I_PTR = 21,    // |...|i|....|: (BC)/(DE)
    I_IDX = 22,    // DD:(IX) FD:(IY)
    R_DXY = 23,    // |....|x|...|: 0=IXH/IYH/IXU/IYU 1=IXL/IYL
    R_SXY = 24,    // |........|x|: 0=IXH/IYH/IXU/IYU 1=IXL/IYL
    M_FIDX = 25,   // |..|eee|...|: M_BIXH + M_PIXH
    M_BIXH = 26,   // |...|ee|...|: 0=(SP+d16) 1=(HL+IX) 2=(HL+IY) 3=(IX+IY)
    M_PIXH = 27,   // |...|ee|...|: 0=<r16> 1=(IX+d16) 2=(IY+d16) 3=(HL+d16)
    M_BIXL = 28,   // |.......|ee|: 0=(SP+d16) 1=(HL+IX) 2=(HL+IY) 3=(IX+IY)
    M_PIXL = 29,   // |.......|ee|: 0=<r16> 1=(IX+d16) 2=(IY+d16) 3=(HL+d16)
    M_MPTR = 30,   // |...|e|....|: 0=(HL) 1=(a16)
    M_IDX16 = 31,  // |...|e|....|: 0=(IX+d16) 1=(IY+d16)
    M_SR8X = 32,   // |......|rrr|: B/C/D/E/_/_/_/A
    M_DR8X = 33,   // |..|rrr|...|: B/C/D/E/_/_/_/A
    M_IDX = 34,    // Accept M_IDX8 and M_IDX16
    R_IDX = 35,    // DD:IX, FD:IY
    M_SPX = 36,    // Stack pointer relative: (SP+d8)
    M_REL16 = 37,  // 16-bit PC-Relative: <r16>, 00:8-bit DD:16-bit FD:24-bit
    M_PDX = 38,    // Pointer index: (HL+d16)/(IX+d16)/(IY+d16)
    M_SPDX = 39,   // Stack index: (SP+d16)
    M_PCDX = 40,   // Program counter relative: (PC+d16)
    M_BDX = 41,    // Base and index: (HL+IX)/(HL+IY)/(IX+IY)
    M_EPU = 42,    // EPU 4-byte tmeplate
    I_PTRL = 43,   // |........|p|: 0=(BC) 1=(DE)
    R_PTRL = 44,   // |........|p|: 0=BC 1=DE
    R_PTRH = 45,   // |...|p|....|: 0=BC 1=DE
    M_REL24 = 46,  // 24-bit PC-Relative: ED:8-bit DD:16-bit FD:24-bit
    R_IDXL = 47,   // |........|y|: 0=IX 1=IY
    M_DD = 48,     // |.......|dd|: Decoder Directive
    M_IO16 = 49,   // 16-bit I/O address: (nnnn)
    M_LW = 50,     // Control bit LW
    M_LCK = 51,    // Control bit LCK
    M_XM = 52,     // Control bit XM
    R_ALT = 53,    // Alternate register
    I_BASE = 54,
    I_HL = I_BASE + REG_HL,  // (HL)
    I_SP = I_BASE + REG_SP,  // (SP)
    I_C = I_BASE + REG_C,    // (C)
    R_BASE = 66,
    R_BC = R_BASE + REG_BC,
    R_DE = R_BASE + REG_DE,
    R_HL = R_BASE + REG_HL,
    R_IX = R_BASE + REG_IX,
    R_IY = R_BASE + REG_IY,
    R_AF = R_BASE + REG_AF,
    R_C = R_BASE + REG_C,  // or CC_C
    R_A = R_BASE + REG_A,
    R_H = R_BASE + REG_H,
    R_L = R_BASE + REG_L,
    R_SP = R_BASE + REG_SP,
    R_I = R_BASE + REG_I,
    R_R = R_BASE + REG_R,
    R_DEHL = R_BASE + REG_DEHL,
    R_USP = R_BASE + REG_USP,
    R_SR = R_BASE + REG_SR,
    R_DSR = R_BASE + REG_DSR,
    R_XSR = R_BASE + REG_XSR,
    R_YSR = R_BASE + REG_YSR,
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint8_t _dst;
        uint8_t _src;
        uint8_t _mask;

        static constexpr Flags create(uint8_t mask, AddrMode dst, AddrMode src) {
            return Flags{dst, src, mask};
        }

        AddrMode dst() const { return AddrMode(_dst); }
        AddrMode src() const { return AddrMode(_src); }
        uint8_t mask() const { return _mask; }
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const {
        return Flags{pgm_read_byte(&_flags_P._dst), pgm_read_byte(&_flags_P._src),
                pgm_read_byte(&_flags_P._mask)};
    }

private:
    const Flags _flags_P;
};

}  // namespace z80
}  // namespace libasm

#endif  // __LIBASM_ENTRY_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
