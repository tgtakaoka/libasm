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

#ifndef __ENTRY_MN1610_H__
#define __ENTRY_MN1610_H__

#include <stdint.h>

#include "config_mn1610.h"
#include "entry_base.h"

namespace libasm {
namespace mn1610 {

enum AddrMode : uint8_t {
    M_NONE = 0,   // No operand
    M_SKIP = 1,   // Skip specifier
    M_ILVL = 2,   // 2-bit interrupt level
    M_BIT = 3,    // Bit number of 16-bit
    M_IM4 = 4,    // 4-bit immediate data (non-negative)
    M_IM8 = 5,    // 8-bit immediate data
    M_IM8W = 6,   // 8-bit immediate in 16-bit word
    M_IOA = 7,    // 8-bit I/O address
    M_IM16 = 8,   // 16-bit immediate
    M_ABS = 9,    // Absolute address (16 bit offset of 16/18/20 bit segmented address)
    M_COP = 10,   // Carry mode: 0, 1, C
    M_EOP = 11,   // E-register operation: RE, SE, CM
    M_RD = 12,    // Destination register: R0-R4, X0-X1, SP, STR
    M_RDG = 13,   // Destination register: R0-R4, X0-X1, SP (no STR)
    M_RS = 14,    // Source register: R0-R4, X0-X1, SP, STR
    M_RSG = 15,   // Source register: R0-R4, X0-X1, SP, STR (no STR)
    M_SB = 16,    // Segment base register: CSRB, SSRB, TSR0, TSR1
    M_RB = 17,    // Segment base register: M_SB, OSR0-OSR3
    M_RBW = 18,   // Segment base register: M_RB without CSBR
    M_RP = 19,    // Special register
    M_RHR = 20,   // Hardware register (readable) SIR
    M_RHW = 21,   // Hardware register (writable) SOR
    M_IABS = 22,  // Indirect absolute: (addr)
    M_RI = 23,    // Register indirect: (Ri):i=1-4, (Xj):j=0-1
    M_RIAU = 24,  // Register indirect with auto update: M_RI, -(Ri), (Ri)+
    M_R0 = 25,    // R0
    M_DR0 = 26,   // DR0: R0:R1
    M_RI1 = 27,   // (R1)
    M_RI2 = 28,   // (R2)
    M_GEN = 29,   // Generic addressing: D, d(IC), (D), (d(IC)), D(Xn), (D)(Xn)
    M_INDX = 30,  // v(r)
    M_IXID = 31,  // (v(r))
    M_IDIX = 32,  // (v)(r)
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint8_t _opr1;
        uint8_t _opr2;
        uint8_t _opr3;
        uint8_t _opr4;

        static constexpr Flags create(AddrMode opr1, AddrMode opr2, AddrMode opr3, AddrMode opr4) {
            return Flags{static_cast<uint8_t>(opr1), static_cast<uint8_t>(opr2),
                    static_cast<uint8_t>(opr3), static_cast<uint8_t>(opr4)};
        }

        Flags read() const {
            return Flags{pgm_read_byte(&_opr1), pgm_read_byte(&_opr2), pgm_read_byte(&_opr3),
                    pgm_read_byte(&_opr4)};
        }
        AddrMode mode1() const { return AddrMode(_opr1); }
        AddrMode mode2() const { return AddrMode(_opr2); }
        AddrMode mode3() const { return AddrMode(_opr3); }
        AddrMode mode4() const { return AddrMode(_opr4); }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : Base(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    const Flags _flags;
};

}  // namespace mn1610
}  // namespace libasm

#endif  // __ENTRY_MN1610_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
