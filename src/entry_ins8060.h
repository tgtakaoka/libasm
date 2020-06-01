
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

enum AddrMode {
    INHR  = 1,  // Inherent
    PNTR  = 2,  // Pointer, Pn
    IMM8  = 3,  // Immediate, nn
    REL8  = 4,  // PC Relative, label
    DISP  = 5,  // Displacement, REL8 + dd(Pn), E(Pn)
    INDX  = 6,  // Indexed, DISP + @dd(Pn), @E(Pn)
    UNDEF = 0,  // Undefined instruction
};

struct Entry {
    const Config::opcode_t opCode;
    const uint8_t flags;
    const char *name;

    static inline AddrMode _addrMode(uint8_t flags) {
        return AddrMode(flags & addrMode_gm);
    }
    static constexpr uint8_t _flags(AddrMode addrMode) {
        return static_cast<uint8_t>(addrMode);
    }

private:
    static constexpr uint8_t addrMode_gm = 0x07;
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
