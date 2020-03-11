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

#ifndef __ENTRY_MC68000_H__
#define __ENTRY_MC68000_H__

#include "config_mc68000.h"

struct Entry {
    const target::insn_t insnCode;
    const host::uint_t flags;
    const char *name;
    static constexpr host::uint_t code_max = 10;
    static constexpr host::uint_t name_max = 7;

    static inline InsnFormat _insnFormat(host::uint_t flags) {
        return InsnFormat((flags >> insnFmt_gp) & insnFmt_gm);
    }
    static constexpr host::uint_t _flags(InsnFormat iformat) {
        return (host::uint_t(iformat) << insnFmt_gp);
    }

private:
    static constexpr host::uint_t insnFmt_gp = 0;
    static constexpr host::uint_t insnFmt_gm = 0x1f;
};

#endif // __ENTRY_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
