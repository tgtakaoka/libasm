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

#include "float80_hard.h"

#ifdef LIBASM_HAS_LONG_DOUBLE

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include "ieee_float.h"
#include "str_buffer.h"

namespace libasm {

__float80_hard &__float80_hard::set(uint16_t tag, uint64_t sig) {
    _tag = tag;
    _sig = sig;
    return *this;
}

__float80_hard &__float80_hard::set(int64_t i64) {
    _f80 = static_cast<long double>(i64);
    return *this;
}

__float80_hard &__float80_hard::set(uint64_t u64) {
    _f80 = static_cast<long double>(u64);
    return *this;
}

__float80_hard &__float80_hard::set(const float32_t &f32) {
    union {
        uint32_t u32;
        float f32;
    } data;
    data.u32 = f32.bits();
    _f80 = static_cast<long double>(data.f32);
    return *this;
}

__float80_hard &__float80_hard::set(const float64_t &f64) {
    union {
        uint64_t u64;
        double f64;
    } data;
    data.u64 = f64.bits();
    _f80 = static_cast<long double>(data.f64);
    return *this;
}

float32_t &__float80_hard::get(float32_t &f32) const {
    union {
        uint32_t u32;
        float f32;
    } data;
    data.f32 = static_cast<float>(_f80);
    return f32.set(data.u32);
}

float64_t &__float80_hard::get(float64_t &f64) const {
    union {
        uint64_t u64;
        double f64;
    } data;
    data.f64 = static_cast<double>(_f80);
    return f64.set(data.u64);
}

__float80_hard __float80_hard::pow10(int_fast16_t n) const {
    return __float80_hard(_f80 * powl(10.0, n));
}

int __float80_hard::read(const char *str, char **end) {
    errno = 0;
    _f80 = strtold(str, end);
    return errno;
}

int __float80_hard::ecvt(char *buf, uint_fast8_t len, uint_fast8_t prec) const {
    return snprintf(buf, len, "%.*Le", prec, _f80);
}

int __float80_hard::fcvt(char *buf, uint_fast8_t len, uint_fast8_t prec) const {
    return snprintf(buf, len, "%.*Lf", prec, _f80);
}

int __float80_hard::gcvt(char *buf, uint_fast8_t len, uint_fast8_t prec) const {
    return snprintf(buf, len, "%.*Lg", prec, _f80);
}

__float80_hard __float80_hard::compose(bool negative, int_fast16_t exp, fixed64_t &sig) {
    exp = normalize(exp, sig);
    if (exp > EXP_BASE)
        return infinity(negative);
    const auto sigval = sig.value(MANT_DIG);
    if (exp < -EXP_BASE || sigval == 0)
        return zero(negative);
    return bits(negative, exp + EXP_BASE, sigval);
}

__float80_hard __float80_hard::bits(bool sign, uint16_t exp, uint64_t sig) {
    return __float80_hard((sign ? SGN_MASK : 0) | (exp & EXP_MASK), sig);
}

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
