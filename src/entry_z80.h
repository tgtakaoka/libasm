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
namespace common {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_IM8 = 1,    // Immediate 8-bit: nn
    M_IM16 = 2,   // Immediate 16-bit: nnnn
    M_DM16 = 3,   // Immediate 16-bit: nnnn
    M_LM16 = 4,   // Immediate 16-bit: nnnn
    M_ABS = 5,    // Direct address 16-bit: (nnnn)
    M_JABS = 6,   // Direct address 16-bit:  nnnn
    M_DABS = 7,   // Direct address 16-bit: (nnnn)
    M_IOA = 8,    // I/O address 8-bit: (nn)
    M_REL8 = 9,   // 8-bit PC-Relative: nnnn
    M_IDX8 = 10,  // Indexed: DD:(IX+d8) ED:(IY+d8)
    M_CC4 = 11,   // |...|cc|...|: NZ/Z/NC/C
    M_CC8 = 12,   // |..|ccc|...|: NZ/Z/NC/C/PO/PE/P/M
    M_R16 = 13,   // |..|pp|....|: BC/DE/HL/SP
    M_R16X = 14,  // |..|pp|....|: BC/DE/Ix/SP
    M_SRC = 15,   // |......|rrr|: B/C/D/E/H/L/(HL)/A
    M_DST = 16,   // |..|rrr|...|: B/C/D/E/H/L/(HL)/A
    M_SR8 = 17,   // |......|rrr|: B/C/D/E/H/L/_/A
    M_DR8 = 18,   // |..|rrr|...|: B/C/D/E/H/L/_/A
    M_VEC = 19,   // |..|vvv|...|: vector 0~7
    M_BIT = 20,   // |..|bbb|...|: bit 0-7
    M_IMMD = 21,  // |...|mm|...|: interrupt mode 0-3
    I_PTR = 22,   // |...|i|....|: (BC)/(DE)
    R_PTRH = 23,  // |...|p|....|: 0=BC 1=DE
    M_IDX = 24,   // Accept M_IDX8 and M_IDX16
    I_IDX = 25,   // DD:(IX) FD:(IY)
    R_IDX = 26,   // DD:IX, FD:IY
    R_ALT = 27,   // Alternate register
    // Z280
    M_FIDX = 28,   // |..|eee|...|: M_BIXH + M_PIXH
    M_BIXH = 29,   // |...|ee|...|: 0=(SP+d16) 1=(HL+IX) 2=(HL+IY) 3=(IX+IY)
    M_PIXH = 30,   // |...|ee|...|: 0=<r16> 1=(IX+d16) 2=(IY+d16) 3=(HL+d16)
    M_BIXL = 31,   // |.......|ee|: 0=(SP+d16) 1=(HL+IX) 2=(HL+IY) 3=(IX+IY)
    M_PIXL = 32,   // |.......|ee|: 0=<r16> 1=(IX+d16) 2=(IY+d16) 3=(HL+d16)
    M_MPTR = 33,   // |...|e|....|: 0=(HL) 1=(a16)
    M_IDX16 = 34,  // |...|e|....|: 0=(IX+d16) 1=(IY+d16)
    M_PDX = 35,    // Pointer index: (HL+d16)/(IX+d16)/(IY+d16)
    M_SPDX = 36,   // Stack index: (SP+d16)
    M_PCDX = 37,   // Program counter relative: (PC+d16)
    M_BDX = 38,    // Base and index: (HL+IX)/(HL+IY)/(IX+IY)
    M_EPU = 39,    // EPU 4-byte tmeplate
    // Z280/Z380
    M_SR8X = 40,   // |......|rrr|: B/C/D/E/_/_/_/A
    M_DR8X = 41,   // |..|rrr|...|: B/C/D/E/_/_/_/A
    R_SXY = 42,    // |........|x|: 0=IXH/IYH/IXU/IYU 1=IXL/IYL
    R_DXY = 43,    // |....|x|...|: 0=IXH/IYH/IXU/IYU 1=IXL/IYL
    M_REL16 = 44,  // 16-bit PC-Relative: <r16>, 00:8-bit DD:16-bit FD:24-bit
    // Z380
    I_PTRL = 45,   // |........|p|: 0=(BC) 1=(DE)
    R_PTRL = 46,   // |........|p|: 0=BC 1=DE
    R_IDXL = 47,   // |........|y|: 0=IX 1=IY
    M_DD = 48,     // |.......|dd|: Decoder Directive
    M_IO16 = 49,   // 16-bit I/O address: (nnnn)
    M_SPX = 50,    // Stack pointer relative: (SP+d8)
    M_REL24 = 51,  // 24-bit PC-Relative: ED:8-bit DD:16-bit FD:24-bit
    M_LW = 52,     // Control bit LW
    M_LCK = 53,    // Control bit LCK
    M_XM = 54,     // Control bit XM
    M_XM16 = 55,   // Immediate 16-bit: nnnn
    M_XABS = 56,   // Direct address 16-bit: (nnnn)
    // Z80/Z180/Z280/Z380
    I_BASE = 57,
    I_HL = I_BASE + REG_HL,  // (HL)
    I_SP = I_BASE + REG_SP,  // (SP)
    I_C = I_BASE + REG_C,    // (C)
    R_BASE = I_C + 1,
    R_BC = R_BASE + REG_BC,
    R_DE = R_BASE + REG_DE,
    R_HL = R_BASE + REG_HL,
    R_SP = R_BASE + REG_SP,
    R_IX = R_BASE + REG_IX,
    R_IY = R_BASE + REG_IY,
    R_AF = R_BASE + REG_AF,
    R_C = R_BASE + REG_C,  // or CC_C
    R_H = R_BASE + REG_H,
    R_L = R_BASE + REG_L,
    R_A = R_BASE + REG_A,
    R_I = R_BASE + REG_I,
    R_R = R_BASE + REG_R,
    // Z280
    R_DEHL = R_BASE + REG_DEHL,
    R_USP = R_BASE + REG_USP,
    // Z380
    R_SR = R_BASE + REG_SR,
    R_DSR = R_BASE + REG_DSR,
    R_XSR = R_BASE + REG_XSR,
    R_YSR = R_BASE + REG_YSR,
};

struct Entry final : entry::Base<Config::opcode_t> {
    // opecode preifx
    static constexpr Config::prefix_t EXT = 0xED;
    static constexpr Config::prefix_t IX = 0xDD;
    static constexpr Config::prefix_t IY = 0xFD;
    static constexpr Config::prefix_t BIT = 0xCB;
    static constexpr Config::prefix_t EXTBIT = 0xEDCB;
    static constexpr Config::prefix_t IXEXT = 0xDDED;
    static constexpr Config::prefix_t IYEXT = 0xFDED;
    static constexpr Config::prefix_t IXBIT = 0xDDCB;
    static constexpr Config::prefix_t IYBIT = 0xFDCB;

    struct Flags final {
        uint8_t _dst;
        uint8_t _src;
        uint8_t _mask;

        static constexpr Flags create(uint8_t mask, AddrMode dst, AddrMode src) {
            return Flags{dst, src, mask};
        }
        static constexpr Flags imcapable(uint8_t mask, AddrMode dst, AddrMode src) {
            return Flags{static_cast<uint8_t>(dst | im_bm), src, mask};
        }
        static constexpr Flags lmcapable(uint8_t mask, AddrMode dst, AddrMode src) {
            return Flags{dst, static_cast<uint8_t>(src | lm_bm), mask};
        }
        static constexpr Flags ilmcapable(uint8_t mask, AddrMode dst, AddrMode src) {
            return Flags{
                    static_cast<uint8_t>(dst | im_bm), static_cast<uint8_t>(src | lm_bm), mask};
        }

        AddrMode dst() const { return AddrMode(_dst & dst_gm); }
        AddrMode src() const { return AddrMode(_src & src_gm); }
        bool imCapable() const { return _dst & im_bm; }
        bool lmCapable() const { return _src & lm_bm; }
        uint8_t mask() const { return _mask; }

    private:
        static constexpr uint8_t dst_gm = 0x7F;
        static constexpr uint8_t src_gm = 0x7F;
        static constexpr uint8_t im_bm = 0x80;
        static constexpr uint8_t lm_bm = 0x80;
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const {
        return Flags{pgm_read_byte(&_flags_P._dst), pgm_read_byte(&_flags_P._src),
                pgm_read_byte(&_flags_P._mask)};
    }

    static bool ixBit(Config::prefix_t prefix) { return prefix == IXBIT || prefix == IYBIT; }
    static Config::opcode_t encodeIndex(RegName ix) { return (ix == REG_IX) ? IX : IY; }
    static RegName decodeIndex(Config::prefix_t prefix) {
        if (prefix == IX || prefix == IXEXT || prefix == IXBIT)
            return REG_IX;
        if (prefix == IY || prefix == IYEXT || prefix == IYBIT)
            return REG_IY;
        return REG_UNDEF;
    }

private:
    const Flags _flags_P;
};

}  // namespace common
}  // namespace z80
}  // namespace libasm

#endif  // __LIBASM_ENTRY_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
