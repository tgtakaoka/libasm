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

#include "type_traits.h"

namespace libasm {

enum AddressWidth : uint8_t {
    ADDRESS_12BIT = 12,
    ADDRESS_13BIT = 13,
    ADDRESS_16BIT = 16,
    ADDRESS_20BIT = 20,
    ADDRESS_24BIT = 24,
    ADDRESS_32BIT = 32,
};

enum AddressUnit : uint8_t {
    ADDRESS_BYTE1 = 1,
    ADDRESS_BYTE2 = 2,
};

enum OpCodeWidth : uint8_t {
    OPCODE_8BIT = 8,
    OPCODE_16BIT = 16,
};

enum Endian : uint8_t {
    ENDIAN_BIG = 0,
    ENDIAN_LITTLE = 1,
};

struct ConfigBase {
    virtual AddressWidth addressWidth() const = 0;
    virtual AddressUnit addressUnit() const = 0;
    virtual OpCodeWidth opCodeWidth() const = 0;
    virtual Endian endian() const = 0;
    virtual uint8_t codeMax() const = 0;
    virtual uint8_t nameMax() const = 0;
};

namespace {
template <typename A>
struct __address_helper {
    typedef A uintptr_t;
    typedef typename make_signed<A>::type ptrdiff_t;
};
template <AddressWidth width>
struct __address_type;
template <>
struct __address_type<ADDRESS_12BIT> : public __address_helper<uint16_t> {};
template <>
struct __address_type<ADDRESS_16BIT> : public __address_helper<uint16_t> {};
template <>
struct __address_type<ADDRESS_20BIT> : public __address_helper<uint32_t> {};
template <>
struct __address_type<ADDRESS_24BIT> : public __address_helper<uint32_t> {};
template <>
struct __address_type<ADDRESS_32BIT> : public __address_helper<uint32_t> {};

template <typename C>
struct __opcode_helper {
    typedef C opcode_t;
};
template <OpCodeWidth width>
struct __opcode_type;
template <>
struct __opcode_type<OPCODE_8BIT> : public __opcode_helper<uint8_t> {};
template <>
struct __opcode_type<OPCODE_16BIT> : public __opcode_helper<uint16_t> {};
}  // namespace

template <AddressWidth AddrWE, OpCodeWidth CodeWE, Endian EndianE, uint8_t MaxCode, uint8_t MaxName,
        AddressUnit AddrUE = ADDRESS_BYTE1>
struct ConfigImpl : public ConfigBase {
    typedef typename __address_type<AddrWE>::uintptr_t uintptr_t;
    typedef typename __address_type<AddrWE>::ptrdiff_t ptrdiff_t;
    typedef typename __opcode_type<CodeWE>::opcode_t opcode_t;
    static constexpr Endian ENDIAN = EndianE;
    static constexpr uint8_t MAX_CODE = MaxCode;
    static constexpr uint8_t MAX_NAME = MaxName;

    AddressWidth addressWidth() const override { return AddrWE; }
    AddressUnit addressUnit() const override { return AddrUE; }
    OpCodeWidth opCodeWidth() const override { return CodeWE; }
    Endian endian() const override { return EndianE; }
    uint8_t codeMax() const override { return MaxCode; }
    uint8_t nameMax() const override { return MaxName; }
};

}  // namespace libasm

#endif  // __CONFIG_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
