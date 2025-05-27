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

#ifndef __LIBASM_ENTRY_TMS320_H__
#define __LIBASM_ENTRY_TMS320_H__

#include <stdint.h>

#include "config_tms320.h"
#include "entry_base.h"

namespace libasm {
namespace tms320 {

enum AddrMode : uint8_t {
    // T=table, P=parser. M_UI1~M_CNST must be a constant.
    M_NONE = 0,  // TP: No operand
    // mode3, mode2
    M_NARP = 1,  // T_: Next ARP                    ---- ---- ---- xyyy
    // mode1, mode2
    // M_UI1..M_PM16 must be contiguous
    M_UI1 = 2,    // T_: 1-bit unsigned constant    ---- ---- ---- ---k
    M_UI2 = 3,    // T_: 2-bit unsigned constant    ---- ---- ---- --kk
    M_UI8 = 4,    // T_: 8-bit unsigned constant    ---- ---- kkkk kkkk
    M_UI9 = 5,    // T_: 9-bit unsigned constant    ---- ---k kkkk kkkk
    M_SI13 = 6,   // T_: 13-bit signed constant     ---k kkkk kkkk kkkk
    M_SI16 = 7,   // TP: 16-bit signed constant
    M_UI16 = 8,   // TP: 16-bit unsigned constant
    M_ARK = 9,    // T_: 3-bit AR register          ---- ---- ---- -rrr
    M_AR = 10,    // TP: AR[0-7] Auxiliary register ---- -rrr ---- ----
    M_PA = 11,    // TP: PA[0-15] Port address      ---- pppp ---- ----
    M_LS0 = 12,   // T_: 0 constant for SACL        ---- -000 ---- ----
    M_LS3 = 13,   // T_: 3-bit left shift           ---- -xxx ---- ----
    M_LS4 = 14,   // T_: 4-bit left shift           ---- ssss ---- ----
    M_LS16 = 15,  // TP: 16 left shift for ADDH/SUBH/ZALH
    M_BIT = 16,   // T_: 4-bit bit position         ---- bbbb ---- ----
    M_PM12 = 17,  // T_: 12-bit program address     ---- pppp pppp pppp
    M_PM16 = 18,  // T_: 16-bit program address
    M_MAM = 19,   // T_: Direct address or M_IND    ---- ---- ixxx xxxx
    M_IND = 20,   // T_: Indirect addressing        ---- ---- 1vpm ----
    M_MAR = 21,   // T_: Modify AR                  ---- ---- 1vpm ----
    // M_STN..M_UIM16 must be ocontiguous
    M_STN = 22,    // T_: Status register number     ---- ---k ---- ----
    M_IMU8 = 23,   // TP: 8-bit unsigned immediate   ---- ---- kkkk kkkk
    M_IMU9 = 24,   // T_: 9-bit unsigned immediate   ---- ---k kkkk kkkk
    M_IM13 = 25,   // T_: 13-bit signed immediate    ---k kkkk kkkk kkkk
    M_IM16 = 26,   // TP: 16-bit signed immediate
    M_IMU16 = 27,  // TP: 16-bit unsigned immediate
    M_CC = 28,     // TP: Condition code name
    M_CTL = 29,    // TP: Control bit name
    M_CCCTL = 30,  // TP: CC_C/CC_TC or REG_C/REG_TC
    // M_ARP..M_DBR0 must be contiguous in this order.
    // See AsmTms320::encodeIndirect
    M_ARP = 31,   // _P: "*"   Indirect addressing
    M_INC = 32,   // _P: "*+"  Indirect then auto increment addressing
    M_DEC = 33,   // _P: "*-"  Indirect then auto decrement addressing
    M_INC0 = 34,  // _P: "*0+"  Indirect then auto increment by AR0 addressing
    M_DEC0 = 35,  // _P: "*0-"  Indirect then auto decrement by AR0 addressing
    M_IBR0 = 36,  // _P: "*BR+"  Indirect then auto increment by bit-reverse AR0 addressing
    M_DBR0 = 37,  // _P: "*BR-"  Indirect then auto decrement by bit-reverse AR0 addressing
};

enum CodeFormat : uint8_t {
    CF_0000 = 0,   // 0x0000
    CF_0001 = 1,   // 0x0001
    CF_0003 = 2,   // 0x0003
    CF_0007 = 3,   // 0x0007
    CF_007F = 4,   // 0x007F
    CF_00FF = 5,   // 0x00FF
    CF_01FF = 6,   // 0x01FF
    CF_0700 = 7,   // 0x0700
    CF_07FF = 8,   // 0x07FF
    CF_0F00 = 9,   // 0x0F00
    CF_0FFF = 10,  // 0x0FFF
    CF_1FFF = 11,  // 0x1FFF
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
                    0x007F,  // CF_007F = 4
                    0x00FF,  // CF_00FF = 5
                    0x01FF,  // CF_01FF = 6
                    0x0700,  // CF_0700 = 7
                    0x07FF,  // CF_07FF = 8
                    0x0F00,  // CF_0F00 = 9
                    0x0FFF,  // CF_0FFF = 11
                    0x1FFF,  // CF_1FFF = 12
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

}  // namespace tms320
}  // namespace libasm

#endif  // __LIBASM_ENTRY_TMS320_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
