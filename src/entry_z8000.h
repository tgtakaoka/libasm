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

#ifndef __ENTRY_Z8000_H__
#define __ENTRY_Z8000_H__

#include <stdint.h>

#include "config_z8000.h"
#include "entry_base.h"

namespace libasm {
namespace z8000 {

enum CpuType : uint8_t {
    Z8001,
    Z8002,
};

enum OprSize : uint8_t {
    SZ_NONE = 0,
    SZ_BYTE = 1,
    SZ_WORD = 2,
    SZ_LONG = 3,
    SZ_QUAD = 4,
    SZ_ADDR = 5,
};

enum AddrMode : uint8_t {
    M_NO = 0,
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
    M_DR = 26,    // Double-Sized Register: WORD:RRn LONG:RQn
    M_WR07 = 27,  // R0~R7
    M_WR = 28,    // R0-R15
    M_IRIO = 29,  // Indirect Register IO Address: @R1~R15
    M_ERROR = 31,
};

enum PostMode : uint8_t {
    P_NO = 0,    // No Post Word
    P_0XX0 = 1,  // 0 is part of instruction
    P_0XX8 = 2,  // 8 is part of instruction
    P_0XXE = 3,  // E is part of instruction
    P_0X0X = 4,  // Any post word
    P_0X00 = 5,  // Any post word
    P_0XXX = 6,  // Any post word
};

enum CodeMask : uint8_t {
    CM_0x0000 = 0,
    CM_0x0003 = 1,
    CM_0x000F = 2,
    CM_0x00F0 = 3,
    CM_0x00F2 = 4,
    CM_0x00F7 = 5,
    CM_0x00FF = 6,
    CM_0x0F7F = 7,
    CM_0x0FFF = 8,
    CM_0xC0F0 = 9,
    CM_0xC0FF = 10,
};

enum ModeField : uint8_t {
    MF_NO = 0,
    MF_C0 = 1,
    MF_C4 = 2,
    MF_C8 = 3,
    MF_P0 = 4,
    MF_P4 = 5,
    MF_P8 = 6,
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint8_t _dst;
        uint8_t _src;
        uint8_t _ext;
        uint8_t _size;

        static constexpr Flags create(AddrMode dst, ModeField dstField, AddrMode src,
                ModeField srcField, AddrMode ex1, AddrMode ex2, PostMode post, CodeMask mask,
                OprSize size) {
            return Flags{Entry::_opr(dst, dstField), Entry::_opr(src, srcField),
                    Entry::_exopr(ex1, ex2, post), Entry::_size(mask, size)};
        }
        Flags read() const {
            return Flags{pgm_read_byte(&_dst), pgm_read_byte(&_src), pgm_read_byte(&_ext),
                    pgm_read_byte(&_size)};
        }

        AddrMode dstMode() const { return Entry::_mode(_dst); }
        AddrMode srcMode() const { return Entry::_mode(_src); }
        AddrMode ex1Mode() const { return toAddrMode(Ex1Mode((_ext >> ex1Mode_gp) & ex1Mode_gm)); }
        AddrMode ex2Mode() const { return toAddrMode(Ex2Mode((_ext >> ex2Mode_gp) & ex2Mode_gm)); }
        ModeField dstField() const { return Entry::_field(_dst); }
        ModeField srcField() const { return Entry::_field(_src); }
        PostMode postMode() const { return PostMode((_ext >> postVal_gp) & postVal_gm); }
        OprSize oprSize() const { return OprSize((_size >> oprSize_gp) & oprSize_gm); }
        uint8_t postMask() const { return Entry::_postMask(postMode()); }
        uint8_t postVal() const { return Entry::_postVal(postMode()); }
        uint16_t codeMask() const { return Entry::_codeMask(_size); }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr uint8_t _opr(AddrMode mode, ModeField field) {
        return (static_cast<uint8_t>(mode) << addrMode_gp) |
               (static_cast<uint8_t>(field) << modeField_gp);
    }
    static constexpr uint8_t _exopr(AddrMode ex1, AddrMode ex2, PostMode post) {
        return (static_cast<uint8_t>(toEx1Mode(ex1)) << ex1Mode_gp) |
               (static_cast<uint8_t>(toEx2Mode(ex2)) << ex2Mode_gp) |
               (static_cast<uint8_t>(post) << postVal_gp);
    }
    static constexpr uint8_t _size(CodeMask mask, OprSize size) {
        return (static_cast<uint8_t>(mask) << codeMask_gp) |
               (static_cast<uint8_t>(size) << oprSize_gp);
    }

    static inline AddrMode _mode(uint8_t opr) {
        return AddrMode((opr >> addrMode_gp) & addrMode_gm);
    }
    static inline ModeField _field(uint8_t opr) {
        return ModeField((opr >> modeField_gp) & modeField_gm);
    }
    static inline uint8_t _postVal(PostMode post) {
        if (post == P_0XX0)
            return 0x0;
        if (post == P_0XX8)
            return 0x8;
        if (post == P_0XXE)
            return 0xE;
        return 0;
    }
    static inline uint8_t _postMask(PostMode post) {
        if (post == P_0XX0)
            return 0xF;
        if (post == P_0XX8)
            return 0xF;
        if (post == P_0XXE)
            return 0xF;
        return 0;
    }
    static inline uint16_t _codeMask(uint8_t size) {
        switch (CodeMask((size >> codeMask_gp) & codeMask_gm)) {
        case CM_0x0003:
            return 0x0003;
        case CM_0x000F:
            return 0x000F;
        case CM_0x00F0:
            return 0x00F0;
        case CM_0x00F2:
            return 0x00F2;
        case CM_0x00F7:
            return 0x00F7;
        case CM_0x00FF:
            return 0x00FF;
        case CM_0x0F7F:
            return 0x0F7F;
        case CM_0x0FFF:
            return 0x0FFF;
        case CM_0xC0F0:
            return 0xC0F0;
        case CM_0xC0FF:
            return 0xC0FF;
        default:
            return 0;
        }
    }

    enum Ex1Mode : uint8_t {
        E1_NO = 0,
        E1_CNT = 1,
        E1_WR = 2,
        E1_ERROR = 3,
    };
    enum Ex2Mode : uint8_t {
        E2_NO = 0,
        E2_CC = 1,
        E2_ERROR = 3,
    };
    static constexpr Ex1Mode toEx1Mode(AddrMode mode) {
        return mode == M_NO ? E1_NO
                            : ((mode == M_CNT || mode == M_IM)
                                              ? E1_CNT
                                              : ((mode == M_WR || mode == M_R) ? E1_WR : E1_ERROR));
        /*
        switch (mode) {
        case M_NO:  return E1_NO;
        case M_CNT:
        case M_IM:  return E1_CNT;
        case M_WR:
        case M_R:   return E1_WR;
        default:    return E1_ERROR;
        }
        */
    }
    static inline AddrMode toAddrMode(Ex1Mode mode) {
        if (mode == E1_NO)
            return M_NO;
        if (mode == E1_CNT)
            return M_CNT;
        if (mode == E1_WR)
            return M_WR;
        return M_ERROR;
    }
    static constexpr Ex2Mode toEx2Mode(AddrMode mode) {
        return mode == M_NO ? E2_NO : (mode == M_CC ? E2_CC : E2_ERROR);
        /*
        switch (mode) {
        case M_NO: return E2_NO;
        case M_CC: return E2_CC;
        default:   return E2_ERROR;
        }
        */
    }
    static inline AddrMode toAddrMode(Ex2Mode mode) {
        if (mode == E2_NO)
            return M_NO;
        if (mode == E2_CC)
            return M_CC;
        return M_ERROR;
    }

    // |dst|, |src|
    static constexpr uint8_t addrMode_gm = 0x1f;
    static constexpr uint8_t modeField_gm = 0x7;
    static constexpr uint8_t addrMode_gp = 0;
    static constexpr uint8_t modeField_gp = 5;
    // |ext|
    static constexpr uint8_t ex1Mode_gm = 0x3;
    static constexpr uint8_t ex2Mode_gm = 0x3;
    static constexpr uint8_t postVal_gm = 0x7;
    static constexpr uint8_t ex1Mode_gp = 0;
    static constexpr uint8_t ex2Mode_gp = 2;
    static constexpr uint8_t postVal_gp = 4;
    // |size|
    static constexpr uint8_t codeMask_gm = 0xf;
    static constexpr uint8_t oprSize_gm = 0x7;
    static constexpr uint8_t codeMask_gp = 0;
    static constexpr uint8_t oprSize_gp = 4;
};

}  // namespace z8000
}  // namespace libasm

#endif  // __ENTRY_Z8000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
