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

#include <ctype.h>
#include <math.h>
#include "float80.h"
#include "str_buffer.h"
#include "str_scanner.h"
#include "text_common.h"

using namespace libasm::text::common;

namespace libasm {

const char *__float80<false>::str() const {
    static char buf[40];
    write(buf, sizeof(buf));
    return buf;
}

void __float80<false>::print(const char *msg) const {
    printf("%s%04X.%08lX-%08lX", msg ? msg : "", _tag, _significand >> 32,
            _significand & UINT32_MAX);
}

void __float80<false>::println(const char *msg) const {
    print(msg);
    printf("\n");
}

__float80<false>::__float80(int64_t i64) {
    if (i64 == 0) {
        _tag = 0;
        _significand = 0;
    } else {
        const auto negative = i64 < 0;
        _significand = negative ? -i64 : i64;
        const auto shifts = nlz(_significand);
        _significand <<= shifts;
        _tag = (negative ? SGN_MASK : 0) | (63 - shifts + EXP_BASE);
    }
}

__float80<false>::__float80(uint64_t u64) {
    if (u64 == 0) {
        _tag = 0;
        _significand = 0;
    } else {
        const auto shifts = nlz(u64);
        _significand = u64 << shifts;
        _tag = 63 - shifts + EXP_BASE;
    }
}

bool __float80<false>::isZero() const {
    return (_tag & EXP_MASK) == 0 && _significand == 0;
}

bool __float80<false>::isNegative() const {
    return _tag & SGN_MASK;
}

bool __float80<false>::isFractional() const {
    return exponent() < 0;
}

bool __float80<false>::isInf() const {
    return (~_tag & EXP_MASK) == 0 && (_significand & SIG_BITS) == 0;
}

bool __float80<false>::isNan() const {
    return (~_tag & EXP_MASK) == 0 && (_significand & SIG_BITS) != 0;
}

int_fast16_t __float80<false>::exponent() const {
    return (_tag & EXP_MASK) - EXP_BASE;
}

int64_t __float80<false>::to_i64(const __float80<false> &v) {
    const auto negative = v.isNegative();
    const auto shifts = v.exponent() - 63;
    if (shifts <= -64)
        return 0;
    if (shifts <= 0) {
        const auto u64 = v.significand() >> (-shifts);
        if (negative)
            return shifts == 0 ? INT64_MIN : -static_cast<int64_t>(u64);
        return u64 <= INT64_MAX ? u64 : INT64_MAX;
    }
    return negative ? INT64_MIN : INT64_MAX;
}

uint64_t __float80<false>::to_u64(const __float80<false> &v) {
    const auto negative = v.isNegative();
    if (negative)
        return 0;
    const auto shifts = v.exponent() - 63;
    if (shifts <= 0)
        return v.significand() >> (-shifts);
    return negative ? 0 : UINT64_MAX;
}

double __float80<false>::to_f64(const __float80<false> &v) {
    if (v.isZero())
        return v.isNegative() ? -0.0 : +0.0;
    if (v.isNan())
        return v.isNegative() ? -NAN : +NAN;
    const auto exp = v.exponent();
    if (exp > 1023)
        return v.isNegative() ? -INFINITY : INFINITY;
    if (exp < -1022 - 53)
        return v.isNegative() ? -0.0 : +0.0;
    const auto sig = v.significand() >> (63 - 53);
    const auto f64 = ldexp(sig, exp - 53);
    return v.isNegative() ? -f64 : f64;
}

__float80<false> __float80<false>::normalize(bool negative, int_fast16_t exp, uint128 &sig) {
    printf("@@ normalize: neg=%d exp=%ld sig=%s\n", negative, exp, sig.str());
    exp -= sig.normalize();
    auto significand = static_cast<uint64_t>(sig);
    if (significand == 0)
        return zero(negative);
    if (exp > EXP_BASE)
        return infinity(negative);
    if (exp < -EXP_BASE) {
        if (exp < -EXP_BASE - 63) {
            // Zero
            significand = 0;
        } else {
            // Subnormal
            sig.shr(63);
            significand = static_cast<uint64_t>(sig);
        }
        exp = -EXP_BASE;
    }
    compose(negative, exp + EXP_BASE, significand).println("@@ ");
    return compose(negative, exp + EXP_BASE, significand);
}

__float80<false> __float80<false>::neg(const __float80<false> &v) {
    return compose(!v.isNegative(), v._tag, v._significand);
}

__float80<false> __float80<false>::add(const __float80<false> &a, const __float80<false> &b) {
    if (a.isZero())
        return b;
    if (b.isZero())
        return a;
    if (a.isNan())
        return a;
    if (b.isNan())
        return b;
    if (a.isInf()) {
        if (!b.isInf())
            return infinity(a.isNegative());
        return (a.isNegative() ^ b.isNegative()) ? not_a_number() : infinity(a.isNegative());
    }
    if (b.isInf())
        return infinity(b.isNegative());

    const auto expa = a.exponent();
    const auto expb = b.exponent();
    const auto expdiff = expa - expb;
    const auto &fsummand = (expdiff < 0) ? b : a;
    const auto &faddend = (expdiff < 0) ? a : b;
    uint128 summand(fsummand.significand(), fsummand.isNegative());
    uint128 addend(faddend.significand(), faddend.isNegative());
    addend.shr(expdiff < 0 ? -expdiff : expdiff);
    summand.add(addend);
    const auto negative = summand.negative();
    if (negative)
        summand.negate();
    return normalize(negative, expdiff < 0 ? expb : expa, summand);
}

__float80<false> __float80<false>::mul(const __float80<false> &a, const __float80<false> &b) {
    if (a.isNan())
        return a;
    if (b.isNan())
        return b;
    const auto negative = a.isNegative() ^ b.isNegative();
    if (a.isZero())
        return b.isInf() ? not_a_number(negative) : zero(negative);
    if (b.isZero())
        return a.isInf() ? not_a_number(negative) : zero(negative);
    if (a.isInf() || b.isInf())
        return infinity(negative);

    uint128 product(a.significand());
    auto expa = a.exponent() - product.normalize();
    const auto shifts = nlz(b._significand);
    auto expb = b.exponent() - shifts;
    const auto multiplier = b._significand << shifts;
    /* 2.0 > product and multiplier >= 0.5 */
    product.mul(multiplier);
    /* 4.0 > product >= 0.25 */
    return normalize(negative, expa + expb, product);
}

__float80<false> __float80<false>::div(const __float80<false> &a, const __float80<false> &b) {
    if (a.isNan())
        return a;
    if (b.isNan())
        return b;
    const auto negative = a.isNegative() ^ b.isNegative();
    if (a.isZero())
        return b.isZero() ? not_a_number(negative) : zero(negative);
    if (b.isZero())
        return a.isInf() ? not_a_number(negative) : infinity(negative);
    if (a.isInf() && b.isInf())
        return not_a_number(negative);
    if (a.isInf())
        return infinity(negative);
    if (b.isInf())
        return zero(negative);

    uint128 quotient(a.significand());
    const auto expa = a.exponent() - quotient.normalize();
    const auto shifts = nlz(b._significand);
    const auto expb = b.exponent() - shifts;
    const auto divisor = b.significand() << shifts;
    /* 2.0 > quotient and divisor >= 0.5 */
    quotient.div(divisor);
    /* 4.0 > quotient and divisor >= 0.25 */
    return normalize(negative, expa - expb, quotient);
}

__float80<false> __float80<false>::pow10(int_fast16_t n) const {
    if (n == 0)
        return *this;
    uint128 p10{UINT64_C(0x8000'0000'0000'0000)};  // 1.0
    auto exp = p10.power10(n < 0 ? -n : n);
    uint128 v{significand()};
    print("@@ pow10: "); printf(" n=%ld\n", n);
    v.println("@@      v=");
    if (n > 0) {
        v.mul(p10);
        exp += exponent();
        v.print("@@   *  v="); printf(" exp=%ld\n", exp);
    } else {
        v.div(p10);
        exp = exponent() - exp;
        v.print("@@   /  v="); printf(" exp=%ld\n", exp);
    }
    exp -= v.normalize();
    v.print("@@      v="); printf(" exp=%ld\n", exp);
    if (exp > EXP_BASE)
        return infinity(false);
    if (exp < -EXP_BASE)
        return zero(false);
    return compose(false, exp + EXP_BASE, static_cast<uint64_t>(v));
}

__float80<false> __float80<false>::read(const char *str, char **end) {
    StrScanner p = str;
    printf("@@ read: str=%s\n", str);
    const auto sigsign = p.expect('-');
    if (p.iexpectWord_P(TEXT_NAN)) {
        if (end)
            *end = const_cast<char *>(p.str());
        return not_a_number(sigsign);
    }
    if (p.iexpectWord_P(TEXT_INF)) {
        if (end)
            *end = const_cast<char *>(p.str());
        return infinity(sigsign);
    }
    const __float80<false> TEN{static_cast<uint64_t>(10)};
    __float80<false> sig;
    int_fast8_t digits = 0;
    int_fast8_t frac = -1;
    while (true) {
        if (isdigit(*p)) {
            ++digits;
            if (frac >= 0)
                ++frac;
            const __float80<false> digit{static_cast<uint64_t>(*p - '0')};
            sig = add(mul(sig, TEN), digit);
            ++p;
        } else if (*p == '.' && digits && frac < 0) {
            frac = 0;
            ++p;
        } else {
            break;
        }
    }
    if (frac < 0)
        frac = 0;
    printf("@@ read: digits=%d frac=%d\n", digits, frac);
    sig.println("@@       sig=");
    auto v = sig.pow10(-frac);
    v.println("@@         v=");
    int_fast16_t exp = 0;
    if (digits && p.iexpect('E')) {
        const auto expsign = p.expect('-');
        if (expsign == 0)
            p.expect('+');
        while (isdigit(*p)) {
            exp *= 10;
            exp += *p++ - '0';
        }
        if (expsign)
            exp = -exp;
    }
    if (end)
        *end = const_cast<char *>(p.str());
    v = v.pow10(exp);
    v.println("@@         v=");
    return sigsign ? -v : v;
}

int16_t __float80<false>::write_digits(StrBuffer &out) const {
    if (isZero()) {
        out.letter('0');
        return 0;
    }
    constexpr auto log10_2 = 0.3010299956639812;
    // Estimate approximate exponent of 10.
    int_fast16_t exp = floor(exponent() * log10_2);
    auto v = static_cast<uint64_t>(pow10(18 - exp));
    constexpr auto ten19 = UINT64_C(10'000'000'000'000'000'000);
    while (v >= ten19) {
        v /= 10;
        exp++;
    }
    // v < 10^19
    constexpr auto ten18 = UINT64_C(1'000'000'000'000'000'000);
    while (v < ten18) {
        v *= 10;
        exp--;
    }
    // v >= 10^16
    for (int_fast8_t i = 0; i < 19 && v; ++i) {
        const auto d = v / ten18;
        if (i == 1)
            out.letter('.');
        out.letter(d + '0');
        v %= ten18;
        v *= 10;
    }

    return exp;
}

int __float80<false>::write(char *buf, uint_fast8_t len) const {
    StrBuffer out{buf, len};
    if (isNan()) {
        out.text_P(TEXT_NAN);
    } else if (isInf()) {
        if (isNegative())
            out.letter('-');
        out.text_P(TEXT_INF);
    } else {
        auto v = *this;
        if (v.isNegative()) {
            out.letter('-');
            v = -v;
        }
        const auto exp = v.write_digits(out);
        if (exp)
            out.letter('E').int16(exp);
    }
    return out.len();
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
