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

#ifndef __ENTRY_I8051_H__
#define __ENTRY_I8051_H__

#include <stdint.h>

#include "config_i8051.h"
#include "entry_base.h"

namespace libasm {
namespace i8051 {

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_REL = 1,     // Relative
    M_AREG = 2,    // A register
    M_RREG = 3,    // Rn register
    M_CREG = 4,    // C register
    M_DREG = 5,    // DPTR register
    M_ABREG = 6,   // AB register
    M_ADR8 = 7,    // Internal 8-bit RAM address
    M_ADR11 = 8,   // 11-bit program code address
    M_ADR16 = 9,   // 16-bot program code address
    M_BITAD = 10,  // 8-bit bit address
    M_NOTAD = 11,  // not(/) 8-bit address
    M_IMM8 = 12,   // 8-bit Immediate data
    M_IMM16 = 13,  // 16-bit Immediate data
    M_IDIRR = 14,  // @Rn indirect
    M_IDIRD = 15,  // @DPTR indirect
    M_INDXD = 16,  // @A+DPTR
    M_INDXP = 17,  // @A+PC
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint16_t _attr;

        static constexpr Flags create(AddrMode dst, AddrMode src, AddrMode ext) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(dst) << dstMode_gp) |
                                               (static_cast<uint16_t>(src) << srcMode_gp) |
                                               (static_cast<uint16_t>(ext) << extMode_gp))};
        }
        Flags read() const { return Flags{pgm_read_word(&_attr)}; }

        AddrMode dstMode() const { return AddrMode((_attr >> dstMode_gp) & addrMode_gm); }
        AddrMode srcMode() const { return AddrMode((_attr >> srcMode_gp) & addrMode_gm); }
        AddrMode extMode() const { return AddrMode((_attr >> extMode_gp) & addrMode_gm); }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr uint8_t addrMode_gm = 0x1f;
    static constexpr uint8_t dstMode_gp = 0;
    static constexpr uint8_t srcMode_gp = 5;
    static constexpr uint8_t extMode_gp = 10;
};

}  // namespace i8051
}  // namespace libasm

#endif  // __ENTRY_I8051_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
