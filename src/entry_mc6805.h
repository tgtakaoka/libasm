/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __ENTRY_MC6805_H__
#define __ENTRY_MC6805_H__

#include <stdint.h>

#include "config_mc6805.h"
#include "entry_base.h"

namespace libasm {
namespace mc6805 {

enum CpuType : uint8_t {
    MC6805,
    MC146805,
    MC68HC05,
};

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_DIR = 1,  // Direct page
    M_EXT = 2,  // Extended
    M_IDX = 3,  // Indexed X with 8-bit offset
    M_REL = 4,  // Relative
    M_IMM = 5,  // Immediate
    M_IX0 = 6,  // Indexed X with no offset
    M_IX2 = 7,  // Indexed X with 16-bit offset
    M_BNO = 8,  // Bit number in opcode
    M_GEN = 9,  // Generic: M_IMM/M_DIR/M_EXT/M_IDX/M_IX2/M_IX0
    M_MEM = 10, // Generic memory, M_DIR/M_IDX/M_IX0
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint16_t _attr;

        static constexpr Flags create(
                AddrMode op1, AddrMode op2, AddrMode op3, bool undef = false) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(op1) << op1_gp) |
                                               (static_cast<uint16_t>(op2) << op2_gp) |
                                               (static_cast<uint16_t>(op3) << op3_gp) |
                                               (static_cast<uint16_t>(undef ? 1 : 0) << undef_bp))};
        }
        Flags read() const { return Flags{pgm_read_word(&_attr)}; }

        AddrMode mode1() const { return AddrMode((_attr >> op1_gp) & mode_gm); }
        AddrMode mode2() const { return AddrMode((_attr >> op2_gp) & mode_gm); }
        AddrMode mode3() const { return AddrMode((_attr >> op3_gp) & mode_gm); }
        bool undefined() const { return (_attr & (1 << undef_bp)) != 0; }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    const Flags _flags;

    static constexpr int op1_gp = 0;
    static constexpr int op2_gp = 4;
    static constexpr int op3_gp = 8;
    static constexpr uint8_t mode_gm = 0xF;
    static constexpr int undef_bp = 15;
};

}  // namespace mc6805
}  // namespace libasm

#endif  // __ENTRY_MC6805_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
