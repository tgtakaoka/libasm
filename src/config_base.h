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

#ifndef __CONFIG_BASE_H__
#define __CONFIG_BASE_H__

#include "config_host.h"

namespace libasm {

enum Endian : host::uint_t {
    ENDIAN_BIG,
    ENDIAN_LITTLE,
};

template<
    typename AddrT,
    typename DiffT,
    typename OpCodeT,
    typename InsnT,
    Endian EndianE,
    host::uint_t CodeMax,
    host::uint_t NameMax
    >
struct ConfigBase {
    typedef AddrT   uintptr_t;
    typedef DiffT   ptrdiff_t;
    typedef OpCodeT opcode_t;
    typedef InsnT   insn_t;

    static constexpr Endian endian = EndianE;
    static constexpr host::uint_t code_max = CodeMax;
    static constexpr host::uint_t name_max = NameMax;
};

} // namespace libasm

#endif // __CONFIG_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
