#include <stdio.h>
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

#include <stdio.h>
#include "uint128.h"

namespace libasm {

const char *uint128::str() const {
    static char buf[36];
    snprintf(buf, sizeof(buf), "%08X-%08X-%08X-%08X", _v96, _v64, _v32, _v0);
    return buf;
}

void uint128::print(const char *msg) const {
    printf("%s%s", msg ? msg : "", str());
}

void uint128::println(const char *msg) const {
    print(msg);
    printf("\n");
}

uint128::uint128(uint32_t v64, uint32_t v32, uint32_t v0)
    : _v96(0), _v64(v64), _v32(v32), _v0(v0) {}

uint128::uint128(uint64_t v, bool negative) : _v96(0), _v64(v >> 32), _v32(v), _v0(0) {
    if (negative)
        negate();
}

uint128::operator uint64_t() const {
    const uint_fast8_t round =
            _v0 > UINT32_C(0x8000'0000) || (_v0 == UINT32_C(0x8000'0000) && (_v32 & 1)) ? 1 : 0;
    const auto v = (static_cast<uint64_t>(_v64) << 32) + _v32 + round;
    return negative() ? -v : v;
}

uint128 uint128::operator-() const {
    auto v = *this;
    v.negate();
    return v;
}

uint128 uint128::operator+(const uint128 &rhs) const {
    auto v = *this;
    v.add(rhs);
    return v;
}

uint128 uint128::operator-(const uint128 &rhs) const {
    return *this + (-rhs);
}

uint128 uint128::operator*(const uint128 &rhs) const {
    auto v = *this;
    v.mul(rhs);
    return v;
}

uint128 uint128::operator/(const uint128 &rhs) const {
    auto v = *this;
    v.div(rhs);
    return v;
}

bool uint128::zero() const {
    return _v96 == 0 && _v64 == 0 && _v32 == 0 && _v0 == 0;
}

bool uint128::negative() const {
    return static_cast<int32_t>(_v96) < 0;
}

bool uint128::overflow() const {
    return _v96 != 0 && (_v96 + 1) != 0;
}

void uint128::negate() {
    _v0 = ~_v0;
    _v32 = ~_v32;
    _v64 = ~_v64;
    _v96 = ~_v96;
    if (_v0 += 1)
        return;
    if (_v32 += 1)
        return;
    if (_v64 += 1)
        return;
    _v96 += 1;
}

void uint128::shr(uint_fast16_t n, bool arithmetic) {
    const auto sign = arithmetic && negative() ? UINT32_C(0xFFFF'FFFF) : 0;
    while (n >= 32) {
        n -= 32;
        const auto sticky = _v0 ? 1 : 0;
        _v0 = _v32 | sticky;
        _v32 = _v64;
        _v64 = _v96;
        _v96 = sign;
    }
    if (n) {
        const auto nl = -n & 31;
        const auto sticky = (_v0 & ((1 << n) - 1)) ? 1 : 0;
        _v0 = (_v0 >> n) | (_v32 << nl) | sticky;
        _v32 = (_v32 >> n) | (_v64 << nl);
        _v64 = (_v64 >> n) | (_v96 << nl);
        _v96 = (_v96 >> n) | (sign << nl);
    }
}

void uint128::shl(uint_fast8_t n) {
    while (n >= 32) {
        n -= 32;
        _v96 = _v64;
        _v64 = _v32;
        _v32 = _v0;
        _v0 = 0;
    }
    if (n) {
        const auto nr = -n & 31;
        _v96 = (_v96 << n) | (_v64 >> nr);
        _v64 = (_v64 << n) | (_v32 >> nr);
        _v32 = (_v32 << n) | (_v0 >> nr);
        _v0 = (_v0 << n);
    }
}

int_fast8_t uint128::normalize() {
    if (_v96) {
        const auto shifts = nlz(_v96) - 32;
        shr(-shifts);
        return shifts;
    }
    const auto v = static_cast<uint64_t>(*this);
    if (v == 0 && _v0 == 0)
        return 0;
    const auto shifts = v ? nlz(v) : nlz(_v0) + 64;
    shl(shifts);
    return shifts;
}

void uint128::add(const uint128 &addend) {
    const auto v0 = static_cast<uint64_t>(_v0) + addend._v0;
    _v0 = v0;
    const auto v32 = static_cast<uint64_t>(_v32) + addend._v32 + (v0 >> 32);
    _v32 = v32;
    const auto v64 = static_cast<uint64_t>(_v64) + addend._v64 + (v32 >> 32);
    _v64 = v64;
    _v96 += addend._v96 + (v64 >> 32);
}

void uint128::mul(const uint128 &multiplier) {
    const auto v64 = static_cast<uint64_t>(multiplier._v64);
    const auto v32 = static_cast<uint64_t>(multiplier._v32);
    const auto v0 = static_cast<uint64_t>(multiplier._v0);
    const auto v0_v32 = _v0 * v32;
    const auto v0_v64 = _v0 * v64;
    const auto v32_v0 = _v32 * v0;
    const auto v64_v0 = _v64 * v0;
    const auto v32_v32 = _v32 * v32;
    const auto v32_v64 = _v32 * v64;
    const auto v64_v32 = _v64 * v32;
    const auto v64_v64 = _v64 * v64;
    const auto p0 = (v0_v32 >> 32) + (v32_v0 >> 32) + (v0_v64 & UINT32_MAX) +
                    (v64_v0 & UINT32_MAX) + (v32_v32 & UINT32_MAX);
    const auto p32 = (p0 >> 32) + (v0_v64 >> 32) + (v64_v0 >> 32) + (v32_v32 >> 32) +
                     (v32_v64 & UINT32_MAX) + (v64_v32 & UINT32_MAX);
    const auto p64 = (p32 >> 32) + (v32_v64 >> 32) + (v64_v32 >> 32) + v64_v64;
    _v0 = p0;
    _v32 = p32;
    _v64 = p64;
    _v96 = p64 >> 32;
    shr(31, false);
}

void uint128::div(const uint128 &v) {
    uint32_t dividend[] = {0, 0, 0, _v0, _v32, _v64, 0};
    uint32_t divisor[] = {v._v0, v._v32, v._v64};
    _v96 = div(dividend, divisor, 3);
    _v64 = div(dividend, divisor, 2);
    _v32 = div(dividend, divisor, 1);
    _v0 = div(dividend, divisor, 0);
    shr(1, false);
}

/**
 * Multiword division.
 *
 * Kunuth, Donald E. "The Art of Computer Programming, Volume 2, 3rd
 * ed.: Seminumerical Algorithms.", 4.3.1 Algorithm D. (b=2^32, m=6,
 * n=3).
 */
uint32_t uint128::div(uint32_t u[], const uint32_t v[], int_fast8_t j) {
    constexpr int_fast8_t n = 3;
    constexpr auto b = UINT64_C(0x1'0000'0000);
    const auto u64 = (static_cast<uint64_t>(u[j + n]) << 32) + u[j + n - 1];
    auto qhat = u64 / v[n - 1];
    auto rhat = u64 - qhat * v[n - 1];
    while (qhat >= b || qhat * v[n - 2] > (rhat << 32) + u[j + n - 2]) {
        --qhat;
        rhat += v[n - 1];
        if (rhat >= b)
            break;
    }
    uint32_t k = 0;
    uint64_t t;
    for (int_fast8_t i = 0; i < n; ++i) {
        const auto p = qhat * v[i];
        t = u[i + j] - k - (p & UINT32_MAX);
        u[i + j] = t;
        k = (p >> 32) - (t >> 32);
    }
    t = u[j + n] - k;
    u[j + n] = t;
    uint32_t q = qhat;
    if (t < 0) {
        --q;
        k = 0;
        for (int_fast8_t i = 0; i < n; ++i) {
            t = u[i + j] + v[i] + k;
            u[i + j] = t;
            k = t >> 32;
        }
        u[j + n] += k;
    }
    return q;
}

namespace {

struct pow10 {
    uint128 sig;
    int16_t exp;
    int16_t mask;
};

const struct pow10 POW10[] = {
        {{0xC460'5202, 0x8A20'979A, 0xC94C'1540}, 13606, 4096},  // 1.0e4096
        {{0x9E8B'3B5D, 0xC53D'5DE4, 0xA74D'28CE}, 6803, 2048},   // 1.0e2048
        {{0xC976'7586, 0x8175'0C17, 0x650D'3d29}, 3401, 1024},   // 1.0e1024
        {{0xE319'A0AE, 0xA60E'91C6, 0xCC65'5C55}, 1700, 512},    // 1.0e512
        {{0xAA7E'EBFB, 0x9DF9'DE8D, 0xDDBB'901C}, 850, 256},     // 1.0e256
        {{0x93BA'47C9, 0x80E9'8CDF, 0xC66F'336C}, 425, 128},     // 1.0e128
        {{0xC278'1F49, 0xFFCF'A6D5, 0x3CBF'6B72}, 212, 64},      // 1.0e64
        {{0x9DC5'ADA8, 0x2B70'B59D, 0xF020'0000}, 106, 32},      // 1.0e32
        {{0x8E1B'C9BF, 0x0400'0000, 0x0000'0000}, 53, 16},       // 1.0e16
        {{0xBEBC'2000, 0x0000'0000, 0x0000'0000}, 26, 8},        // 1.0e8
        {{0x9C40'0000, 0x0000'0000, 0x0000'0000}, 13, 4},        // 1.0e4
        {{0xC800'0000, 0x0000'0000, 0x0000'0000}, 6, 2},         // 1.0e2
        {{0xA000'0000, 0x0000'0000, 0x0000'0000}, 3, 1},         // 10.0
};

}  // namespace

int_fast16_t uint128::power10(uint_fast16_t n) {
    int_fast16_t exp = 0;
    for (int_fast8_t i = 0; i < 13; ++i) {
        const auto *p = POW10 + i;
        if (n & p->mask) {
            mul(p->sig);
            exp += p->exp;
            exp -= normalize();
        }
    }
    return exp;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
