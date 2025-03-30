/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef __LIBASM_ENTRY_F3850_H__
#define __LIBASM_ENTRY_F3850_H__

#include <stdint.h>

#include "config_f3850.h"
#include "entry_base.h"
#include "reg_f3850.h"

namespace libasm {
namespace f3850 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_A = REG_A,    // Accumulator
    M_W = REG_W,    // Status
    M_IS = REG_IS,  // ISAR (Indirect scratchpad address register)
    M_KU = REG_KU,  // High 8-bit of K (R12)
    M_KL = REG_KL,  // Low 8-bit of K (R13)
    M_QU = REG_QU,  // High 8-bit of Q (R14)
    M_QL = REG_QL,  // Low 8-bit of Q (R15)
    M_H = REG_H,    // Data pointer scratchpad (R10:R11)
    M_K = REG_K,    // Stack pointer (R12:R13)
    M_Q = REG_Q,    // Data pointer scratchpad (R14:R15)
    M_P = REG_P,    // Program counter (PC1)
    M_P0 = REG_P0,  // Subroutine address (PC0)
    M_DC = REG_DC,  // Data pointer (DC0)
    M_J = 14,       // Scratchpad R9
    M_REG = 15,     // Scratchpad register number (0~14 and register aliases)
    M_C1 = 16,      // Immediate constant 1
    M_C4 = 17,      // Immediate constant 4
    M_IM3 = 18,     // 3-bit Immediate constant (0~7)
    M_IM4 = 19,     // 4-bit Immediate constant (0~15)
    M_IM8 = 20,     // 8-bit Immediate constant
    M_ADDR = 21,    // 16-bit Absolute address
    M_REL = 22,     // 8-bit Relative address
    M_IOS = 23,     // 3-bit I/O address (0,1,4~15)
    M_IOA = 24,     // 8-bit I/O address (4~255)
};

enum CodeFormat : uint8_t {
    CF_00 = 0,  // 0x00
    CF_07 = 1,  // 0x07
    CF_0F = 2,  // 0x0F
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        static constexpr Flags create(CodeFormat cf, AddrMode opr1, AddrMode opr2) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(opr1) << opr1_gp) |
                                               (static_cast<uint16_t>(opr2) << opr2_gp) |
                                               (static_cast<uint16_t>(cf) << cf_gp))};
        }

        AddrMode mode1() const { return AddrMode((_attr >> opr1_gp) & mode_gm); }
        AddrMode mode2() const { return AddrMode((_attr >> opr2_gp) & mode_gm); }
        uint8_t mask() const {
            static constexpr uint8_t MASK[] PROGMEM = {
                    0x00,  // CF_00
                    0x07,  // CF_07
                    0x0F,  // CF_0F
            };
            return pgm_read_byte(&MASK[(_attr >> cf_gp) & cf_gm]);
        }

    private:
        static constexpr int opr1_gp = 0;
        static constexpr int opr2_gp = 6;
        static constexpr int cf_gp = 12;
        static constexpr uint8_t mode_gm = 0x3F;
        static constexpr uint8_t cf_gm = 0x03;
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace f3850
}  // namespace libasm

#endif  // __LIBASM_ENTRY_F3850_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
