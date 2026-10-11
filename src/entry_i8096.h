/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __LIBASM_ENTRY_I8096_H__
#define __LIBASM_ENTRY_I8096_H__

#include <stdint.h>

#include "config_i8096.h"
#include "entry_base.h"

namespace libasm {
namespace i8096 {

enum AaMode : uint8_t {
    AA_REG = 0,    // Register direct
    AA_IMM = 1,    // Immediate
    AA_INDIR = 2,  // Indirect
    AA_IDX = 3,    // Indexed
};

// Each operand position stores only the modes that can appear there, so the
// fields are sized per position: the third operand takes just five of these,
// which is why they come first.
enum AddrMode : uint8_t {
    // src2 and up: 3 bits
    M_NONE = 0,
    M_BAOP = 1,    // Byte addressing
    M_WAOP = 2,    // Word addressing
    M_IMM8 = 3,    // 8-bit immediate (M_BAOP:1)
    M_REL8 = 4,    // sign-extended 8-bit relative
    // dst and src1: 5 bits
    M_BREG = 5,
    M_WREG = 6,
    M_LREG = 7,
    M_REL11 = 8,   // sign-extended 11-bit relative
    M_REL16 = 9,   // signed 16-bit relative
    M_BITNO = 10,  // bit number (0~7)
    M_COUNT = 11,  // #n (0~15) or M_BREG
    M_INDIR = 12,  // indirect (M_BAOP:2, M_WAOP:2)
    M_BIDX = 13,   //  indexed (M_BAOP:3)
    M_WIDX = 14,   //  indexed (M_WAOP:3)
    M_IMM16 = 15,  // 16-bit immediate (M_WAOP:1)
    M_EINDIR = 16, // extended indirect: [treg]
    M_EIDX = 17,   // extended indexed: disp24[treg]
    M_REL24 = 18,  // signed 24-bit relative
    M_QREG = 19,   // quad-word register, divisible by eight
    M_EBIND = 20,  // EBR's extended indirect; bit 0 of the operand byte is set
                   // to tell it from BR, which shares opcode E3
    // not in TableI8096
    M_AINC = 21,   // auto increment
    M_IDX8 = 22,   // 8-bit indexed (M_BAOP:3, M_WAOP:3)
    M_IDX16 = 23,  // 16-bit indexed (M_BAOP:3, M_WAOP:3)
    M_REGNO = 24,  // 8-bit register number
    M_ADDR = 25,   // 16-bit address
};

enum CodeFormat : uint8_t {
    CF_00 = 0,  // 0x00
    CF_03 = 1,  // 0x03
    CF_07 = 2,  // 0x07
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        static constexpr Flags create(CodeFormat cf, AddrMode dst, AddrMode src1, AddrMode src2) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(dst) << dst_gp) |
                                               (static_cast<uint16_t>(src1) << src1_gp) |
                                               (static_cast<uint16_t>(src2) << src2_gp) |
                                               (static_cast<uint16_t>(cf) << cf_gp))};
        }

        static constexpr Flags undef(CodeFormat cf, AddrMode dst, AddrMode src1) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(dst) << dst_gp) |
                                               (static_cast<uint16_t>(src1) << src1_gp) |
                                               (static_cast<uint16_t>(cf) << cf_gp) | undef_bm)};
        }

        AddrMode dst() const { return AddrMode((_attr >> dst_gp) & mode_gm); }
        AddrMode src1() const { return AddrMode((_attr >> src1_gp) & mode_gm); }
        AddrMode src2() const { return AddrMode((_attr >> src2_gp) & src2_gm); }
        bool undefined() const { return _attr & undef_bm; }
        uint8_t mask() const {
            static constexpr uint8_t MASK[] PROGMEM = {
                    0x00,  // CF_00 = 0
                    0x03,  // CF_03 = 1
                    0x07,  // CF_07 = 2
            };
            return pgm_read_byte(&MASK[(_attr >> cf_gp) & cf_gm]);
        }

    private:
        static constexpr int dst_gp = 0;
        static constexpr int src1_gp = 5;
        static constexpr int src2_gp = 10;
        static constexpr int cf_gp = 13;
        static constexpr int undef_bp = 15;
        static constexpr uint_fast8_t mode_gm = 0x1F;
        static constexpr uint_fast8_t src2_gm = 0x7;
        static constexpr uint_fast8_t cf_gm = 0x3;
        static constexpr uint16_t undef_bm = (1 << undef_bp);
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace i8096
}  // namespace libasm

#endif  // __LIBASM_ENTRY_I8096_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
