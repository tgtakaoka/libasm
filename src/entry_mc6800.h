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

enum CpuType : uint8_t {
    MC6800,
    MC6801,
    HD6301,
    MC68HC11,
};

enum AddrMode : uint8_t {
    INH         = 0,   // Inherent
    DIR         = 1,   // Direct page
    EXT         = 2,   // Extended
    IDX         = 3,   // Indexed
    REL         = 4,   // Relative
    IMM         = 5,   // Immediate
    // HD6301
    IMM_DIR     = 6,   // Immediate,Direct Page
    IMM_IDX     = 7,   // Immediate,Indexed
    BIT_DIR     = 8,   // Bit number,Direct
    BIT_IDX     = 9,   // Bit number,Indexed
    // MC68HC11
    IDY         = 10,  // Indexed Y
    DIR_IMM     = 11,  // Direct page,Immediate
    IDX_IMM     = 12,  // Indexed X,Immediate
    IDY_IMM     = 13,  // Indexed Y,Immediate
    DIR_IMM_REL = 14,  // Direct page,Immediate,Relative
    IDX_IMM_REL = 15,  // Indexed X,Immediate,Relative
    IDY_IMM_REL = 16,  // Indexed Y,Immediate,Relative
};

enum InsnAdjust : uint8_t {
    ADJ_ZERO = 0,
    ADJ_AB01 = 1,  // Accumulator A:+0, B:+1
    ADJ_AB16 = 2,  // Accumulator A:+0, B:+$10
    ADJ_AB64 = 3,  // Accumulator A:+0, B:+$40
};

enum OprSize : uint8_t {
    SZ_BYTE = 0,
    SZ_WORD = 1,
    SZ_NONE = 2,   // unknown, in Table == SZ_BYTE
};

struct Entry {
    const Config::opcode_t opCode;
    const uint8_t flags;
    const char *name;

    static inline AddrMode _addrMode(uint8_t flags) {
        return AddrMode(flags & addrMode_gm);
    }
    static inline InsnAdjust _insnAdjust(uint8_t flags) {
        return InsnAdjust((flags >> insnAdjust_gp) & insnAdjust_gm);
    }
    static inline OprSize _oprSize(uint8_t flags) {
        return OprSize((flags >> oprSize_gp) & oprSize_gm);
    }
    static constexpr uint8_t _flags(
        AddrMode addrMode, InsnAdjust insnAdjust, OprSize oprSize) {
        return static_cast<uint8_t>(addrMode)
            | (static_cast<uint8_t>(insnAdjust) << insnAdjust_gp)
            | (static_cast<uint8_t>(oprSize)    << oprSize_gp);
    }

private:
    static constexpr uint8_t addrMode_gm = 0x1F;
    static constexpr uint8_t insnAdjust_gm = 0x3;
    static constexpr uint8_t oprSize_gm = 0x1;
    static constexpr int insnAdjust_gp = 5;
    static constexpr int oprSize_gp = 7;
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
