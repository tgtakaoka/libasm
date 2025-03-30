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

#ifndef __LIBASM_ENTRY_INS8070_H__
#define __LIBASM_ENTRY_INS8070_H__

#include <stdint.h>

#include "config_ins8070.h"
#include "entry_base.h"

namespace libasm {
namespace ins8070 {

enum OprSize : uint8_t {
    SZ_NONE = Size::SZ_NONE,
    SZ_BYTE = Size::SZ_BYTE,
    SZ_WORD = Size::SZ_WORD,
};

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_AR = 1,    // Accumulator: A
    M_ER = 2,    // Extension register: E
    M_EA = 3,    // Double precision Accumulator: EA
    M_TR = 4,    // Temporary register: T
    M_SR = 5,    // Status register: S
    M_P23 = 6,   // Pointer Register: P2/P3
    M_SP = 7,    // SP register
    M_PTR = 8,   // Base Register: PC/SP/P2/P3
    M_IMM = 9,   // Immediate constant
    M_ADR = 10,  // 16 bit constant
    M_VEC = 11,  // 4 bit constant
    M_IDX = 12,  // Pointer relative: nn,Pn
    M_PCR = 13,  // PC Relative
    M_GEN = 14,  // nnnn/nnnn,PC/nn,SP/nn,Pn/@nn,Pn/nn,@Pn/0xFFnn
};

enum CodeFormat : uint8_t {
    CF_00 = 0,  // 0x00
    CF_01 = 1,  // 0x01
    CF_03 = 2,  // 0x03
    CF_07 = 3,  // 0x07
    CF_0F = 4,  // 0x0F
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        static constexpr Flags create(CodeFormat cf, AddrMode dst, AddrMode src, OprSize size) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(dst) << dst_gp) |
                                               (static_cast<uint16_t>(src) << src_gp) |
                                               (static_cast<uint16_t>(size) << size_gp) |
                                               (static_cast<uint16_t>(cf) << cf_gp))};
        }

        static constexpr Flags exec(CodeFormat cf, AddrMode dst) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(dst) << dst_gp) |
                                               (static_cast<uint16_t>(cf) << cf_gp) | exec_bm)};
        }

        static constexpr Flags undef(CodeFormat cf, AddrMode dst, AddrMode src) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(dst) << dst_gp) |
                                               (static_cast<uint16_t>(src) << src_gp) |
                                               (static_cast<uint16_t>(cf) << cf_gp) | undef_bm)};
        }

        AddrMode dst() const { return AddrMode((_attr >> dst_gp) & mode_gm); }
        AddrMode src() const { return AddrMode((_attr >> src_gp) & mode_gm); }
        OprSize size() const { return OprSize((_attr >> size_gp) & size_gm); }
        bool execute() const { return _attr & exec_bm; }
        bool undefined() const { return _attr & undef_bm; }
        uint8_t mask() const {
            static constexpr uint8_t MASK[] PROGMEM = {
                    0x00,  // CF_00 = 0
                    0x01,  // CF_01 = 1
                    0x03,  // CF_03 = 2
                    0x07,  // CF_07 = 3
                    0x0F,  // CF_0F = 4
            };
            return pgm_read_byte(&MASK[(_attr >> cf_gp) & cf_gm]);
        }

    private:
        static constexpr int dst_gp = 0;
        static constexpr int src_gp = 4;
        static constexpr int size_gp = 8;
        static constexpr int cf_gp = 10;
        static constexpr int exec_bp = 14;
        static constexpr int undef_bp = 15;
        static constexpr uint_fast8_t mode_gm = 0x0F;
        static constexpr uint_fast8_t size_gm = 0x03;
        static constexpr uint_fast8_t cf_gm = 0x07;
        static constexpr uint16_t exec_bm = (1 << exec_bp);
        static constexpr uint16_t undef_bm = (1 << undef_bp);
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace ins8070
}  // namespace libasm

#endif  // __LIBASM_ENTRY_INS8070_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
