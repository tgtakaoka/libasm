#include <stdio.h>
/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __LIBASM_VALUE_H__
#define __LIBASM_VALUE_H__

#include <stdint.h>

#include "config_base.h"
#include "error_reporter.h"
#include "str_scanner.h"

namespace libasm {

enum Radix : uint8_t {
    RADIX_NONE = 0,
    RADIX_2 = 2,
    RADIX_8 = 8,
    RADIX_10 = 10,
    RADIX_16 = 16,
};

struct Value {
    Value() : _uint32(0), _type(V_UNDEF) {}

    /**
     * Parse text |scan| as a number of |radix| based.
     *
     * - Returns OK when |scan| is recognized as valid |radix| based number, and updates |scan| at
     *   the end of number.
     * - Returns OVERFLOW_RANGE when a number exceeds UINT_MAX, and updates |scan| at the end of a
     *   number.
     * - Returns ILLEGAL_CONSTANT when there is no valid digit found at |scan|, and |scan} is
     *   unchanged.
     */
    Error parseNumber(StrScanner &scan, Radix radix);

    bool isUndefined() const { return _type == V_UNDEF; }
    bool isSigned() const { return _type == V_INT32; }
    bool isUnsigned() const { return _type == V_UINT32 || isUndefined(); }
#ifdef ASM_NOFLOAT
    constexpr bool isInt32() const {
        return true;
    }
    constexpr bool isInt64() const {
        return false;
    }
    constexpr bool isFloat() const {
        return false;
    }
#else
    bool isInt32() const {
        return isUnsigned() || isSigned();
    }
    bool isInt64() const {
        return isInt32() || _type == V_INT64 || _type == V_UINT64;
    }
    bool isFloat() const {
        return _type == V_FLOAT64;
    }
#endif
    bool overflowUint8() const {
        return ConfigBase::overflowUint8(_uint32);
    }
    bool overflowUint16() const {
        return ConfigBase::overflowUint16(_uint32);
    }
#ifdef ASM_NOFLOAT
    constexpr bool overflowUint32() const {
        return false;
    }
#else
    bool overflowUint32() const {
        return ConfigBase::overflowUint32(getInt64());
    }
#endif
    bool overflowUint(uint8_t bitw) const {
        return ConfigBase::overflowUint(_uint32, bitw);
    }

    int32_t getSigned() const {
        return static_cast<int32_t>(_uint32);
    }
    uint32_t getUnsigned() const {
        return _uint32;
    }
#ifndef ASM_NOFLOAT
    int64_t getInt64() const;
    double getFloat() const;
#endif

    Value &setSigned(int32_t value) {
        _uint32 = value;
        _type = value < 0 ? V_INT32 : V_UINT32;
        return *this;
    }
    Value &setUnsigned(uint32_t value) {
        _uint32 = value;
        _type = V_UINT32;
        return *this;
    }
#ifndef ASM_NOFLOAT
    Value &setInt64(int64_t value) {
        _uint64 = value;
        _type = V_INT64;
        return *this;
    }
    Value &setFloat(double value) {
        _float64 = value;
        _type = V_FLOAT64;
        return *this;
    }
#endif
    Value &clear() {
        _uint32 = 0;
        _type = V_UNDEF;
        return *this;
    }

    bool isZero() const;
    bool negateOverflow() const;
    bool operator==(const Value &rhs) const;
    bool operator<(const Value &rhs) const;
    Value operator-() const;
    Value operator+(const Value &rhs) const;
    Value operator-(const Value &rhs) const;
    Value operator*(const Value &rhs) const;
    Value operator/(const Value &rhs) const;
    Value operator%(const Value &rhs) const;
    Value operator<<(const Value &rhs) const;
    Value operator>>(const Value &rhs) const;
    Value exponential(const Value &rhs) const;

    static Error parseNumber(StrScanner &scan, Radix radix, uint32_t &value);
#ifndef ASM_NOFLOAT
    static Error parseNumber(StrScanner &scan, Radix radix, uint64_t &value);
#endif

#define DEBUG_VALUE 0
#if DEBUG_VALUE
    void print() const {
        switch (_type) {
        case V_UNDEF:
            printf("<undef>");
            break;
        case V_INT32:
            printf("<i32>%d", static_cast<int32_t>(_uint32));
            break;
        case V_UINT32:
            printf("<u32>%u", _uint32);
            break;
#ifndef ASM_NOFLOAT
        case V_INT64:
            printf("<i64>%ld", static_cast<int64_t>(_uint64));
            break;
        case V_UINT64:
            printf("<u64>%ld", _uint64);
            break;
        case V_FLOAT64:
            printf("<f64>%lg", _float64);
            break;
#endif
        }
    }
#else
    void print() const {}
#endif

private:
    enum ValueType : uint8_t {
        V_UNDEF = 0,
        V_UINT32 = 1,
        V_INT32 = 2,
#ifndef ASM_NOFLOAT
        V_UINT64 = 3,
        V_INT64 = 4,
        V_FLOAT64 = 5,
#endif
    };

    union {
        uint32_t _uint32;
#ifndef ASM_NOFLOAT
        uint64_t _uint64;
        double _float64;
#endif
    };
    ValueType _type;
};

}  // namespace libasm
#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
