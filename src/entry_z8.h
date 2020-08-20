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

enum CpuType {
    Z8,
    Z86C,
    SUPER8,
};

enum AddrMode {
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
    M_XL  = 17,  // Indexed Long: nnnn(rnn)
    // Those (18-) happen only in assembler internal.
    M_W   = 18,  // Register or Working register: Rxy x==RP
    M_IW  = 19,  // Indirect Register or Working register: @Rxy x==RP
    M_IWW = 20,  // Indirect Register Pair or Working register Pair: @RRxy x==RP
    M_WW  = 21,  // Register or Working register pair: RRxy: x=RP
    M_ERROR = 31,
};

// Post byte format
enum PostFormat {
    P0   = 0,  // No Post Byte check necessary
    P1_0 = 1,  // Least 1 bit is 0
    P1_1 = 2,  // Least 1 bit is 1
    P2_0 = 3,  // Least 2 bits are 00
    P2_1 = 4,  // Least 2 bits are 01
    P2_2 = 5,  // Least 2 bits are 10
    P4_0 = 6,  // Least 4 bits are 0000
    P4_1 = 7,  // Least 4 bits are 0001
};

struct Entry {
    const Config::opcode_t opCode;
    const uint16_t flags;
    const char *name;

    static inline AddrMode _dstMode(uint16_t flags) {
        return AddrMode((flags >> dstMode_gp) & addrMode_gm);
    }
    static inline AddrMode _srcMode(uint16_t flags) {
        return AddrMode((flags >> srcMode_gp) & addrMode_gm);
    }
    static inline AddrMode _extMode(uint16_t flags) {
        return AddrMode((flags >> extMode_gp) & extMode_gm);
    }
    static inline PostFormat _postFormat(uint16_t flags) {
        return PostFormat((flags >> postFmt_gp) & postFmt_gm);
    }
    static inline AddrMode addrExt(AddrMode mode) {
        if (static_cast<uint8_t>(mode) < 8) return mode;
        return M_ERROR;
    }
    static constexpr uint16_t _flags(
        AddrMode dst, AddrMode src, AddrMode ext, PostFormat post) {
        return ((static_cast<uint16_t>(dst) & addrMode_gm) << dstMode_gp)
            | ((static_cast<uint16_t>(src)  & addrMode_gm) << srcMode_gp)
            | ((static_cast<uint16_t>(ext)  & extMode_gm)  << extMode_gp)
            | ((static_cast<uint16_t>(post) & postFmt_gm)  << postFmt_gp);
    }

private:
    static constexpr uint8_t addrMode_gm = 0x1f;
    static constexpr uint8_t extMode_gm  = 0x7;
    static constexpr uint8_t postFmt_gm  = 0x7;
    static constexpr uint8_t dstMode_gp  = 0;
    static constexpr uint8_t srcMode_gp  = 5;
    static constexpr uint8_t extMode_gp  = 10;
    static constexpr uint8_t postFmt_gp  = 13;
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
