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

#ifndef __LIBASM_ENTRY_TMS7000_H__
#define __LIBASM_ENTRY_TMS7000_H__

#include <stdint.h>

#include "config_tms7000.h"
#include "entry_base.h"

namespace libasm {
namespace tms7000 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_A = 1,      // Register A
    M_B = 2,      // Regoster B
    M_RN = 3,     // Register N
    M_ST = 4,     // Register ST
    M_IM8 = 5,    // 8-bit Immediate data
    M_PN = 6,     // Port N
    M_REL = 7,    // 8-bit Relative
    M_ABS = 8,    // 16-bit Absolute
    M_IDIR = 9,   // Register indirect
    M_BIDX = 10,  // 16-bit index with B
    M_IM16 = 11,  // 16-bit immediate
    M_BIMM = 12,  // 16-bit immediate with B
    M_TRAP = 13,  // Trap vector: 0~23
    // for assembler
    M_ADRR = 14,  // 8-bit immediate, register address
    M_ADRP = 15,  // 8-bit immediate, port address
};

namespace {
static constexpr int src_gp = 0;
static constexpr int dst_gp = 4;
static constexpr int ext_gp = 8;
static constexpr uint16_t mode_gm = 0xF;
}  // namespace

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        static constexpr Flags create(AddrMode src, AddrMode dst, AddrMode ext) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(src) << src_gp) |
                                               (static_cast<uint16_t>(dst) << dst_gp) |
                                               (static_cast<uint16_t>(ext) << ext_gp))};
        }

        AddrMode src() const { return AddrMode((_attr >> src_gp) & mode_gm); }
        AddrMode dst() const { return AddrMode((_attr >> dst_gp) & mode_gm); }
        AddrMode ext() const { return AddrMode((_attr >> ext_gp) & mode_gm); }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opCode), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace tms7000
}  // namespace libasm

#endif  // __LIBASM_ENTRY_TMS7000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
