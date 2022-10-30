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

#include <stdint.h>

#include "config_mc6800.h"
#include "entry_base.h"

namespace libasm {
namespace mc6800 {

enum CpuType : uint8_t {
    MC6800,
    MB8861,
    MC6801,
    HD6301,
    MC68HC11,
};

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_DIR = 1,   // Direct page
    M_EXT = 2,   // Extended
    M_IDX = 3,   // Indexed
    M_REL = 4,   // Relative
    M_IM8 = 5,   // 8-bit Immediate
    M_IM16 = 6,  // 16-bit Immediate
    M_GN8 = 7,   // Generic, M_IM8/M_DIR/M_IDX/M_EXT
    M_GN16 = 8,  // Generic, M_IM16/M_DIR/M_IDX/M_EXT
    M_GMEM = 9,  // Generic memory, M_IDX/M_EXT
    // HD6301
    M_BMM = 10,  // Bit number or Immediate (for disassembler)
    M_BIT = 11,  // Bit number (for assembler)
    // MC68HC11
    M_IDY = 12,  // Indexed Y
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
};  // namespace mc6800

}  // namespace mc6800
}  // namespace libasm

#endif  // __ENTRY_MC6800_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
