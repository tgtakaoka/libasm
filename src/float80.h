/*
 * Copyright 2024 Tadashi G. Takaoka
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

#ifndef __LIBASM_FLOAT80_H__
#define __LIBASM_FLOAT80_H__

#include <stdint.h>
#include <float.h>

namespace libasm {

/** Library interface of float80. */
template <bool HAS_LONG_DOUBLE>
struct __float80 {
    /** constructors */
    __float80();
    __float80(uint16_t exp, uint64_t sig);
    __float80(const __float80 &f80) = default;
    __float80 &operator=(const __float80 &f80) = default;
    __float80(int64_t i64);
    __float80(uint64_t u64);

    /** cast operators */
    explicit operator int64_t() const;
    explicit operator uint64_t() const;
    explicit operator double() const;

    /** operator overloads */
    __float80 operator-() const;
    __float80 operator+(const __float80 &rhs) const;
    __float80 operator-(const __float80 &rhs) const;
    __float80 operator*(const __float80 &rhs) const;
    __float80 operator/(const __float80 &rhs) const;

    bool isZero() const;        // == 0
    bool isNegative() const;    // < 0
    bool isFractional() const;  // < 1
    bool isInf() const;
    bool isNan() const;

    /** unsigned |this| * 10^|n| */
    __float80 pow10(int_fast16_t n) const;

    uint16_t tag() const;
    int_fast16_t exponent() const;
    uint64_t significand() const;

    /** read string representation; |end| points next char. */
    static __float80 read(const char *str, char **end = nullptr);
    /** write to string representation; returns |errno| */
    int write(char *buf, uint_fast8_t len) const;

    /** Compose zero */
    static __float80 zero(bool sign);
    /** Compose infinity */
    static __float80 infinity(bool sign = false);
    /** Compose not a number */
    static __float80 not_a_number(bool sign = false, uint64_t sig = 0);
};

#define FLOAT80_DEBUG
#if defined(LDBL_DIG) && !defined(FLOAT80_DEBUG)
using float80_t = __float80<LDBL_DIG >= 18>;
#else
using float80_t = __float80<false>;
#endif

}  // namespace libasm

#if defined(LDBL_DIG) && LDBL_DIG >= 18 && !defined(FLOAT80_DEBUG)
#include "float80_native.h"
#endif
#include "float80_impl.h"

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
