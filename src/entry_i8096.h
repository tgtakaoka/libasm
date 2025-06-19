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

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_BREG = 1,
    M_WREG = 2,
    M_LREG = 3,
    M_BAOP = 4,    // Byte addressing
    M_WAOP = 5,    // Word addressing
    M_REL8 = 6,    // sign-extended 8-bit relative
    M_REL11 = 7,   // sign-extended 11-bit relative
    M_REL16 = 8,   // signed 16-bit relative
    M_BITNO = 9,   // bit number (0~7)
    M_COUNT = 10,  // #n (0~15) or M_BREG
    M_INDIR = 11,  // indirect (M_BAOP:2, M_WAOP:2)
    M_IMM8 = 12,   // 8-bit immediate (M_BAOP:1)
    M_IMM16 = 13,  // 16-bit immediate (M_WAOP:1)
    // not in TableI8096
    M_AINC = 14,   // auto increment
    M_IDX8 = 15,   // 8-bit indexed (M_BAOP:3, M_WAOP:3)
    M_IDX16 = 16,  // 16-bit indexed (M_BAOP:3, M_WAOP:3)
    M_ADDR = 17,   // 16-bit address or register number
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
        AddrMode src2() const { return AddrMode((_attr >> src2_gp) & mode_gm); }
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
        static constexpr int src1_gp = 4;
        static constexpr int src2_gp = 8;
        static constexpr int cf_gp = 12;
        static constexpr int undef_bp = 15;
        static constexpr uint_fast8_t mode_gm = 0xF;
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
