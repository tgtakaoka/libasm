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

#ifndef __LIBASM_ENTRY_Z8000_H__
#define __LIBASM_ENTRY_Z8000_H__

#include <stdint.h>

#include "config_z8000.h"
#include "entry_base.h"

namespace libasm {
namespace z8000 {

enum OprSize : uint8_t {
    SZ_NONE = Size::SZ_NONE,
    SZ_BYTE = Size::SZ_BYTE,
    SZ_WORD = Size::SZ_WORD,
    SZ_QUAD = Size::SZ_QUAD,
    SZ_OCTA = Size::SZ_OCTA,
    SZ_ADDR = Size::SZ_ADDR,
};

enum AddrMode : uint8_t {
    M_NONE = 0,
    // dst/src/ex2
    M_CC = 1,  // Condition Code:
               // F/Z/NZ/C/NC/PL/MI/NE/EQ/OV/NOV/PE/PO/GE/LT/GT/LE/UGE/ULT/UGT/ULE
    // src/ex1
    M_CNT = 2,  // Count: #1~16
    M_WR = 3,   // R0-R15
    // dst/src
    M_R = 4,      // Register: Rn/RHn/RLn/RRn/RQn
    M_IM = 5,     // Immediate: #nn
    M_IR = 6,     // Indirect Register: @Rn/@RRn
    M_DA = 7,     // Direct Address: nnnn
    M_X = 8,      // Index: nnnn(Rn)
    M_RA = 9,     // 16-bit Relative: nnnn -32768~+32767
    M_BA = 10,    // Base Address: Rn(#nnnn)/RRn(#nnnn)
    M_BX = 11,    // Base Index: Rn(Rn)/RRn(Rn)
    M_GENI = 12,  // Generic and Immediate: IM/IR/DA/X/R
    M_GEND = 13,  // Generic Data: IR/DA/X/R
    M_GENA = 14,  // Generic Address: IR/DA/X
    M_IO = 15,    // IO Address: nnnn/#nnnn
    M_IM8 = 16,   // System call: #nn
    M_BIT = 17,   // Bit number: #0~15
    M_QCNT = 18,  // Quick Count: #1/2
    M_SCNT = 19,  // Signed Count: #-32~32
    M_NCNT = 20,  // Signed Negative Count: #-32~32
    M_INTR = 21,  // Interrupt type: VI/NVI
    M_CTL = 22,   // Control Register:
                  // FCW/REFRESH/NSPSRG/NSPOFF/PSAPSEG/PSAPOFF/FLAGS
    M_FLAG = 23,  // Flags: C/Z/S/P/V
    M_RA12 = 24,  // 12-bit Relative: (-2048~+2047)*22
    M_RA8 = 25,   // 8-bit Relative: (-128~+127)*2
    M_RA7 = 26,   // 7-bit Positive Relative: -(0~127)*2
    M_DBLR = 27,  // Double-Sized Register: WORD:RRn LONG:RQn
    M_WR07 = 28,  // R0~R7
    M_IRIO = 29,  // Indirect Register IO Address: @R1~R15
};

enum PostFormat : uint8_t {
    PF_NONE = 0,  // No Post Word
    PF_0XX0 = 1,  // 0s are part of instruction
    PF_0XX8 = 2,  // 0 and 8 are part of instruction
    PF_0XXE = 3,  // 0 and E are part of instruction
    PF_0X0X = 4,  // 0s are part of instruction
    PF_0X00 = 5,  // 0s are part of instruction
    PF_0XXX = 6,  // 0 is part of instruction
};

enum CodeFormat : uint8_t {
    CF_0000 = 0,
    CF_0003 = 1,
    CF_000F = 2,
    CF_00F0 = 3,
    CF_00F2 = 4,
    CF_00F7 = 5,
    CF_00FF = 6,
    CF_0F7F = 7,
    CF_0FFF = 8,
    CF_C0F0 = 9,
    CF_C0FF = 10,
    CF_X0FF = 11,  // CF_C0FF with extra check
};

enum OprPos : uint8_t {
    OP_NO = 0,
    OP_C0 = 1,
    OP_C4 = 2,
    OP_C8 = 3,
    OP_P0 = 4,
    OP_P4 = 5,
    OP_P8 = 6,
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _mode;
        uint16_t _attr;

        static constexpr Flags create(AddrMode dst, AddrMode src, AddrMode ex1, AddrMode ex2,
                OprSize size, OprPos dstPos, OprPos srcPos, CodeFormat cf, PostFormat pf) {
            return Flags{mode(dst, src, ex1, ex2, size), attr(dstPos, srcPos, cf, pf)};
        }

        static const Flags create(AddrMode dst, AddrMode src, AddrMode ex1, AddrMode ex2) {
            return Flags{mode(dst, src, ex1, ex2, SZ_NONE), attr(OP_NO, OP_NO, CF_0000, PF_NONE)};
        }

        AddrMode dst() const { return AddrMode((_mode >> dst_gp) & dst_gm); }
        AddrMode src() const { return AddrMode((_mode >> src_gp) & src_gm); }
        AddrMode ex1() const { return AddrMode((_mode >> ex1_gp) & ex1_gm); }
        AddrMode ex2() const { return AddrMode((_mode >> ex2_gp) & ex2_gm); }
        OprSize size() const { return OprSize((_mode >> size_gp) & size_gm); }
        OprPos dstPos() const { return OprPos((_attr >> dstPos_gp) & dstPos_gm); }
        OprPos srcPos() const { return OprPos((_attr >> srcPos_gp) & srcPos_gm); }
        PostFormat postFormat() const { return PostFormat((_attr >> pf_gp) & pf_gm); }
        CodeFormat codeFormat() const { return CodeFormat((_attr >> cf_gp) & cf_gm); }

        Config::opcode_t postVal() const {
            static constexpr Config::opcode_t POSTFIXES[] PROGMEM = {
                    0x0000,  // PF_NONE
                    0x0000,  // PF_0XX0
                    0x0008,  // PF_0XX8
                    0x000E,  // PF_0XXE
                    0x0000,  // PF_0X0X
                    0x0000,  // PF_0X00
                    0x0000,  // PF_0XXX
            };
            return pgm_read_word(POSTFIXES + postFormat());
        }

        Config::opcode_t postMask() const {
            static constexpr Config::opcode_t POSTMASKS[] PROGMEM = {
                    0x0000,  // PF_NONE
                    0xF00F,  // PF_0XX0
                    0xF00F,  // PF_0XX8
                    0xF00F,  // PF_0XXE
                    0xF0F0,  // PF_0X0X
                    0xF0FF,  // PF_0X00
                    0xF000,  // PF_0XXX
            };
            return pgm_read_word(POSTMASKS + postFormat());
        }

        Config::opcode_t codeMask() const {
            static constexpr Config::opcode_t CODEMASKS[] PROGMEM = {
                    0x0000,  // CF_0000
                    0x0003,  // CF_0003
                    0x000F,  // CF_000F
                    0x00F0,  // CF_00F0
                    0x00F2,  // CF_00F2
                    0x00F7,  // CF_00F7
                    0x00FF,  // CF_00FF
                    0x0F7F,  // CF_0F7F
                    0x0FFF,  // CF_0FFF
                    0xC0F0,  // CF_C0F0
                    0xC0FF,  // CF_C0FF
                    0xC0FF,  // CF_X0FF
            };
            return pgm_read_word(CODEMASKS + codeFormat());
        }

    private:
        static constexpr uint16_t mode(
                AddrMode dst, AddrMode src, AddrMode ex1, AddrMode ex2, OprSize size) {
            return (dst << dst_gp) | (src << src_gp) | (ex1 << ex1_gp) | (ex2 << ex2_gp) |
                   (size << size_gp);
        }

        static constexpr uint16_t attr(OprPos dstPos, OprPos srcPos, CodeFormat cf, PostFormat pf) {
            return (dstPos << dstPos_gp) | (srcPos << srcPos_gp) | (cf << cf_gp) | (pf << pf_gp);
        }

        // |_mode|
        static constexpr int dst_gp = 0;
        static constexpr int src_gp = 5;
        static constexpr int ex1_gp = 10;
        static constexpr int ex2_gp = 12;
        static constexpr int size_gp = 13;
        static constexpr auto dst_gm = UINT16_C(0x1F);
        static constexpr auto src_gm = UINT16_C(0x1F);
        static constexpr auto ex1_gm = UINT16_C(0x3);
        static constexpr auto ex2_gm = UINT16_C(0x1);
        static constexpr auto size_gm = UINT16_C(0x7);
        // |_attr|
        static constexpr int dstPos_gp = 0;
        static constexpr int srcPos_gp = 3;
        static constexpr int cf_gp = 6;
        static constexpr int pf_gp = 10;
        static constexpr auto dstPos_gm = UINT16_C(0x7);
        static constexpr auto srcPos_gm = UINT16_C(0x7);
        static constexpr auto cf_gm = UINT16_C(0xF);
        static constexpr auto pf_gm = UINT16_C(0x7);
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opCode), _flags_P(flags) {}

    Flags readFlags() const {
        return Flags{pgm_read_word(&_flags_P._mode), pgm_read_word(&_flags_P._attr)};
    }

private:
    const Flags _flags_P;
};

}  // namespace z8000
}  // namespace libasm

#endif  // __LIBASM_ENTRY_Z8000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
