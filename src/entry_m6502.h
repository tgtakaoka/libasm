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

#ifndef __ENTRY_M6502_H__
#define __ENTRY_M6502_H__

#include "config_m6502.h"

struct Entry {
    const target::insn_t insnCode;
    const host::uint_t flags;
    const char *name;
    static constexpr host::uint_t code_max = 4;
    static constexpr host::uint_t name_max = 4;

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

#endif // __ENTRY_M6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
