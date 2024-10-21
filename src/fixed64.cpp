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

#include "fixed64.h"
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {

const char *__fixed64::str() const {
    static char buf[40];
    StrBuffer out{buf, sizeof(buf)};
    out.hex(_v[_HIGH]);
    for (int_fast8_t i = _MSW; i > _LSW; i -= 2)
        out.letter('-').hex(_v[i]).hex(_v[i - 1]);
    out.letter('-').hex(_v[_LOW]);
    return buf;
}

__fixed64 &__fixed64::set(uint64_t u64, uint16_t low) {
    set_low(low);
    set_value(u64);
    clear_high();
    return *this;
}

void __fixed64::set_value(uint64_t u64) {
    _v[_LSW] = static_cast<uint16_t>(u64);
    u64 >>= _BITS;
    _v[_LSW + 1] = static_cast<uint16_t>(u64);
    u64 >>= _BITS;
    _v[_MSW - 1] = static_cast<uint16_t>(u64);
    u64 >>= _BITS;
    _v[_MSW] = static_cast<uint16_t>(u64);
}

void __fixed64::set_low(uint16_t low) {
    _v[_LOW] = low;
}

void __fixed64::clear_high() {
    _v[_HIGH] = 0;
}

uint64_t __fixed64::value(uint_fast8_t bits) const {
    uint64_t u64 = _v[_MSW];
    u64 <<= _BITS;
    u64 |= _v[_MSW - 1];
    u64 <<= _BITS;
    u64 |= _v[_LSW + 1];
    u64 <<= _BITS;
    u64 |= _v[_LSW];
    return u64 >> (64 - bits);
}

uint16_t __fixed64::get_low() const {
    return _v[_LOW];
}

uint16_t __fixed64::get_high() const {
    return _v[_HIGH];
}

__fixed64 __fixed64::operator-() const {
    auto v = *this;
    v.negate();
    return v;
}

__fixed64 __fixed64::operator+(const __fixed64 &rhs) const {
    auto v = *this;
    v.add(rhs);
    return v;
}

__fixed64 __fixed64::operator-(const __fixed64 &rhs) const {
    return *this + (-rhs);
}

__fixed64 __fixed64::operator*(const __fixed64 &rhs) const {
    auto v = *this;
    v.mul(rhs);
    return v;
}

__fixed64 __fixed64::operator/(const __fixed64 &rhs) const {
    auto v = *this;
    v.div(rhs);
    return v;
}

bool __fixed64::zero() const {
    return value(64) == 0 && get_low() == 0;
}

bool __fixed64::negative() const {
    return static_cast<int16_t>(get_high()) < 0;
}

bool __fixed64::normal() const {
    return get_high() == 0 && (_v[_MSW] & UINT16_C(0x8000)) != 0;
}

int __fixed64::compare(const __fixed64 &rhs) const {
    for (int_fast8_t i = _HIGH; i >= _LOW; --i) {
        if (_v[i] != rhs._v[i])
            return _v[i] > rhs._v[i] ? 1 : -1;
    }
    return 0;
}

void __fixed64::negate() {
    uint16_t carry = 1;
    for (int_fast8_t i = _LOW; i <= _HIGH; ++i) {
        const auto v = static_cast<uint32_t>(static_cast<uint16_t>(~_v[i])) + carry;
        _v[i] = static_cast<uint16_t>(v);
        carry = v >> _BITS;
    }
}

void __fixed64::shift_right(uint_fast16_t n, bool arithmetic) {
    const auto sign = arithmetic && negative() ? UINT16_MAX : 0;
    while (n >= _BITS) {
        n -= _BITS;
        const auto sticky = _v[_LOW] ? 1 : 0;
        for (int_fast8_t i = _LOW; i <= _MSW; ++i)
            _v[i] = _v[i + 1];
        _v[_LOW] |= sticky;
        _v[_HIGH] = sign;
    }
    if (n) {
        const auto nl = -n & 15;
        const auto sticky = (_v[_LOW] & ((1 << n) - 1)) ? 1 : 0;
        for (int_fast8_t i = _LOW; i <= _MSW; ++i)
            _v[i] = (_v[i] >> n) | (_v[i + 1] << nl);
        _v[_HIGH] = (_v[_HIGH] >> n) | (sign << nl);
        _v[_LOW] |= sticky;
    }
}

void __fixed64::shift_left(uint_fast16_t n) {
    while (n >= _BITS) {
        n -= _BITS;
        for (int_fast8_t i = _HIGH; i > _LOW; --i)
            _v[i] = _v[i - 1];
        _v[_LOW] = 0;
    }
    if (n) {
        const auto nr = -n & 15;
        for (int_fast8_t i = _HIGH; i > _LOW; --i)
            _v[i] = (_v[i] << n) | (_v[i - 1] >> nr);
        _v[_LOW] <<= n;
    }
}

int_fast16_t __fixed64::normalize() {
    if (_v[_HIGH]) {
        int_fast8_t shifts = _BITS - leading_zero(_v[_HIGH]);
        shift_right(shifts, true);
        return shifts;
    }
    int_fast16_t shifts = 0;
    for (int_fast8_t i = _MSW; i >= _LOW; --i) {
        if (_v[i]) {
            shifts += leading_zero(_v[i]);
            break;
        }
        shifts += _BITS;
    }
    if (shifts == _BITS * (_HIGH - _LOW))
        return 0;  // value() == 0
    shift_left(shifts);
    return -shifts;
}

bool __fixed64::round_off(uint_fast8_t bits) {
    constexpr auto FULL_BITS = _BITS * (_MSW + 1);
    if (bits >= FULL_BITS || bits == 0) {
        // no need to round
        return false;
    }
    const auto lsb = FULL_BITS - bits;  // least significand bit
    const auto roundb = lsb - 1;        // round bit
    const auto roundi = roundb / _BITS;
    const auto roundm = UINT16_C(1) << (roundb % _BITS);
    if (_v[roundi] & roundm) {
        auto intermediate = true;
        for (uint_fast8_t i = 0; i < roundi; ++i) {
            if (_v[i])
                intermediate = false;
        }
        if ((_v[roundi] & (roundm - 1)) != 0)
            intermediate = false;
        const auto lsbi = lsb / _BITS;
        const auto lsbm = UINT16_C(1) << (lsb % _BITS);
        const auto even = (_v[lsbi] & lsbm) == 0;
        if (!intermediate || !even) {
            uint16_t carry = lsbm;
            for (int_fast8_t i = lsbi; i <= _HIGH; ++i) {
                const auto v = static_cast<uint32_t>(_v[i]) + carry;
                _v[i] = static_cast<uint16_t>(v);
                carry = v >> _BITS;
            }
        }
    }
    for (uint_fast8_t i = 0; i < roundi; ++i)
        _v[i] = 0;
    _v[roundi] &= ~((static_cast<uint32_t>(roundm) << 1) - 1);
    return _v[_HIGH] != 0;
}

void __fixed64::add(const __fixed64 &addend) {
    uint16_t carry = 0;
    for (int_fast8_t i = _LOW; i <= _HIGH; ++i) {
        const auto v = static_cast<uint32_t>(_v[i]) + addend._v[i] + carry;
        _v[i] = static_cast<uint16_t>(v);
        carry = v >> _BITS;
    }
}

void __fixed64::mul(const __fixed64 &multiplier) {
    const auto &multiplicand = *this;
    uint16_t w[_HIGH * 2];
    mul(w, multiplicand._v, multiplier._v);
    for (int_fast8_t i = _LOW; i <= _HIGH; ++i)
        _v[i] = w[_MSW + i];
    shift_right(_BITS - 1);  // adjust decimal point
}

void __fixed64::mul(uint16_t w[], const uint16_t u[], const uint16_t v[]) {
    constexpr int_fast8_t m = _HIGH;
    constexpr int_fast8_t n = _HIGH;
    for (int_fast8_t i = 0; i < m; ++i)
        w[i] = 0;
    for (int_fast8_t j = 0; j < n; ++j) {
        uint16_t k = 0;
        const auto vj = static_cast<uint32_t>(v[j]);
        for (int_fast8_t i = 0; i < m; ++i) {
            const auto t = u[i] * vj + w[i + j] + k;
            w[i + j] = static_cast<uint16_t>(t);
            k = t >> _BITS;
        }
        w[j + m] = k;
    }
}

void __fixed64::div(const __fixed64 &divisor) {
    constexpr int_fast8_t m = _HIGH * 2;
    constexpr int_fast8_t n = _HIGH;
    uint16_t dividend[m + 1];
    for (int_fast8_t i = _LOW; i < _HIGH; ++i) {
        dividend[i] = 0;
        dividend[_HIGH + i] = _v[i];
    }
    dividend[m] = 0;
    for (int_fast8_t j = m - n; j >= 0; --j)
        _v[j] = div(dividend, divisor._v, j);
    shift_right(1);  // adjust decimal point
}

/**
 * Multiword division.
 *
 * Kunuth, Donald E. "The Art of Computer Programming, Volume 2, 3rd
 * ed.: Seminumerical Algorithms.", 4.3.1 Algorithm D. (b=2^16, m=10,
 * n=5).
 */
uint16_t __fixed64::div(uint16_t u[], const uint16_t v[], int_fast8_t j) {
    constexpr int_fast8_t n = _HIGH;
    constexpr auto b = UINT32_C(0x10000);
    const auto u32 = (static_cast<uint32_t>(u[j + n]) << 16) + u[j + n - 1];
    auto qhat = u32 / v[n - 1];
    auto rhat = u32 - qhat * v[n - 1];
    while (qhat >= b || qhat * v[n - 2] > (rhat << 16) + u[j + n - 2]) {
        --qhat;
        rhat += v[n - 1];
        if (rhat >= b)
            break;
    }
    uint16_t k = 0;
    int32_t t;
    for (int_fast8_t i = 0; i < n; ++i) {
        const auto p = qhat * v[i];
        t = u[i + j] - k - (p & UINT16_MAX);
        u[i + j] = t;
        k = (p >> 16) - (t >> 16);
    }
    t = u[j + n] - k;
    u[j + n] = t;
    uint16_t q = qhat;
    if (t < 0) {
        --q;
        k = 0;
        for (int_fast8_t i = 0; i < n; ++i) {
            t = u[i + j] + v[i] + k;
            u[i + j] = t;
            k = t >> 16;
        }
        u[j + n] += k;
    }
    return q;
}

namespace {

/**
 * Table of __fixed64 power of 10
 *   execute ./fixed64_pow10.sh
 */

const struct {
    const __fixed64 sig;   // siginificand of 10^|exp10|
    const int16_t exp2;    // exponent of 2
    const uint16_t exp10;  // exponent of 10
} P10[] = {
        {__fixed64(0xC46052028A20979A, 0xC94C), 13606, 4096},  // 1E4096Q
        {__fixed64(0x9E8B3B5DC53D5DE4, 0xA74D), 6803, 2048},   // 1E2048Q
        {__fixed64(0xC976758681750C17, 0x650D), 3401, 1024},   // 1E1024Q
        {__fixed64(0xE319A0AEA60E91C6, 0xCC65), 1700, 512},    // 1E512Q
        {__fixed64(0xAA7EEBFB9DF9DE8D, 0xDDBC), 850, 256},     // 1E256Q
        {__fixed64(0x93BA47C980E98CDF, 0xC66F), 425, 128},     // 1E128Q
        {__fixed64(0xC2781F49FFCFA6D5, 0x3CBF), 212, 64},      // 1E64Q
        {__fixed64(0x9DC5ADA82B70B59D, 0xF020), 106, 32},      // 1E32Q
        {__fixed64(0x8E1BC9BF04000000, 0x0000), 53, 16},       // 1E16Q
        {__fixed64(0xBEBC200000000000, 0x0000), 26, 8},        // 1E8Q
        {__fixed64(0x9C40000000000000, 0x0000), 13, 4},        // 1E4Q
        {__fixed64(0xC800000000000000, 0x0000), 6, 2},         // 1E2Q
        {__fixed64(0xA000000000000000, 0x0000), 3, 1},         // 1E1Q
};
}  // namespace

int_fast16_t __fixed64::power10(int_fast16_t n) {
    if (n == 0)
        return 0;
    uint_fast16_t exp10 = n < 0 ? -n : n;
    int_fast16_t exp2 = 0;
    for (const auto &p : P10) {
        if (exp10 >= p.exp10) {
            exp10 -= p.exp10;
            if (n < 0) {
                div(p.sig);
                exp2 -= p.exp2;
            } else {
                mul(p.sig);
                exp2 += p.exp2;
            }
            exp2 += normalize();
        }
    }
    return exp2;
}

namespace {
const struct digits {
    __fixed64 sig;
    int_fast16_t exp;
} DIGITS[16] = {
        {fixed64_t{UINT64_C(0x0000000000000000)}, 0},  // 0
        {fixed64_t{UINT64_C(0x8000000000000000)}, 0},  // 1
        {fixed64_t{UINT64_C(0x8000000000000000)}, 1},  // 2
        {fixed64_t{UINT64_C(0xC000000000000000)}, 1},  // 3
        {fixed64_t{UINT64_C(0x8000000000000000)}, 2},  // 4
        {fixed64_t{UINT64_C(0xA000000000000000)}, 2},  // 5
        {fixed64_t{UINT64_C(0xC000000000000000)}, 2},  // 6
        {fixed64_t{UINT64_C(0xE000000000000000)}, 2},  // 7
        {fixed64_t{UINT64_C(0x8000000000000000)}, 3},  // 8
        {fixed64_t{UINT64_C(0x9000000000000000)}, 3},  // 9
        {fixed64_t{UINT64_C(0xA000000000000000)}, 3},  // 10
        {fixed64_t{UINT64_C(0xB000000000000000)}, 3},  // 11
        {fixed64_t{UINT64_C(0xC000000000000000)}, 3},  // 12
        {fixed64_t{UINT64_C(0xD000000000000000)}, 3},  // 13
        {fixed64_t{UINT64_C(0xE000000000000000)}, 3},  // 14
        {fixed64_t{UINT64_C(0xF000000000000000)}, 3},  // 15
};
}  // namespace

int_fast16_t __fixed64::normalize(int_fast16_t exp, int_fast16_t &exp10) {
    if (exp == 0 && zero()) {
        exp10 = 0;
        return 0;
    }
    // estimate exponent of 10
    // log10(2) = 0.3010299956 ~ 0.30103 ~ 18734 / 62233
    exp10 = static_cast<int32_t>(exp) * INT32_C(18734) / INT32_C(62233);
    // normalize 1.0 <= |sig| < 10.0
    exp += power10(-exp10);
    const auto &TEN = DIGITS[10];
    while (exp > TEN.exp || (exp == TEN.exp && *this >= TEN.sig)) {
        // |sig| >= 10.0
        exp += power10(-1);
        ++exp10;
    }
    const auto &ONE = DIGITS[1];
    while (exp < ONE.exp || (exp == ONE.exp && *this < ONE.sig)) {
        // |sig| < 1.0
        exp += power10(1);
        --exp10;
    }
    return exp;
}

bool __fixed64::decompose(int_fast16_t exp, uint8_t *digits, uint_fast8_t ndigits) {
    const auto &ONE = DIGITS[1];
    uint_fast8_t digit = 0;  // keeps last digit
    // |ndigits| + 1 loop to calculate extra digit for erounding off.
    for (uint_fast16_t i = 0; i <= ndigits; ++i) {
        if (exp < ONE.exp || (exp == ONE.exp && *this < ONE.sig)) {
            // sig < 1.0
            digit = 0;
        } else {
            // digit = (int)sig;
            shift_left(exp + 1);
            digit = get_high();
            // sig -= digit;
            clear_high();
            shift_right(exp + 1);
            exp += normalize();
        }
        if (i == ndigits)
            break;
        digits[i] = digit;
        // sig *= 10;
        exp += power10(1);
    }
    // check rounding off
    if (digit < 5)
        return false;
    // round up
    for (int_fast16_t i = ndigits; --i >= 0;) {
        if (++digits[i] <= 9)
            return false;
        digits[i] = 0;
    }
    // '999..9' overflows to '1000..0'
    digits[0] = 1;
    return true;
}

int_fast16_t __fixed64::dec_digit(int_fast16_t exp, uint_fast8_t digit) {
    if (!(exp == 0 && zero())) {
        const auto &TEN = DIGITS[10];
        mul(TEN.sig);  // x10
        exp += TEN.exp + normalize();
    }
    return add_digit(exp, digit);
}

int_fast16_t __fixed64::hex_digit(int_fast16_t exp, uint_fast8_t digit) {
    if (!(exp == 0 && zero())) {
        shift_left(4);  // x16
        exp += normalize();
    }
    return add_digit(exp, digit);
}

int_fast16_t __fixed64::add_digit(int_fast16_t exp, uint_fast8_t digit) {
    digit &= 0xF;
    if (digit) {
        auto d = DIGITS[digit];
        if (exp >= d.exp) {
            d.sig.shift_right(exp - d.exp);
        } else {
            shift_right(d.exp - exp);
            exp = d.exp;
        }
        add(d.sig);
        exp += normalize();
    }
    return exp;
}

int_fast16_t __fixed64::read(StrScanner &scan) {
    uint_fast8_t digits = 0;
    int_fast8_t frac = -1;
    int_fast16_t exp = 0;
    set(0);  // clear significand to 0
    while (true) {
        if (isdigit(*scan)) {
            ++digits;
            if (frac >= 0)
                ++frac;
            exp = dec_digit(exp, *scan++ - '0');
        } else if (digits && frac < 0 && scan.expect('.')) {
            frac = 0;
        } else {
            break;
        }
    }
    if (frac > 0) {
        // Adjust fraction part if exists
        exp += power10(-frac);
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
