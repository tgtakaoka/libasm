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

#ifndef __LIBASM_ENTRY_Z8_H__
#define __LIBASM_ENTRY_Z8_H__

#include <stdint.h>

#include "config_z8.h"
#include "entry_base.h"

namespace libasm {
namespace z8 {

enum AddrMode : uint8_t {
    M_NONE = 0,  // No operand
    M_IM = 1,    // Immediate: #nn
    M_IMb = 2,   // Bit position: #n
    M_IML = 3,   // Immediate Long: #nnnn
    M_r = 4,     // Working register: rn
    M_Ir = 5,    // Indirect Working register: @rn
    M_rr = 6,    // Working register pair: rrn
    M_Irr = 7,   // Indirect Working Register Pair: @rrn
    M_R = 8,     // Register: Rn
    M_IR = 9,    // Indirect Register: @Rn
    M_RR = 10,   // Register Pair: RRn
    M_IRR = 11,  // Indirect Register Pair: @RRn
    M_RA = 12,   // Relative Address: nnnn
    M_DA = 13,   // Direct Address: nnnn
    M_X = 14,    // Indexed: nn(rn)
    M_XS = 15,   // Indexed Short: nn(rrn)
    M_XL = 16,   // Indexed Long: nnnn(rrn)
    M_cc = 17,   // Condition Code: cc
    M_IA = 18,   // Indirect address for CALL #IA
    // Those (19-) happen only in assembler internal.
    M_W = 19,    // Register or Working register: Rxy x==RP
    M_IW = 20,   // Indirect Register or Working register: @Rxy x==RP
    M_WW = 21,   // Register or Working register pair: RRxy: x=RP
    M_IWW = 22,  // Indirect Register Pair or Working register Pair: @RRxy x==RP
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

enum OprPos : uint8_t {
    OP_NONE = 0,
    OP_CODE = 1,  // in the instruction high 4-bit
    OP_BYT1 = 2,  // in the first 8-bit operand
    OP_BYT2 = 3,  // in the second 8-bit operand
    OP_B1HI = 4,  // in the high 4-bit of the first 8-bit operand
    OP_B1LO = 5,  // in the low 4-bit of the first 8-bit operand
    OP_W1BE = 6,  // in the first 16-bit big endian operand
    OP_W2BE = 7,  // in the second 16-bit big endian operand
    OP_W2LE = 8,  // in the second 16-bit little endian operand
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint16_t _mode;
        uint16_t _attr;

        static constexpr Flags create(AddrMode dst, AddrMode src, AddrMode ext,
                PostFormat postFormat, OprPos dstPos, OprPos srcPos, OprPos extPos) {
            return Flags{mode(dst, src, ext), attr(dstPos, srcPos, extPos, postFormat)};
        }
        Flags read() const { return Flags{pgm_read_word(&_mode), pgm_read_word(&_attr)}; }

        AddrMode dst() const { return AddrMode((_mode >> dst_gp) & mode_gm); }
        AddrMode src() const { return AddrMode((_mode >> src_gp) & mode_gm); }
        AddrMode ext() const { return AddrMode((_mode >> ext_gp) & mode_gm); }
        OprPos dstPos() const { return OprPos((_attr >> dstPos_gp) & oprPos_gm); }
        OprPos srcPos() const { return OprPos((_attr >> srcPos_gp) & oprPos_gm); }
        OprPos extPos() const { return OprPos((_attr >> extPos_gp) & oprPos_gm); }
        PostFormat postFormat() const {
            return PostFormat((_attr >> postFormat_gp) & postFormat_gm);
        }
        Config::opcode_t postVal() const { return Entry::postVal(postFormat()); }
        void setSrcPos(OprPos pos) { _attr = attr(dstPos(), pos, extPos(), postFormat()); }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : Base(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    const Flags _flags;

    static Config::opcode_t postVal(PostFormat postFormat) {
        static constexpr Config::opcode_t val[] = {
                0x00,  // PF_NONE
                0x00,  // PF1_0
                0x01,  // PF1_1
                0x00,  // PF2_0
                0x01,  // PF2_1
                0x02,  // PF2_2
                0x00,  // PF4_0
                0x01,  // PF4_1
        };
        return val[postFormat];
    }

    static constexpr uint16_t mode(AddrMode dst, AddrMode src, AddrMode ext) {
        return (static_cast<uint16_t>(dst) << dst_gp) | (static_cast<uint16_t>(src) << src_gp) |
               (static_cast<uint16_t>(ext) << ext_gp);
    }

    static constexpr uint16_t attr(
            OprPos dstPos, OprPos srcPos, OprPos extPos, PostFormat postFormat) {
        return (static_cast<uint16_t>(dstPos) << dstPos_gp) |
               (static_cast<uint16_t>(srcPos) << srcPos_gp) |
               (static_cast<uint16_t>(extPos) << extPos_gp) |
               (static_cast<uint16_t>(postFormat) << postFormat_gp);
    }

    // |_mode|
    static constexpr uint8_t mode_gm = 0x1F;
    static constexpr int dst_gp = 0;
    static constexpr int src_gp = 5;
    static constexpr int ext_gp = 10;
    // |_attr|
    static constexpr uint8_t oprPos_gm = 0xF;
    static constexpr uint8_t postFormat_gm = 0x7;
    static constexpr int dstPos_gp = 0;
    static constexpr int srcPos_gp = 4;
    static constexpr int extPos_gp = 8;
    static constexpr int postFormat_gp = 12;
};

}  // namespace z8
}  // namespace libasm

#endif  // __LIBASM_ENTRY_Z8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
