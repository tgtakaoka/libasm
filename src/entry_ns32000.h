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

#ifndef __LIBASM_ENTRY_NS32000_H__
#define __LIBASM_ENTRY_NS32000_H__

#include <stdint.h>

#include "config_ns32000.h"
#include "entry_base.h"

namespace libasm {
namespace ns32000 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_GREG = 1,    // Generic Register: Rn
    M_FREG = 2,    // Floating Register: Fn
    M_RREL = 3,    // Register Relative: disp(Rn)
    M_MREL = 4,    // Memory Relative: disp1(disp2(FP/SP/SB))
    M_IMM = 5,     // Immediate: value
    M_ABS = 6,     // Absolute: @disp
    M_EXT = 7,     // External: EXT(disp1)+disp2
    M_TOS = 8,     // Top of Stack: TOS
    M_MEM = 9,     // Memory Space: disp(FP/SP/SB/PC)
    M_PREG = 10,   // Dedicated Register
    M_MREG = 11,   // MMU Register
    M_CONF = 12,   // Configuration
    M_SOPT = 13,   // String instruction option
    M_RLST = 14,   // Register list
    M_GENR = 16,   // Generic read (incl. Immediate)
    M_GENC = 17,   // Generic Count (incl. Immediate)
    M_GENW = 18,   // Generic write/rmw/regaddr (excl. Immediate)
    M_GENA = 19,   // Generic address (excl. Immediate and Register)
    M_FENR = 20,   // Floating Generic read (incl. Immediate)
    M_FENW = 21,   // Floating Generic write/rmw (excl. Immediate)
    M_DISP = 22,   // Displacement
    M_INT4 = 23,   // 4-bit signed Integer
    M_REL = 24,    // Relative address
    M_BFOFF = 25,  // Bit Field Offset: 0~7
    M_BFLEN = 26,  // Bit Field Length: 1~32
    M_LEN32 = 27,  // Length: 1~32
    M_LEN16 = 28,  // Length: 1~16
    M_ZERO = 29,   // 00000 of GEN field
};

enum Ex2Mode : uint8_t {
    EM2_NONE = 0,
    EM2_IMM = 1,
    EM2_BFLEN = 2,
    EM2_LEN32 = 3,
    EM2_ERROR = 4,
};

enum Ex2Pos : uint8_t {
    EP2_NONE = 0,
    EP2_IMPL = 1,
    EP2_DISP = 2,
    EP2_ERROR = 3,
};

enum OprPos : uint8_t {
    P_NONE = 0,
    P_GEN1 = 1,   // Generic operand 1
    P_GEN2 = 2,   // Generic operand 2
    P_SHORT = 3,  // Short operand
    P_REG = 4,    // Register operand
    P_IMPL = 5,   // Implied Immediate
    P_DISP = 6,   // Displacement
};

enum OprSize : uint8_t {
    SZ_NONE = Size::SZ_NONE,
    SZ_BYTE = Size::SZ_BYTE,  // 8-bit integer (Byte)
    SZ_WORD = Size::SZ_WORD,  // 16-bit integer (Word)
    SZ_QUAD = Size::SZ_QUAD,  // 32-bit integer/float (Double, Float)
    SZ_OCTA = Size::SZ_OCTA,  // 64-bit integer/float (Quad, Long)
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint8_t _src;
        uint8_t _dst;
        uint8_t _ex1;
        uint8_t _ex2;

        static constexpr Flags create(AddrMode src, OprPos srcp, AddrMode dst, OprPos dstp,
                AddrMode ex1, OprPos ex1p, Ex2Mode ex2, Ex2Pos ex2p, OprSize size) {
            return Entry::Flags{Entry::opr(src, srcp), Entry::opr(dst, dstp), Entry::opr(ex1, ex1p),
                    Entry::ex2(ex2, ex2p, size)};
        }

        static const Flags create(AddrMode src, AddrMode dst, AddrMode ex1, AddrMode ex2) {
            return Entry::Flags{Entry::opr(src, P_NONE), Entry::opr(dst, P_NONE),
                    Entry::opr(ex1, P_NONE), Entry::ex2(toEx2Mode(ex2), EP2_NONE, SZ_NONE)};
        }

        Flags read() const {
            return Flags{pgm_read_byte(&_src), pgm_read_byte(&_dst), pgm_read_byte(&_ex1),
                    pgm_read_byte(&_ex2)};
        }
        AddrMode src() const { return mode(_src); }
        AddrMode dst() const { return mode(_dst); }
        AddrMode ex1() const { return mode(_ex1); }
        AddrMode ex2() const { return toAddrMode(Ex2Mode((_ex2 >> ex2Mode_gp) & ex2Mode_gm)); }
        OprPos srcPos() const { return pos(_src); }
        OprPos dstPos() const { return pos(_dst); }
        OprPos ex1Pos() const { return pos(_ex1); }
        OprPos ex2Pos() const { return toOprPos(Ex2Pos((_ex2 >> ex2Pos_gp) & ex2Pos_gm)); }
        OprSize size() const { return OprSize((_ex2 >> size_gp) & size_gm); }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : Base(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    const Flags _flags;

    static inline AddrMode mode(uint8_t opr) { return AddrMode((opr >> mode_gp) & mode_gm); }

    static inline OprPos pos(uint8_t opr) { return OprPos((opr >> pos_gp) & pos_gm); }

    static constexpr uint8_t opr(AddrMode mode, OprPos pos) {
        return (static_cast<uint8_t>(mode) << mode_gp) | (static_cast<uint8_t>(pos) << pos_gp);
    }

    static constexpr uint8_t ex2(Ex2Mode mode, Ex2Pos pos, OprSize size) {
        return (static_cast<uint8_t>(mode << ex2Mode_gp)) |
               (static_cast<uint8_t>(pos << ex2Pos_gp)) | (static_cast<uint8_t>(size) << size_gp);
    }

    static inline Ex2Mode toEx2Mode(AddrMode mode) {
        switch (mode) {
        case M_NONE:
            return EM2_NONE;
        case M_IMM:
            return EM2_IMM;
        case M_BFLEN:
            return EM2_BFLEN;
        case M_LEN32:
            return EM2_LEN32;
        default:
            return EM2_ERROR;
        }
    }

    static AddrMode toAddrMode(Ex2Mode mode);
    static OprPos toOprPos(Ex2Pos pos);

    // |src|,|dst|,|ex1|
    static constexpr uint8_t mode_gp = 0;
    static constexpr uint8_t pos_gp = 5;
    static constexpr uint8_t mode_gm = 0x1F;
    static constexpr uint8_t pos_gm = 0x07;
    // |ex2|
    static constexpr uint8_t ex2Mode_gp = 0;
    static constexpr uint8_t ex2Pos_gp = 3;
    static constexpr uint8_t size_gp = 5;
    static constexpr uint8_t ex2Mode_gm = 0x07;
    static constexpr uint8_t ex2Pos_gm = 0x03;
    static constexpr uint8_t size_gm = 0x07;
};

}  // namespace ns32000
}  // namespace libasm

#endif  // __ENTRY_NS32000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
