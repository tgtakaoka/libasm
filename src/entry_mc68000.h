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

#ifndef __ENTRY_MC68000_H__
#define __ENTRY_MC68000_H__

#include <stdint.h>

#include "config_mc68000.h"
#include "entry_base.h"

namespace libasm {
namespace mc68000 {

enum OprSize : uint8_t {
    SZ_BYTE = 0,
    SZ_WORD = 1,
    SZ_LONG = 2,
    SZ_NONE = 3,
    SZ_DATA = 4,  // _ss|___|___ BYTE=0/WORD=1/LONG=2
    SZ_ADR6 = 5,  // __s|___|___ WORD=0/LONG=1
    SZ_ADR8 = 6,  // s__|___|___ WORD=0/LONG=1
    SZ_ERROR = 7,
};

enum InsnSize : uint8_t {
    ISZ_BYTE = SZ_BYTE,  // .B
    ISZ_WORD = SZ_WORD,  // .W
    ISZ_LONG = SZ_LONG,  // .L
    ISZ_NONE = SZ_NONE,  // __
    ISZ_DATA = SZ_DATA,  // ==OprSize
};

enum AddrMode : uint8_t {
    M_DREG = 0,       // Dn: Data Register Direct
    M_AREG = 1,       // An: Address Register Direct
    M_AIND = 2,       // (An): Address Register Indirect
    M_PINC = 3,       // (An)+: Address Register Indirect with Postincrement
    M_PDEC = 4,       // -(An): Address Register Indirect with Predecrement
    M_DISP = 5,       // (d16,An): Address Register Indirect with Displacement
    M_INDX = 6,       // (d8,An,Xn): Address Register Indirect with Index
    M_AWORD = 8 + 0,  // (xxxx).W: Absolute Short Addressing
    M_ALONG = 8 + 1,  // (xxxx).L: Absolute Long Addressing
    M_PCDSP = 8 + 2,  // (d16,PC): Program Counter Indirect with Displacement
    M_PCIDX = 8 + 3,  // (d8,PC,Xn): Program Counter Indirect with Index
    M_IMDAT = 8 + 4,  // #imm: Immediate Data

    M_ERROR = 64,  // TODO: Remove
    M_LABEL = 65,  // label

    // for assembler operand parsing
    M_NONE = 16,   // no operand
    M_MULT = 17,   // MOVEM register list
    M_SR = 18,     // SR register
    M_CCR = 19,    // CCR register
    M_USP = 20,    // USP register
    M_RADDR = 21,  // Readable Address:  Dn/An/(An)/(An)+/-(An)/(*,An)/(Abs)/(*,PC)/#xxxx
    M_RDATA = 22,  // Readable Data:     Dn   /(An)/(An)+/-(An)/(*,An)/(Abs)/(*,PC)/#xxxx
    M_WADDR = 23,  // Readable Address:  Dn/An/(An)/(An)+/-(An)/(*,An)/(Abs)
    M_WDATA = 24,  // Writable Data:     Dn   /(An)/(An)+/-(An)/(*.An)/(Abs)
    M_RMEM = 25,   // Readable Memory:         (An)/(An)+/-(An)/(*,An)/(Abs)/(*,PC)
    M_WMEM = 26,   // Writable Memory:         (An)/(An)+/-(An)/(*,An)/(Abs)
    M_JADDR = 27,  // Jumpable Address:        (An)            /(*,An)/(Abs)/(*,PC)
    M_IADDR = 28,  // Increment Address:       (An)/(An)+      /(*,An)/(Abs)/(*,PC)
    M_DADDR = 29,  // Decrement Address:       (An)/     /-(An)/(*,An)/(Abs)
    M_REL16 = 30,  // 16-bit Relative
    M_REL8 = 31,   // 8/16-bit Relative
    M_IMBIT = 32,  // Bit number: #0~#7/#15/#31
    M_IM3 = 33,    // 3-bit Immediate: #1~#8
    M_IM8 = 34,    // 8-bit Immediate
    M_IMVEC = 35,  // 3-bit Trap Vector
    M_IMDSP = 36,  // 16-bit Signed Displacement
};

enum OprPos : uint8_t {
    OP_10 = 0,  // ___|___|mmm|rrr
    OP_23 = 1,  // rrr|mmm|___|___
    OP__0 = 2,  // ___|___|___|rrr
    OP__3 = 3,  // rrr|___|___|___
    OP___ = 4,  // ___|___|___|___
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint8_t _src;
        uint8_t _dst;
        uint8_t _pos;
        uint8_t _size;

        static constexpr Flags create(AddrMode src, AddrMode dst, OprPos srcPos, OprPos dstPos,
                OprSize oSize, InsnSize iSize, bool withSize, bool alias) {
            return Flags{static_cast<uint8_t>(src), static_cast<uint8_t>(dst),
                    Entry::_pos(srcPos, dstPos, alias), Entry::_size(oSize, iSize, withSize)};
        }
        Flags read() const {
            return Flags{pgm_read_byte(&_src), pgm_read_byte(&_dst), pgm_read_byte(&_pos),
                    pgm_read_byte(&_size)};
        }

        AddrMode srcMode() const { return AddrMode(_src); }
        AddrMode dstMode() const { return AddrMode(_dst); }
        OprPos srcPos() const { return OprPos((_pos >> srcPos_gp) & oprPos_gm); }
        OprPos dstPos() const { return OprPos((_pos >> dstPos_gp) & oprPos_gm); }
        bool alias() const { return ((_pos >> alias_bp) & 1) ? true : false; }
        OprSize oprSize() const { return OprSize((_size >> oprSize_gp) & oprSize_gm); }
        InsnSize insnSize() const { return InsnSize((_size >> insnSize_gp) & insnSize_gm); }
        bool withSize() const { return (_size & (1 << withSize_gp)) != 0; }

        void setSrcMode(AddrMode mode) { _src = static_cast<uint8_t>(mode); }
        void setDstMode(AddrMode mode) { _dst = static_cast<uint8_t>(mode); }
        void setInsnSize(InsnSize size) { _size = Entry::_size(oprSize(), size, withSize()); }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr uint8_t _pos(OprPos src, OprPos dst, bool alias) {
        return (static_cast<uint8_t>(src) << srcPos_gp) | (static_cast<uint8_t>(dst) << dstPos_gp) |
               (alias ? (1 << alias_bp) : 0);
    }
    static constexpr uint8_t _size(OprSize opr, InsnSize insn, bool withSize) {
        return (static_cast<uint8_t>(opr) << oprSize_gp) |
               (static_cast<uint8_t>(insn) << insnSize_gp) |
               (static_cast<uint8_t>(withSize ? 1 : 0) << withSize_gp);
    }

    // |pos|
    static constexpr uint8_t oprPos_gm = 0x7;
    static constexpr int srcPos_gp = 0;
    static constexpr int dstPos_gp = 3;
    static constexpr int alias_bp = 7;
    // |size|
    static constexpr uint8_t oprSize_gm = 0x7;
    static constexpr uint8_t insnSize_gm = 0x7;
    static constexpr int oprSize_gp = 0;
    static constexpr int insnSize_gp = 3;
    static constexpr int withSize_gp = 6;
};

}  // namespace mc68000
}  // namespace libasm

#endif  // __ENTRY_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
