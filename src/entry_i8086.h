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

#ifndef __ENTRY_I8086_H__
#define __ENTRY_I8086_H__

#include <stdint.h>

#include "config_i8086.h"
#include "entry_base.h"

namespace libasm {
namespace i8086 {

enum CpuType : uint8_t {
    I8086,
    I80186,
    V30,
};

// |mod|reg|r/m|
// mod=11: r/m=reg
// mod=00: disp=void
// mod=01: disp=int8_t
// mod=10: disp=uint16_t
//  r/m=000: [BX+SI+disp]
//  r/m=001: [BX+DI+disp]
//  r/m=010: [BP+SI+disp]
//  r/m=011: [BP+DI+disp]
//  r/m=100: [SI+disp]
//  r/m=101: [DI+disp]
//  r/m=110: [BP+disp], [disp]
//  r/m=111: [BX+disp]

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_AL = 1,     // Byte Accumulator: AL
    M_CL = 2,     // Bit Counter: CL
    M_BREG = 3,   // Byte Register: AL, AH, BL, BH, CL, CH, DL, DH
    M_AX = 4,     // Word Accumulator: AX
    M_DX = 5,     // Dynamic I/O Address: DX
    M_WREG = 6,   // Word Register: AX, BX, CX, DX, SP, BP, SI, DI
    M_SREG = 7,   // Segment Register: ES, CS, SS, DS
    M_BMOD = 8,   // Byte memory/register mode
    M_WMOD = 9,   // Word memory/register mode
    M_BMEM = 10,  // Byte memory: BYTE PTR [addr]
    M_WMEM = 11,  // Word memory: WORD PTR [addr]
    M_BDIR = 12,  // Byte Direct mode: BYTE PTR [nnnn]
    M_WDIR = 13,  // Word Direct mode: WORD PTR [nnnn]
    M_IMM = 14,   // 16-bit Immediate Constant
    M_IMM8 = 15,  // Sign-extended 8-bit Immediate
    M_VAL1 = 16,  // Constant 1: for bit counter
    M_VAL3 = 17,  // Constant 3: for INT type
    M_REL = 18,   // Relative: 16-bit displacement
    M_REL8 = 19,  // Relative: 8-bit displacement
    M_IOA = 20,   // I/O Address
    M_FAR = 21,   // Far address: seg:off
    M_ISTR = 22,  // String instruction: MOVSi/CMPSi/STOSi/LODSi/SCASi
    M_MEM = 23,   // Memory address: [addr]
    M_DIR = 24,   // Direct mode: [nnnn]
    M_CS = 25,    // Code Segment Register: CS
    M_UI16 = 26,  // 16-bit unsigned immediate
    M_UI8 = 27,   // 8-bit unsigned immediate
    M_BIT = 28,   // 3/4-bit shift count/bit number constant
};

enum OprPos : uint8_t {
    P_NONE = 0,
    P_OPR = 1,   // In operand
    P_OREG = 2,  // In opcode:      __|___|reg
    P_OSEG = 3,  // In opcode:      ___|ss|___
    P_OMOD = 4,  // In opcode:      mo|___|r/m
    P_MOD = 5,   // In mod-reg-r/m: mo|___|r/m
    P_REG = 6,   // In mod-reg-r/m: __|reg|___
    P_MREG = 7,  // In mod-reg-r/m: mo|reg|reg
};

enum OprSize : uint8_t {
    SZ_NONE = 0,
    SZ_BYTE = 1,  // Byte
    SZ_WORD = 2,  // Word
    SZ_SOFF = 3,  // Segment:Offset
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint8_t _dst;
        uint8_t _src;
        uint8_t _ext;
        uint8_t _size;

        static constexpr Flags create(AddrMode dst, AddrMode src, AddrMode ext, OprPos dstPos,
                OprPos srcPos, OprPos extPos, OprSize size) {
            return Entry::Flags{Entry::_opr(dst, dstPos), Entry::_opr(src, srcPos),
                    Entry::_opr(ext, extPos), Entry::_size(size, false)};
        }

        static constexpr Flags strInst(AddrMode dst, AddrMode src, OprSize size) {
            return Entry::Flags{Entry::_opr(dst, P_NONE), Entry::_opr(src, P_NONE),
                    Entry::_opr(M_NONE, P_NONE), Entry::_size(size, true)};
        }

        Flags read() const {
            return Flags{pgm_read_byte(&_dst), pgm_read_byte(&_src), pgm_read_byte(&_ext),
                    pgm_read_byte(&_size)};
        }
        AddrMode dst() const { return AddrMode((_dst >> mode_gp) & mode_gm); }
        AddrMode src() const { return AddrMode((_src >> mode_gp) & mode_gm); }
        AddrMode ext() const { return AddrMode((_ext >> mode_gp) & mode_gm); }
        OprPos dstPos() const { return OprPos((_dst >> pos_gp) & pos_gm); }
        OprPos srcPos() const { return OprPos((_src >> pos_gp) & pos_gm); }
        OprPos extPos() const { return OprPos((_ext >> pos_gp) & pos_gm); }
        OprSize size() const { return OprSize((_size >> size_gp) & size_gm); }
        bool stringInst() const { return _size & strInst_bm; }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr uint8_t _opr(AddrMode mode, OprPos pos) {
        return (static_cast<uint8_t>(mode) << mode_gp) | (static_cast<uint8_t>(pos) << pos_gp);
    }
    static constexpr uint8_t _size(OprSize size, bool strInst) {
        return (static_cast<uint8_t>(size) << size_gp) | (strInst ? strInst_bm : 0);
    }

    // |dst|, |src|, |ext|
    static constexpr int mode_gp = 0;
    static constexpr int pos_gp = 5;
    static constexpr uint8_t mode_gm = 0x1F;
    static constexpr uint8_t pos_gm = 0x07;
    // |size|
    static constexpr int size_gp = 0;
    static constexpr int strInst_bp = 4;
    static constexpr uint8_t size_gm = 0x03;
    static constexpr uint8_t strInst_bm = (1 << strInst_bp);
};

}  // namespace i8086
}  // namespace libasm

#endif  // __ENTRY_I8086_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
