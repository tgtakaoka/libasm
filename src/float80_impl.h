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

#ifndef __LIBASM_FLOAT80_IMPL_H__
#define __LIBASM_FLOAT80_IMPL_H__

#include <stdint.h>
#include "float80.h"
#include "uint128.h"

#define GRS 1

namespace libasm {

struct StrBuffer;

/** Explicit instantiation with software emulation. */
template <>
struct __float80<false> {  // sizeof(long double) < 10
    __float80() : _tag(0), _significand(0) {}
    __float80(uint16_t tag, uint64_t sig) : _tag(tag), _significand(sig) {}
    __float80(const __float80 &f80) = default;
    __float80 &operator=(const __float80 &f80) = default;
    __float80(int64_t i64);
    __float80(uint64_t u64);
    __float80(const char *str) { *this = read(str); }

    explicit operator int64_t() const { return to_i64(*this); }
    explicit operator uint64_t() const { return to_u64(*this); }
    explicit operator double() const { return to_f64(*this); }

    __float80 operator-() const { return neg(*this); }
    __float80 operator+(const __float80 &rhs) const { return add(*this, rhs); }
    __float80 operator-(const __float80 &rhs) const { return add(*this, -rhs); }
    __float80 operator*(const __float80 &rhs) const { return mul(*this, rhs); }
    __float80 operator/(const __float80 &rhs) const { return div(*this, rhs); }

    bool isZero() const;        // == 0
    bool isNegative() const;    // < 0
    bool isFractional() const;  // < 1
    bool isInf() const;
    bool isNan() const;

    __float80 pow10(int_fast16_t n) const;

    uint16_t tag() const { return _tag; }
    int_fast16_t exponent() const;
    uint64_t significand() const { return _significand; }

    static __float80 read(const char *str, char **end = nullptr);
    int write(char *buf, uint_fast8_t len) const;

    /** Compose zero */
    static __float80 zero(bool sign) { return compose(sign, 0, 0); }
    /** Compose infinity */
    static __float80 infinity(bool sign = false) { return compose(sign, EXP_MASK, SIG_NORM); }
    /** Compose not a number */
    static __float80 not_a_number(bool sign = false, uint64_t sig = 0) {
        return compose(sign, EXP_MASK, UINT64_C(0xC000'0000'0000'0000) | sig);
    }

    /** For debug and testing */
    const char *str() const;
    void print(const char *msg = nullptr) const;
    void println(const char *msg = nullptr) const;

private:
    static constexpr auto SGN_MASK = UINT16_C(0x8000);
    static constexpr auto EXP_MASK = UINT16_C(0x7FFF);
    static constexpr auto EXP_BASE = UINT16_C(0x3FFF);
    uint16_t _tag;
    static constexpr auto SIG_NORM = UINT64_C(0x8000'0000'0000'0000);
    static constexpr auto SIG_BITS = UINT64_C(0x7FFF'FFFF'FFFF'FFFF);
    uint64_t _significand;

    int16_t write_digits(StrBuffer &out) const;

    /** Compose __float80 */
    static __float80 compose(bool sign, uint16_t exp, uint64_t sig) {
        return __float80{static_cast<uint16_t>((sign ? SGN_MASK : 0) | (exp & EXP_MASK)), sig};
    }
    /** Convert to int64_t */
    static int64_t to_i64(const __float80 &v);
    /** Convert to uint64_t */
    static uint64_t to_u64(const __float80 &v);
    /** Convert to double */
    static double to_f64(const __float80 &v);
    /** Negate |v| */
    static __float80 neg(const __float80 &v);
    /** Add |a| and |b| */
    static __float80 add(const __float80 &a, const __float80 &b);
    /** Multiply non-zero |a| and |b| */
    static __float80 mul(const __float80 &a, const __float80 &b);
    /** Divide non-zero |a| by |b| */
    static __float80 div(const __float80 &a, const __float80 &b);
    /** Normalize */
    static __float80 normalize(bool negative, int_fast16_t exp, uint128 &sig);
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
