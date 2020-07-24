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

enum AddrMode {
    INH     = 0,   // Inherent
    DIR     = 1,   // Direct Page
    EXT     = 2,   // Extended
    IDX     = 3,   // Indexed
    REL     = 4,   // Relative
    LREL    = 5,   // Long Relative
    IM8     = 6,   // Immediate 8-bit
    IM16    = 7,   // Immediate 16-bit
    IM32    = 8,   // Immediate 32-bit
    PSH_PUL = 9,   // Push Pull
    REG_REG = 10,  // Inter register
    // HD6309
    IM8_DIR = 11,  // Immediate and Direct Page
    IM8_EXT = 12,  // Immediate and Extended
    IM8_IDX = 13,  // Immediate and Indexed
    BITOP   = 14,  // Bit Operation
    TFR_MEM = 15,  // Transfer Memory
};

struct Entry {
    const Config::opcode_t opCode;
    const uint8_t flags;
    const char *name;

    static inline AddrMode _addrMode(uint8_t flags) {
        return AddrMode((flags >> addrMode_gp) & addrMode_gm);
    }

    static constexpr uint8_t _flags(AddrMode addrMode) {
        return (static_cast<uint8_t>(addrMode) << addrMode_gp);
    }

private:
    static constexpr uint8_t addrMode_gm = 0xf;
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
