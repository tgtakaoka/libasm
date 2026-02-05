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
    // dst/src/ext
    M_WIMM = 1,  // 16-bit Immediate Constant
    M_BIMM = 2,  // Sign-extended 8-bit Immediate
    // dst/src
    M_AL = 3,     // Byte Accumulator: AL
    M_CL = 4,     // Bit Counter: CL
    M_BREG = 5,   // Byte Register: AL, AH, BL, BH, CL, CH, DL, DH
    M_AX = 6,     // Word Accumulator: AX
    M_DX = 7,     // Dynamic I/O Address: DX
    M_WREG = 8,   // Word Register: AX, BX, CX, DX, SP, BP, SI, DI
    M_SREG = 9,   // Segment Register: ES, CS, SS, DS
    M_BMOD = 10,  // Byte memory/register mode
    M_WMOD = 11,  // Word memory/register mode
    // M_BMEM to M_WDIR must be contiguous
    M_BMEM = 12,  // Byte memory: BYTE PTR [addr]
    M_WMEM = 13,  // Word memory: WORD PTR [addr]
    M_DMEM = 14,  // Word memory: WORD PTR [addr]
    M_FMEM = 15,  // i8087 float word, DWORD/QWORD/TBYTE PTR
    M_MEM = 16,   // Memory address: no sized M_BMEM/M_WMEM
    M_BDIR = 17,  // Byte Direct mode: BYTE PTR [nnnn]
    M_WDIR = 18,  // Word Direct mode: WORD PTR [nnnn]
    M_VAL1 = 19,  // Constant 1: for bit counter
    M_VAL3 = 20,  // Constant 3: for INT type
    M_REL = 21,   // Relative: 16-bit displacement
    M_REL8 = 22,  // Relative: 8-bit displacement
    M_IOA = 23,   // I/O Address
    M_SEG = 24,   // Segment: nnnn
    M_OFF = 25,   // Offset: nnnn
    M_FAR = 26,   // Far address: M_SEG:M_OFF
    M_CS = 28,    // Code Segment Register: CS
    M_UI16 = 29,  // 16-bit unsigned immediate
    M_UI8 = 30,   // 8-bit unsigned immediate
    M_BIT = 31,   // 3/4-bit shift count/bit number constant
    M_ST0 = 32,   // i8087 stack top ST(0)
    M_STI = 33,   // i8087 stack ST(i)
    // Assembler
    M_DIR = 34,  // Direct mode: [nnnn]
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
    // dst/src/ext
    P_OPR = 1,  // In operand
    // dst/src
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
        uint16_t _mode;
        uint16_t _attr;

        static constexpr Flags create(CodeFormat cf, AddrMode dst, AddrMode src, AddrMode ext,
                OprPos dpos, OprPos spos, OprPos epos, OprSize size, bool lockCapable) {
            return Flags{mode(dst, src, ext), attr(dpos, spos, epos, size, cf, false, lockCapable)};
        }

        static constexpr Flags strInst(CodeFormat cf, AddrMode dst, AddrMode src, OprSize size) {
            return Flags{
                    mode(dst, src, M_NONE), attr(P_NONE, P_NONE, P_NONE, size, cf, true, false)};
        }

        AddrMode dst() const { return AddrMode((_mode >> dst_gp) & dst_gm); }
        AddrMode src() const { return AddrMode((_mode >> src_gp) & src_gm); }
        AddrMode ext() const { return AddrMode((_mode >> ext_gp) & ext_gm); }
        OprPos dstPos() const { return OprPos((_attr >> dpos_gp) & dpos_gm); }
        OprPos srcPos() const { return OprPos((_attr >> spos_gp) & spos_gm); }
        OprPos extPos() const { return OprPos((_attr >> epos_gp) & epos_gm); }
        OprSize size() const { return OprSize((_attr >> size_gp) & size_gm); }
        bool stringInsn() const { return _attr & strInsn_bm; }
        bool lockCapable() const { return _attr & lockCapable_bm; }
        uint8_t mask() const {
            static constexpr uint8_t MASK[] PROGMEM = {
                    0000,  // CF_00 = 0
                    0007,  // CF_07 = 1
                    0030,  // CF_30 = 2
            };
            return pgm_read_byte(&MASK[(_attr >> cf_gp) & cf_gm]);
        }

    private:
        static constexpr uint16_t mode(AddrMode dst, AddrMode src, AddrMode ext) {
            return static_cast<uint16_t>((dst << dst_gp) | (src << src_gp) | (ext << ext_gp));
        }
        static constexpr uint16_t attr(OprPos dpos, OprPos spos, OprPos epos, OprSize size,
                CodeFormat cf, bool strInsn, bool lockCapable) {
            return static_cast<uint16_t>((dpos << dpos_gp) | (spos << spos_gp) | (epos << epos_gp) |
                                         (size << size_gp) | (cf << cf_gp) |
                                         (strInsn ? strInsn_bm : 0) |
                                         (lockCapable ? lockCapable_bm : 0));
        }

        // |_mode|
        static constexpr auto dst_gp = 0;
        static constexpr auto src_gp = 6;
        static constexpr auto ext_gp = 12;
        static constexpr uint_fast8_t dst_gm = 0x3F;
        static constexpr uint_fast8_t src_gm = 0x3F;
        static constexpr uint_fast8_t ext_gm = 0x03;
        // |_attr|
        static constexpr auto dpos_gp = 0;
        static constexpr auto spos_gp = 3;
        static constexpr auto epos_gp = 6;
        static constexpr auto size_gp = 7;
        static constexpr auto cf_gp = 10;
        static constexpr auto strInsn_bp = 12;
        static constexpr auto lockCapable_bp = 13;
        static constexpr uint_fast8_t dpos_gm = 0x7;
        static constexpr uint_fast8_t spos_gm = 0x7;
        static constexpr uint_fast8_t epos_gm = 0x1;
        static constexpr uint_fast8_t size_gm = 0x7;
        static constexpr uint_fast8_t cf_gm = 0x3;
        static constexpr uint_fast16_t strInsn_bm = (1 << strInsn_bp);
        static constexpr uint_fast16_t lockCapable_bm = (1 << lockCapable_bp);
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const {
        return Flags{pgm_read_word(&_flags_P._mode), pgm_read_word(&_flags_P._attr)};
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
