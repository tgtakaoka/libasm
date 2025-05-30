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

#ifndef __LIBASM_CONFIG_BASE_H__
#define __LIBASM_CONFIG_BASE_H__

#include <stdint.h>

#include "config_host.h"
#include "error_reporter.h"
#include "option_base.h"
#include "str_scanner.h"
#include "type_traits.h"

namespace libasm {

enum AddressWidth : uint8_t {
    ADDRESS_12BIT = 12,
    ADDRESS_13BIT = 13,
    ADDRESS_14BIT = 14,
    ADDRESS_15BIT = 15,
    ADDRESS_16BIT = 16,
    ADDRESS_18BIT = 18,
    ADDRESS_20BIT = 20,
    ADDRESS_23BIT = 23,
    ADDRESS_24BIT = 24,
    ADDRESS_32BIT = 32,
};

enum AddressUnit : uint8_t {
    ADDRESS_BYTE = 1,  // memory is byte addressable.
    ADDRESS_WORD = 2,  // memory is word addressable.
};

enum OpCodeWidth : uint8_t {
    OPCODE_8BIT = 8,
    OPCODE_12BIT = 12,
    OPCODE_16BIT = 16,
};

enum Endian : uint8_t {
    ENDIAN_BIG,
    ENDIAN_LITTLE,
};

enum Radix : uint8_t {
    RADIX_NONE = 0,
    RADIX_2 = 2,
    RADIX_8 = 8,
    RADIX_10 = 10,
    RADIX_16 = 16,
};

enum Size : uint8_t {
    SZ_NONE = 0,
    SZ_BYTE = 1,  // 1 byte
    SZ_WORD = 2,  // 2 byte
    SZ_QUAD = 3,  // 4 byte
    SZ_OCTA = 4,  // 8 byte
    SZ_DATA = 5,  // operand dependent
    SZ_ADDR = 6,  // address
};

/** Interface for CPU configuration  */
struct ConfigBase {
    virtual AddressWidth addressWidth() const = 0;
    virtual AddressUnit addressUnit() const = 0;
    virtual OpCodeWidth opCodeWidth() const = 0;
    virtual Endian endian() const = 0;
    virtual uint8_t codeMax() const = 0;
    virtual uint8_t nameMax() const = 0;
    virtual const /*PROGMEM*/ char *listCpu_P() const = 0;
    virtual const /*PROGMEM*/ char *cpu_P() const = 0;

    /**
     * Check |addr| is in the range of |addressWidth()|.  This also
     * check whether |addr| is word aligned when |align| is true.
     */
    Error checkAddr(uint32_t addr, bool align = false) const;

    /**
     * In addition to check by |checkAddr(uint32_t, bool)|, also check
     * whether |addr| and |base| are only different in the most least
     * |width| bits.
     */
    Error checkAddr(uint32_t addr, uint32_t base, uint8_t width, bool align = false) const;

    /**
     * Sign extend least significant |bitw| bits of |u32| as |bitw|
     * bits signed integer.
     */
    static int32_t signExtend(uint32_t u32, uint_fast8_t bitw);

    /**
     * Check |delta| is in the range of |bitw| bits width signed integer.
     */
    static bool overflowDelta(int32_t s32, uint_fast8_t bitw);
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
struct __address_type<ADDRESS_13BIT> : public __address_helper<uint16_t> {};
template <>
struct __address_type<ADDRESS_14BIT> : public __address_helper<uint16_t> {};
template <>
struct __address_type<ADDRESS_15BIT> : public __address_helper<uint16_t> {};
template <>
struct __address_type<ADDRESS_16BIT> : public __address_helper<uint16_t> {};
template <>
struct __address_type<ADDRESS_18BIT> : public __address_helper<uint32_t> {};
template <>
struct __address_type<ADDRESS_20BIT> : public __address_helper<uint32_t> {};
template <>
struct __address_type<ADDRESS_23BIT> : public __address_helper<uint32_t> {};
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
struct __opcode_type<OPCODE_12BIT> : public __opcode_helper<uint16_t> {};
template <>
struct __opcode_type<OPCODE_16BIT> : public __opcode_helper<uint16_t> {};
}  // namespace

/** Interface for setting CPU */
struct ConfigSetter {
    virtual Error setCpuName(StrScanner &scan) = 0;
    virtual Error setFpuName(StrScanner &scan) = 0;
    bool setCpu(const char *name) {
        StrScanner scan{name};
        return setCpuName(scan) == OK;
    }
    bool setFpu(const char *name) {
        StrScanner scan{name};
        return setFpuName(scan) == OK;
    }
};

/** Base for instruction table fo |CPUTYPE|. */
template <typename CPUTYPE>
struct InsnTable {
    virtual const /*PROGMEM*/ char *listCpu_P() const = 0;
    virtual const /*PROGMEM*/ char *cpuName_P(CPUTYPE cpuType) const = 0;
    virtual Error searchCpuName(StrScanner &scan, CPUTYPE &cpuType) const = 0;
};

/** Base for holding a CPU configuration. */
template <typename CPUTYPE, AddressWidth AddrWE, AddressUnit AddrUE, OpCodeWidth CodeWE,
        Endian EndianE, uint8_t MaxCode, uint8_t MaxName>
struct ConfigImpl : ConfigBase, ConfigSetter {
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
    const /*PROGMEM*/ char *listCpu_P() const override { return _table.listCpu_P(); }
    const /*PROGMEM*/ char *cpu_P() const override { return _table.cpuName_P(_cpuType); }

    CPUTYPE cpuType() const { return _cpuType; }
    virtual void setCpuType(CPUTYPE cpuType) { _cpuType = cpuType; }

    Error setCpuName(StrScanner &scan) override {
        CPUTYPE cpuType;
        const auto error = _table.searchCpuName(scan, cpuType);
        if (error == OK)
            setCpuType(cpuType);
        return error;
    }

    Error setFpuName(StrScanner &) override { return FLOAT_NOT_SUPPORTED; }

private:
    const InsnTable<CPUTYPE> &_table;
    CPUTYPE _cpuType;

protected:
    ConfigImpl(const InsnTable<CPUTYPE> &table, CPUTYPE defaultCpu)
        : _table(table), _cpuType(defaultCpu) {}
};

}  // namespace libasm

#endif  // __LIBASM_CONFIG_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
