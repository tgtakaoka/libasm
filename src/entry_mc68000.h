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

#include "config_mc68000.h"

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

enum InsnSize {
    ISZ_BYTE = SZ_BYTE,  // .B
    ISZ_WORD = SZ_WORD,  // .W
    ISZ_LONG = SZ_LONG,  // .L
    ISZ_NONE = SZ_NONE,  // __
    ISZ_DATA = SZ_DATA,  // ==OprSize
};

enum AddrMode {
    M_DREG  = 0,    // Dn: Data Register Direct
    M_AREG  = 1,    // An: Address Register Direct
    M_AIND  = 2,    // (An): Address Register Indirect
    M_PINC  = 3,    // (An)+: Address Register Indirect with Postincrement
    M_PDEC  = 4,    // -(An): Address Register Indirect with Predecrement
    M_DISP  = 5,    // (d16,An): Address Register Indirect with Displacement
    M_INDX  = 6,    // (d8,An,Xn): Address Register Indirect with Index
    M_AWORD = 8+0,  // (xxxx).W: Absolute Short Addressing
    M_ALONG = 8+1,  // (xxxx).L: Absolute Long Addressing
    M_PCDSP = 8+2,  // (d16,PC): Program Counter Indirect with Displacement
    M_PCIDX = 8+3,  // (d8,PC,Xn): Program Counter Indirect with Index
    M_IMDAT = 8+4,  // #imm: Immediate Data

    M_ERROR = 64,   // TODO: Remove
    M_LABEL = 65,   // label

    // for assembler operand parsing
    M_NONE  = 16,   // no operand
    M_MULT  = 17,   // MOVEM register list
    M_SR    = 18,   // SR register
    M_CCR   = 19,   // CCR register
    M_USP   = 20,   // USP register
    M_RADDR = 21,   // Readable Address:  Dn/An/(An)/(An)+/-(An)/(*,An)/(Abs)/(*,PC)/#xxxx
    M_RDATA = 22,   // Readable Data:     Dn   /(An)/(An)+/-(An)/(*,An)/(Abs)/(*,PC)/#xxxx
    M_WADDR = 23,   // Readable Address:  Dn/An/(An)/(An)+/-(An)/(*,An)/(Abs)
    M_WDATA = 24,   // Writable Data:     Dn   /(An)/(An)+/-(An)/(*.An)/(Abs)
    M_RMEM  = 25,   // Readable Memory:   Dn   /(An)/(An)+/-(An)/(*,An)/(Abs)/(*,PC)
    M_WMEM  = 26,   // Writable Memory:         (An)/(An)+/-(An)/(*,An)/(Abs)
    M_JADDR = 27,   // Jumpable Address:        (An)            /(*,An)/(Abs)/(*,PC)
    M_IADDR = 28,   // Increment Address:       (An)/(An)+      /(*,An)/(Abs)/(*,PC)
    M_DADDR = 29,   // Decrement Address:       (An)/     /-(An)/(*,An)/(Abs)
    M_REL16 = 30,   // 16-bit Relative
    M_REL8  = 31,   // 8/16-bit Relative
    M_IMBIT = 32,   // Bit number: #0~#7/#15/#31
    M_IM3   = 33,   // 3-bit Immediate: #1~#8
    M_IM8   = 34,   // 8-bit Immediate
    M_IMVEC = 35,   // 3-bit Trap Vector
    M_IMDSP = 36,   // 16-bit Signed Displacement
};

enum OprPos {
    OP_10 = 0,  // ___|___|mmm|rrr
    OP_23 = 1,  // rrr|mmm|___|___
    OP__0 = 2,  // ___|___|___|rrr
    OP__3 = 3,  // rrr|___|___|___
    OP___ = 4,  // ___|___|___|___
};

enum InsnFormat {
    IMPLIED  = 0,  // 1 111 111 111 111 111
    ADDR_REG = 1,  // 1 111 111 111 111 AAA
    DATA_REG = 2,  // 1 111 111 111 111 DDD
    MOVE_USP = 3,  // 1 111 111 111 11d AAA d:0=AtoU,1=UtoA
    TRAP_VEC = 4,  // 1 111 111 111 11V VVV
    DATA_DST = 5,  // 1 111 111 111 MMM DDD
    DEST_OPR = 6,  // 1 111 111 111 MMM XXX
    SIGN_EXT = 7,  // 1 111 111 11S 111 DDD S:0=W,1=L
    RELATIVE = 8,  // 1 111 111 1dd ddd ddd
    DEST_SIZ = 9,  // 1 111 111 1SS MMM XXX SS:00=B,01=W,10=L
    MOVE_MLT = 10, // 1 111 1d1 11S MMM XXX d:0=RtoM,1=MtoR
    MOVE_SR  = 11, // 1 111 1dd 111 MMM XXX dd:00=SRtoM,10=CCRtoM,11=MtoSR
    AREG_LNG = 12, // 1 111 AAA 111 MMM XXX
    AREG_SIZ = 13, // 1 111 AAA S11 MMM XXX S:0=W,1=L
    DREG_DST = 14, // 1 111 DDD 111 MMM XXX
    MOVE_QIC = 15, // 1 111 DDD 1dd ddd ddd
    MOVE_PER = 16, // 1 111 DDD 1dS 111 AAA d:0=DtoM,1=MtoD S:0=W,1=L
    DATA_QIC = 17, // 1 111 ddd 1SS MMM XXX ddd:0=8 SS:00=B,01=W,10=L
    DREG_SIZ = 18, // 1 111 DDD 1SS MMM XXX SS:00=B,01=W,10=L
    DMEM_SIZ = 19, // 1 111 DDD dSS MMM XXX d:0=toD,1=Dto SS:00=B,01=W,10=L
    DREG_ROT = 20, // 1 111 rrr 1SS d11 DDD d:0=Ir/1=Dr
    DMEM_DST = 21, // 1 111 XXX 111 11R YYY R:0=D,1=M
    REGS_EXG = 22, // 1 111 XXX 1rr 11r YYY rrr:010=DD,011=AA,101=DA
    CMPM_SIZ = 23, // 1 111 XXX 1SS 111 YYY SS:00=B,01=W,10=L
    DMEM_OPR = 24, // 1 111 XXX 1SS 11R YYY R:0=D,1=M SS:00=B,01=W,10=L
    MOVA_OPR = 25, // 1 1SS AAA 111 MMM XXX SS:11=W,10=L
    MOVE_OPR = 26, // 1 1SS XXX MMM MMM YYY SS:01=B,11=W,10=L
    FMT_NEW  = 31, // Ignore, new format entry
};

struct Entry {
    const Config::opcode_t opCode;
    const uint8_t fmt;
    const uint32_t flags;
    const char *name;

    static inline InsnFormat _insnFormat(uint8_t fmt) {
        return InsnFormat((fmt >> insnFmt_gp) & insnFmt_gm);
    }
    static constexpr uint8_t _fmt(InsnFormat iformat) {
        return (static_cast<uint8_t>(iformat) << insnFmt_gp);
    }
    static inline AddrMode _mode(uint8_t opr) {
        return AddrMode((opr >> addrMode_gp) & addrMode_gm);
    }
    static constexpr uint8_t _opr(AddrMode mode) {
        return (static_cast<uint8_t>(mode) << addrMode_gp);
    }
    static inline OprPos _srcPos(uint8_t pos) {
        return OprPos((pos >> srcPos_gp) & oprPos_gm);
    }
    static inline OprPos _dstPos(uint8_t pos) {
        return OprPos((pos >> dstPos_gp) & oprPos_gm);
    }
    static constexpr uint8_t _pos(OprPos src, OprPos dst) {
        return (static_cast<uint8_t>(src) << srcPos_gp)
            | (static_cast<uint8_t>(dst) << dstPos_gp);
    }
    static inline OprSize _oprSize(uint8_t size) {
        return OprSize((size >> oprSize_gp) & oprSize_gm);
    }
    static inline InsnSize _insnSize(uint8_t size) {
        return InsnSize((size >> insnSize_gp) & insnSize_gm);
    }
    static constexpr uint8_t _size(OprSize opr, InsnSize insn) {
        return (static_cast<uint8_t>(opr) << oprSize_gp)
            | (static_cast<uint8_t>(insn) << insnSize_gp);
    }
    static constexpr uint32_t _flags(
        uint8_t src, uint8_t dst, uint8_t pos, uint8_t size) {
        return (static_cast<uint32_t>(src) << src_gp)
            | (static_cast<uint32_t>(dst) << dst_gp)
            | (static_cast<uint32_t>(pos) << pos_gp)
            | (static_cast<uint32_t>(size) << size_gp);
    }
    static constexpr uint8_t _src(uint32_t flags) {
        return static_cast<uint8_t>(flags >> src_gp);
    }
    static constexpr uint8_t _dst(uint32_t flags) {
        return static_cast<uint8_t>(flags >> dst_gp);
    }
    static constexpr uint8_t _pos(uint32_t flags) {
        return static_cast<uint8_t>(flags >> pos_gp);
    }
    static constexpr uint8_t _size(uint32_t flags) {
        return static_cast<uint8_t>(flags >> size_gp);
    }

private:
    static constexpr int insnFmt_gp = 0;
    static constexpr uint8_t insnFmt_gm = 0x7f;
    // |src|, |dst|
    static constexpr uint8_t addrMode_gm = 0x3f;
    static constexpr int     addrMode_gp = 0;
    // |pos|
    static constexpr uint8_t oprPos_gm = 0xf;
    static constexpr int     srcPos_gp = 0;
    static constexpr int     dstPos_gp = 4;
    // |size|
    static constexpr uint8_t oprSize_gm  = 0xf;
    static constexpr uint8_t insnSize_gm = 0xf;
    static constexpr int     oprSize_gp  = 0;
    static constexpr int     insnSize_gp = 4;
    // |flags|
    static constexpr int src_gp  = 0;
    static constexpr int dst_gp  = 8;
    static constexpr int pos_gp  = 16;
    static constexpr int size_gp = 24;
};

} // namespace mc68000
} // namespace libasm

#endif // __ENTRY_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
