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

#ifndef __LIBASM_FIXED64_H__
#define __LIBASM_FIXED64_H__

#include <stdint.h>

namespace libasm {

struct StrScanner;

/**
 * Number of leading zero.
 */
template <typename T>
uint_fast8_t leading_zero(T v) {
    uint_fast8_t zeros = sizeof(T) * 8;
    uint_fast8_t shifts = zeros / 2;
    do {
        const auto x = v >> shifts;
        if (x) {
            zeros -= shifts;
            v = x;
        }
        shifts >>= 1;
    } while (shifts);
    return zeros - v;
}

/**
 * 64-bit fixed point number with 16-bit high and low guards.
 *
 * Normalized value |n| is 1.0 <= |n| < 2.0, and subnormalized value
 * |s| is 0 <= |s| < 1.0.
 */
struct __fixed64 {
    /** Constructors */
    __fixed64(uint64_t u64 = 0, uint16_t low = 0) { set(u64, low); }
    __fixed64(const __fixed64 &u64) = default;
    __fixed64 &operator=(const __fixed64 &u64) = default;

    __fixed64 &set(uint64_t sig, uint16_t low = 0);
    uint64_t value() const;
    uint16_t get_high() const;
    void clear_high();

    __fixed64 operator-() const;
    __fixed64 operator+(const __fixed64 &rhs) const;
    __fixed64 operator-(const __fixed64 &rhs) const;
    __fixed64 operator*(const __fixed64 &rhs) const;
    __fixed64 operator/(const __fixed64 &rhs) const;
    bool operator==(const __fixed64 &rhs) const { return compare(rhs) == 0; }
    bool operator!=(const __fixed64 &rhs) const { return compare(rhs) != 0; }
    bool operator<=(const __fixed64 &rhs) const { return compare(rhs) <= 0; }
    bool operator<(const __fixed64 &rhs) const { return compare(rhs) < 0; }
    bool operator>=(const __fixed64 &rhs) const { return compare(rhs) >= 0; }
    bool operator>(const __fixed64 &rhs) const { return compare(rhs) > 0; }

    bool zero() const;
    bool negative() const;
    bool normal() const;

    /** Negate |this| */
    void negate();
    /** Shift right |this| */
    void shift_right(uint_fast16_t n, bool arithmetic = false);
    /** Logical shift left |this| */
    void shift_left(uint_fast16_t n);
    /** Normalize unsigned |this| and return adjusting exponet of 2 */
    int_fast16_t normalize();
    /** Round to |bits| bit with nearest even. Return true if overflow to _v[_HI] */
    bool round_off(uint_fast8_t bits);

    /** Add signed |this| and signed |addend| */
    void add(const __fixed64 &addend);
    /** Multiply normalized unsigned |this| by normalized unsigned |multiplier| */
    void mul(const __fixed64 &multiplier);
    /** Divide normalized unsigned |this| by normalized unsigned |divisor| */
    void div(const __fixed64 &divisor);
    /** Unsigned |this| * 10^|n|. Return exponent of 2 */
    int_fast16_t power10(int_fast16_t n);

    /** For testing */
    const char *str() const;

    /**
     * Normalize |exp| and |this| floating point number where 1.0 <=
     * |this| < 10.0, and returns 10 based exponent to |exp10|.
     */
    int_fast16_t normalize(int_fast16_t exp, int_fast16_t &exp10);

    /**
     * Convert normalized |exp| and |this|, where 1.0 <= |this| <
     * 10.0, to |prec| decimal digits |digits[0..ndigits]| with
     * rounding off at |prec|+1. Returns true if rounding off causes
     * overflow at most significant digit.
     */
    bool decompose(int_fast16_t exp, uint8_t *digits, uint_fast8_t ndigits);

    /**
     * Read integral and fraction from |scan| and returns exponent of
     * 2 and significand in |this|.
     */
    int_fast16_t read(StrScanner &scan);

    /** Multiply by 10 and add one decimal digit |digit| */
    int_fast16_t dec_digit(int_fast16_t exp, uint_fast8_t digit);
    /** Multiply by 16 and add one hexadecimal digit |digit| */
    int_fast16_t hex_digit(int_fast16_t exp, uint_fast8_t digit);

private:
    static constexpr auto _BITS = 16;
    static constexpr auto _LOW = 0;   // low guard
    static constexpr auto _LSW = 1;   // least significant
    static constexpr auto _MSW = 4;   // most significant
    static constexpr auto _HIGH = 5;  // high guard
    static constexpr auto _SIZE = _HIGH + 1;
    uint16_t _v[_SIZE];

    uint16_t get_low() const;
    void set_value(uint64_t u64);
    void set_low(uint16_t lo);
    int compare(const __fixed64 &rhs) const;

    /**
     * Multiply mormalized |u[_HI]| by normalized |v[_HI]| and return
     * product |w[_HI*2]|
     */
    static void mul(uint16_t w[], const uint16_t u[], const uint16_t v[]);
    /**
     * Divide normalized |u[_HI*2+1]| by normalized |v[_HI]| and
     * return quotient
     */
    static uint16_t div(uint16_t u[], const uint16_t v[], int_fast8_t j);
    /** Add one hexadecimal digit |digit| */
    int_fast16_t add_digit(int_fast16_t exp, uint_fast8_t digit);
};

using fixed64_t = __fixed64;

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
