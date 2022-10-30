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

#include <stdint.h>

#include "config_z8.h"
#include "entry_base.h"

namespace libasm {
namespace z8 {

enum CpuType : uint8_t {
    Z8,
    Z86C,
    SUPER8,
};

enum AddrMode : uint8_t {
    // Those (0~7) happen all operands including extra.
    M_NONE = 0,  // No operand
    M_IM = 1,    // Immediate: #nn
    M_r = 2,     // Working register: rn
    M_RA = 3,    // Relative Address: nnnn
    M_IMb = 4,   // Bit position: #n
    M_DA = 5,    // Direct Address: nnnn
    M_R = 6,     // Register: Rn
    M_RR = 7,    // Register Pair: RRn
    // Those (6~16)  happen on destination and source operands.
    M_IR = 8,    // Indirect Register: @Rn
    M_Ir = 9,    // Indirect Working register: @rn
    M_IRR = 10,  // Indirect Register Pair: @RRn
    M_Irr = 11,  // Indirect Working Register Pair: @rrn
    M_X = 12,    // Indexed: nn(rn)
    M_cc = 13,   // Condition Code: cc
    M_rr = 14,   // Working register pair: rrn
    // Super8
    M_IML = 15,  // Immediate Long: #nnnn
    M_XS = 16,   // Indexed Short: nn(rrn)
    M_XL = 17,   // Indexed Long: nnnn(rrn)
    // Those (18-) happen only in assembler internal.
    M_W = 18,    // Register or Working register: Rxy x==RP
    M_IW = 19,   // Indirect Register or Working register: @Rxy x==RP
    M_IWW = 20,  // Indirect Register Pair or Working register Pair: @RRxy x==RP
    M_WW = 21,   // Register or Working register pair: RRxy: x=RP
};

// Post byte format
enum PostFormat : uint8_t {
    PF_NONE = 0,  // No Post Byte check necessary
    PF1_0 = 1,    // Least 1 bit is 0
    PF1_1 = 2,    // Least 1 bit is 1
    PF2_0 = 3,    // Least 2 bits are 00
    PF2_1 = 4,    // Least 2 bits are 01
    PF2_2 = 5,    // Least 2 bits are 10
    PF4_0 = 6,    // Least 4 bits are 0000
    PF4_1 = 7,    // Least 4 bits are 0001
};

// Destination and Source order
enum OprOrder : uint8_t {
    ORDER_NONE = 0,  // one or no operand.
    DST_FIRST = 0,   // destination first, source second.
    SRC_FIRST = 1,   // source first, destination second.
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint8_t _dst;
        uint8_t _src;
        uint8_t _ext;
        uint8_t _fmt;

        static constexpr Flags create(
                AddrMode dst, AddrMode src, AddrMode ext, OprOrder order, PostFormat post) {
            return Flags{static_cast<uint8_t>(dst), static_cast<uint8_t>(src),
                    static_cast<uint8_t>(ext), Entry::fmt(order, post)};
        }
        Flags read() const {
            return Flags{pgm_read_byte(&_dst), pgm_read_byte(&_src), pgm_read_byte(&_ext),
                    pgm_read_byte(&_fmt)};
        }

        AddrMode dst() const { return AddrMode(_dst); }
        AddrMode src() const { return AddrMode(_src); }
        AddrMode ext() const { return AddrMode(_ext); }
        PostFormat postFormat() const { return PostFormat(_fmt & post_gm); }
        bool dstFirst() const { return OprOrder((_fmt >> order_gp) & order_gm) == DST_FIRST; }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr uint8_t fmt(OprOrder order, PostFormat post) {
        return static_cast<uint8_t>(post & post_gm) |
               (static_cast<uint8_t>(order & order_gm) << order_gp);
    }

    // |fmt|
    static constexpr uint8_t post_gm = 0x7;
    static constexpr uint8_t order_gm = 0x1;
    static constexpr int8_t order_gp = 7;
};

}  // namespace z8
}  // namespace libasm

#endif  // __ENTRY_Z8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
