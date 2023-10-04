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

#ifndef __ENTRY_F3850_H__
#define __ENTRY_F3850_H__

#include <stdint.h>

#include "config_f3850.h"
#include "entry_base.h"
#include "reg_f3850.h"

namespace libasm {
namespace f3850 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_A = uint8_t(REG_A),    // Accumulator
    M_W = uint8_t(REG_W),    // Status
    M_IS = uint8_t(REG_IS),  // ISAR (Indirect scratchpad address register)
    M_KU = uint8_t(REG_KU),  // High 8-bit of K (R12)
    M_KL = uint8_t(REG_KL),  // Low 8-bit of K (R13)
    M_QU = uint8_t(REG_QU),  // High 8-bit of Q (R14)
    M_QL = uint8_t(REG_QL),  // Low 8-bit of Q (R15)
    M_H = uint8_t(REG_H),    // Data pointer scratchpad (R10:R11)
    M_K = uint8_t(REG_K),    // Stack pointer (R12:R13)
    M_Q = uint8_t(REG_Q),    // Data pointer scratchpad (R14:R15)
    M_P = uint8_t(REG_P),    // Program counter (PC1)
    M_P0 = uint8_t(REG_P0),  // Subroutine address (PC0)
    M_DC = uint8_t(REG_DC),  // Data pointer (DC0)
    M_J = 14,                // Scratchpad R9
    M_REG = 15,              // Scratchpad register number (0~15 and register aliases)
    M_C1 = 16,               // Immediate constant 1
    M_C4 = 17,               // Immediate constant 4
    M_IM3 = 18,              // 3-bit Immediate constant (0~7)
    M_IM4 = 19,              // 4-bit Immediate constant (0~15)
    M_IM8 = 20,              // 8-bit Immediate constant
    M_ADDR = 21,             // 16-bit Absolute address
    M_REL = 22,              // 8-bit Relative address
    M_UNDEF = 23,
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        static constexpr Flags create(AddrMode opr1, AddrMode opr2) {
            return Flags{static_cast<uint16_t>((static_cast<uint8_t>(opr1) << opr1_gp) |
                                               (static_cast<uint8_t>(opr2) << opr2_gp))};
        }

        static constexpr Flags undef() {
            return Flags{static_cast<uint16_t>((static_cast<uint8_t>(M_UNDEF) << opr1_gp) |
                                               (static_cast<uint8_t>(M_UNDEF) << opr2_gp))};
        }

        Flags read() const { return Flags{pgm_read_word(&_attr)}; }
        AddrMode mode1() const { return AddrMode((_attr >> opr1_gp) & mode_gm); }
        AddrMode mode2() const { return AddrMode((_attr >> opr2_gp) & mode_gm); }
        bool undefined() const { return mode1() == M_UNDEF; }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : Base(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    const Flags _flags;

    static constexpr int opr1_gp = 0;
    static constexpr int opr2_gp = 8;
    static constexpr uint8_t mode_gm = 0xFF;
};

}  // namespace f3850
}  // namespace libasm

#endif  // __ENTRY_F3850_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
