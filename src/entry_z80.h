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

#ifndef __ENTRY_Z80_H__
#define __ENTRY_Z80_H__

#include "config_z80.h"

struct Entry {
    const target::opcode_t opc;
    const host::uint_t flags1;
    const host::uint_t flags2;
    const char *name;
    static constexpr host::uint_t code_max = 4;
    static constexpr host::uint_t name_max = 4;

    static inline InsnFormat _insnFormat(host::uint_t flags1) {
        return InsnFormat(flags1 & insnFormat_mask);
    }
    static inline OprFormat _oprFormat(host::uint_t flags) {
        return OprFormat((flags >> oprFormat_shift) & oprFormat_mask);
    }
    static inline AddrMode _addrMode(host::uint_t flags2) {
        return AddrMode(flags2 & addrMode_mask);
    }
    static constexpr host::uint_t _flags1(
        OprFormat leftOpr, InsnFormat iformat) {
        return (host::uint_t(leftOpr) << oprFormat_shift)
            | host::uint_t(iformat);
    }
    static constexpr host::uint_t _flags2(
        OprFormat rightOpr, AddrMode addrMode) {
        return (host::uint_t(rightOpr) << oprFormat_shift)
            | host::uint_t(addrMode);
    }

private:
    static constexpr host::uint_t insnFormat_mask = 0x7;
    static constexpr host::uint_t oprFormat_shift = 3;
    static constexpr host::uint_t oprFormat_mask = 0x1f;
    static constexpr host::uint_t addrMode_mask = 0x7;
};

#endif // __ENTRY_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
