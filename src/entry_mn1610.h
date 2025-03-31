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

#ifndef __LIBASM_ENTRY_MN1610_H__
#define __LIBASM_ENTRY_MN1610_H__

#include <stdint.h>

#include "config_mn1610.h"
#include "entry_base.h"

namespace libasm {
namespace mn1610 {

enum AddrMode : uint8_t {
    M_NONE = 0,  // No operand
    // mode1, mode2, mode3, mode4
    M_SKIP = 1,  // Skip specifier
    // mode1, mode2, mode3
    M_COP = 2,   // Carry mode: 0, 1, C
    M_ABS = 3,   // Absolute address (16 bit offset of 16/18/20 bit segmented address)
    M_RIAU = 4,  // Register indirect with auto update: M_RI, -(Ri), (Ri)+
    M_R0 = 5,    // R0
    // mode1
    M_GEN = 6,   // Generic addressing: D, d(IC), (D), (d(IC)), D(Xn), (D)(Xn)
    M_IABS = 7,  // Indirect absolute: (addr)
    M_ILVL = 8,  // 2-bit interrupt level
    M_RB = 9,    // Segment base register: M_SB, OSR0-OSR3
    M_RBW = 10,  // Segment base register: M_RB without CSBR
    M_RD = 11,   // Destination register: R0-R4, X0-X1, SP, STR
    M_RDG = 12,  // Destination register: R0-R4, X0-X1, SP (no STR)
    M_RDS = 13,  // Destination/Source register: R0-R4, X0-X1, SP, STR
    M_RS = 14,   // Source register: R0-R4, X0-X1, SP, STR
    M_RSG = 15,  // Source register: R0-R4, X0-X1, SP (no STR)
    M_RI = 16,   // Register indirect: (Ri):i=1-4, (Xj):j=0-1
    M_RI1 = 17,  // (R1)
    M_RI2 = 18,  // (R2)
    M_DR0 = 19,  // DR0: R0:R1
    M_RP = 20,   // Special register
    // mode1, mode2
    M_BIT = 21,   // Bit number of 16-bit
    M_IM4 = 22,   // 4-bit immediate data (non-negative)
    M_IM8 = 23,   // 8-bit immediate data
    M_IM8W = 24,  // 8-bit immediate in 16-bit word
    M_IM16 = 25,  // 16-bit immediate
    M_IOA = 26,   // 8-bit I/O address
    M_EOP = 27,   // E-register operation: RE, SE, CM
    M_SB = 28,    // Segment base register: CSRB, SSRB, TSR0, TSR1
    M_RHR = 29,   // Hardware register (readable) SIR
    M_RHW = 30,   // Hardware register (writable) SOR
    // for assembler parser, not in instruction table
    M_INDX = 31,  // v(r)
    M_IXID = 32,  // (v(r))
    M_IDIX = 33,  // (v)(r)
};

enum CodeFormat : uint8_t {
    CF_0000 = 0,   // 0x0000
    CF_0003 = 1,   // 0x0003
    CF_0007 = 2,   // 0x0007
    CF_0037 = 3,   // 0x0037
    CF_0070 = 4,   // 0x0070
    CF_0077 = 5,   // 0x0077
    CF_00F0 = 6,   // 0x00F0
    CF_00F3 = 7,   // 0x00F3
    CF_00F7 = 8,   // 0x00F7
    CF_00FB = 9,   // 0x00FB
    CF_00FF = 10,  // 0x00FF
    CF_0700 = 11,  // 0x0700
    CF_0703 = 12,  // 0x0703
    CF_07C3 = 13,  // 0x07C3
    CF_07F0 = 14,  // 0x07F0
    CF_07F3 = 15,  // 0x07F3
    CF_07F7 = 16,  // 0x07F7
    CF_07FF = 17,  // 0x07FF
    CF_38FF = 18,  // 0x38FF
    CF_3FFF = 19,  // 0x3FFF
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint32_t _attr;

        static constexpr Flags create(
                CodeFormat cf, AddrMode opr1, AddrMode opr2, AddrMode opr3, AddrMode opr4) {
            return Flags{static_cast<uint32_t>((static_cast<uint32_t>(opr1) << mode1_gp) |
                                               (static_cast<uint32_t>(opr2) << mode2_gp) |
                                               (static_cast<uint32_t>(opr3) << mode3_gp) |
                                               (static_cast<uint32_t>(opr4) << mode4_gp) |
                                               (static_cast<uint32_t>(cf) << cf_gp))};
        }

        AddrMode mode1() const { return AddrMode((_attr >> mode1_gp) & mode1_gm); }
        AddrMode mode2() const { return AddrMode((_attr >> mode2_gp) & mode2_gm); }
        AddrMode mode3() const { return AddrMode((_attr >> mode3_gp) & mode3_gm); }
        AddrMode mode4() const { return AddrMode((_attr >> mode4_gp) & mode4_gm); }
        Config::opcode_t mask() const {
            static constexpr Config::opcode_t MASK[] = {
                    0x0000,  // CF_0000 = 0
                    0x0003,  // CF_0003 = 1
                    0x0007,  // CF_0007 = 2
                    0x0037,  // CF_0037 = 3
                    0x0070,  // CF_0070 = 4
                    0x0077,  // CF_0077 = 5
                    0x00F0,  // CF_00F0 = 6
                    0x00F3,  // CF_00F3 = 7
                    0x00F7,  // CF_00F7 = 8
                    0x00FB,  // CF_00FB = 9
                    0x00FF,  // CF_00FF = 10
                    0x0700,  // CF_0700 = 11
                    0x0703,  // CF_0703 = 12
                    0x07C3,  // CF_07C3 = 13
                    0x07F0,  // CF_07F0 = 14
                    0x07F3,  // CF_07F3 = 15
                    0x07F7,  // CF_07F7 = 16
                    0x07FF,  // CF_07FF = 17
                    0x38FF,  // CF_38FF = 18
                    0x3FFF,  // CF_3FFF = 19
            };
            return pgm_read_word(&MASK[(_attr >> cf_gp) & cf_gm]);
        }

    private:
        static constexpr int mode1_gp = 0;
        static constexpr int mode2_gp = 5;
        static constexpr int mode3_gp = 10;
        static constexpr int mode4_gp = 13;
        static constexpr int cf_gp = 14;
        static constexpr uint_fast8_t mode1_gm = 0x1F;
        static constexpr uint_fast8_t mode2_gm = 0x1F;
        static constexpr uint_fast8_t mode3_gm = 0x7;
        static constexpr uint_fast8_t mode4_gm = 0x1;
        static constexpr uint_fast8_t cf_gm = 0x1F;
    };

    constexpr Entry(Config::opcode_t opc, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opc), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_dword(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace mn1610
}  // namespace libasm

#endif  // __LIBASM_ENTRY_MN1610_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
