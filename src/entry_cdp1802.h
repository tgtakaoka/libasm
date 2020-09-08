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

namespace libasm {
namespace cdp1802 {

enum AddrMode : uint8_t {
    REGN = 1,  // Register number, 0~15
    REG1 = 2,  // Register number, 1~15
    IMPL = 3,  // Implied
    IMM8 = 4,  // Immediate, 0~15
    PAGE = 5,  // Page offset
    ADDR = 6,  // Absolute address
    IOAD = 7,  // IO address, 1~7
    UNDF = 0,  // Undefined instruction
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

} // namespace cdp1802
} // namespace libasm

#endif // __ENTRY_CDP1802_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
