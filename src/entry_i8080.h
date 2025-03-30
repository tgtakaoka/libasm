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

#ifndef __LIBASM_ENTRY_I8080_H__
#define __LIBASM_ENTRY_I8080_H__

#include <stdint.h>

#include "config_i8080.h"
#include "entry_base.h"
#include "reg_i8080.h"

namespace libasm {
namespace i8080 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_IM8 = 1,   // Immediate 8-bit
    M_IM16 = 2,  // Immediate 16-bit
    M_ABS = 3,   // Direct address 16-bit: nnnn
    M_IOA = 4,   // I/O address 8-bit
    M_PTR = 5,   // |..|pp|....|: B/D/H/SP
    M_STK = 6,   // |..|pp|....|: B/D/H/PSW
    M_IDX = 7,   // |...|i|....|: B/D
    M_SRC = 8,   // |......|rrr|: B/C/D/E/H/L/M/A
    M_DST = 9,   // |..|rrr|...|: B/C/D/E/H/L/M/A
    M_VEC = 10,  // |..|vvv|...|: 0~7
    M_CC = 11,   // |..|ccc|...|: NZ/Z/NC/C/PO/PE/P/M
    R_BASE = 12,
    R_C = R_BASE + REG_C,
    R_H = R_BASE + REG_H,
    R_A = R_BASE + REG_A,
    R_BC = R_BASE + REG_BC,
    R_DE = R_BASE + REG_DE,
    R_HL = R_BASE + REG_HL,
    R_SP = R_BASE + REG_SP,
    R_AF = R_BASE + REG_AF,
    R_IM = R_BASE + REG_IM,
    I_BASE = R_BASE + 16,
    I_C = I_BASE + REG_C,
    I_HL = I_BASE + REG_HL,
    I_SP = I_BASE + REG_SP,
};

enum CodeFormat : uint8_t {
    CF_00 = 0,  // 0x00
    CF_07 = 1,  // 0x07
    CF_10 = 2,  // 0x10
    CF_30 = 3,  // 0x30
    CF_38 = 4,  // 0x38
    CF_3F = 5,  // 0x3F
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        static constexpr Flags create(CodeFormat cf, AddrMode dst, AddrMode src) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(dst) << dst_gp) |
                                               (static_cast<uint16_t>(src) << src_gp) |
                                               (static_cast<uint16_t>(cf) << cf_gp))};
        }

        static constexpr Flags undef(CodeFormat cf, AddrMode dst, AddrMode src) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(dst) << dst_gp) |
                                               (static_cast<uint16_t>(src) << src_gp) |
                                               (static_cast<uint16_t>(cf) << cf_gp) | undef_bm)};
        }

        AddrMode dst() const { return AddrMode((_attr >> dst_gp) & mode_gm); }
        AddrMode src() const { return AddrMode((_attr >> src_gp) & mode_gm); }
        uint8_t mask() const {
            static constexpr uint8_t MASK[] PROGMEM = {
                    0x00,  // CF_00 = 0
                    0x07,  // CF_07 = 1
                    0x10,  // CF_10 = 2
                    0x30,  // CF_30 = 3
                    0x38,  // CF_38 = 4
                    0x3F,  // CF_3F = 5
            };
            return pgm_read_byte(&MASK[(_attr >> cf_gp) & cf_gm]);
        }
        bool undefined() const { return _attr & undef_bm; }

    private:
        static constexpr int dst_gp = 0;
        static constexpr int src_gp = 6;
        static constexpr int cf_gp = 12;
        static constexpr uint16_t mode_gm = 0x3F;
        static constexpr uint16_t cf_gm = 0x7;
        static constexpr uint16_t undef_bm = UINT16_C(1 << 15);
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace i8080
}  // namespace libasm

#endif  // __LIBASM_ENTRY_I8080_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
