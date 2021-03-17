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

#ifndef __ENTRY_NS32000_H__
#define __ENTRY_NS32000_H__

#include "config_ns32000.h"
#include "entry_base.h"

namespace libasm {
namespace ns32000 {

enum FpuType : uint8_t {
    FPU_NONE = 0,
    FPU_NS32081 = 1,
};
enum MmuType : uint8_t {
    MMU_NONE = 0,
    MMU_NS32082 = 1,
};

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
    M_LEN8 = 29,   // Length: 1~8
    M_LEN4 = 30,   // Length: 1~4
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
    SZ_BYTE = 0,    // 8-bit integer (Byte)
    SZ_WORD = 1,    // 16-bit integer (Word)
    SZ_DOUBLE = 2,  // 32-bit integer (Double)
    SZ_QUAD = 3,    // 64-bit integer (Quad)
    SZ_FLOAT = 4,   // 32-bit float (Float)
    SZ_LONG = 5,    // 64-bit float (Long)
    SZ_NONE = 6,
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint8_t _src;
        uint8_t _dst;
        uint8_t _ex1;
        uint8_t _ex2;

        static constexpr Flags create(AddrMode src, OprPos srcp, AddrMode dst,
                OprPos dstp, AddrMode ex1, OprPos ex1p, AddrMode ex2,
                OprPos ex2p, OprSize size) {
            return Entry::Flags{Entry::_opr(src, srcp), Entry::_opr(dst, dstp),
                    Entry::_opr(ex1, ex1p), Entry::_ex2(ex2, ex2p, size)};
        }
        Flags read() const {
            return Flags{pgm_read_byte(&_src), pgm_read_byte(&_dst),
                    pgm_read_byte(&_ex1), pgm_read_byte(&_ex2)};
        }

        AddrMode srcMode() const { return _mode(_src); }
        AddrMode dstMode() const { return _mode(_dst); }
        AddrMode ex1Mode() const { return _mode(_ex1); }
        AddrMode ex2Mode() const {
            return toAddrMode(Ex2Mode((_ex2 >> ex2Mode_gp) & ex2Mode_gm));
        }
        OprPos srcPos() const { return _pos(_src); }
        OprPos dstPos() const { return _pos(_dst); }
        OprPos ex1Pos() const { return _pos(_ex1); }
        OprPos ex2Pos() const {
            return toOprPos(Ex2Pos((_ex2 >> ex2Pos_gp) & ex2Pos_gm));
        }
        OprSize oprSize() const {
            return OprSize((_ex2 >> oprSize_gp) & oprSize_gm);
        }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static inline AddrMode _mode(uint8_t opr) {
        return AddrMode((opr >> oprMode_gp) & oprMode_gm);
    }
    static inline OprPos _pos(uint8_t opr) {
        return OprPos((opr >> oprPos_gp) & oprPos_gm);
    }
    static constexpr uint8_t _opr(AddrMode mode, OprPos pos) {
        return (static_cast<uint8_t>(mode) << oprMode_gp) |
               (static_cast<uint8_t>(pos) << oprPos_gp);
    }
    static constexpr uint8_t _ex2(AddrMode mode, OprPos pos, OprSize size) {
        return (static_cast<uint8_t>(toEx2Mode(mode) << ex2Mode_gp)) |
               (static_cast<uint8_t>(toEx2Pos(pos) << ex2Pos_gp)) |
               (static_cast<uint8_t>(size) << oprSize_gp);
    }

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
    static constexpr Ex2Mode toEx2Mode(AddrMode mode) {
        return mode == M_NONE
                       ? EM2_NONE
                       : (mode == M_IMM ? EM2_IMM
                                        : (mode == M_BFLEN
                                                          ? EM2_BFLEN
                                                          : (mode == M_LEN32
                                                                            ? EM2_LEN32
                                                                            : EM2_ERROR)));
        /*
        switch (mode) {
        case M_NONE:  return EM2_NONE;
        case M_IMM:   return EM2_IMM;
        case M_BFLEN: return EM2_BFLEN;
        case M_LEN32: return EM2_LEN32;
        default:      return EM2_ERROR;
        }
        */
    }
    static constexpr Ex2Pos toEx2Pos(OprPos pos) {
        return pos == P_NONE ? EP2_NONE
                             : (pos == P_IMPL ? EP2_IMPL
                                              : (pos == P_DISP ? EP2_DISP
                                                               : EP2_ERROR));
        /*
        switch (pos) {
        case P_NONE: return EP2_NONE;
        case P_IMPL: return EP2_IMPL;
        case P_DISP: return EP2_DISP;
        default:     return EP2_ERROR;
        }
        */
    }
    static inline AddrMode toAddrMode(Ex2Mode mode) {
        switch (mode) {
        case EM2_NONE:
            return M_NONE;
        case EM2_IMM:
            return M_IMM;
        case EM2_BFLEN:
            return M_BFLEN;
        case EM2_LEN32:
            return M_LEN32;
        default:
            return M_NONE;
        }
    }
    static inline OprPos toOprPos(Ex2Pos pos) {
        if (pos == EP2_NONE)
            return P_NONE;
        if (pos == EP2_IMPL)
            return P_IMPL;
        return (pos == EP2_DISP) ? P_DISP : P_NONE;
    }

    // |src|,|dst|,|ex1|
    static constexpr uint8_t oprMode_gm = 0x1f;
    static constexpr uint8_t oprPos_gm = 0x7;
    static constexpr uint8_t oprMode_gp = 0;
    static constexpr uint8_t oprPos_gp = 5;
    // |ex2|
    static constexpr uint8_t ex2Mode_gm = 0x7;
    static constexpr uint8_t ex2Pos_gm = 0x3;
    static constexpr uint8_t oprSize_gm = 0x7;
    static constexpr uint8_t ex2Mode_gp = 0;
    static constexpr uint8_t ex2Pos_gp = 3;
    static constexpr uint8_t oprSize_gp = 5;
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
