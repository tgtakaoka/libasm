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

enum AddrMode : host::uint_t {
    REGN,  // Register number, 0~15
    REG1,  // Register number, 1~15
    IMPL,  // Implied
    IMM8,  // Immediate, 0~15
    PAGE,  // Page offset
    ADDR,  // Absolute address
    IOAD,  // IO address, 1~7
    UNDF,  // Undefined instruction
};

struct Entry {
    const Config::opcode_t opCode;
    const host::uint_t flags;
    const char *name;

    static inline AddrMode _addrMode(host::uint_t flags) {
        return AddrMode(flags & addrMode_gm);
    }
    static constexpr host::uint_t _flags(AddrMode addrMode) {
        return host::uint_t(addrMode);
    }

private:
    static constexpr host::uint_t addrMode_gm = 0x07;
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