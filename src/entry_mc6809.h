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

enum OprSize : host::uint_t {
    SZ_NONE = 0,
    SZ_BYTE = 1,                // 8 bit operation
    SZ_WORD = 2,                // 16 bit operation
    SZ_LONG = 3,                // 32 bit operation
};

enum AddrMode : host::uint_t {
    INHR,
    DIRP,
    EXTD,
    INDX,
    REL,
    IMM,
    STKOP,
    REGS,
    // HD6309
    IMMDIR,
    IMMEXT,
    IMMIDX,
    BITOP,
    TFRM,
    // Pseudo instruction
    PSEUDO,
};

struct Entry {
    const Config::opcode_t opCode;
    const host::uint_t flags;
    const char *name;

    static inline OprSize _oprSize(host::uint_t flags) {
        return OprSize((flags >> oprSize_gp) & oprSize_gm);
    }

    static inline AddrMode _addrMode(host::uint_t flags) {
        return AddrMode((flags >> addrMode_gp) & addrMode_gm);
    }

    static constexpr host::uint_t _flags(OprSize oprSize, AddrMode addrMode) {
        return (host::uint_t(oprSize) << oprSize_gp)
            | (host::uint_t(addrMode) << addrMode_gp);
    }

private:
    static constexpr host::uint_t oprSize_gp = 4;
    static constexpr host::uint_t oprSize_gm = 0x03;
    static constexpr host::uint_t addrMode_gp = 0;
    static constexpr host::uint_t addrMode_gm = 0x0f;
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
