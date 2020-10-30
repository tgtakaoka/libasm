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

#include "config_i8086.h"

namespace libasm {
namespace i8086 {

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
    M_AL   = 1,  // Byte Accumulator: AL
    M_CL   = 2,  // Bit Counter: CL
    M_BREG = 3,  // Byte Register: AL, AH, BL, BH, CL, CH, DL, DH
    M_AX   = 4,  // Word Accumulator: AX
    M_DX   = 5,  // Dynamic I/O Address: DX
    M_WREG = 6,  // Word Register: AX, BX, CX, DX, SP, BP, SI, DI
    M_SREG = 7,  // Segment Register: ES, CS, SS, DS
    M_BMOD = 8,  // Byte memory/register mode
    M_WMOD = 9,  // Word memory/register mode
    M_BMEM = 10, // Byte memory: BYTE PTR [addr]
    M_WMEM = 11, // Word memory: WORD PTR [addr]
    M_BDIR = 12, // Byte Direct mode: BYTE PTR [nnnn]
    M_WDIR = 13, // Word Direct mode: WORD PTR [nnnn]
    M_IMM  = 14, // Immediate Constant
    M_IMM8 = 15, // Sign-extended 8-bit Immediate
    M_VAL1 = 16, // Constant 1: for bit counter
    M_VAL3 = 17, // Constant 3: for INT type
    M_REL  = 18, // Relative: 16-bit displacement
    M_REL8 = 19, // Relative: 8-bit displacement
    M_IOA  = 20, // I/O Address
    M_FAR  = 21, // Far address: seg:off
    M_ISTR = 22, // String instruction: MOVSi/CMPSi/STOSi/LODSi/SCASi
    M_MEM  = 23, // Memory address: [addr]
    M_DIR  = 24, // Direct mode: [nnnn]
    M_CS   = 25, // Code Segment Register: CS
};

enum OprPos : uint8_t {
    P_NONE = 0,
    P_OREG = 1,  // In opcode: ____|_rrr
    P_OSEG = 2,  // In opcode: ___s|s___
    P_MOD  = 3,  // In mod-reg-r/m: mo__|_r/m
    P_REG  = 4,  // In mod-reg-r/m: __re|g___
    P_OMOD = 5,  // In opcode:      mo__|_r/m
    P_OPR  = 6,  // In operand
};

enum OprSize : uint8_t {
    SZ_NONE = 0,
    SZ_BYTE = 1, // Byte
    SZ_WORD = 2, // Word
    SZ_SOFF = 3, // Segment:Offset
};

struct Entry {
    const Config::opcode_t opCode;
    const uint32_t flags;
    const char *name;

    static inline uint8_t _dst(uint32_t flags) {
        return static_cast<uint8_t>(flags >> dst_gp);
    }
    static inline uint8_t _src(uint32_t flags) {
        return static_cast<uint8_t>(flags >> src_gp);
    }
    static inline uint8_t _pos(uint32_t flags) {
        return static_cast<uint8_t>(flags >> pos_gp);
    }
    static inline uint8_t _size(uint32_t flags) {
        return static_cast<uint8_t>(flags >> size_gp);
    }
    static inline AddrMode _mode(uint8_t opr) {
        return AddrMode((opr >> mode_gp) & mode_gm);
    }
    static inline OprPos _dstPos(uint8_t pos) {
        return OprPos((pos >> dstPos_gp) & pos_gm);
    }
    static inline OprPos _srcPos(uint8_t pos) {
        return OprPos((pos >> srcPos_gp) & pos_gm);
    }
    static inline OprSize _size(uint8_t size) {
        return OprSize((size >> oprSize_gp) & oprSize_gm);
    }
    static inline bool _strInst(uint8_t size) {
        return size & (1 << strInst_bp);
    }

    static constexpr uint8_t _opr(AddrMode mode) {
        return (static_cast<uint8_t>(mode) << mode_gp);
    }
    static constexpr uint8_t _pos(OprPos dstPos, OprPos srcPos) {
        return (static_cast<uint8_t>(dstPos) << dstPos_gp)
            | (static_cast<uint8_t>(srcPos) << srcPos_gp);
    }
    static constexpr uint8_t _size(OprSize size, bool strInst) {
        return (static_cast<uint8_t>(size) << oprSize_gp)
            | (strInst ? (1 << strInst_bp) : 0);
    }
    static constexpr uint32_t _flags(
            AddrMode dst, AddrMode src,
            OprPos dstPos, OprPos srcPos,
            OprSize size, bool strInst) {
        return (static_cast<uint32_t>(_opr(dst)) << dst_gp)
            | (static_cast<uint32_t>(_opr(src)) << src_gp)
            | (static_cast<uint32_t>(_pos(dstPos, srcPos)) << pos_gp)
            | (static_cast<uint32_t>(_size(size, strInst)) << size_gp);
    }

private:
    static constexpr int dst_gp = 0;
    static constexpr int src_gp = 8;
    static constexpr int pos_gp = 16;
    static constexpr int size_gp = 24;
    // |dst|, |src|
    static constexpr int     mode_gp = 0;
    static constexpr uint8_t mode_gm = 0x1F;
    // |pos|
    static constexpr int dstPos_gp = 0;
    static constexpr int srcPos_gp = 4;
    static constexpr uint8_t pos_gm  = 0x07;
    // |size|
    static constexpr int     oprSize_gp = 0;
    static constexpr int     strInst_bp = 4;
    static constexpr uint8_t oprSize_gm = 0x03;
};

} // namespace i8086
} // namespace libasm

#endif // __ENTRY_I8086_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
