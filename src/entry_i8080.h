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

#ifndef __ENTRY_I8080_H__
#define __ENTRY_I8080_H__

#include "config_i8080.h"

namespace libasm {
namespace i8080 {

struct Entry {
    const target::insn_t insnCode;
    const host::uint_t flags;
    static constexpr host::uint_t code_max = 3;
    static constexpr host::uint_t name_max = 4;
    const char *name;

    static inline InsnFormat _insnFormat(host::uint_t flags) {
        return InsnFormat((flags >> insnFormat_shift) & insnFormat_mask);
    }
    static inline AddrMode _addrMode(host::uint_t flags) {
        return AddrMode(flags & addrMode_mask);
    }
    static constexpr host::uint_t _flags(AddrMode addrMode, InsnFormat iformat) {
        return (host::uint_t(iformat) << insnFormat_shift) | host::uint_t(addrMode);
    }

private:
    static constexpr host::uint_t insnFormat_shift = 4;
    static constexpr host::uint_t insnFormat_mask = 0x7;
    static constexpr host::uint_t addrMode_mask = 0x07;
};

} // namespace i8080
} // namespace libasm

#endif // __ENTRY_I8080_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
