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

#ifndef __ENTRY_Z8_H__
#define __ENTRY_Z8_H__

#include "config_z8.h"

namespace libasm {
namespace z8 {

enum CpuType : uint8_t {
    Z8,
    Z86C,
    SUPER8,
};

enum AddrMode : uint8_t {
    // Those (0~7) happen all operands including extra.
    M_NO  = 0,   // No operand
    M_IM  = 1,   // Immediate: #nn
    M_r   = 2,   // Working register: rn
    M_RA  = 3,   // Relative Address: nnnn
    M_IMb = 4,   // Bit position: #n
    M_DA  = 5,   // Direct Address: nnnn
    M_R   = 6,   // Register: Rn
    M_RR  = 7,   // Register Pair: RRn
    // Those (6~16)  happen on destination and source operands.
    M_IR  = 8,   // Indirect Register: @Rn
    M_Ir  = 9,   // Indirect Working register: @rn
    M_IRR = 10,   // Indirect Register Pair: @RRn
    M_Irr = 11,  // Indirect Working Register Pair: @rrn
    M_X   = 12,  // Indexed: nn(rn)
    M_cc  = 13,  // Condition Code: cc
    M_rr  = 14,  // Working register pair: rrn
    // Super8
    M_IML = 15,  // Immediate Long: #nnnn
    M_XS  = 16,  // Indexed Short: nn(rrn)
    M_XL  = 17,  // Indexed Long: nnnn(rrn)
    // Those (18-) happen only in assembler internal.
    M_W   = 18,  // Register or Working register: Rxy x==RP
    M_IW  = 19,  // Indirect Register or Working register: @Rxy x==RP
    M_IWW = 20,  // Indirect Register Pair or Working register Pair: @RRxy x==RP
    M_WW  = 21,  // Register or Working register pair: RRxy: x=RP
};

// Post byte format
enum PostFormat : uint8_t {
    P0   = 0,  // No Post Byte check necessary
    P1_0 = 1,  // Least 1 bit is 0
    P1_1 = 2,  // Least 1 bit is 1
    P2_0 = 3,  // Least 2 bits are 00
    P2_1 = 4,  // Least 2 bits are 01
    P2_2 = 5,  // Least 2 bits are 10
    P4_0 = 6,  // Least 4 bits are 0000
    P4_1 = 7,  // Least 4 bits are 0001
};

// Destination and Source order
enum DstSrc : uint8_t {
    DS_NO   = 0, // one or no operand.
    DST_SRC = 0, // destination first, source second.
    SRC_DST = 1, // source first, destination second.
};

struct Entry {
    const Config::opcode_t opCode;
    const uint32_t flags;
    const char *name;

    static inline AddrMode _mode(uint8_t opr) {
        return AddrMode(opr & addrMode_gm);
    }
    static inline PostFormat _postFmt(uint8_t fmt) {
        return PostFormat(fmt & postFmt_gm);
    }
    static inline bool _dstSrc(uint8_t fmt) {
        return DstSrc((fmt >> dstSrc_gp) & dstSrc_gm) == DST_SRC;
    }
    static inline uint8_t _dst(uint32_t flags) {
        return static_cast<uint8_t>(flags >> dst_gp);
    }
    static inline uint8_t _src(uint32_t flags) {
        return static_cast<uint8_t>(flags >> src_gp);
    }
    static inline uint8_t _ext(uint32_t flags) {
        return static_cast<uint8_t>(flags >> ext_gp);
    }
    static inline uint8_t _fmt(uint32_t flags) {
        return static_cast<uint8_t>(flags >> fmt_gp);
    }
    static constexpr uint8_t _opr(AddrMode mode) {
        return static_cast<uint8_t>(mode & addrMode_gm);
    }
    static constexpr uint8_t _fmt(DstSrc dstSrc, PostFormat postFmt) {
        return static_cast<uint8_t>(postFmt & postFmt_gm)
            | (static_cast<uint8_t>(dstSrc & dstSrc_gm) << dstSrc_gp);
    }
    static constexpr uint32_t _flags(
            uint8_t dst, uint8_t src, uint8_t ext, uint8_t fmt) {
        return (static_cast<uint32_t>(dst) << dst_gp)
            | (static_cast<uint32_t>(src) << src_gp)
            | (static_cast<uint32_t>(ext) << ext_gp)
            | (static_cast<uint32_t>(fmt) << fmt_gp);
    }
private:
    // |dst|, |src|, |ext|
    static constexpr int8_t dst_gp = 0;
    static constexpr int8_t src_gp = 8;
    static constexpr int8_t ext_gp = 16;
    static constexpr uint8_t addrMode_gm = 0x1f;
    // |fmt|
    static constexpr int8_t fmt_gp = 24;
    static constexpr uint8_t postFmt_gm = 0x7;
    static constexpr uint8_t dstSrc_gm  = 0x1;
    static constexpr int8_t dstSrc_gp   = 7;
};

} // namespace z8
} // namespace libasm

#endif // __ENTRY_Z8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
