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
    SZ_ADDR = Size::SZ_ADDR,  // __s|___|___ WORD=0/LONG=1
    SZ_ADR8 = 7,              // s__|___|___ WORD=0/LONG=1
    SZ_SNGL = 8,              // Single precision real (32 bits/4 bytes)
    SZ_DUBL = 9,              // Double precision real (64 bits/8 bytes)
    SZ_XTND = 10,             // Extended precision real (96 bits/12 bytes)
    SZ_PBCD = 11,             // Packed BCD real (96 bits/12 bytes)
    SZ_FDAT = 12,             // MC68881 float type
    SZ_DATH = 13,             // _ss|___|___|___ BYTE=0/WORD=1/LONG=2
    SZ_CAS1 = 14,             // _ss|___|___|___ BYTE=1/WORD=2/LONG=3
    SZ_CAS2 = 15,             // _ss|___|___|___        WORD=2/LONG=3
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
    M_NONE = 0,
    // for assembler operand parsing; M_DREG-M_INDX must be contiguous and in this order.
    M_DREG = 1,    // Dn: Data Register Direct
    M_AREG = 2,    // An: Address Register Direct
    M_AIND = 3,    // (An): Address Register Indirect
    M_PINC = 4,    // (An)+: Address Register Indirect with Postincrement
    M_PDEC = 5,    // -(An): Address Register Indirect with Predecrement
    M_DISP = 6,    // (d16,An): Address Register Indirect with Displacement
    M_INDX = 7,    // (d8,An,Xn.SIZE): Address Register Indirect with Index
    M_IMDAT = 8,   // #imm: Immediate Data
    M_IMFLT = 9,   // #imm: Floating point immediate
    M_AWORD = 10,  // (xxxx).W: Absolute Short Addressing
    M_ALONG = 11,  // (xxxx).L: Absolute Long Addressing
    M_PCDSP = 12,  // (d16,PC): Program Counter Indirect with Displacement
    M_PCIDX = 13,  // (d8,PC,Xn.SIZE): Program Counter Indirect with Index
    M_GREG = 14,   // Data/Address register: Rn
    // for instruction table; M_RADDR-M_DADDR must be contigous.
    M_RADDR = 15,  // Readable Address:  Dn/An/(An)/(An)+/-(An)/(*,An)/(Abs)/(*,PC)/#xxxx
    M_RDATA = 16,  // Readable Data:     Dn/  /(An)/(An)+/-(An)/(*,An)/(Abs)/(*,PC)/#xxxx
    M_WADDR = 17,  // Readable Address:  Dn/An/(An)/(An)+/-(An)/(*,An)/(Abs)
    M_WDATA = 18,  // Writable Data:     Dn/  /(An)/(An)+/-(An)/(*.An)/(Abs)
    M_BITFR = 19,  // Bitfield read:     Dn/  /(An)/           /(*,An)/(Abs)/(*,PC)
    M_BITFW = 20,  // Bitfield write:    Dn/  /(An)/           /(*,An)/(Abs)
    M_RMEM = 21,   // Readable Memory:         (An)/(An)+/-(An)/(*,An)/(Abs)/(*,PC)
    M_WMEM = 22,   // Writable Memory:         (An)/(An)+/-(An)/(*,An)/(Abs)
    M_JADDR = 23,  // Jumpable Address:        (An)            /(*,An)/(Abs)/(*,PC)
    M_IADDR = 24,  // Increment Address:       (An)/(An)+      /(*,An)/(Abs)/(*,PC)
    M_DADDR = 25,  // Decrement Address:       (An)/     /-(An)/(*,An)/(Abs)
    // for instruction table
    M_REL8 = 26,   // 8/16-bit Relative (Bcc)
    M_REL16 = 27,  // 16-bit Relative (DBcc/FDBcc)
    M_IMBIT = 28,  // Bit number: #0~#7/#15/#31
    M_IM3 = 29,    // 3-bit Immediate: #1~#8
    M_IM8 = 30,    // 8-bit Immediate
    M_IMVEC = 31,  // 3-bit Trap Vector
    M_IMDSP = 32,  // 16-bit Signed Displacement
    M_LABEL = 33,  // label
    M_MULT = 34,   // MOVEM register list
    M_SR = 35,     // SR register
    M_CCR = 36,    // CCR register
    M_USP = 37,    // USP register
    // MC68010
    M_CREG = 38,  // Control register: SFC, DFC, USP, VBR, etc.
    // MC68881
    M_FPREG = 39,  // FPn: Floatng point data register
    M_FPMLT = 40,  // FMOVEM.X register list; FPn
    M_FCMLT = 41,  // FMOVEM.L register list; FPCR/FPSR/FPIAR
    M_FSICO = 42,  // FPSINCOS FPc:FPs
    M_KDREG = 43,  // <ea>{Dn}
    M_KFACT = 44,  // <ea>{#k}
    M_FPCR = 45,   // FPCR register
    M_FPSR = 46,   // FPSR register
    M_FPIAR = 47,  // FPIAR register
    M_IMROM = 48,  // MC68881 ROM constant
    M_REL32 = 49,  // 32-bit Relative; 1111|ccc|01s|___|___: s=0 16bit, s=1 32bit (FBcc)
    // MC68020
    M_BITOW = 50,  // Bitfield offset/width: {Dn/n:Dn/n}
    M_DPAIR = 51,  // Data register pair: Dn:Dn
    M_PPAIR = 52,  // Pointer pair: (Rn):(Rn)
};

enum OprPos : uint8_t {
    OP_10 = 0,    // __|___|___|mmm|rrr
    OP_23 = 1,    // __|rrr|mmm|___|___
    OP__0 = 2,    // __|___|___|___|rrr
    OP__3 = 3,    // __|rrr|___|___|___
    OP___ = 4,    // __|___|___|___|___
    EX_RX = 5,    // __|xxx|___|_______ : format or source register
    EX_RY = 6,    // __|___|yyy|_______ : destination register
    EX_SC = 7,    // __|___|sss|____ccc : FPSINCOS
    EX_SL = 8,    // __|___|__|ffffffff : static register list
    EX_DL = 9,    // __|___|___|rrr____ : dynamic register list
    EX_SK = 10,   // __|___|___|kkkkkkk : static k-factor
    EX_DK = 11,   // __|___|___|rrr____ : dynamic k-factor
    EX_GR = 12,   // a|rrr|____________ : Dn/An
    EX_RC = 13,   // _____|cccccccccccc : Control register
    EX_OW = 14,   // _____|dooooodwwwww : Bitfield offset and width
    EX_DC = 15,   // _______________ccc : Dc
    EX_DU = 16,   // _______uuu|___|___ : Du
    EX_DR = 17,   // _|ddd|____________ : Dn
    EX_QR = 18,   // _|qqq|________|rrr : Dr:Dq
    EX_QQ = 19,   // _|qqq|________|qqq : Dq
    EX_DCP = 20,  // ______________|ccc : Dc1:Dc2 for CAS2
    EX_DUP = 21,  // _______uuu|___|___ : Du1:Du2 for CAS2
};

enum CodeFormat : uint8_t {
    CF_0000 = 0,   // 0000000
    CF_0007 = 1,   // 0000007
    CF_0017 = 2,   // 0000017
    CF_0077 = 3,   // 0000077
    CF_0100 = 4,   // 0000100
    CF_0107 = 5,   // 0000107
    CF_0177 = 6,   // 0000177
    CF_0377 = 7,   // 0000377
    CF_3000 = 8,   // 0003000
    CF_3077 = 9,   // 0003077
    CF_7007 = 10,  // 0007007
    CF_7077 = 11,  // 0007077
    CF_7107 = 12,  // 0007107
    CF_7307 = 13,  // 0007307
    CF_7377 = 14,  // 0007377
    CF_7400 = 15,  // 0007400
    CF_7477 = 16,  // 0007477
    CF_7777 = 17,  // 0007777
};

enum PostFormat : uint8_t {
    PF_0000 = 0,   // 0x0000
    PF_0070 = 1,   // 0x0070
    PF_00FF = 2,   // 0x00FF
    PF_01C7 = 3,   // 0x01C7
    PF_0380 = 4,   // 0x0380
    PF_0387 = 5,   // 0x0387
    PF_03F0 = 6,   // 0x03F0
    PF_03FF = 7,   // 0x03FF
    PF_0FFF = 8,   // 0x0FFF
    PF_1C00 = 9,   // 0x1C00
    PF_1F80 = 10,  // 0x1F80
    PF_1F87 = 11,  // 0x1F87
    PF_7007 = 12,  // 0x7007
    PF_7FFF = 13,  // 0x7FFF
    PF_F000 = 14,  // 0xF000
    PF_F1C7 = 15,  // 0xF1C7
    PF_FFFF = 16,  // 0xFFFF
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _src;
        uint16_t _dst;
        uint16_t _ext;
        uint16_t _attr;
        Config::opcode_t _post;

        static constexpr Flags create(AddrMode src, AddrMode dst, AddrMode ext, OprPos spos,
                OprPos dpos, OprPos epos, OprSize osize, InsnSize isize, CodeFormat cf) {
            return Flags{mode(src, spos, cf), mode(dst, dpos, PF_0000), mode(ext, epos),
                    attr(osize, isize), 0x0000};
        }

        static constexpr Flags create(AddrMode src, AddrMode dst, AddrMode ext, OprPos spos,
                OprPos dpos, OprPos epos, OprSize osize, InsnSize isize, CodeFormat cf,
                Config::opcode_t post, PostFormat pf) {
            return Flags{mode(src, spos, cf), mode(dst, dpos, pf), mode(ext, epos),
                    attr(osize, isize, true), post};
        }

        AddrMode src() const { return AddrMode((_src >> src_gp) & src_gm); }
        AddrMode dst() const { return AddrMode((_dst >> dst_gp) & dst_gm); }
        AddrMode ext() const { return AddrMode((_ext >> ext_gp) & ext_gm); }
        OprPos srcPos() const { return OprPos((_src >> spos_gp) & spos_gm); }
        OprPos dstPos() const { return OprPos((_dst >> dpos_gp) & dpos_gm); }
        OprPos extPos() const { return OprPos((_ext >> epos_gp) & epos_gm); }
        OprSize oprSize() const { return OprSize((_attr >> osize_gp) & osize_gm); }
        InsnSize insnSize() const { return InsnSize((_attr >> isize_gp) & isize_gm); }
        bool hasPostVal() const { return (_attr & pval_bm) != 0; }
        Config::opcode_t postVal() const { return _post; }

        void setInsnSize(InsnSize size) { _attr = attr(oprSize(), size, hasPostVal()); }

        Config::opcode_t insnMask() const {
            static constexpr Config::opcode_t CMASK[] PROGMEM = {
                    00000,  // CF_0000 = 0
                    00007,  // CF_0007 = 1
                    00017,  // CF_0017 = 2
                    00077,  // CF_0077 = 3
                    00100,  // CF_0100 = 4
                    00107,  // CF_0107 = 5
                    00177,  // CF_0177 = 6
                    00377,  // CF_0377 = 7
                    03000,  // CF_3000 = 8
                    03077,  // CF_3077 = 9
                    07007,  // CF_7007 = 10
                    07077,  // CF_7077 = 11
                    07107,  // CF_7107 = 12
                    07307,  // CF_7307 = 13
                    07377,  // CF_7377 = 14
                    07400,  // CF_7400 = 15
                    07477,  // CF_7477 = 16
                    07777,  // CF_7777 = 17
            };
            return pgm_read_word(CMASK + codeFormat());
        }

        Config::opcode_t postMask() const {
            static constexpr Config::opcode_t PMASK[] PROGMEM = {
                    0x0000,  // PF_0000 = 0
                    0x0070,  // PF_0070 = 1
                    0x00FF,  // PF_00FF = 2
                    0x01C7,  // PF_01C7 = 3
                    0x0380,  // PF_0380 = 4
                    0x0387,  // PF_0387 = 5
                    0x03F0,  // PF_03F0 = 6
                    0x03FF,  // PF_03FF = 7
                    0x0FFF,  // PF_0FFF = 8
                    0x1C00,  // PF_1C00 = 9
                    0x1F80,  // PF_1F80 = 10
                    0x1F87,  // PF_1F87 = 11
                    0x7007,  // PF_7007 = 12
                    0x7FFF,  // PF_7FFF = 13
                    0xF000,  // PF_F000 = 14
                    0xF1C7,  // PF_F1C7 = 15
                    0xFFFF,  // PF_FFFF = 16
            };
            return pgm_read_word(PMASK + postFormat());
        }

    private:
        CodeFormat codeFormat() const { return CodeFormat((_src >> cf_gp) & cf_gm); }
        PostFormat postFormat() const { return PostFormat((_dst >> pf_gp) & pf_gm); }

        // |_src|
        static constexpr int src_gp = 0;
        static constexpr int spos_gp = 6;
        static constexpr int cf_gp = 11;
        static constexpr auto src_gm = UINT16_C(0x3F);
        static constexpr auto spos_gm = UINT16_C(0x1F);
        static constexpr auto cf_gm = UINT16_C(0x1F);
        static constexpr uint16_t mode(AddrMode src, OprPos spos, CodeFormat cf) {
            return (src << src_gp) | (spos << spos_gp) | (cf << cf_gp);
        }
        // |_dst|
        static constexpr int dst_gp = 0;
        static constexpr int dpos_gp = 6;
        static constexpr int pf_gp = 11;
        static constexpr auto dst_gm = UINT16_C(0x3F);
        static constexpr auto dpos_gm = UINT16_C(0x1F);
        static constexpr auto pf_gm = UINT16_C(0x1F);
        static constexpr uint16_t mode(AddrMode dst, OprPos dpos, PostFormat pf) {
            return (dst << dst_gp) | (dpos << spos_gp) | (pf << pf_gp);
        }
        // |_ext|
        static constexpr int ext_gp = 0;
        static constexpr int epos_gp = 6;
        static constexpr auto ext_gm = UINT16_C(0x3F);
        static constexpr auto epos_gm = UINT16_C(0x1F);
        static constexpr uint16_t mode(AddrMode ext, OprPos epos) {
            return (ext << ext_gp) | (epos << epos_gp);
        }
        // |_attr|
        static constexpr int osize_gp = 0;
        static constexpr int isize_gp = 4;
        static constexpr int pval_bp = 8;
        static constexpr auto osize_gm = UINT16_C(0x0F);
        static constexpr auto isize_gm = UINT16_C(0x0F);
        static constexpr auto pval_bm = UINT16_C(1) << pval_bp;
        static constexpr uint16_t attr(OprSize osize, InsnSize isize, bool pval = false) {
            return (osize << osize_gp) | (isize << isize_gp) | (pval ? pval_bm : 0);
        }
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const {
        return Flags{pgm_read_word(&_flags_P._src), pgm_read_word(&_flags_P._dst),
                pgm_read_word(&_flags_P._ext), pgm_read_word(&_flags_P._attr),
                pgm_read_word(&_flags_P._post)};
    }

private:
    const Flags _flags_P;
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
