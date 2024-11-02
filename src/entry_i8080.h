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

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint8_t _dst;
        uint8_t _src;

        static constexpr Flags create(AddrMode dst, AddrMode src) { return Flags{dst, src}; }

        static constexpr Flags undef(AddrMode dst, AddrMode src) {
            return Flags{dst, static_cast<uint8_t>(src | undef_bm)};
        }

        AddrMode dst() const { return AddrMode(_dst); }
        AddrMode src() const { return AddrMode(_src & ~undef_bm); }
        bool undefined() const { return _src & undef_bm; }

    private:
        static constexpr uint8_t undef_bm = 0x80;
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opCode), _flags_P(flags) {}

    Flags readFlags() const {
        return Flags{pgm_read_byte(&_flags_P._dst), pgm_read_byte(&_flags_P._src)};
    }

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
