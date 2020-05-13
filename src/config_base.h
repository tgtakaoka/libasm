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

enum AddressWidth : host::uint_t {
    ADDRESS_16BIT = 16,
    ADDRESS_24BIT = 24,
    ADDRESS_32BIT = 32,
};

enum OpCodeWidth : host::uint_t {
    OPCODE_8BIT,
    OPCODE_16BIT,
};

enum Endian : host::uint_t {
    ENDIAN_BIG,
    ENDIAN_LITTLE,
};

struct ConfigBase {
    virtual AddressWidth addressWidth() const = 0;
    virtual int8_t addressBits() const = 0;
    virtual OpCodeWidth opCodeWidth() const = 0;
    virtual host::uint_t codeMax() const = 0;
    virtual Endian endian() const = 0;
    virtual host::uint_t nameMax() const = 0;

    virtual const char *listCpu() const = 0;
    virtual bool setCpu(const char *cpu) = 0;
    virtual const char *getCpu() const = 0;
};

template<
    AddressWidth AddrWE,
    typename AddrT,
    typename DiffT,
    OpCodeWidth CodeWE,
    typename OpCodeT,
    host::uint_t CodeMax,
    Endian EndianE,
    host::uint_t NameMax
    >
struct ConfigImpl : virtual public ConfigBase {
    typedef AddrT   uintptr_t;
    typedef DiffT   ptrdiff_t;
    typedef OpCodeT opcode_t;

    static constexpr host::uint_t CODE_MAX = CodeMax;
    static constexpr Endian ENDIAN = EndianE;
    static constexpr host::uint_t NAME_MAX = NameMax;

    AddressWidth addressWidth() const override { return AddrWE; }
    int8_t addressBits() const override { return int8_t(AddrWE); }
    OpCodeWidth opCodeWidth() const override { return CodeWE; }
    host::uint_t codeMax() const override { return CodeMax; }
    Endian endian() const override { return EndianE; }
    host::uint_t nameMax() const override { return NameMax; }
};

} // namespace libasm

#endif // __CONFIG_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
