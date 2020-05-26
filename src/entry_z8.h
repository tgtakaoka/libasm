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

enum AddrMode : host::uint_t {
    M_NO,   // No operand
    M_R,    // Register:                  Rn
    M_IR,   // Indirect Register:         @Rn
    M_r,    // Working register:          rn
    M_Ir,   // Indirect Working register: @rn
    M_IRR,  // Indirect Register Pair:    @RRn
    M_Irr,  // Indirect Working Register Pair: @rrn
    M_IM,   // Immediate:                 #nn
    M_X,    // Indexed:                   nn(rn)
    M_DA,   // Direct Address:            nnnn
    M_RA,   // Relative Address:          nnnn
    M_cc,   // Condition Code:            cc
};

struct Entry {
    const Config::opcode_t opCode;
    const host::uint_t flags;
    const char *name;

    static inline AddrMode _dstMode(host::uint_t flags) {
        return AddrMode((flags >> dstMode_gp) & addrMode_gm);
    }
    static inline AddrMode _srcMode(host::uint_t flags) {
        return AddrMode((flags >> srcMode_gp) & addrMode_gm);
    }
    static constexpr host::uint_t _flags(AddrMode dst, AddrMode src) {
        return (host::uint_t(dst) << dstMode_gp)
            | (host::uint_t(src) << srcMode_gp);
    }

private:
    static constexpr host::uint_t addrMode_gm = 0x0f;
    static constexpr host::uint_t dstMode_gp = 0;
    static constexpr host::uint_t srcMode_gp = 4;
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
