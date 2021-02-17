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

#ifndef __ENTRY_CDP1802_H__
#define __ENTRY_CDP1802_H__

#include "config_cdp1802.h"
#include "entry_base.h"

namespace libasm {
namespace cdp1802 {

enum CpuType : uint8_t {
    CDP1802,
    CDP1804,
    CDP1804A,
};

enum AddrMode : uint8_t {
    NONE = 0,
    REGN = 1,  // Register number, 0~15
    REG1 = 2,  // Register number, 1~15
    IMM8 = 3,  // Immediate, 8 bit
    PAGE = 4,  // Page offset, 8 bit
    ADDR = 5,  // Absolute address, 16 bit
    IOAD = 6,  // IO address, 1~7
    UNDF = 7,  // Undefined instruction
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint8_t _attr;

        static constexpr Flags create(AddrMode op1, AddrMode op2) {
            return Flags{static_cast<uint8_t>(
                    (static_cast<uint8_t>(op1) << op1_gp) |
                    (static_cast<uint8_t>(op2) << op2_gp))};
        }
        Flags read() const { return Flags{pgm_read_byte(&_attr)}; }

        AddrMode mode1() const { return AddrMode((_attr >> op1_gp) & mode_gm); }
        AddrMode mode2() const { return AddrMode((_attr >> op2_gp) & mode_gm); }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr int op1_gp = 0;
    static constexpr int op2_gp = 4;
    static constexpr uint8_t mode_gm = 0x0F;
};

}  // namespace cdp1802
}  // namespace libasm

#endif  // __ENTRY_CDP1802_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
