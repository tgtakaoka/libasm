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

#ifndef __LIBASM_ENTRY_MC68000_H__
#define __LIBASM_ENTRY_MC68000_H__

#include <stdint.h>

#include "config_mc68000.h"
#include "entry_base.h"

namespace libasm {
namespace mc68000 {

enum OprSize : uint8_t {
    SZ_NONE = Size::SZ_NONE,
    SZ_BYTE = Size::SZ_BYTE,  // Byte (8 bits)
    SZ_WORD = Size::SZ_WORD,  // Word (16 bits/2 bytes)
    SZ_LONG = Size::SZ_QUAD,  // Long (32 bits/4 bytes)
    SZ_DATA = Size::SZ_DATA,  // _ss|___|___ BYTE=0/WORD=1/LONG=2
    SZ_ADDR = Size::SZ_ADDR,  // __LIBASM_s|___|___ WORD=0/LONG=1
    SZ_ADR8 = 7,              // s__|___|___ WORD=0/LONG=1
    SZ_SNGL = 8,              // Single precision real (32 bits/4 bytes)
    SZ_DUBL = 9,              // Double precision real (64 bits/8 bytes)
    SZ_XTND = 10,             // Extended precision real (96 bits/12 bytes)
    SZ_PBCD = 11,             // Packed BCD real (96 bits/12 bytes)
    SZ_FDAT = 12,             // MC68881 float type
};

enum InsnSize : uint8_t {
    ISZ_NONE = Size::SZ_NONE,
    ISZ_BYTE = Size::SZ_BYTE,  // .B
    ISZ_WORD = Size::SZ_WORD,  // .W
    ISZ_LONG = Size::SZ_QUAD,  // .L
    ISZ_FIXD = Size::SZ_OCTA,  // Fixed size
    ISZ_DATA = Size::SZ_DATA,
    ISZ_ERROR = 7,
    ISZ_SNGL = SZ_SNGL,  // .S
    ISZ_DUBL = SZ_DUBL,  // .D
    ISZ_XTND = SZ_XTND,  // .X
    ISZ_PBCD = SZ_PBCD,  // .P
    ISZ_FDAT = SZ_FDAT,  // .f : ___|fff|___|_______ : MC68881 size
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

    // MC68881
    M_FPREG = 37,  // FPn: Floatng point data register
    M_FPMLT = 38,  // FMOVEM.X register list; FPn
    M_FCMLT = 39,  // FMOVEM.L register list; FPCR/FPSR/FPIAR
    M_FSICO = 40,  // FPSINCOS FPc:FPs
    M_KDREG = 41,  // <ea>{Dn}
    M_KFACT = 42,  // <ea>{#k}
    M_FPCR = 43,   // FPCR register
    M_FPSR = 44,   // FPSR register
    M_FPIAR = 45,  // FPIAR register
    M_IMROM = 46,  // MC68881 ROM constant
    M_REL32 = 47,  // 32-bit Relative; 1111|ccc|01s|___|___: s=0 16bit, s=1 32bit
    M_IMFLT = 48,  // Floating point immediate
};

enum OprPos : uint8_t {
    OP_10 = 0,   // __LIBASM___|___|___|mmm|rrr
    OP_23 = 1,   // __LIBASM___|rrr|mmm|___|___
    OP__0 = 2,   // __LIBASM___|___|___|___|rrr
    OP__3 = 3,   // __LIBASM___|rrr|___|___|___
    OP___ = 4,   // __LIBASM___|___|___|___|___
    EX_RX = 5,   // __LIBASM__|xxx|___|_______ : format or source register
    EX_RY = 6,   // __LIBASM__|___|yyy|_______ : destination register
    EX_SC = 7,   // __LIBASM__|___|sss|____ccc : FPSINCOS
    EX_SL = 8,   // __LIBASM__|___|__|ffffffff : static register list
    EX_DL = 9,   // __LIBASM__|___|___|rrr____ : dynamic register list
    EX_SK = 10,  // __LIBASM__|___|___|kkkkkkk : static k-factor
    EX_DK = 11,  // __LIBASM__|___|___|rrr____ : dynamic k-factor
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint8_t _src;
        uint8_t _dst;
        uint8_t _pos;
        uint8_t _size;
        Config::opcode_t _postVal;

        static constexpr Flags create(AddrMode src, AddrMode dst, OprPos srcPos, OprPos dstPos,
                OprSize oSize, InsnSize iSize) {
            return Flags{static_cast<uint8_t>(src), static_cast<uint8_t>(dst),
                    Entry::_pos(srcPos, dstPos), Entry::_size(oSize, iSize), 0};
        }

        static constexpr Flags create(AddrMode src, AddrMode dst, OprPos srcPos, OprPos dstPos,
                OprSize oSize, InsnSize iSize, Config::opcode_t postVal) {
            return Flags{static_cast<uint8_t>(src | hasPostVal_bm), static_cast<uint8_t>(dst),
                    Entry::_pos(srcPos, dstPos), Entry::_size(oSize, iSize), postVal};
        }

        Flags read() const {
            return Flags{pgm_read_byte(&_src), pgm_read_byte(&_dst), pgm_read_byte(&_pos),
                    pgm_read_byte(&_size), pgm_read_word(&_postVal)};
        }
        AddrMode src() const { return AddrMode(_src & mode_gm); }
        AddrMode dst() const { return AddrMode(_dst); }
        OprPos srcPos() const { return OprPos((_pos >> srcPos_gp) & pos_gm); }
        OprPos dstPos() const { return OprPos((_pos >> dstPos_gp) & pos_gm); }
        OprSize oprSize() const { return OprSize((_size >> oprSize_gp) & size_gm); }
        InsnSize insnSize() const { return InsnSize((_size >> insnSize_gp) & size_gm); }
        bool hasPostVal() const { return (_src & hasPostVal_bm) != 0; }
        Config::opcode_t postVal() const { return _postVal; }

        void setAddrMode(AddrMode src, AddrMode dst) {
            _src = static_cast<uint8_t>(src | (_src & hasPostVal_bm));
            _dst = static_cast<uint8_t>(dst);
        }
        void setInsnSize(InsnSize size) { _size = Entry::_size(oprSize(), size); }
        Config::opcode_t postMask() const { return postMask(dstPos()) | postMask(srcPos()); }
        static Config::opcode_t postMask(OprPos pos);
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : Base(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    const Flags _flags;

    static constexpr uint8_t _pos(OprPos src, OprPos dst) {
        return (static_cast<uint8_t>(src) << srcPos_gp) | (static_cast<uint8_t>(dst) << dstPos_gp);
    }

    static constexpr uint8_t _size(OprSize opr, InsnSize insn) {
        return (static_cast<uint8_t>(opr) << oprSize_gp) |
               (static_cast<uint8_t>(insn) << insnSize_gp);
    }

    // |src|
    static constexpr int mode_gp = 0;
    static constexpr int hasPostVal_bp = 7;
    static constexpr uint8_t mode_gm = 0x7F;
    static constexpr uint8_t hasPostVal_bm = (1 << hasPostVal_bp);

    // |pos|
    static constexpr int srcPos_gp = 0;
    static constexpr int dstPos_gp = 4;
    static constexpr uint8_t pos_gm = 0x0F;
    // |size|
    static constexpr int oprSize_gp = 0;
    static constexpr int insnSize_gp = 4;
    static constexpr uint8_t size_gm = 0x0F;
};

}  // namespace mc68000
}  // namespace libasm

#endif  // __LIBASM_ENTRY_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
