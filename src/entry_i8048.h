/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __LIBASM_ENTRY_I8048_H__
#define __LIBASM_ENTRY_I8048_H__

#include <stdint.h>

#include "config_i8048.h"
#include "entry_base.h"

namespace libasm {
namespace i8048 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_A = 1,       // A register
    M_R = 2,       // R[0-7] register
    M_IR = 3,      // @R[01] indirect
    M_IMM8 = 4,    // 8-bit Immediate data arithmetic
    M_BIT8 = 5,    // 8-bit Immediate data logical
    M_BITN = 6,    // Bit number 0-7
    M_BUS = 7,     // BUS port
    M_P12 = 8,     // P[12] port
    M_PEXT = 9,    // P[4-7] port
    M_AD11 = 10,   // 11-bit program code address + MB as MSB
    M_AD08 = 11,   // 8-bit internal page address
    M_IA = 12,     // @A indirect
    M_PSW = 13,    // PSW register
    M_C = 14,      // C flag
    M_I = 15,      // I flag
    M_F = 16,      // F0/F1 flag
    M_RB = 17,     // RB0/RB1 flag
    M_MB = 18,     // MB0/MB1 flag
    M_T = 19,      // T register
    M_CNT = 20,    // CNT register
    M_TCNT = 21,   // TCNT register
    M_TCNTI = 22,  // TCNTI register
    M_CLK = 23,    // CLK output
    // MSM80C39/MSM80C48
    M_IR3 = 24,  // @R3
    M_P = 25,    // P
    M_P1 = 26,   // P1
    M_P2 = 27,   // P2
};

enum CodeFormat : uint8_t {
    CF_00 = 0,  // 0x00
    CF_01 = 1,  // 0x01
    CF_03 = 2,  // 0x03
    CF_07 = 3,  // 0x07
    CF_10 = 4,  // 0x10
    CF_20 = 5,  // 0x20
    CF_E0 = 6,  // 0xE0
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        static constexpr Flags create(CodeFormat cf, AddrMode dst, AddrMode src) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(dst) << dst_gp) |
                                               (static_cast<uint16_t>(src) << src_gp) |
                                               (static_cast<uint16_t>(cf) << cf_gp))};
        }

        AddrMode dst() const { return AddrMode((_attr >> dst_gp) & mode_gm); }
        AddrMode src() const { return AddrMode((_attr >> src_gp) & mode_gm); }
        uint8_t mask() const {
            static constexpr uint8_t MASK[] PROGMEM = {
                0x00, // CF_00 = 0
                0x01, // CF_01 = 1
                0x03, // CF_03 = 2
                0x07, // CF_07 = 3
                0x10, // CF_10 = 4
                0x20, // CF_20 = 5
                0xE0, // CF_E0 = 6
            };
            return pgm_read_byte(&MASK[(_attr >> cf_gp) & cf_gm]);
        }

    private:
        static constexpr int dst_gp = 0;
        static constexpr int src_gp = 5;
        static constexpr int cf_gp = 10;
        static constexpr uint_fast8_t mode_gm = 0x1F;
        static constexpr uint_fast8_t cf_gm = 0x7;
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace i8048
}  // namespace libasm

#endif  // __LIBASM_ENTRY_I8048_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
