/*
 * Copyright 2024 Tadashi G. Takaoka
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

#ifndef __LIBASM_ENTRY_PDP11_H__
#define __LIBASM_ENTRY_PDP11_H__

#include <stdint.h>

#include "config_pdp11.h"
#include "entry_base.h"

namespace libasm {
namespace pdp11 {

enum OprSize : uint8_t {
    SZ_NONE = Size::SZ_NONE,
    SZ_BYTE = Size::SZ_BYTE,  // Byte
    SZ_WORD = Size::SZ_WORD,  // Word; 16-bit binary
    SZ_DWRD = Size::SZ_QUAD,  // Double word; 32-bit binary
    SZ_FLOT = Size::SZ_DATA,  // Float; 32-bit DEC floating point
    SZ_DUBL = Size::SZ_OCTA,  // Double; 64-bit DEC floating point
    SZ_INTG = 7,              // Integer
    SZ_LONG = 8,              // Long
};

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_GENR = 1,   // Register: R0~R7/SP/PC
    M_GENS = 2,   // General source: #nnn/Rn/(Rn)/(Rn)+/-(Rn)/@(Rn)+/@-R(Rn)/X(Rn)/@X(Rn)
    M_GEND = 3,   // General destination: Rn/(Rn)/(Rn)+/-(Rn)/@(Rn)+/@-R(Rn)/X(Rn)/@X(Rn)
    M_GENA = 4,   // General address: (Rn)/(Rn)+/-(Rn)/@(Rn)+/@-R(Rn)/X(Rn)/@X(Rn)
    M_GENF = 5,   // General float source: #nnn/ACn/(Rn)/(Rn)+/-(Rn)/@(Rn)+/@-R(Rn)/X(Rn)/@X(Rn)
    M_GENG = 6,   // General float destination: ACn/(Rn)/(Rn)+/-(Rn)/@(Rn)+/@-R(Rn)/X(Rn)/@X(Rn)
    M_AC03 = 7,   // Restricted float register: AC0~AC3
    M_REL8 = 8,   // Relative 8-bit: -256~+254
    M_REL6 = 9,   // Relative 6-bit: -126~0
    M_IMM8 = 10,  // Immediate 8-bit: 0~255
    M_IMM6 = 11,  // Immediate 6-bit: 0~63
    M_IMM3 = 12,  // Immediate 3-bit: 0~7
};

enum OprPos : uint8_t {
    P_NONE = 0,
    P_0077 = 1,
    P_7700 = 2,
    P_0700 = 3,
    P_0007 = 4,
    P_0377 = 5,
    P_0300 = 6,
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint8_t _src;
        uint8_t _dst;
        uint8_t _attr;

        static constexpr Flags create(
                OprSize size, AddrMode src, OprPos spos, AddrMode dst, OprPos dpos) {
            return Flags{attr(src, spos), attr(dst, dpos), static_cast<uint8_t>(size)};
        }

        AddrMode src() const { return mode(_src); }
        AddrMode dst() const { return mode(_dst); }
        OprPos srcPos() const { return pos(_src); }
        OprPos dstPos() const { return pos(_dst); }
        OprSize size() const { return OprSize(_attr); }

    private:
        static constexpr uint8_t attr(AddrMode mode, OprPos pos) {
            return (static_cast<uint8_t>(mode) << mode_gp) | (static_cast<uint8_t>(pos) << pos_gp);
        }
        static constexpr AddrMode mode(uint8_t attr) {
            return AddrMode((attr >> mode_gp) & mode_gm);
        }
        static constexpr OprPos pos(uint8_t attr) { return OprPos((attr >> pos_gp) & pos_gm); }

        // |_src|, |_dst|
        static constexpr int mode_gp = 0;
        static constexpr int pos_gp = 4;
        static constexpr uint8_t mode_gm = 0xF;
        static constexpr uint8_t pos_gm = 0x7;
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opCode), _flags_P(flags) {}

    Flags readFlags() const {
        return Flags{pgm_read_byte(&_flags_P._src), pgm_read_byte(&_flags_P._dst),
                pgm_read_byte(&_flags_P._attr)};
    }

    static Config::opcode_t insnMask(OprPos pos) {
        static constexpr Config::opcode_t MASKS[] PROGMEM = {
                00000,  // P_NONE
                00077,  // P_0077
                07700,  // P_7700
                00700,  // P_0700
                00007,  // P_0007
                00377,  // P_0377
                00300,  // P_0300
        };
        return pgm_read_word(&MASKS[uint8_t(pos)]);
    }

private:
    const Flags _flags_P;
};

}  // namespace pdp11
}  // namespace libasm

#endif  // __LIBASM_ENTRY_PDP11_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
