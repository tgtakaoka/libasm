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

struct Entry {
    const target::opcode_t opc;
    const host::uint_t flags;
    const char *name;
    static constexpr host::uint_t code_max = 3;
    static constexpr host::uint_t name_max = 3;

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
private:
    static constexpr host::uint_t addrMode_gm = 0x7;
    static constexpr host::uint_t insnAdjust_gp = 3;
    static constexpr host::uint_t insnAdjust_gm = 0x3;
    static constexpr host::uint_t oprSize_gp = 5;
    static constexpr host::uint_t oprSize_gm = 0x3;

};

} // namespace m6502
} // namespace libasm

#endif // __ENTRY_MC6800_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
