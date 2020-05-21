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

#ifndef __ENTRY_MC6800_H__
#define __ENTRY_MC6800_H__

#include "config_mc6800.h"

namespace libasm {
namespace mc6800 {

enum CpuType : host::uint_t {
    MC6800,
    MC6801,
    HD6301,
    MC68HC11,
};

enum AddrMode : host::uint_t {
    INH,  // Inherent
    DIR,  // Direct page
    EXT,  // Extended
    IDX,  // Indexed
    REL,  // Relative
    IMM,  // Immediate
    // HD6301
    IMM_DIR, // Immediate,Direct Page
    IMM_IDX, // Immediate,Indexed
    BIT_DIR, // Bit number,Direct
    BIT_IDX, // Bit number,Indexed
    // MC68HC11
    IDY,         // Indexed Y
    DIR_IMM,     // Direct page,Immediate
    IDX_IMM,     // Indexed X,Immediate
    IDY_IMM,     // Indexed Y,Immediate
    DIR_IMM_REL, // Direct page,Immediate,Relative
    IDX_IMM_REL, // Indexed X,Immediate,Relative
    IDY_IMM_REL, // Indexed Y,Immediate,Relative
};

enum InsnAdjust : host::uint_t {
    ADJ_ZERO = 0,
    ADJ_AB01 = 1,  // Accumulator A:+0, B:+1
    ADJ_AB16 = 2,  // Accumulator A:+0, B:+$10
    ADJ_AB64 = 3,  // Accumulator A:+0, B:+$40
};

enum OprSize : host::uint_t {
    SZ_BYTE = 0,
    SZ_WORD = 1,
    SZ_NONE = 2,   // unknown, in Table ==SZ_BYTE
};

struct Entry {
    const Config::opcode_t opCode;
    const host::uint_t flags;
    const char *name;

    static inline AddrMode _addrMode(host::uint_t flags) {
        return AddrMode(flags & addrMode_gm);
    }

    static inline InsnAdjust _insnAdjust(host::uint_t flags) {
        return InsnAdjust((flags >> insnAdjust_gp) & insnAdjust_gm);
    }

    static inline OprSize _oprSize(host::uint_t flags) {
        return OprSize((flags >> oprSize_gp) & oprSize_gm);
    }

    static constexpr host::uint_t _flags(
        AddrMode addrMode, InsnAdjust insnAdjust, OprSize oprSize) {
        return host::uint_t(addrMode)
            | (host::uint_t(insnAdjust) << insnAdjust_gp)
            | (host::uint_t(oprSize)   << oprSize_gp);
    }

    static host::uint_t _set(host::uint_t flags, AddrMode addrMode) {
        return (flags & ~addrMode_gm) | host::uint_t(addrMode);
    }

private:
    static constexpr host::uint_t addrMode_gm = 0x1F;
    static constexpr host::uint_t insnAdjust_gp = 5;
    static constexpr host::uint_t insnAdjust_gm = 0x3;
    static constexpr host::uint_t oprSize_gp = 7;
    static constexpr host::uint_t oprSize_gm = 0x1;

};

} // namespace mc6800
} // namespace libasm

#endif // __ENTRY_MC6800_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
