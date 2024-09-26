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

#ifndef __LIBASM_ENTRY_INS8060_H__
#define __LIBASM_ENTRY_INS8060_H__

#include <stdint.h>

#include "config_ins8060.h"
#include "entry_base.h"

namespace libasm {
namespace ins8060 {

enum AddrMode : uint8_t {
    M_NONE = 0,  // Inherent
    M_PNTR = 1,  // Pointer, Pn
    M_IMM8 = 2,  // Immediate, nn
    M_REL8 = 3,  // PC Relative, label
    M_DISP = 4,  // Displacement, M_REL8 + dd(Pn), E(Pn)
    M_INDX = 5,  // Indexed, M_DISP + @dd(Pn), @E(Pn)
    M_UNDEF = 6,
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint8_t _attr;

        static constexpr Flags create(AddrMode mode) {
            return Flags{static_cast<uint8_t>(static_cast<uint8_t>(mode) << mode_gp)};
        }

        AddrMode mode() const { return AddrMode(_attr); }

    private:
        static constexpr int mode_gp = 0;
        static constexpr uint8_t mode_gm = 0x07;
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const /* PROGMEM */ char *name_P)
        : Base(name_P, opCode), _flags_P(flags) {}

    Flags readFlags() const { return Flags{pgm_read_byte(&_flags_P._attr)}; }

private:
    const Flags _flags_P;
};

}  // namespace ins8060
}  // namespace libasm

#endif  // __LIBASM_ENTRY_INS8060_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
