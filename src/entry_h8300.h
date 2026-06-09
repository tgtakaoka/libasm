/*
 * Copyright 2026 Tadashi G. Takaoka
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

#ifndef __LIBASM_ENTRY_H8300_H__
#define __LIBASM_ENTRY_H8300_H__

#include <stdint.h>

#include "config_h8300.h"
#include "entry_base.h"

namespace libasm {
namespace h8300 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_REG8 = 1,    // 8-bit register: R0H..R7L
    M_REG16 = 2,   // 16-bit register: R0..R7
    M_ADREG = 3,   // address regider: R0..R6,SP
    M_INDIR = 4,   // register indirect: @Rn
    M_IDX16 = 5,   // 16-bit displacement: @(d:16,Rn)
    M_PINC = 6,    // post-increment: @Rn+
    M_PDEC = 7,    // pre-decrement: @-Rn
    M_ABS8 = 8,    // absolute 8-bit: @aa:8
    M_ABS16 = 9,   // absolute 16-bit: @aa:16
    M_REL8 = 10,   // PC-relative 8-bit (branches)
    M_IMM8 = 11,   // 8-bit immediate: #imm:8
    M_IMM16 = 12,  // 16-bit immediate: #imm:16
    M_MIND8 = 13,  // memory-indirect: @@aa:8
    M_IMM3 = 14,   // 3-bit bit number: #bit:3
    M_VAL1 = 15,   // implicit #1 (ADDS/SUBS)
    M_VAL2 = 16,   // implicit #2 (ADDS/SUBS)
    M_CCR = 17,    // CCR control register
    // H8/300H
    M_REG32 = 18,  // 32-bit register: ER0..ER7
    M_IMM32 = 19,  // 32-bit immediate: #imm:32
    M_IDX24 = 20,  // 24-bit displacement: @(d:24,ERn)
    M_ABS24 = 21,  // 24-bit absolute: @aa:24
    M_REL16 = 22,  // PC-relative 16-bit (long branches)
    M_VAL4 = 23,   // implicit #4 (ADDS.L/SUBS.L)
    // H8S additions:
    M_RLIST = 24,  // register list (LDM/STM): op.reg = first ER, op.val = count
    M_EXR = 25,    // EXR extended control register
    M_IDX32 = 26,  // 32-bit displacement: @(d:32,ERn)
    M_ABS32 = 27,  // 32-bit absolute: @aa:32 (advanced-mode-only data form)
};

// Instruction name size suffix
enum InsnSize : uint8_t {
    ISZ_NONE = 0,  // no size suffix
    ISZ_DATA = 1,  // depends on OprSize
    ISZ_ADDR = 2,  // size follows the M_ADREG register width (SZ_WORD or SZ_LONG)
};

enum OprSize : uint8_t {
    SZ_NONE = Size::SZ_NONE,
    SZ_BYTE = Size::SZ_BYTE,  // byte
    SZ_WORD = Size::SZ_WORD,  // word, aligned to even address
    SZ_LONG = Size::SZ_QUAD,  // longword (32-bit, H8/300H)
};

enum OprPos : uint8_t {
    POS____ = 0,
    POS_F__ = 1,  // ____xxxx ________
    POS__FF = 2,  // ________ xxxxxxxx
    POS__F_ = 3,  // ________ xxxx____
    POS__7_ = 4,  // ________ _xxx____
    POS___F = 5,  // ________ ____xxxx
    POS___7 = 6,  // ________ _____xxx (3-bit field at bits[2:0], H8/300H)
    POS_PRX = 7,  // in prefix
};

// Super-prefix word consumed before any normal prefix.
enum SuperPrefix : uint8_t {
    SPRX_NONE = 0,
    SPRX_0100 = 1,
    SPRX_0140 = 2,
    SPRX_0141 = 3,  // H8S LDC/STC/ANDC/ORC/XORC with EXR
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint8_t _src;
        uint8_t _dst;
        uint8_t _pos;

        static constexpr Flags create(InsnSize isz, OprSize osz, AddrMode smode, AddrMode dmode,
                OprPos spos, OprPos dpos) {
            return Flags{src(smode, isz), dst(dmode, osz), pos(spos, dpos)};
        }

        InsnSize insnSize() const { return InsnSize((_src >> isz_gp) & isz_gm); }
        OprSize oprSize() const { return OprSize((_dst >> osz_gp) & osz_gm); }
        AddrMode src() const { return AddrMode((_src >> mode_gp) & mode_gm); }
        AddrMode dst() const { return AddrMode((_dst >> mode_gp) & mode_gm); }
        OprPos srcPos() const { return OprPos((_pos >> spos_gp) & pos_gm); }
        OprPos dstPos() const { return OprPos((_pos >> dpos_gp) & pos_gm); }
        uint16_t opCodeMask() const { return posMask(srcPos()) | posMask(dstPos()); }

    private:
        static constexpr uint8_t src(AddrMode smode, InsnSize isz) {
            return static_cast<uint8_t>((smode << mode_gp) | (isz << isz_gp));
        }
        static constexpr uint8_t dst(AddrMode dmode, OprSize osz) {
            return static_cast<uint8_t>((dmode << mode_gp) | (osz << osz_gp));
        }
        static constexpr uint8_t pos(OprPos spos, OprPos dpos) {
            return static_cast<uint8_t>((spos << spos_gp) | (dpos << dpos_gp));
        }
        // |_src|, |_dst|
        static constexpr int mode_gp = 0;
        static constexpr uint_fast8_t mode_gm = 0x1F;
        // |_src| InsnSize widened to 2 bits at bits 6-7 to fit ISZ_ADDR.
        static constexpr int isz_gp = 6;
        static constexpr uint_fast8_t isz_gm = 0x03;
        // |_dst|
        static constexpr int osz_gp = 6;
        static constexpr uint_fast8_t osz_gm = 0x03;
        // |_pos|
        static constexpr int spos_gp = 0;
        static constexpr int dpos_gp = 4;
        static constexpr uint_fast8_t pos_gm = 0x7;
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const {
        return Flags{pgm_read_byte(&_flags_P._src), pgm_read_byte(&_flags_P._dst),
                pgm_read_byte(&_flags_P._pos)};
    }

    static Config::opcode_t decodeOperand(Config::opcode_t opc, OprPos pos) {
        return (opc & posMask(pos)) >> bitGroupPos(pos);
    }

    static Config::opcode_t posMask(OprPos pos) {
        static constexpr Config::opcode_t MASK[] PROGMEM = {
                0x0000,  // POS____
                0x0F00,  // POS_F__
                0x00FF,  // POS__FF
                0x00F0,  // POS__F_
                0x0070,  // POS__7_
                0x000F,  // POS___F
                0x0007,  // POS___7
                0x0000,  // POS_PRX
        };
        return pgm_read_word(&MASK[pos]);
    }

    static uint_fast8_t bitGroupPos(OprPos pos) {
        static constexpr uint8_t GPOS[] PROGMEM = {
                0,  // POS____
                8,  // POS_F__
                0,  // POS__FF
                4,  // POS__F_
                4,  // POS__7_
                0,  // POS___F
                0,  // POS___7
                0,  // POS_PRX
        };
        return pgm_read_word(&GPOS[pos]);
    }

private:
    const Flags _flags_P;
};

}  // namespace h8300
}  // namespace libasm

#endif  // __LIBASM_ENTRY_H8300_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
