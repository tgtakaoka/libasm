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

namespace libasm {
namespace ns32000 {

enum AddrMode : uint8_t {
    M_NONE  = 0,
    M_GREG  = 1,  // Generic Register: Rn
#ifdef ENABLE_FLOAT
    M_FREG  = 2,  // Floating Register: Fn
#endif
    M_RREL  = 3,  // Register Relative: disp(Rn)
    M_MREL  = 4,  // Memory Relative: disp1(disp2(FP/SP/SB))
    M_IMM   = 5,  // Immediate: value
    M_ABS   = 6,  // Absolute: @disp
    M_EXT   = 7,  // External: EXT(disp1)+disp2
    M_TOS   = 8,  // Top of Stack: TOS
    M_MEM   = 9,  // Memory Space: disp(FP/SP/SB/PC)
    M_PREG  = 10, // Dedicated Register
#ifdef ENABLE_MMU
    M_MREG  = 11, // MMU Register
#endif
    M_CONF  = 12, // Configuration
    M_SOPT  = 13, // String instruction option
    M_PUSH  = 14, // Register list for push
    M_POP   = 15, // Register list for pop
    M_GENR  = 16, // Generic (incl. Immediate)
    M_GENC  = 17, // Generic Count (incl. Immediate)
    M_GENW  = 18, // Generic (excl. Immediate)
    M_GENA  = M_GENW, // Generic (excl. Immediate)
#ifdef ENABLE_FLOAT
    M_FENR  = 19, // Floating Generic (incl. Immediate)
    M_FENW  = 20, // Floating Generic (excl. Immediate)
#endif
    M_DISP  = 21, // Displacement
    M_INT4  = 22, // 4-bit signed Integer
    M_REL   = 23, // Relative address
    M_BFOFF = 24, // Bit Field Offset: 0~7
    M_BFLEN = 25, // Bit Field Length: 1~32
    M_LEN32 = 26, // Length: 1~32
    M_LEN16 = 27, // Length: 1~16
    M_LEN8  = 28, // Length: 1~8
    M_LEN4  = 29, // Length: 1~4
    M_EMPTY = 30, // Empty List: [] (M_CONF/M_SOPT/M_PUSH/M_POP)
};

enum OprPos : uint8_t {
    P_NONE  = 0,
    P_GEN1  = 1,  // Generic operand 1
    P_GEN2  = 2,  // Generic operand 2
    P_SHORT = 3,  // Short operand
    P_REG   = 4,  // Register operand
    P_IMPL  = 5,  // Implied Immediate
    P_DISP  = 6,  // Displacement
};

enum OprSize : uint8_t {
    SZ_BYTE   = 0,  // 8-bit integer (Byte)
    SZ_WORD   = 1,  // 16-bit integer (Word)
    SZ_LONG   = 2,  // 32-bit integer (Double)
    SZ_QUAD   = 3,  // 64-bit integer (Quad)
#ifdef ENABLE_FLOAT
    SZ_FLOAT  = 4,  // 32-bit float (Float)
    SZ_DOUBLE = 5,  // 64-bit float (Long)
#endif
    SZ_NONE   = 6,
};

struct Entry {
    const Config::opcode_t opCode;
    const uint32_t flags;
    const char *name;

    static inline AddrMode _mode(uint8_t opr) {
        return AddrMode((opr >> oprMode_gp) & oprMode_gm);
    }
    static inline OprPos _pos(uint8_t opr) {
        return OprPos((opr >> oprPos_gp) & oprPos_gm);
    }
    static inline AddrMode _ex2Mode(uint8_t ex2) {
        return ex2Mode(Ex2Mode((ex2 >> ex2Mode_gp) & ex2Mode_gm));
    }
    static inline OprPos _ex2Pos(uint8_t ex2) {
        return ex2Pos(Ex2Pos((ex2 >> ex2Pos_gp) & ex2Pos_gm));
    }
    static inline OprSize _oprSize(uint8_t ex2) {
        return OprSize((ex2 >> oprSize_gp) & oprSize_gm);
    }
    static inline uint8_t _src(uint32_t flags) {
        return static_cast<uint8_t>(flags >> src_gp);
    }
    static inline uint8_t _dst(uint32_t flags) {
        return static_cast<uint8_t>(flags >> dst_gp);
    }
    static inline uint8_t _ex1(uint32_t flags) {
        return static_cast<uint8_t>(flags >> ex1_gp);
    }
    static inline uint8_t _ex2(uint32_t flags) {
        return static_cast<uint8_t>(flags >> ex2_gp);
    }
    static constexpr uint8_t _opr(AddrMode mode, OprPos pos) {
        return (static_cast<uint8_t>(mode) << oprMode_gp)
            | (static_cast<uint8_t>(pos) << oprPos_gp);
    }
    static constexpr uint8_t _ex2(AddrMode mode, OprPos pos, OprSize size) {
        return (static_cast<uint8_t>(modeEx2(mode) << ex2Mode_gp))
            | (static_cast<uint8_t>(posEx2(pos) << ex2Pos_gp))
            | (static_cast<uint8_t>(size) << oprSize_gp);
    }
    static constexpr uint32_t _flags(
            uint8_t src, uint8_t dst, uint8_t ex1, uint8_t ex2) {
        return (static_cast<uint32_t>(src) << src_gp)
            | (static_cast<uint32_t>(dst) << dst_gp)
            | (static_cast<uint32_t>(ex1) << ex1_gp)
            | (static_cast<uint32_t>(ex2) << ex2_gp);
    }

private:
    enum Ex2Mode : uint8_t {
        EM2_NONE = 0,
        EM2_IMM  = 1,
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
    static constexpr Ex2Mode modeEx2(AddrMode mode) {
        return mode == M_NONE ? EM2_NONE
            : (mode == M_IMM  ? EM2_IMM
            : (mode == M_BFLEN ? EM2_BFLEN
            : (mode == M_LEN32 ? EM2_LEN32
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
    static constexpr Ex2Pos posEx2(OprPos pos) {
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
    static inline AddrMode ex2Mode(Ex2Mode mode) {
        switch (mode) {
        case EM2_NONE:  return M_NONE;
        case EM2_IMM:   return M_IMM;
        case EM2_BFLEN: return M_BFLEN;
        case EM2_LEN32: return M_LEN32;
        default:        return M_NONE;
        }
    }
    static inline OprPos ex2Pos(Ex2Pos pos) {
        if (pos == EP2_NONE) return P_NONE;
        if (pos == EP2_IMPL) return P_IMPL;
        return (pos == EP2_DISP) ? P_DISP : P_NONE;
    }

    // |src|,|dst|,|ex1|
    static constexpr uint8_t oprMode_gm = 0x1f;
    static constexpr uint8_t oprPos_gm  = 0x7;
    static constexpr uint8_t oprMode_gp = 0;
    static constexpr uint8_t oprPos_gp  = 5;
    // |ex2|
    static constexpr uint8_t ex2Mode_gm = 0x7;
    static constexpr uint8_t ex2Pos_gm  = 0x3;
    static constexpr uint8_t oprSize_gm = 0x7;
    static constexpr uint8_t ex2Mode_gp = 0;
    static constexpr uint8_t ex2Pos_gp  = 3;
    static constexpr uint8_t oprSize_gp = 5;
    // |flags|
    static constexpr uint8_t src_gp = 0;
    static constexpr uint8_t dst_gp = 8;
    static constexpr uint8_t ex1_gp = 16;
    static constexpr uint8_t ex2_gp = 24;
};

} // namespace ns32000
} // namespace libasm

#endif // __ENTRY_NS32000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
