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

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint8_t _dst;
        uint8_t _src;
        uint8_t _ext;
        uint8_t _fmt;

        static constexpr Flags create(
                AddrMode dst, AddrMode src, AddrMode ext,
                DstSrc dstSrc, PostFormat postFmt) {
            return Flags{
                static_cast<uint8_t>(dst),
                static_cast<uint8_t>(src),
                static_cast<uint8_t>(ext),
                Entry::_fmt(dstSrc, postFmt)
            };
        }
        Flags read() const {
            return Flags{
                pgm_read_byte(&_dst),
                pgm_read_byte(&_src),
                pgm_read_byte(&_ext),
                pgm_read_byte(&_fmt)
            };
        }

        AddrMode dstMode() const { return AddrMode(_dst); }
        AddrMode srcMode() const { return AddrMode(_src); }
        AddrMode extMode() const { return AddrMode(_ext); }
        PostFormat postFmt() const { return PostFormat(_fmt & postFmt_gm); }
        bool dstSrc() const {
            return DstSrc((_fmt >> dstSrc_gp) & dstSrc_gm) == DST_SRC;
        }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr uint8_t _fmt(DstSrc dstSrc, PostFormat postFmt) {
        return static_cast<uint8_t>(postFmt & postFmt_gm)
            | (static_cast<uint8_t>(dstSrc & dstSrc_gm) << dstSrc_gp);
    }

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
