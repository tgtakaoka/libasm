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

#ifndef __LIBASM_ENTRY_I8086_H__
#define __LIBASM_ENTRY_I8086_H__

#include <stdint.h>

#include "config_i8086.h"
#include "entry_base.h"

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
    M_FMOD = 29,  // i8087 float word, DWORD/QWORD/TBYTE PTR
    M_ST0 = 30,   // i8087 stack top ST(0)
    M_STI = 31,   // i8087 stack ST(i)
};

enum OprSize : uint8_t {
    SZ_NONE = Size::SZ_NONE,
    SZ_BYTE = Size::SZ_BYTE,
    SZ_WORD = Size::SZ_WORD,
    SZ_DWORD = Size::SZ_QUAD,
    SZ_QWORD = Size::SZ_OCTA,
    SZ_TBYTE = Size::SZ_DATA,  // 10 byte
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

enum CodeFormat : uint8_t {
    CF_00 = 0,  // 0000
    CF_07 = 1,  // 0007
    CF_30 = 2,  // 0030
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint8_t _dst;
        uint8_t _src;
        uint8_t _ext;
        uint8_t _attr;

        static constexpr Flags create(CodeFormat cf, AddrMode dst, AddrMode src, AddrMode ext,
                OprPos dstPos, OprPos srcPos, OprPos extPos, OprSize size) {
            return Flags{_opr(dst, dstPos), _opr(src, srcPos), _opr(ext, extPos),
                    _size(size, cf, false, false)};
        }

        static constexpr Flags fpuInst(CodeFormat cf, AddrMode dst, AddrMode src, OprPos dstPos,
                OprPos srcPos, OprSize size) {
            return Flags{_opr(dst, dstPos), _opr(src, srcPos), _opr(M_NONE, P_NONE),
                    _size(size, cf, false, true)};
        }

        static constexpr Flags strInst(CodeFormat cf, AddrMode dst, AddrMode src, OprSize size) {
            return Flags{_opr(dst, P_NONE), _opr(src, P_NONE), _opr(M_NONE, P_NONE),
                    _size(size, cf, true, false)};
        }

        AddrMode dst() const { return AddrMode((_dst >> mode_gp) & mode_gm); }
        AddrMode src() const { return AddrMode((_src >> mode_gp) & mode_gm); }
        AddrMode ext() const { return AddrMode((_ext >> mode_gp) & mode_gm); }
        OprPos dstPos() const { return OprPos((_dst >> pos_gp) & pos_gm); }
        OprPos srcPos() const { return OprPos((_src >> pos_gp) & pos_gm); }
        OprPos extPos() const { return OprPos((_ext >> pos_gp) & pos_gm); }
        OprSize size() const { return OprSize((_attr >> size_gp) & size_gm); }
        bool stringInst() const { return _attr & strInst_bm; }
        bool fpuInst() const { return _attr & fpuInst_bm; }
        uint8_t mask() const {
            static constexpr uint8_t MASK[] PROGMEM = {
                    0000,  // CF_00 = 0
                    0007,  // CF_07 = 1
                    0030,  // CF_30 = 2
            };
            return pgm_read_byte(&MASK[(_attr >> cf_gp) & cf_gm]);
        }

    private:
        static constexpr uint8_t _opr(AddrMode mode, OprPos pos) {
            return (static_cast<uint8_t>(mode) << mode_gp) | (static_cast<uint8_t>(pos) << pos_gp);
        }
        static constexpr uint8_t _size(OprSize size, CodeFormat cf, bool strInst, bool fpuInst) {
            return (static_cast<uint8_t>(size) << size_gp) | (static_cast<uint8_t>(cf) << cf_gp) |
                   (strInst ? strInst_bm : 0) | (fpuInst ? fpuInst_bm : 0);
        }

        // |_dst|, |_src|, |_ext|
        static constexpr int mode_gp = 0;
        static constexpr int pos_gp = 5;
        static constexpr uint_fast8_t mode_gm = 0x1F;
        static constexpr uint_fast8_t pos_gm = 0x07;
        // |_attr|
        static constexpr int size_gp = 0;
        static constexpr int cf_gp = 3;
        static constexpr int strInst_bp = 5;
        static constexpr int fpuInst_bp = 6;
        static constexpr uint_fast8_t size_gm = 0x07;
        static constexpr uint_fast8_t cf_gm = 0x03;
        static constexpr uint_fast8_t strInst_bm = (1 << strInst_bp);
        static constexpr uint_fast8_t fpuInst_bm = (1 << fpuInst_bp);
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const {
        return Flags{pgm_read_byte(&_flags_P._dst), pgm_read_byte(&_flags_P._src),
                pgm_read_byte(&_flags_P._ext), pgm_read_byte(&_flags_P._attr)};
    }

private:
    const Flags _flags_P;
};

}  // namespace i8086
}  // namespace libasm

#endif  // __LIBASM_ENTRY_I8086_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
