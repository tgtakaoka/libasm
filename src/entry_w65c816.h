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

#ifndef __ENTRY_W65C816_H__
#define __ENTRY_W65C816_H__

#include "config_mos6502.h"
#include "config_w65c816.h"
#include "entry_mos6502.h"

namespace libasm {
namespace w65c816 {

enum CpuType {
    MOS6502 = mos6502::CpuType::MOS6502,
    W65SC02 = mos6502::CpuType::W65SC02,
    R65C02  = mos6502::CpuType::R65C02,
    W65C02S = mos6502::CpuType::W65C02S,
    W65C816,
};

enum AddrMode {
    // MOS6502
    IMPL         = mos6502::AddrMode::IMPL,
    ACCM         = mos6502::AddrMode::ACCM,
    IMMA         = mos6502::AddrMode::IMMA,
    IMMX         = mos6502::AddrMode::IMMX,
    ABS          = mos6502::AddrMode::ABS,
    ZPG          = mos6502::AddrMode::ZPG,
    ZPG_IDX      = mos6502::AddrMode::ZPG_IDX,
    ZPG_IDY      = mos6502::AddrMode::ZPG_IDY,
    ABS_IDX      = mos6502::AddrMode::ABS_IDX,
    ABS_IDY      = mos6502::AddrMode::ABS_IDY,
    REL          = mos6502::AddrMode::REL,
    ZPG_IDX_IDIR = mos6502::AddrMode::ZPG_IDX_IDIR,
    ZPG_IDIR_IDY = mos6502::AddrMode::ZPG_IDIR_IDY,
    ABS_IDIR     = mos6502::AddrMode::ABS_IDIR,

    // W65SC02
    ABS_IDX_IDIR = mos6502::AddrMode::ABS_IDX_IDIR,
    ZPG_IDIR     = mos6502::AddrMode::ZPG_IDIR,
    ZPG_REL      = mos6502::AddrMode::ZPG_REL,

    // W65C816
    ABS_LONG          = 17,  // Absolute Long: al
    ABS_LONG_IDX      = 18,  // Absolute Long Indexed: al,x
    ABS_IDIR_LONG     = 19,  // Absolute Indirect Long: [a]
    REL_LONG          = 20,  // Relative Long: rl
    SP_REL            = 21,  // Stack Relative: off,s
    SP_REL_IDIR_IDY   = 22,  // Stack Relative Indirect Indexed: (off,s),y
    ZPG_IDIR_LONG     = 23,  // Zero Page Indirect Long: [zp]
    ZPG_IDIR_LONG_IDY = 24,  // Zero Page Indirect Long Indexed: [zp],y
    BLOCK_MOVE        = 25,  // Block Move: #ss,#dd
    IMM8              = 26,  // Immediate Byte.
};

struct Entry {
    const Config::opcode_t opCode;
    const uint8_t flags;
    const char *name;

    static inline CpuType _cpuType(uint8_t flags) {
        return CpuType((flags >> cputype_gp) & cputype_gm);
    }
    static inline AddrMode _addrMode(uint8_t flags) {
        return AddrMode(flags & addrMode_gm);
    }
    static constexpr uint8_t _flags(CpuType cpuType, AddrMode addrMode) {
        return (static_cast<uint8_t>(cpuType) << cputype_gp)
            | static_cast<uint8_t>(addrMode);
    }
    static uint8_t _set(uint8_t flags, AddrMode addrMode) {
        return (flags & ~addrMode_gm) | static_cast<uint8_t>(addrMode);
    }

private:
    static constexpr uint8_t cputype_gm = 0x7;
    static constexpr uint8_t addrMode_gm = 0x1f;
    static constexpr uint8_t cputype_gp = 5;
};

} // namespace w65c816
} // namespace libasm

#endif // __ENTRY_W65C816_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
