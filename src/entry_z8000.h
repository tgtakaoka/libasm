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
    M_R = 1,      // Register: Rn/RHn/RLn/RRn/RQn
    M_IM = 2,     // Immediate: #nn
    M_IR = 3,     // Indirect Register: @Rn/@RRn
    M_DA = 4,     // Direct Address: nnnn
    M_X = 5,      // Index: nnnn(Rn)
    M_RA = 6,     // 16-bit Relative: nnnn -32768~+32767
    M_BA = 7,     // Base Address: Rn(#nnnn)/RRn(#nnnn)
    M_BX = 8,     // Base Index: Rn(Rn)/RRn(Rn)
    M_GENI = 9,   // Generic and Immediate: IM/IR/DA/X/R
    M_GEND = 10,  // Generic Data: IR/DA/X/R
    M_GENA = 11,  // Generic Address: IR/DA/X
    M_IO = 12,    // IO Address: nnnn/#nnnn
    M_IM8 = 13,   // System call: #nn
    M_BIT = 14,   // Bit number: #0~15
    M_CNT = 15,   // Count: #1~16
    M_QCNT = 16,  // Quick Count: #1/2
    M_SCNT = 17,  // Signed Count: #-32~32
    M_NCNT = 18,  // Signed Negative Count: #-32~32
    M_CC = 19,    // Condition Code:
                  // F/Z/NZ/C/NC/PL/MI/NE/EQ/OV/NOV/PE/PO/GE/LT/GT/LE/UGE/ULT/UGT/ULE
    M_INTR = 20,  // Interrupt type: VI/NVI
    M_CTL = 21,   // Control Register:
                  // FCW/REFRESH/NSPSRG/NSPOFF/PSAPSEG/PSAPOFF/FLAGS
    M_FLAG = 22,  // Flags: C/Z/S/P/V
    M_RA12 = 23,  // 12-bit Relative: (-2048~+2047)*22
    M_RA8 = 24,   // 8-bit Relative: (-128~+127)*2
    M_RA7 = 25,   // 7-bit Positive Relative: -(0~127)*2
    M_DBLR = 26,  // Double-Sized Register: WORD:RRn LONG:RQn
    M_WR07 = 27,  // R0~R7
    M_WR = 28,    // R0-R15
    M_IRIO = 29,  // Indirect Register IO Address: @R1~R15
    M_ERROR = 255,
};

enum Ex1Mode : uint8_t {
    E1_NONE = 0,
    E1_CNT = 1,
    E1_WR = 2,
    E1_ERROR = 3,
};

enum Ex2Mode : uint8_t {
    E2_NONE = 0,
    E2_CC = 1,
    E2_ERROR = 2,
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
        uint8_t _dst;
        uint8_t _src;
        uint8_t _ext;
        uint8_t _attr;

        static constexpr Flags create(AddrMode dst, OprPos dstPos, AddrMode src, OprPos srcPos,
                Ex1Mode ex1, Ex2Mode ex2, PostFormat postFormat, CodeFormat codeFormat,
                OprSize size) {
            return Flags{opr(dst, dstPos), opr(src, srcPos), ext(ex1, ex2, postFormat),
                    attr(codeFormat, size)};
        }

        static const Flags create(AddrMode dst, AddrMode src, AddrMode ex1, AddrMode ex2) {
            return Flags{opr(dst, OP_NO), opr(src, OP_NO),
                    ext(toEx1Mode(ex1), toEx2Mode(ex2), PF_NONE), attr(CF_0000, SZ_NONE)};
        }

        AddrMode dst() const { return mode(_dst); }
        AddrMode src() const { return mode(_src); }
        AddrMode ex1() const { return toAddrMode(Ex1Mode((_ext >> ex1Mode_gp) & ex1Mode_gm)); }
        AddrMode ex2() const { return toAddrMode(Ex2Mode((_ext >> ex2Mode_gp) & ex2Mode_gm)); }
        OprPos dstPos() const { return pos(_dst); }
        OprPos srcPos() const { return pos(_src); }
        PostFormat postFormat() const {
            return PostFormat((_ext >> postFormat_gp) & postFormat_gm);
        }
        CodeFormat codeFormat() const {
            return CodeFormat((_attr >> codeFormat_gp) & codeFormat_gm);
        }
        OprSize size() const { return OprSize((_attr >> size_gp) & size_gm); }

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
        static constexpr uint8_t opr(AddrMode mode, OprPos pos) {
            return (static_cast<uint8_t>(mode) << mode_gp) |
                   (static_cast<uint8_t>(pos) << modePos_gp);
        }

        static constexpr uint8_t ext(Ex1Mode ex1, Ex2Mode ex2, PostFormat postFormat) {
            return (static_cast<uint8_t>(ex1) << ex1Mode_gp) |
                   (static_cast<uint8_t>(ex2) << ex2Mode_gp) |
                   (static_cast<uint8_t>(postFormat) << postFormat_gp);
        }

        static constexpr uint8_t attr(CodeFormat codeFormat, OprSize size) {
            return (static_cast<uint8_t>(codeFormat) << codeFormat_gp) |
                   (static_cast<uint8_t>(size) << size_gp);
        }

        static inline AddrMode mode(uint8_t opr) { return AddrMode((opr >> mode_gp) & mode_gm); }
        static inline OprPos pos(uint8_t opr) { return OprPos((opr >> modePos_gp) & modePos_gm); }

        static AddrMode toAddrMode(Ex1Mode mode) {
            static constexpr AddrMode EX1MODES[] PROGMEM = {
                    M_NONE,   // E1_NONE
                    M_CNT,    // E1_CNT
                    M_WR,     // E1_WR
                    M_ERROR,  // E1_ERROR
            };
            return AddrMode(pgm_read_byte(EX1MODES + mode));
        }

        static inline Ex1Mode toEx1Mode(AddrMode mode) {
            switch (mode) {
            case M_NONE:
                return E1_NONE;
            case M_CNT:
            case M_IM:
                return E1_CNT;
            case M_WR:
            case M_R:
                return E1_WR;
            default:
                return E1_ERROR;
            }
        }

        static inline Ex2Mode toEx2Mode(AddrMode mode) {
            switch (mode) {
            case M_NONE:
                return E2_NONE;
            case M_CC:
                return E2_CC;
            default:
                return E2_ERROR;
            }
        }

        static AddrMode toAddrMode(Ex2Mode mode) {
            static constexpr AddrMode EX2MODES[] PROGMEM = {
                    M_NONE,   // E2_NONE
                    M_CC,     // E2_CC
                    M_ERROR,  // E2_ERROR
            };
            return AddrMode(pgm_read_byte(EX2MODES + mode));
        }

        // |_dst|, |_src|
        static constexpr int mode_gp = 0;
        static constexpr int modePos_gp = 5;
        static constexpr uint8_t mode_gm = 0x1f;
        static constexpr uint8_t modePos_gm = 0x7;
        // |_ext|
        static constexpr int ex1Mode_gp = 0;
        static constexpr int ex2Mode_gp = 2;
        static constexpr int postFormat_gp = 4;
        static constexpr uint8_t ex1Mode_gm = 0x3;
        static constexpr uint8_t ex2Mode_gm = 0x3;
        static constexpr uint8_t postFormat_gm = 0x7;
        // |_attr|
        static constexpr int codeFormat_gp = 0;
        static constexpr int size_gp = 4;
        static constexpr uint8_t codeFormat_gm = 0xf;
        static constexpr uint8_t size_gm = 0x7;
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opCode), _flags_P(flags) {}

    Flags readFlags() const {
        return Flags{pgm_read_byte(&_flags_P._dst), pgm_read_byte(&_flags_P._src),
                pgm_read_byte(&_flags_P._ext), pgm_read_byte(&_flags_P._attr)};
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
