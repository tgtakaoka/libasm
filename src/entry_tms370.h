/*
 * Copyright 2025 Tadashi G. Takaoka
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

#ifndef __LIBASM_ENTRY_TMS370_H__
#define __LIBASM_ENTRY_TMS370_H__

#include <stdint.h>

#include "config_tms370.h"
#include "entry_base.h"

namespace libasm {
namespace tms370 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_A = 1,      // Register A
    M_B = 2,      // Regoster B
    M_RN = 3,     // Register N
    M_ST = 4,     // Register ST
    M_PN = 5,     // Port N
    M_REL = 6,    // 8-bit Relative
    M_LREL = 7,   // 16-bit Relative
    M_BREL = 8,   // 16-bit relative index with B
    M_ABS = 9,    // 16-bit Absolute
    M_IDIR = 10,  // Register indirect
    M_BIDX = 11,  // 16-bit index with B
    M_SIDX = 12,  // 8-bit indexed with SP
    M_RIDX = 13,  // 8-bit indexed with Rn
    M_IM8 = 14,   // 8-bit Immediate data
    M_SIM8 = 15,  // Signed 8-bit immediate
    M_IM16 = 16,  // 16-bit immediate
    M_BIMM = 17,  // 16-bit immediate with B
    M_RIMM = 18,  // 8-bit signed immediate with Rn
    M_TVEC = 19,  // Trap vector: 0~15
    // for assembler
    M_ADRR = 20,  // 8-bit immediate, register address
    M_ADRP = 21,  // 8-bit immediate, port address
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        static constexpr Flags create(AddrMode src, AddrMode dst, AddrMode ext) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(src) << src_gp) |
                                               (static_cast<uint16_t>(dst) << dst_gp) |
                                               (static_cast<uint16_t>(ext) << ext_gp))};
        }

        AddrMode src() const { return AddrMode((_attr >> src_gp) & src_gm); }
        AddrMode dst() const { return AddrMode((_attr >> dst_gp) & dst_gm); }
        AddrMode ext() const { return AddrMode((_attr >> ext_gp) & ext_gm); }

    private:
        static constexpr auto src_gp = 0;
        static constexpr auto dst_gp = 5;
        static constexpr auto ext_gp = 10;
        static constexpr uint16_t src_gm = 0x1F;
        static constexpr uint16_t dst_gm = 0x1F;
        static constexpr uint16_t ext_gm = 0x1F;
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opCode), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace tms370
}  // namespace libasm

#endif  // __LIBASM_ENTRY_TMS370_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
