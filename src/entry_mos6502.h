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

#ifndef __ENTRY_MOS6502_H__
#define __ENTRY_MOS6502_H__

#include "config_mos6502.h"

namespace libasm {
namespace mos6502 {

enum CpuType : host::uint_t {
    MOS6502,
    W65SC02,
    R65C02,
    W65C02S,
};

enum AddrMode : host::uint_t {
    // MOS6502
    IMPL,                // Implied
    ACCM,                // Accumulator A
    IMMA,                // Immediate Accumulator: #nn
    IMMX,                // Immediate Index register: #nn
    ABS,                 // Absolute: abs
    ZPG,                 // Zero Page: zp
    ZPG_IDX,             // Zero Page Indexed: zp,X
    ZPG_IDY,             // Zero Page Indexed: zp,Y
    ABS_IDX,             // Absolute Indexed: abs,X
    ABS_IDY,             // Absolute Indexed: abs,Y
    REL,                 // Relative: abs
    ZPG_IDX_IDIR,        // Indexed Indirect: (zp,X)
    ZPG_IDIR_IDY,        // Indirect Indexed: (zp),Y
    ABS_IDIR,            // Absolute Indirect: (abs)

    // W65SC02
    ABS_IDX_IDIR,        // Indexed Absolute Indirect: (abs,X)
    ZPG_IDIR,            // Zero Page Indirect: (zp)
    ZPG_REL,             // Zero Page Relative: zp,abs
};

struct Entry {
    const Config::opcode_t opCode;
    const host::uint_t flags;
    const char *name;

    static inline CpuType _cpuType(host::uint_t flags) {
        return CpuType((flags >> cputype_gp) & cputype_gm);
    }
    static inline AddrMode _addrMode(host::uint_t flags) {
        return AddrMode(flags & addrMode_gm);
    }
    static constexpr host::uint_t _flags(CpuType cpuType, AddrMode addrMode) {
        return (host::uint_t(cpuType) << cputype_gp)
            | host::uint_t(addrMode);
    }

private:
    static constexpr host::uint_t cputype_gp = 5;
    static constexpr host::uint_t cputype_gm = 0x7;
    static constexpr host::uint_t addrMode_gm = 0x1f;
};

} // namespace mos6502
} // namespace libasm

#endif // __ENTRY_MOS6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
