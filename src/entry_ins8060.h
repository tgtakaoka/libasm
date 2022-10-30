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

#include <stdint.h>

#include "config_ins8060.h"
#include "entry_base.h"

namespace libasm {
namespace ins8060 {

enum AddrMode : uint8_t {
    M_NONE = 1,  // Inherent
    M_PNTR = 2,  // Pointer, Pn
    M_IMM8 = 3,  // Immediate, nn
    M_REL8 = 4,  // PC Relative, label
    M_DISP = 5,  // Displacement, M_REL8 + dd(Pn), E(Pn)
    M_INDX = 6,  // Indexed, M_DISP + @dd(Pn), @E(Pn)
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint8_t _attr;

        static constexpr Flags create(AddrMode mode) {
            return Flags{static_cast<uint8_t>(static_cast<uint8_t>(mode) << mode_gp)};
        }

        static constexpr Flags undef() {
            return Flags{
                    static_cast<uint8_t>((static_cast<uint8_t>(M_NONE) << mode_gp) | undef_bm)};
        }

        Flags read() const { return Flags{pgm_read_byte(&_attr)}; }
        AddrMode mode() const { return AddrMode(_attr); }
        bool undefined() const { return _attr & undef_bm; }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr int mode_gp = 0;
    static constexpr int undef_bp = 7;
    static constexpr uint8_t mode_gm = 0x07;
    static constexpr uint8_t undef_bm = (1 << undef_bp);
};

}  // namespace ins8060
}  // namespace libasm

#endif  // __ENTRY_INS8060_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
