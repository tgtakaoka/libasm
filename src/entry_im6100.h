/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef __LIBASM_ENTRY_IM6100_H__
#define __LIBASM_ENTRY_IM6100_H__

#include <stdint.h>

#include "config_im6100.h"
#include "entry_base.h"

namespace libasm {
namespace im6100 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_GR1 = 1,  // Operation Group 1: |111|0__|___|___|
    M_GR2 = 2,  // Operation Group 2: |111|1__|___|__0|
    M_GR3 = 3,  // Operation Group 3: |111|1__|___|__1|
    M_MEM = 4,  // Memory reference:  |opc|IM| offset |
    M_CMP = 5,  // Composed
    M_IOT = 6,  // IOT instruction:   |110| device|ctl|
    M_MEX = 7,  // Memory extenstion: |110|010|fld|0__|
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint8_t _attr;
        uint8_t _selector;
        uint16_t _bits;

        static constexpr Flags create(AddrMode mode, uint16_t bits = 0) {
            return Flags{attr(mode, false, false), 0, bits};
        }

        static constexpr Flags create(
                AddrMode mode, uint16_t bits, uint8_t selector, bool multiGroup = false) {
            return Flags{attr(mode, true, multiGroup), selector, bits};
        }

        Flags read() const {
            return Flags{pgm_read_byte(&_attr), pgm_read_byte(&_selector), pgm_read_word(&_bits)};
        }
        AddrMode mode() const { return AddrMode((_attr >> mode_gp) & mode_gm); }
        uint16_t bits() const { return _bits; }
        bool combination() const { return (_attr & (1 << combination_bp)) != 0; }
        bool multiGroup() const { return (_attr & (1 << multiGroup_bp)) != 0; }
        uint8_t selector() const { return _selector; }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : Base(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr uint8_t attr(AddrMode mode, bool combination, bool multiGroup) {
        return ((static_cast<uint8_t>(mode) & mode_gm) << mode_gp) |
               (multiGroup ? (1 << multiGroup_bp) : 0) | (combination ? (1 << combination_bp) : 0);
    }

    static constexpr int mode_gp = 0;
    static constexpr int multiGroup_bp = 6;
    static constexpr int combination_bp = 7;
    static constexpr uint8_t mode_gm = 0x07;
};

}  // namespace im6100
}  // namespace libasm

#endif  // __ENTRY_IM6100_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
