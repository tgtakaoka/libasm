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
#include "error_reporter.h"
#include "float80.h"

namespace libasm {

struct StrScanner;

enum Radix : uint8_t {
    RADIX_NONE = 0,
    RADIX_2 = 2,
    RADIX_8 = 8,
    RADIX_10 = 10,
    RADIX_16 = 16,
};

struct Value {
#if defined(LIBASM_ASM_NOFLOAT)
    using signed_t = int32_t;
    using unsigned_t = uint32_t;
    static constexpr auto SIGNED_MAX = INT32_MAX;
    static constexpr auto SIGNED_MIN = INT32_MIN;
    static constexpr auto UNSIGNED_MAX = UINT32_MAX;
#else
    using signed_t = int64_t;
    using unsigned_t = uint64_t;
    using float_t = float80_t;
    static constexpr auto SIGNED_MAX = INT64_MAX;
    static constexpr auto SIGNED_MIN = INT64_MIN;
    static constexpr auto UNSIGNED_MAX = UINT64_MAX;
#endif
    static constexpr auto INTEGER_BITS = sizeof(unsigned_t) * 8;

    Value() : _unsigned(0), _type(V_UNDEF) {}
    Value(const Value &) = default;
    Value &operator=(const Value &) = default;

    /**
     * Read text |scan| as an integer of |radix| based.
     *
     * - Returns OK when |scan| is recognized as valid |radix| based
     *   integer, and updates |scan| at the end of an integer.
     *
     * - Returns OVERFLOW_RANGE when an integer exceeds UNSIGNED_MAX,
     *   and updates |scan| at the end of an integer.
     *
     * - Returns NOT_AN_EXPECTED when there is no valid integer found
     *   at |scan|, and |scan| is unchanged.
     */
    Error read(StrScanner &scan, Radix radix = RADIX_10);

    bool isUndefined() const { return _type == V_UNDEF; }
    bool isUnsigned() const { return _type == V_UNSIGNED || isUndefined(); }
    bool isSigned() const { return _type == V_SIGNED; }
    bool isInteger() const { return !isFloat(); }
    bool isFloat() const { return _type == V_FLOAT; }
    /** Return true if value exists and is zero */
    bool isZero() const;
    /** Return true if value is signed or float and is negative */
    bool isNegative() const;

    /** Get signed/unsigned 32-bit integer */
    int32_t getSigned() const { return static_cast<int32_t>(_signed); }
    uint32_t getUnsigned() const { return static_cast<uint32_t>(_unsigned); }
    /** Get full precision signed integer */
    signed_t getInteger() const { return _signed; }
#if !defined(LIBASM_ASM_NOFLOAT)
    /** Get value as floating point numbe */
    float_t getFloat() const;
#endif
    Value &setSigned(int32_t s);
    Value &setUnsigned(uint32_t u);
    Value &setInteger(signed_t s);
    Value &setUinteger(unsigned_t s);
#if defined(LIBASM_ASM_NOFLOAT)
    Value &setFloat();
#else
    Value &setFloat(const float_t &f);
    Value &setFloat(float_t &&f) { return setFloat(f); }
#endif
    /** Clear value as undefined */
    Value &clear();

    /** Returns true if value is larger than INT8_MAX or less than INT8_MIN */
    bool overflowInt8() const;
    /** Returns true if value is larger than UINT8_MAX or less than INT8_MIN */
    bool overflowUint8() const;
    /** Returns true if value is larger than INT16_MAX or less than INT16_MIN */
    bool overflowInt16() const;
    /** Returns true if value is larger than UINT16_MAX or less than INT16_MIN */
    bool overflowUint16() const;
    /** Returns true if value is larger than INT32_MAX or less than INT32_MIN */
    bool overflowInt32() const;
    /** Returns true if value is larger than UINT32_MAX or less than INT32_MIN */
    bool overflowUint32() const;
    /** Returns true if value is larger than |max| or less than |min| */
    bool overflow(uint32_t max, int32_t min = 0) const;
    /** Returns true if value is larger than |max| or less than |min| */
    bool overflowInteger(unsigned_t max, signed_t min = 0) const;

    bool negateOverflow() const;
    bool operator==(const Value &rhs) const;
    bool operator<(const Value &rhs) const;
    explicit operator bool() const;
    bool operator!() const;
    bool operator&&(const Value &rhs) const;
    bool operator||(const Value &rhs) const;
    Value operator-() const { return negate(); }
    Value operator+(const Value &rhs) const;
    Value operator-(const Value &rhs) const;
    Value operator*(const Value &rhs) const;
    Value operator/(const Value &rhs) const;
    Value operator%(const Value &rhs) const;
    Value operator~() const;
    Value operator|(const Value &rhs) const;
    Value operator&(const Value &rhs) const;
    Value operator^(const Value &rhs) const;
    Value operator<<(const Value &rhs) const;
    Value operator>>(const Value &rhs) const;
    Value exponential(const Value &rhs) const;

    static Error read(StrScanner &scan, Radix radix, unsigned_t &value);

    const char *str() const;

private:
    enum ValueType : uint8_t {
        V_UNDEF = 0,
        V_UNSIGNED = 1,
        V_SIGNED = 2,
        V_FLOAT = 3,
    };

    union {
        unsigned_t _unsigned;
        signed_t _signed;
#if !defined(LIBASM_ASM_NOFLOAT)
        float_t _float;
#endif
    };
    ValueType _type;

    void setU(unsigned_t u);
    void setS(signed_t s);
    Value negate() const;
    unsigned_t absolute() const;
};

}  // namespace libasm
#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
