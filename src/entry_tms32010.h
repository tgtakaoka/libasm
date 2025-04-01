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
    M_NONE = 0,  // TP: No operand
    // mode3, mode2
    M_NARP = 1,  // T_: Next ARP                    ---- ---- ---- xyyy
    // mode1, mode2
    // M_IM1..M_PM16 must be contiguous
    M_IM1 = 2,    // T_: 1-bit unsigned immediate    ---- ---- ---- ---k
    M_IM2 = 3,    // T_: 2-bit unsigned immediate    ---- ---- ---- --kk
    M_IM8 = 4,    // T_: 8-bit unsigned immediate    ---- ---- kkkk kkkk
    M_IM9 = 5,    // T_: 9-bit unsigned immediate    ---- ---k kkkk kkkk
    M_IM13 = 6,   // T_: 13-bit signed immediate     ---k kkkk kkkk kkkk
    M_IM16 = 7,   // T_: 16-bit unsigned immediate
    M_ARK = 8,    // T_: 3-bit AR register           ---- ---- ---- -rrr
    M_AR = 9,     // TP: AR[0-7] Auxiliary register  ---- -rrr ---- ----
    M_PA = 10,    // TP: PA[0-15] Port address       ---- pppp ---- ----
    M_LS0 = 11,   // T_: 0 constant for SACL         ---- -000 ---- ----
    M_LS3 = 12,   // T_: 3-bit left shift            ---- -xxx ---- ----
    M_LS4 = 13,   // T_: 4-bit left shift            ---- ssss ---- ----
    M_BIT = 14,   // T_: 4-bit bit position          ---- bbbb ---- ----
    M_PM12 = 15,  // T_: 12-bit program address      ---- pppp pppp pppp
    M_PM16 = 16,  // T_: 16-bit program address
    M_MAM = 17,   // T_: Direct address or M_IND     ---- ---- ixxx xxxx
    M_IND = 18,   // T_: Indirect addressing         ---- ---- 1vpm ----
    M_MAR = 19,   // T_: Modify AR                   ---- ---- 1vpm ----
    M_CNST = 20,  // _P: constant
    // M_ARP..M_DBR0 must be contiguous in this order.
    // See AsmTms32010::encodeIndirect
    M_ARP = 21,   // _P: "*"   Indirect addressing
    M_INC = 22,   // _P: "*+"  Indirect then auto increment addressing
    M_DEC = 23,   // _P: "*-"  Indirect then auto decrement addressing
    M_INC0 = 24,  // _P: "*0+"  Indirect then auto increment by AR0 addressing
    M_DEC0 = 25,  // _P: "*0-"  Indirect then auto decrement by AR0 addressing
    M_IBR0 = 26,  // _P: "*BR+"  Indirect then auto increment by bit-reverse AR0 addressing
    M_DBR0 = 27,  // _P: "*BR-"  Indirect then auto decrement by bit-reverse AR0 addressing
};

enum CodeFormat : uint8_t {
    CF_0000 = 0,   // 0x0000
    CF_0001 = 1,   // 0x0001
    CF_0003 = 2,   // 0x0003
    CF_0007 = 3,   // 0x0007
    CF_00FF = 4,   // 0x00FF
    CF_01FF = 5,   // 0x01FF
    CF_0700 = 6,   // 0x0700
    CF_07FF = 7,   // 0x07FF
    CF_0F00 = 8,   // 0x0F00
    CF_0FFF = 9,   // 0x0FFF
    CF_1FFF = 10,  // 0x1FFF
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _attr;

        static constexpr Flags create(CodeFormat cf, AddrMode opr1, AddrMode opr2, AddrMode opr3) {
            return Flags{static_cast<uint16_t>(
                    (opr1 << mode1_gp) | (opr2 << mode2_gp) | (opr3 << mode3_gp) | (cf << cf_gp))};
        }

        AddrMode mode1() const { return AddrMode((_attr >> mode1_gp) & mode1_gm); }
        AddrMode mode2() const { return AddrMode((_attr >> mode2_gp) & mode2_gm); }
        AddrMode mode3() const { return AddrMode((_attr >> mode3_gp) & mode3_gm); }
        uint16_t mask() const {
            static constexpr uint16_t MASK[] PROGMEM = {
                    0x0000,  // CF_0000 = 0
                    0x0001,  // CF_0001 = 1
                    0x0003,  // CF_0003 = 2
                    0x0007,  // CF_0007 = 3
                    0x00FF,  // CF_00FF = 4
                    0x01FF,  // CF_01FF = 5
                    0x0700,  // CF_0700 = 6
                    0x07FF,  // CF_07FF = 7
                    0x0F00,  // CF_0F00 = 8
                    0x0FFF,  // CF_0FFF = 9
                    0x1FFF,  // CF_1FFF = 10
            };
            return pgm_read_word(&MASK[(_attr >> cf_gp) & cf_gm]);
        }

    private:
        static constexpr int mode1_gp = 0;
        static constexpr int mode2_gp = 5;
        static constexpr int mode3_gp = 10;
        static constexpr int cf_gp = 11;
        static constexpr uint_fast8_t mode1_gm = 0x1F;
        static constexpr uint_fast8_t mode2_gm = 0x1F;
        static constexpr uint_fast8_t mode3_gm = 0x01;
        static constexpr uint_fast8_t cf_gm = 0x0F;
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
