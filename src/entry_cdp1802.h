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

enum AddrMode : uint8_t {
    NONE = 0,
    REGN = 1,  // Register number, 0~15
    REG1 = 2,  // Register number, 1~15
    IMM8 = 3,  // Immediate, 0~15
    PAGE = 4,  // Page offset
    ADDR = 5,  // Absolute address
    IOAD = 6,  // IO address, 1~7
    UNDF = 7,  // Undefined instruction
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint8_t _attr;

        static constexpr Flags create(AddrMode mode) {
            return Flags{static_cast<uint8_t>(mode)};
        }
        Flags read() const { return Flags{pgm_read_byte(&_attr)}; }

        AddrMode mode() const { return AddrMode(_attr); }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;
};

} // namespace cdp1802
} // namespace libasm

#endif // __ENTRY_CDP1802_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
