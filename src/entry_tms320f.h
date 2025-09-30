/*
 * Copyright 2025 Tadashi G. Takaoka
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

#ifndef __LIBASM_ENTRY_TMS320F_H__
#define __LIBASM_ENTRY_TMS320F_H__

#include <stdint.h>

#include "config_tms320f.h"
#include "entry_base.h"

namespace libasm {
namespace tms320f {

enum AddrMode : uint8_t {
    M_NONE,  // TP: No operand
    M_IGEN,  // any reg, direct, indirect, signed immediate
    M_UGEN,  // any reg, direct, indirect, unsigned immediate
    M_FGEN,  // R0~R7, direct, indirect, immediate
    M_FGU8,  // R0~R7, direct, indirect, 8-bit unsigned immediate
    M_IREG,  // any reg
    M_FREG,  // R0~R7
    M_SCNT,  // 16-bit shift count
    M_CNT8,  // 8-bit shift count
    M_MEM,   // direct, inditrct
    M_MREG,  // reg, indirect
    M_IDAT,  // any reg, indirect (disp=0,1)
    M_FDAT,  // R0~R7, indirect (disp=0,1)
    M_MSBA,  // 8-bit msb of address
    M_AD24,  // 24-bit absolute address
    M_IREL,  // any register or 16-bit relative
    M_TVEC,  // trap vector 00H~1FH
    R_ARN,   // register AR0~AR7
    R_DP,    // register DP
};

enum CodeFormat : uint8_t {
    CF_00,  // |_________op________|_______?_______|
    CF_GG,  // |0x0|__op__|GG|_dst_|______src______|
    CF_0G,  // |0x0|__op__|GG|00000|______src______|; NOP/RPTS
    CF_G0,  // |0x0|__op_____|_dst_|000000000000000|; PUSH/POP
    CF_TT,  // |001|__op__|TT|_dst_|__src1_|__src2_|
    CF_T0,  // |001|__op__|TT|00000|__src1_|__src2_|; RPTS/IACK/CMPF3/CMPI3/TSTB3
    CF_TR,  // |011_____op___________________|_src_|; TRAPcc
    CF_BR,  // |011__op_|___________abs____________|; BR/BRD/CALL/RPTB
    CF_DB,  // |011_op_|_ar|___op__|______rel______|; DBcc/DBccD
};

enum LsbFormat : uint8_t {
    LF_00,  // 0x0000
    LF_01,  // 0x0001
    LF_20,  // 0x0020~0x003F
    LF_FF,  // 0x0000~0x00FF
    LF_XX,  // any
};

struct Entry final : private entry::Base<uint16_t> {
    struct Flags final {
        uint16_t _modes;
        uint8_t _attr;

        static constexpr Flags create(
                CodeFormat cf, LsbFormat lf, AddrMode opr1, AddrMode opr2, AddrMode opr3) {
            return Flags{static_cast<uint16_t>(
                                 (opr1 << mode1_gp) | (opr2 << mode2_gp) | (opr3 << mode3_gp)),
                    static_cast<uint8_t>((cf << cf_gp) | (lf << lf_gp))};
        }

        AddrMode mode1() const { return AddrMode((_modes >> mode1_gp) & mode1_gm); }
        AddrMode mode2() const { return AddrMode((_modes >> mode2_gp) & mode2_gm); }
        AddrMode mode3() const { return AddrMode((_modes >> mode3_gp) & mode3_gm); }
        uint32_t mask() const {
            static constexpr uint16_t CF[] PROGMEM = {
                    0x0000,  // CF_00: |________op_________|_______?_______|
                    0x007F,  // CF_GG: |0x0|__op__|GG|_dst_|______src______|
                    0x0060,  // CF_0G: |0x0|__op__|GG|00000|______src______|; NOP/RPTS
                    0x001F,  // CF_G0: |0x0|__op_____|_dst_|000000000000000|; PUSH/POP
                    0x007F,  // CF_TT: |001|__op__|TT|_dst_|__src1_|__src2_|
                    0x0060,  // CF_T0: |001|__op__|TT|00000|__src1_|__src2_|; RPTS/IACK/CMPx3
                    0x00FF,  // CF_BR: |011__op__|__________abs____________|; BR/BRD/CALL/RPTB
                    0x01C0,  // CF_DB: |011_op_|_ar|___op__|______rel______|; DBcc/DBccD
            };
            static constexpr uint16_t LF[] PROGMEM = {
                    0x0000,  // LF_00
                    0x0000,  // LF_01
                    0x001F,  // LF_20
                    0x00FF,  // LF_FF
                    0xFFFF,  // LF_XX
            };
            uint32_t mask = pgm_read_word(CF + ((_attr >> cf_gp) & cf_gm));
            mask <<= 16;
            mask |= pgm_read_word(LF + ((_attr >> lf_gp) & lf_gm));
            return ~mask;
        }
        uint16_t lower16() const {
            static constexpr uint16_t LV[] PROGMEM = {
                    0x0000,  // LF_00
                    0x0001,  // LF_01
                    0x0020,  // LF_20
                    0x0000,  // LF_FF
                    0x0000,  // LF_XX
            };
            return pgm_read_word(LV + ((_attr >> lf_gp) & lf_gm));
        }

    private:
        // |_modes|
        static constexpr auto mode1_gp = 0;
        static constexpr auto mode2_gp = 6;
        static constexpr auto mode3_gp = 12;
        static constexpr uint_fast8_t mode1_gm = 0x3F;
        static constexpr uint_fast8_t mode2_gm = 0x3F;
        static constexpr uint_fast8_t mode3_gm = 0x01;
        // |_attr|
        static constexpr auto cf_gp = 0;
        static constexpr uint_fast8_t cf_gm = 0x0F;
        static constexpr auto lf_gp = 4;
        static constexpr uint_fast8_t lf_gm = 0x07;
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opCode), _flags_P(flags) {}

    Flags readFlags() const {
        return Flags{pgm_read_word(&_flags_P._modes), pgm_read_byte(&_flags_P._attr)};
    }

    const /* PROGMEM */ char *name_P() const { return Base::name_P(); }

    Config::opcode_t readOpCode() const {
        Config::opcode_t opc = Base::readOpCode();
        opc <<= 16;
        opc |= readFlags().lower16();
        return opc;
    }

private:
    const Flags _flags_P;
};

}  // namespace tms320f
}  // namespace libasm

#endif  // __LIBASM_ENTRY_TMS320F_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
