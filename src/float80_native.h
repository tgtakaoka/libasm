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

#ifndef __LIBASM_FLOAT80_NATIVE_H__
#define __LIBASM_FLOAT80_NATIVE_H__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "float80.h"

namespace libasm {

/** Explicit instantiation with native long double. */
template <>
struct __float80<true> {  // sizeof(long double) >= 10
    __float80() : _f80(0) {}
    __float80(uint16_t tag, uint64_t sig) {
        _tag = tag;
        _significand = sig;
    }
    __float80(const __float80 &f80) = default;
    __float80 &operator=(const __float80 &f80) = default;
    __float80(int64_t i64) : _f80(i64) {}
    __float80(uint64_t u64) : _f80(u64) {}
    explicit operator int64_t() const { return static_cast<int64_t>(_f80); }
    explicit operator uint64_t() const { return static_cast<uint64_t>(_f80); }
    explicit operator double() const { return static_cast<double>(_f80); }

    __float80 operator-() const { return -_f80; }
    __float80 operator+(const __float80 &rhs) const { return _f80 + rhs._f80; }
    __float80 operator-(const __float80 &rhs) const { return _f80 - rhs._f80; }
    __float80 operator*(const __float80 &rhs) const { return _f80 * rhs._f80; }
    __float80 operator/(const __float80 &rhs) const { return _f80 / rhs._f80; }

    bool isZero() const { return _f80 == 0.0; }
    bool isNegative() const { return _f80 < 0.0; }
    bool isFractional() const { return _f80 < 1.0; }
    bool isInf() const { return fpclassify(_f80) == FP_INFINITE; }
    bool isNan() const { return fpclassify(_f80) == FP_NAN; }

    __float80 pow10(int_fast16_t n) const { return _f80 * powl(10.0, n); }

    uint16_t tag() const { return _tag; }
    int_fast16_t exponent() const { return (_tag & EXP_MASK) - EXP_BASE; }
    uint64_t significand() const { return _significand; }

    static __float80 read(const char *str, char **end = nullptr) { return strtold(str, end); }
    int write(char *buf, uint_fast8_t len) const { return snprintf(buf, len, "%.21Lg", _f80); }

    /** Compose zero */
    static __float80 zero(bool sign) { return compose(sign, 0, 0); }
    /** Compose infinity */
    static __float80 infinity(bool sign = false) { return compose(sign, EXP_MASK, SIG_NORM); }
    /** Compose not a number */
    static __float80 not_a_number(bool sign = false, uint64_t sig = 0) {
        return compose(sign, EXP_MASK, UINT64_C(0xC000'0000'0000'0000));
    }

    /** For debug and testing */
    const char *str() const {
        static char buf[40];
        write(buf, sizeof(buf));
        return buf;
    }

    void print(const char *msg) const {
        printf("%s%04X.%08lX-%08lX", msg ? msg : "", _tag, _significand >> 32,
                _significand & UINT32_MAX);
    }

    void println(const char *msg) const {
        print(msg);
        printf("\n");
    }

private:
    union {
        long double _f80;
        struct {
#if defined(__BYTE_ORDER)
#if __BYTE_ORDER == __BIG_ENDIAN
            uint16_t _tag;
            uint64_t _significand;
#elif __BYTE_ORDER == __LITTLE_ENDIAN
            uint64_t _significand;
            uint16_t _tag;
#else
#error "Unknown __BYTE_ORDER" ## __BYTE_ORDER
#endif
#else
#error "__BYTE_ORDER is undefined"
#endif
        };
    };
    static constexpr auto SGN_MASK = UINT16_C(0x8000);
    static constexpr auto EXP_MASK = UINT16_C(0x7FFF);
    static constexpr auto EXP_BASE = UINT16_C(0x3FFF);
    static constexpr auto SIG_NORM = UINT64_C(0x8000'0000'0000'0000);
    static constexpr auto SIG_BITS = UINT64_C(0x7FFF'FFFF'FFFF'FFFF);

    __float80(long double v) : _f80(v) {}

    /** Compose __float80 */
    static __float80 compose(bool sign, uint16_t exp, uint64_t sig) {
        return __float80{static_cast<uint16_t>((sign ? SGN_MASK : 0) | (exp & EXP_MASK)), sig};
    }
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
