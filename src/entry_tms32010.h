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

#ifndef __LIBASM_ENTRY_TMS32010_H__
#define __LIBASM_ENTRY_TMS32010_H__

#include <stdint.h>

#include "config_tms32010.h"
#include "entry_base.h"

namespace libasm {
namespace tms32010 {

enum AddrMode : uint8_t {
    // T=table, P=parser. M_IM1~M_CNST must be a constant.
    M_NONE = 0,   // TP: No operand
    M_IM1 = 1,    // T_: 1-bit unsigned immediate    ---- ---- ---- ---k
    M_IM8 = 3,    // T_: 8-bit unsigned immediate    ---- ---- kkkk kkkk
    M_IM13 = 5,   // T_: 13-bit signed immediate     ---k kkkk kkkk kkkk
    M_NARP = 7,   // T_: Next ARP                    ---- ---- ---- xyyy
    M_ARK = 8,    // T_: 3-bit AR register           ---- ---- ---- -rrr
    M_AR = 9,     // TP: AR[0-7] Auxiliary register  ---- -rrr ---- ----
    M_PA = 10,    // TP: PA[0-15] Port address       ---- pppp ---- ----
    M_LS0 = 11,   // T_: 0 constant for SACL         ---- -000 ---- ----
    M_LS3 = 12,   // T_: 3-bit left shift            ---- -xxx ---- ----
    M_LS4 = 13,   // T_: 4-bit left shift            ---- ssss ---- ----
    M_PM12 = 15,  // T_: 12-bit program address      ---- pppp pppp pppp
    M_CNST = 17,  // _P: constant
    M_MAM = 18,   // T_: Direct address or M_IND     ---- ---- ixxx xxxx
    M_ARP = 21,   // _P: "*"   Indirect addressing
    M_INC = 22,   // _P: "*+"  Indirect then auto increment addressing
    M_DEC = 23,   // _P: "*-"  Indirect then auto decrement addressing
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        static constexpr Flags create(AddrMode opr1, AddrMode opr2, AddrMode opr3) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(opr1) << opr1_gp) |
                                               (static_cast<uint16_t>(opr2) << opr2_gp) |
                                               (static_cast<uint16_t>(opr3) << opr3_gp))};
        }

        AddrMode mode1() const { return AddrMode((_attr >> opr1_gp) & mode_gm); }
        AddrMode mode2() const { return AddrMode((_attr >> opr2_gp) & mode_gm); }
        AddrMode mode3() const { return AddrMode((_attr >> opr3_gp) & mode_gm); }

    private:
        static constexpr int opr1_gp = 0;
        static constexpr int opr2_gp = 5;
        static constexpr int opr3_gp = 10;
        static constexpr uint8_t mode_gm = 0x1f;
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opCode), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_word(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace tms32010
}  // namespace libasm

#endif  // __LIBASM_ENTRY_TMS32010_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
