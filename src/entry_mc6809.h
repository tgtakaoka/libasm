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

#ifndef __ENTRY_MC6809_H__
#define __ENTRY_MC6809_H__

#include "config_mc6809.h"

namespace libasm {
namespace mc6809 {

enum CpuType {
    MC6809,
    HD6309,
};

enum OprSize {
    SZ_NONE = 0,
    SZ_BYTE = 1, // 8 bit operation
    SZ_WORD = 2, // 16 bit operation
    SZ_LONG = 3, // 32 bit operation
};

enum AddrMode {
    INH     = 0,   // Inherent
    DIR     = 1,   // Direct Page
    EXT     = 2,   // Extended
    IDX     = 3,   // Indexed
    REL     = 4,   // Relative
    IMM     = 5,   // Immediate
    PSH_PUL = 6,   // Push Pull
    REG_REG = 7,   // Inter register
    // HD6309
    IMM_DIR = 8,   // Immediate and Direct Page
    IMM_EXT = 9,   // Immediate and Extended
    IMM_IDX = 10,  // Immediate and Indexed
    BITOP   = 11,  // Bit Operation
    TFR_MEM = 12,  // Transfer Memory
};

struct Entry {
    const Config::opcode_t opCode;
    const uint8_t flags;
    const char *name;

    static inline OprSize _oprSize(uint8_t flags) {
        return OprSize((flags >> oprSize_gp) & oprSize_gm);
    }

    static inline AddrMode _addrMode(uint8_t flags) {
        return AddrMode((flags >> addrMode_gp) & addrMode_gm);
    }

    static constexpr uint8_t _flags(OprSize oprSize, AddrMode addrMode) {
        return (static_cast<uint8_t>(oprSize) << oprSize_gp)
            | (static_cast<uint8_t>(addrMode) << addrMode_gp);
    }

    static uint8_t _set(uint8_t flags, AddrMode addrMode) {
        return (flags & ~addrMode_gm) | static_cast<uint8_t>(addrMode);
    }

private:
    static constexpr uint8_t oprSize_gm = 0x3;
    static constexpr uint8_t addrMode_gm = 0xf;
    static constexpr int oprSize_gp = 4;
    static constexpr int addrMode_gp = 0;
};

} // namespace mc6809
} // namespace libasm

#endif // __ENTRY_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
