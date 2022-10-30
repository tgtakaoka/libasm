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

#ifndef __ENTRY_I8096_H__
#define __ENTRY_I8096_H__

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
    M_BAOP = 4,
    M_WAOP = 5,
    M_REL8 = 6,    // sign-extended 8-bit relative
    M_REL11 = 7,   // sign-extended 11-bit relative
    M_REL16 = 8,   // signed 16-bit relative
    M_BITNO = 9,   // bit number (0~7)
    M_COUNT = 10,  // #n (0~15) or M_BREG
    M_INDIR = 11,  // indirect (M_BAOP:2, M_WAOP:2)
    // not in TableI8096
    M_IMM8 = 12,   // 8-bit immediate (M_BAOP:1)
    M_IMM16 = 13,  // 16-bit immediate (M_WAOP:1)
    M_IDX8 = 14,   // 8-bit indexed (M_BAOP:3, M_WAOP:3)
    M_IDX16 = 15,  // 16-bit indexed (M_BAOP:3, M_WAOP:3)
    M_ADDR = 16,   // 16-bit address or register number
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint16_t _attr;

        static constexpr Flags create(AddrMode dst, AddrMode src1, AddrMode src2) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(dst) << dst_gp) |
                                               (static_cast<uint16_t>(src1) << src1_gp) |
                                               (static_cast<uint16_t>(src2) << src2_gp))};
        }

        static constexpr Flags undef(AddrMode dst, AddrMode src1) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(dst) << dst_gp) |
                                               (static_cast<uint16_t>(src1) << src1_gp) |
                                               (static_cast<uint16_t>(M_NONE) << src2_gp) |
                                               undef_bm)};
        }

        Flags read() const { return Flags{pgm_read_word(&_attr)}; }
        AddrMode dst() const { return AddrMode((_attr >> dst_gp) & mode_gm); }
        AddrMode src1() const { return AddrMode((_attr >> src1_gp) & mode_gm); }
        AddrMode src2() const { return AddrMode((_attr >> src2_gp) & mode_gm); }
        bool undefined() const { return _attr & undef_bm; }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr int dst_gp = 0;
    static constexpr int src1_gp = 5;
    static constexpr int src2_gp = 10;
    static constexpr int undef_bp = 15;
    static constexpr uint8_t mode_gm = 0x1f;
    static constexpr uint16_t undef_bm = (1 << undef_bp);
};

}  // namespace i8096
}  // namespace libasm

#endif  // __ENTRY_I8096_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
