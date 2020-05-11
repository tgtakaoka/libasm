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

#ifndef __ENTRY_INS8060_H__
#define __ENTRY_INS8060_H__

#include "config_ins8060.h"

namespace libasm {
namespace ins8060 {

enum AddrMode : host::uint_t {
    INHR,  // Inherent
    PNTR,  // Pointer, Pn
    IMM8,  // Immediate, nn
    REL8,  // PC Relative, label
    DISP,  // Displacement, REL8 + dd(Pn), E(Pn)
    INDX,  // Indexed, DISP + @dd(Pn), @E(Pn)
    UNDEF, // Undefined instruction
};

struct Entry {
    const Config::opcode_t opCode;
    const host::uint_t flags;
    const char *name;

    static inline AddrMode _addrMode(host::uint_t flags) {
        return AddrMode(flags & addrMode_gm);
    }
    static constexpr host::uint_t _flags(AddrMode addrMode) {
        return host::uint_t(addrMode);
    }

private:
    static constexpr host::uint_t addrMode_gm = 0x07;
};

} // namespace ins8060
} // namespace libasm

#endif // __ENTRY_INS8060_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
