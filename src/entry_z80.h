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

#include "config_z80.h"
#include "entry_base.h"

namespace libasm {
namespace z80 {

enum CpuType : uint8_t {
    Z80,
    I8080,
    I8085,
};

enum AddrMode : uint8_t {
    M_NO = 0,
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
    M_UNKI = 50,     // Undefined instruction
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint8_t _dst;
        uint8_t _src;

        static constexpr Flags create(AddrMode dst, AddrMode src, bool indexBit = false) {
            return Flags{static_cast<uint8_t>(
                                 static_cast<uint8_t>(dst) | (indexBit ? 1 << indexBit_bp : 0)),
                    static_cast<uint8_t>(src)};
        }
        Flags read() const { return Flags{pgm_read_byte(&_dst), pgm_read_byte(&_src)}; }

        AddrMode dstMode() const { return AddrMode(_dst & ~(1 << indexBit_bp)); }
        AddrMode srcMode() const { return AddrMode(_src); }
        bool indexBit() const { return _dst & (1 << indexBit_bp); }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr int indexBit_bp = 7;
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
