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

#ifndef __LIBASM_ENTRY_AM29000_H__
#define __LIBASM_ENTRY_AM29000_H__

#include <stdint.h>

#include "config_am29000.h"
#include "entry_base.h"

namespace libasm {
namespace am29000 {

// Every instruction is one 32-bit word divided into four fields:
//
//   31      24 23      16 15       8 7        0
//  |    OP    |    RC    |    RA    |    RB    |
//
// Each addressing mode names both the operand kind and the field it occupies,
// so an entry needs no separate position table.  Fields an instruction does
// not use are reserved and must read as zero.
enum AddrMode : uint8_t {
    M_NONE = 0,
    M_RC = 1,    // general register, RC field
    M_RA = 2,    // general register, RA field
    M_RB = 3,    // general register, RB field
    M_RBI = 4,   // general register or 8-bit constant, RB field; selects the M bit
    M_IM16 = 5,  // 16-bit constant, I15..I8 in RC and I7..I0 in RB
    M_TGT = 6,   // 16-bit word target, split like M_IM16; selects the A bit
    M_VN = 7,    // trap vector number, RC field
    M_SA = 8,    // special-purpose register, RA field
    M_CE = 9,    // coprocessor enable, bit 23
    M_CNTL = 10,  // load/store control, bits 22-16
    M_ID = 11,    // cache select of INV and IRETINV, bits 17-16
    M_UI = 12,    // CONVERT unsigned-integer select, bit 7
    M_RND = 13,   // CONVERT rounding mode, bits 6-4
    M_FD = 14,    // CONVERT destination format, bits 3-2
    M_FS = 15,    // CONVERT source format, bits 1-0
    M_FMT = 16,   // SQRT and CLASS format, bits 1-0
    M_TFMT = 17,  // MTACC and MFACC format, bits 3-2
    M_ACN = 18,   // accumulator number, bits 1-0
    M_ACNH = 19,  // accumulator number of FMAC and DMAC, bits 17-16
    M_FUNC = 20,  // function select of FMAC and DMAC, bits 21-18
    M_IMH = 21,   // 16-bit constant of CONSTH, placed like M_IM16
};

// The bits an operand of |mode| occupies.
static inline constexpr Config::opcode_t operandMask(AddrMode mode) {
    return mode == M_RC     ? 0x00FF0000
           : mode == M_RA   ? 0x0000FF00
           : mode == M_RB   ? 0x000000FF
           : mode == M_RBI  ? 0x010000FF
           : mode == M_IM16 ? 0x00FF00FF
           : mode == M_IMH  ? 0x00FF00FF
           : mode == M_TGT  ? 0x01FF00FF
           : mode == M_VN   ? 0x00FF0000
           : mode == M_SA   ? 0x0000FF00
           : mode == M_CE   ? 0x00800000
           : mode == M_CNTL ? 0x007F0000
           : mode == M_ID   ? 0x00030000
           : mode == M_UI   ? 0x00000080
           : mode == M_RND  ? 0x00000070
           : mode == M_FD   ? 0x0000000C
           : mode == M_FS   ? 0x00000003
           : mode == M_FMT  ? 0x00000003
           : mode == M_TFMT ? 0x0000000C
           : mode == M_ACN  ? 0x00000003
           : mode == M_ACNH ? 0x00030000
           : mode == M_FUNC ? 0x003C0000
                            : 0;
}

// The manual reserves some values of the floating-point control fields for
// future use, and an encoding which uses one is refused the same way a reserved
// field which is not zero is.  |value| is the field as encoded.
static inline constexpr bool controlValueOk(AddrMode mode, uint8_t value) {
    return mode == M_RND    ? value <= 4   // 101-111 reserved
           : mode == M_FD   ? value <= 2   // 11 reserved
           : mode == M_FS   ? value <= 2   // 11 reserved
           : mode == M_FMT  ? (value == 1 || value == 2)  // 00 and 11 reserved
           : mode == M_TFMT ? value <= 2   // 11 reserved
                            : true;
}

// An instruction takes up to six operands; CONVERT names a destination, a
// source, and four control fields.
static constexpr uint_fast8_t MAX_OPERANDS = 6;

// Arithmetic which a chip may trap to software.  An entry carrying one of
// these is accepted only while the matching Config option is on, which lets
// one table serve a chip with the hardware and one whose trap handlers provide
// the arithmetic instead; see FpuType and IntMulType.
enum Extension : uint8_t {
    EXT_NONE = 0,
    EXT_INTMUL = 1,  // needs the 32-by-32 integer multiplier
    EXT_FPU = 2,     // needs the floating-point unit
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint32_t _attr;

        static constexpr Flags create(AddrMode opr1 = M_NONE, AddrMode opr2 = M_NONE,
                AddrMode opr3 = M_NONE, AddrMode opr4 = M_NONE, AddrMode opr5 = M_NONE,
                AddrMode opr6 = M_NONE, Extension ext = EXT_NONE) {
            return Flags{static_cast<uint32_t>((static_cast<uint32_t>(opr1) << mode1_gp) |
                                               (static_cast<uint32_t>(opr2) << mode2_gp) |
                                               (static_cast<uint32_t>(opr3) << mode3_gp) |
                                               (static_cast<uint32_t>(opr4) << mode4_gp) |
                                               (static_cast<uint32_t>(opr5) << mode5_gp) |
                                               (static_cast<uint32_t>(opr6) << mode6_gp) |
                                               (static_cast<uint32_t>(ext) << ext_gp))};
        }

        Extension extension() const { return Extension((_attr >> ext_gp) & ext_gm); }

        AddrMode mode1() const { return AddrMode((_attr >> mode1_gp) & mode_gm); }
        AddrMode mode2() const { return AddrMode((_attr >> mode2_gp) & mode_gm); }
        AddrMode mode3() const { return AddrMode((_attr >> mode3_gp) & mode_gm); }
        AddrMode mode4() const { return AddrMode((_attr >> mode4_gp) & mode_gm); }
        AddrMode mode5() const { return AddrMode((_attr >> mode5_gp) & mode_gm); }
        AddrMode mode6() const { return AddrMode((_attr >> mode6_gp) & mode_gm); }

        AddrMode mode(uint_fast8_t pos) const {
            switch (pos) {
            case 0:
                return mode1();
            case 1:
                return mode2();
            case 2:
                return mode3();
            case 3:
                return mode4();
            case 4:
                return mode5();
            default:
                return mode6();
            }
        }

        // Whether one of the operands is the coprocessor-enable bit.
        bool hasCe() const {
            for (uint_fast8_t pos = 0; pos < MAX_OPERANDS; pos++) {
                if (mode(pos) == M_CE)
                    return true;
            }
            return false;
        }

        // The bits which belong to operands; every other bit of the word must
        // equal the entry's operation code.
        Config::opcode_t mask() const {
            Config::opcode_t m = 0;
            for (uint_fast8_t pos = 0; pos < MAX_OPERANDS; pos++)
                m |= operandMask(mode(pos));
            return m;
        }

    private:
        static constexpr int mode1_gp = 0;
        static constexpr int mode2_gp = 5;
        static constexpr int mode3_gp = 10;
        static constexpr int mode4_gp = 15;
        static constexpr int mode5_gp = 20;
        static constexpr int mode6_gp = 25;
        static constexpr int ext_gp = 30;
        static constexpr uint_fast8_t mode_gm = 0x1F;
        static constexpr uint_fast8_t ext_gm = 0x3;
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const /*PROGMEM*/ char *name_P)
        : Base(name_P, opCode), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_dword(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace am29000
}  // namespace libasm

#endif  // __LIBASM_ENTRY_AM29000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
