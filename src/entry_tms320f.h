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
    M_NONE = 0,  // TP: No operand
    // M_IGEN-M_FLDM must be contiguous (direct, M_IMM)
    // M_IGEN~M_IDAT must be contiguous (indirect)
    M_IGEN = 1,   // any reg, direct, indirect, 16-bit signed immediate
    M_UGEN = 2,   // any reg, direct, indirect, 16-bit unsigned immediate
    M_GCNT = 3,   // any reg, direct, indirecr, LSB 7-bit signed shift count
    M_FGEN = 4,   // R0~R7, direct, indirect, immediate
    M_FLDM = 5,   // R0~R7, direct, indirect, 12-bit immediate mantissa
    M_MEM = 6,    // direct, inditrct
    M_MREG = 7,   // reg, indirect
    M_FDAT = 8,   // R0~R7, indirect (disp=0,1)
    M_IDAT = 9,   // any reg, indirect (disp=0,1)
    M_FREG = 10,  // R0~R7
    M_IREG = 11,  // any reg
    R_ARN = 12,   // register AR0~AR7
    R_DP = 13,    // register DP
    R_R01 = 14,   // R0/R1
    R_R23 = 15,   // R2/R3
    // M_MSBA~M_TVEC must be contiguous (M_IMM)
    M_MSBA = 16,  // 8-bit msb of address
    M_AD24 = 17,  // 24-bit absolute address
    M_IREL = 18,  // any register or 16-bit relative (+1)
    M_DREL = 19,  // any register or 16-bit relative (+3)
    M_TVEC = 20,  // trap vector 00H~1FH
    // Assembler parser
    M_IDIR = 21,  // Indirect for parallel
    M_DIR = 22,  // @addr; direct addressing
    M_IMM = 23,  // Integer/Float immediate
};

enum OprPos : uint8_t {
    P_NONE,
    P_00FF,  // 0000_00FF; src2, src4
    P_FF00,  // 0000_FF00; src1, src3, dst2
    P_001F,  // 001F_0000; dst
    P_0007,  // 0007_0000; src3, src2
    P_0038,  // 0038_0000; src2, src1
    P_0040,  // 0040_0000; d2
    P_0080,  // 0080_0000; d1
    P_01C0,  // 01C0_0000; dst1
};

enum CodeFormat : uint8_t {
    CF_00,  // |_________op________|_______?_______|
    CF_GG,  // |0x0|__op__|GG|_dst_|______src______|
    CF_0G,  // |0x0|__op__|GG|00000|______src______|; NOP/RPTS
    CF_GX,  // |0x0|__op_____|_dst_|xxxxxxxxxxxxxxx|; PUSH/POP/ROL/ROR
    CF_TT,  // |001|__op__|TT|_dst_|__src1_|__src2_|; op3 src2,src1,dst
    CF_T0,  // |001|__op__|TT|00000|__src1_|__src2_|; RPTS/IACK/CMPF3/CMPI3/TSTB3
    CF_TR,  // |011_____op___________________|_src_|; TRAPcc
    CF_BR,  // |011__op_|___________abs____________|; BR/BRD/CALL/RPTB
    CF_BB,  // |0110__B____________|______src______|: Bcc/BccD
    CF_DB,  // |0110__B|_ar|___op__|______rel______|; DBcc/DBccD
    CF_P4,  // |10__op_|ds1|000|sr3|__dst2_|__src2_|; op1 src2,dst1||op2 src3,dst2
    CF_L4,  // |10__op_|ds1|ss2|000|__src1_|__src2_|; op1 src2,dst2||op2 src1,dst1
    CF_P5,  // |10__op_|ds1|sr1|sr3|__dst2_|__src2_|; op1 src2,src1,dst1||op2 src3,dst2
    CF_P6,  // |10_op_|PPdd|sr1|sr2|__src3_|__src4_|: op1 srcA,srcB,dst1|| op2 srcC,srcD,dst2
};

enum LswFormat : uint8_t {
    LF_00,  // 0x0000
    LF_01,  // 0x0001
    LF_FF,  // 0xFFFF
    LF_XX,  // any
};

struct Entry final : private entry::Base<uint16_t> {
    struct Flags final {
        uint16_t _modes;
        uint16_t _pos;
        uint8_t _attr;

        static constexpr Flags create(CodeFormat cf, LswFormat lf, AddrMode opr1, AddrMode opr2,
                AddrMode opr3, bool unary = false, OprPos pos1 = P_NONE, OprPos pos2 = P_NONE,
                OprPos pos3 = P_NONE) {
            return Flags{static_cast<uint16_t>((opr1 << mode1_gp) | (opr2 << mode2_gp) |
                                               (opr3 << mode3_gp) | (unary << unary_bp)),
                    static_cast<uint16_t>(
                            (pos1 << pos1_gp) | (pos2 << pos2_gp) | (pos3 << pos3_gp)),
                    static_cast<uint8_t>((cf << cf_gp) | (lf << lf_gp))};
        }

        AddrMode mode1() const { return AddrMode((_modes >> mode1_gp) & mode1_gm); }
        AddrMode mode2() const { return AddrMode((_modes >> mode2_gp) & mode2_gm); }
        AddrMode mode3() const { return AddrMode((_modes >> mode3_gp) & mode3_gm); }
        OprPos pos1() const { return OprPos((_pos >> pos1_gp) & pos_gm); }
        OprPos pos2() const { return OprPos((_pos >> pos2_gp) & pos_gm); }
        OprPos pos3() const { return OprPos((_pos >> pos3_gp) & pos_gm); }
        CodeFormat codeFormat() const { return CodeFormat((_attr >> cf_gp) & cf_gm); }
        LswFormat lswFormat() const { return LswFormat((_attr >> lf_gp) & lf_gm); }
        bool maybeUnary() const { return (_modes & (1 << unary_bp)) != 0; }
        bool isParallel() const { return codeFormat() >= CF_P4; }
        uint32_t mask() const {
            static constexpr uint16_t CF_VAR[] PROGMEM = {
                    0x0000,  // CF_00: |________op_________|_______?_______|
                    0x007F,  // CF_GG: |0x0|__op__|GG|_dst_|______src______|
                    0x0060,  // CF_0G: |0x0|__op__|GG|00000|______src______|; NOP/RPTS
                    0x001F,  // CF_GX: |0x0|__op_____|_dst_|000000000000000|; PUSH/POP
                    0x007F,  // CF_TT: |001|__op__|TT|_dst_|__src1_|__src2_|; op3 src2,src1,dst
                    0x0060,  // CF_T0: |001|__op__|TT|00000|__src1_|__src2_|; RPTS/IACK/CMPx3
                    0x0000,  // CF_TR: |011_____op___________________|_src_|; TRAPcc
                    0x00FF,  // CF_BR: |011__op__|__________abs____________|; BR/BRD/CALL/RPTB
                    0x0200,  // CF_BB: |011___B|____op_____|______rel______|; Bcc/BccD
                    0x03C0,  // CF_DB: |011___B|_ar|___op__|______rel______|; DBcc/DBccD
                    0x01C7,  // CF_P4: |10__op_|ds1|000|sr1|__dst2_|__src2_|;
                    0x01F8,  // CF_L4: |10__op_|ds1|ss2|000|__src1_|__src2_|;
                    0x01FF,  // CF_P5: |10__op_|ds1|sr1|sr3|__dst2_|__src2_|;
                    0x00FF,  // CF_P6: |10_op_|PPdd|sr1|sr2|__src3_|__src4_|:
            };
            static constexpr uint16_t LF_VAR[] PROGMEM = {
                    0x0000,  // LF_00
                    0x0000,  // LF_01
                    0x0000,  // LF_FF
                    0xFFFF,  // LF_XX
            };
            const auto cf = codeFormat();
            const auto lf = lswFormat();
            uint32_t var = pgm_read_word(CF_VAR + cf);
            var <<= 16;
            var |= pgm_read_word(LF_VAR + lf);
            return ~var;
        }
        uint16_t lower16() const {
            static constexpr uint16_t LF_VAL[] PROGMEM = {
                    0x0000,  // LF_00
                    0x0001,  // LF_01
                    0xFFFF,  // LF_FF
                    0x0000,  // LF_XX
            };
            const auto lf = lswFormat();
            return pgm_read_word(LF_VAL + lf);
        }

    private:
        // |_modes|
        static constexpr auto mode1_gp = 0;
        static constexpr auto mode2_gp = 5;
        static constexpr auto mode3_gp = 10;
        static constexpr auto unary_bp = 15;
        static constexpr uint_fast8_t mode1_gm = 0x1F;
        static constexpr uint_fast8_t mode2_gm = 0x1F;
        static constexpr uint_fast8_t mode3_gm = 0x1F;
        // |_pos|
        static constexpr auto pos1_gp = 0;
        static constexpr auto pos2_gp = 4;
        static constexpr auto pos3_gp = 8;
        static constexpr uint_fast8_t pos_gm = 0xF;
        // |_attr|
        static constexpr auto cf_gp = 0;
        static constexpr uint_fast8_t cf_gm = 0x0F;
        static constexpr auto lf_gp = 4;
        static constexpr uint_fast8_t lf_gm = 0x03;
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opCode), _flags_P(flags) {}

    Flags readFlags() const {
        return Flags{pgm_read_word(&_flags_P._modes), pgm_read_word(&_flags_P._pos),
                pgm_read_byte(&_flags_P._attr)};
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
