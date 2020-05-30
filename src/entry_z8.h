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

enum AddrMode {
    M_NO  = 0,   // No operand
    M_R   = 1,   // Register: Rn
    M_IR  = 2,   // Indirect Register: @Rn
    M_r   = 3,   // Working register: rn
    M_Ir  = 4,   // Indirect Working register: @rn
    M_IRR = 5,   // Indirect Register Pair: @RRn
    M_Irr = 6,   // Indirect Working Register Pair: @rrn
    M_IM  = 7,   // Immediate: #nn
    M_X   = 8,   // Indexed: nn(rn)
    M_DA  = 9,   // Direct Address: nnnn
    M_RA  = 10,  // Relative Address: nnnn
    M_cc  = 11,  // Condition Code: cc
    M_Rr  = 12,  // Register or Working register: xy x=rp
};

struct Entry {
    const Config::opcode_t opCode;
    const uint8_t flags;
    const char *name;

    static inline AddrMode _dstMode(uint8_t flags) {
        return AddrMode((flags >> dstMode_gp) & addrMode_gm);
    }
    static inline AddrMode _srcMode(uint8_t flags) {
        return AddrMode((flags >> srcMode_gp) & addrMode_gm);
    }
    static constexpr uint8_t _flags(AddrMode dst, AddrMode src) {
        return (static_cast<uint8_t>(dst) << dstMode_gp)
            | (static_cast<uint8_t>(src) << srcMode_gp);
    }

private:
    static constexpr uint8_t addrMode_gm = 0xf;
    static constexpr uint8_t dstMode_gp = 0;
    static constexpr uint8_t srcMode_gp = 4;
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
