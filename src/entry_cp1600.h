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

#ifndef __LIBASM_ENTRY_CP1600_H__
#define __LIBASM_ENTRY_CP1600_H__

#include <stdint.h>

#include "config_cp1600.h"
#include "entry_base.h"

namespace libasm {
namespace cp1600 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_DREG = 1,   // |_|___|___|ddd|: R0~R7
    M_LREG = 2,   // |_|___|___|_dd|: R0~R3
    M_SREG = 3,   // |_|___|sss|___|: R0~R7
    M_BREG = 4,   // |b|b__|___|___|: R4~R6
    M_XREG = 5,   // |_|___|sss|ddd|: R0~R7 sss=ddd
    M_SHCNT = 6,  // |_|___|___|n__|: shift count 1 or 2
    M_INDIR = 7,  // |_|___|iii|___|: R1~R3 indirect, R4/R5 post-inc
    M_BCOND = 8,  // |_|___|_1c|ccc|: external branch condition
    M_BIT0 = 9,   // |_|___|___|__x|: don't care bit 0
    M_BDISP = 10,  // |_|___|S__|___|: branch displacement, word 2
    M_JADDR = 11,  // |_|___|S__|S__|: jump addr-hi in word2 bits 7:2 (mask 00374)
    // 2nd word
    M_NOPP = 12,   // NOPP - consume the always-zero displacement word
    M_DADDR = 13,  // |_|___|000|___|: direct memory address: next word
    M_STACK = 14,  // |_|___|110|___|: R6 stack
    M_IMM16 = 15,  // |_|___|111|___|: immediate next word = data (MVII/ADDI/etc.)
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        static constexpr Flags create(AddrMode src, AddrMode dst) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(src) << src_gp) |
                                               (static_cast<uint16_t>(dst) << dst_gp))};
        }

        AddrMode src() const { return AddrMode((_attr >> src_gp) & src_gm); }
        AddrMode dst() const { return AddrMode((_attr >> dst_gp) & dst_gm); }
        Config::opcode_t opcodeMask() const { return modeMask(src()) | modeMask(dst()); }

    private:
        static Config::opcode_t modeMask(AddrMode mode) {
            static constexpr Config::opcode_t MASK[] PROGMEM = {
                    00000,  // M_NONE
                    00007,  // M_DREG
                    00003,  // M_LREG
                    00070,  // M_SREG
                    01400,  // M_BREG
                    00077,  // M_XREG
                    00004,  // M_SHCNT
                    00070,  // M_INDIR
                    00017,  // M_BCOND
                    00001,  // M_BIT0
                    00040,  // M_BDISP
                    00374,  // M_JADDR (word2 addr-high bits 7:2)
            };
            return mode >= M_NOPP ? 0 : pgm_read_word(&MASK[mode]);
        }

        static constexpr int src_gp = 0;
        static constexpr int dst_gp = 8;
        static constexpr uint_fast8_t src_gm = 0x1F;
        static constexpr uint_fast8_t dst_gm = 0x1F;
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opCode), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace cp1600
}  // namespace libasm

#endif  // __LIBASM_ENTRY_CP1600_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
