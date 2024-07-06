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

#ifndef __LIBASM_UINT128_H__
#define __LIBASM_UINT128_H__

#include <stdint.h>

namespace libasm {

template <typename T>
uint_fast8_t nlz(T v) {
    uint_fast8_t zeros = sizeof(T) * 8;
    uint_fast8_t shifts = zeros / 2;
    do {
        auto x = v >> shifts;
        if (x) {
            zeros -= shifts;
            v = x;
        }
        shifts >>= 1;
    } while (shifts);
    return zeros - v;
}

/**
 * 128 bit fixed point number used for extended 80-bit float siginificand.
 */
struct uint128 {
    uint128(uint32_t v64, uint32_t v32, uint32_t v0);
    uint128(uint64_t v, bool negative = false);
    uint128(const uint128 &u128) = default;
    uint128 &operator=(const uint128 &u128) = default;

    /** Return |_v64|:|_v32| as uint64_t */
    explicit operator uint64_t() const;

    uint128 operator-() const;
    uint128 operator+(const uint128 &rhs) const;
    uint128 operator-(const uint128 &rhs) const;
    uint128 operator*(const uint128 &rhs) const;
    uint128 operator/(const uint128 &rhs) const;

    bool zero() const;
    bool negative() const;
    bool overflow() const;

    /** Negate |this| */
    void negate();
    /** Shift right |this|, LSB is a sticky bit */
    void shr(uint_fast16_t n, bool arithmetic = true);
    /** Logical shift left |this| */
    void shl(uint_fast8_t n);
    /** Normalize unsigned |this| and return shift left amount */
    int_fast8_t normalize();

    /** Add signed |this| and signed |addend| */
    void add(const uint128 &addend);
    /** Multiply normalized unsigned |this| by normalized unsigned |multiplier| */
    void mul(const uint128 &multiplier);
    /** Divide normalized unsigned |this| by normalized unsigned |divisor| */
    void div(const uint128 &divisor);
    /** Unsigned |this| * 10^|n|. Return exponent of 2 */
    int_fast16_t power10(uint_fast16_t n);

    /** For debug and testing */
    const char *str() const;
    void print(const char *msg = nullptr) const;
    void println(const char *msg = nullptr) const;

private:
    uint32_t _v96;  // sign
    uint32_t _v64;  // high(siginificand)
    uint32_t _v32;  // low(significand)
    uint32_t _v0;   // extra

    /** Divide normalized |u[0:6]| by normalized |v[0:2]| and return quotient */
    static uint32_t div(uint32_t u[], const uint32_t v[], int_fast8_t j);
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
