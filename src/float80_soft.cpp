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

#include "float80_soft.h"
#include <ctype.h>
#include <errno.h>
#include "str_buffer.h"
#include "str_scanner.h"
#include "text_common.h"
#include "value.h"

using namespace libasm::text::common;

namespace libasm {

__float80_soft &__float80_soft::set(uint16_t tag, uint64_t sig) {
    _tag = tag;
    _sig = sig;
    return *this;
}

__float80_soft &__float80_soft::set(int64_t i64) {
    if (i64 >= 0)
        return set(static_cast<uint64_t>(i64));
    set(static_cast<uint64_t>(-i64));
    _tag |= SGN_MASK;
    return *this;
}

__float80_soft &__float80_soft::set(uint64_t u64) {
    if (u64 == 0) {
        _tag = 0;
        _sig = 0;
    } else {
        fixed64_t sig;
        const auto exp = sig.set(u64).normalize() + 63;
        _tag = exp + EXP_BASE;
        _sig = sig.value();
    }
    return *this;
}

bool __float80_soft::operator==(const __float80_soft &rhs) const {
    if (isZero() && rhs.isZero())
        return true;
    if (isInf() && rhs.isInf())
        return isNegative() == rhs.isNegative();
    if (isNan() || rhs.isNan())
        return false;
    return (*this - rhs).isZero();
}

bool __float80_soft::operator<(const __float80_soft &rhs) const {
    if (isZero() && rhs.isZero())
        return false;
    if (isInf() && rhs.isInf())
        return isNegative() && !rhs.isNegative();
    if (isNan() || rhs.isNan())
        return false;
    return (*this - rhs).isNegative();
}

__float80_soft __float80_soft::neg(const __float80_soft &v) {
    return bits(!v.isNegative(), v._tag, v._sig);
}

__float80_soft __float80_soft::add(const __float80_soft &a, const __float80_soft &b) {
    if (a.isNan())
        return a;
    if (b.isNan())
        return b;
    // (+INF)+(-INF)=NAN, (-INF)+(+INF)=NAN
    if (a.isInf() && b.isInf())
        return a.isNegative() == b.isNegative() ? infinity(a.isNegative()) : not_a_number();
    if (a.isInf())
        return infinity(a.isNegative());
    if (b.isInf())
        return infinity(b.isNegative());
    // (-0)+(-0)=(-0), otherwise (+0)
    if (a.isZero() && b.isZero())
        return zero(a.isNegative() && b.isNegative());
    if (a.isZero())
        return b;
    if (b.isZero())
        return a;

    fixed64_t asig;
    const auto aexp = a.decompose(asig);
    if (a.isNegative())
        asig.negate();
    fixed64_t bsig;
    const auto bexp = b.decompose(bsig);
    if (b.isNegative())
        bsig.negate();
    const auto expdiff = aexp - bexp;
    fixed64_t sum;
    int_fast16_t exp;
    if (expdiff < 0) {
        sum = bsig;
        exp = bexp;
        asig.shift_right(-expdiff, true);
        sum.add(asig);
    } else {
        sum = asig;
        exp = aexp;
        bsig.shift_right(expdiff, true);
        sum.add(bsig);
    }
    const auto negative = sum.negative();
    if (negative)
        sum.negate();
    return compose(negative, exp, sum);
}

__float80_soft __float80_soft::mul(const __float80_soft &a, const __float80_soft &b) {
    if (a.isNan())
        return a;
    if (b.isNan())
        return b;
    const auto negative = a.isNegative() != b.isNegative();
    // 0*INF=NAN
    if (a.isZero())
        return b.isInf() ? not_a_number(negative) : zero(negative);
    // INF*0=NAN
    if (b.isZero())
        return a.isInf() ? not_a_number(negative) : zero(negative);
    if (a.isInf() || b.isInf())
        return infinity(negative);

    fixed64_t product;
    const auto aexp = a.decompose(product);
    fixed64_t multiplier;
    const auto bexp = b.decompose(multiplier);
    /* 2.0 > product and multiplier >= 0.5 */
    product.mul(multiplier);
    /* 4.0 > product >= 0.25 */
    return compose(negative, aexp + bexp, product);
}

__float80_soft __float80_soft::div(const __float80_soft &a, const __float80_soft &b) {
    if (a.isNan())
        return a;
    if (b.isNan())
        return b;
    const auto negative = a.isNegative() != b.isNegative();
    if (a.isZero()) {
        // 0/0=NAN
        if (b.isZero())
            return not_a_number(negative);
        // 0/INF=0
        if (b.isInf())
            return zero(negative);
    }
    // INF/INF=NAN, INF/x=INF
    if (a.isInf())
        return b.isInf() ? not_a_number(negative) : infinity(negative);
    // x/INF=0
    if (b.isInf())
        return zero(negative);

    fixed64_t quotient;
    const auto aexp = a.decompose(quotient);
    fixed64_t divisor;
    const auto bexp = b.decompose(divisor);
    /* 2.0 > quotient and divisor >= 0.5 */
    quotient.div(divisor);
    /* 4.0 > quotient and divisor >= 0.25 */
    return compose(negative, aexp - bexp, quotient);
}

__float80_soft __float80_soft::pow10(int_fast16_t n) const {
    if (n == 0)
        return *this;
    fixed64_t sig;
    auto exp = decompose(sig);
    exp += sig.power10(n);
    return compose(isNegative(), exp, sig);
}

int __float80_soft::read(const char *str, char **end) {
    StrScanner scan{str};
    const auto sigsign = scan.expect('-');
    if (scan.iexpectWord_P(TEXT_NAN)) {
        if (end)
            *end = const_cast<char *>(scan.str());
        *this = not_a_number(sigsign);
        return 0;
    }
    if (scan.iexpectWord_P(TEXT_INF) || scan.iexpectWord_P(TEXT_INFINITY)) {
        if (end)
            *end = const_cast<char *>(scan.str());
        *this = infinity(sigsign);
        return 0;
    }
    // Read integral and fraction as fixed64_t |sig| and |exp|
    fixed64_t sig;
    auto exp = sig.read(scan);
    int_fast16_t exp10 = 0;
    if (scan.str() > str && scan.iexpect('E')) {
        const auto expsign = scan.expect('-');
        if (expsign == 0)
            scan.expect('+');
        Value::unsigned_t exponent;
        const auto error = Value::read(scan, RADIX_10, exponent);
        if (error == NOT_AN_EXPECTED) {
            if (end)
                *end = const_cast<char *>(str);
            *this = zero(sigsign);
            return 0;  // no conversion
        }
        if (error == OK && exponent <= INT16_MAX) {
            exp10 = expsign ? -exponent : exponent;
        } else {
            exp10 = expsign ? INT16_MIN : INT16_MAX;
        }
    }
    if (end)
        *end = const_cast<char *>(scan.str());
    exp += sig.power10(exp10);
    *this = compose(sigsign, exp, sig);
    return (isInf() || isSubnormal()) ? ERANGE : 0;
}

namespace {
uint_fast8_t min_digits(uint_fast16_t a, uint_fast8_t b) {
    return a < b ? a : b;
}
int_fast16_t max_digits(int_fast16_t a, int_fast8_t b) {
    return a < b ? b : a;
}
}  // namespace

void __float80_soft::ecvt(StrBuffer &out, uint_fast8_t prec, bool suppress) const {
    fixed64_t sig;
    int_fast16_t exp10;
    auto exp = decompose(sig, exp10);
    // number of digits = integral + prec
    auto ndigits = min_digits(1 + prec, MAX_DECIMAL_DIG);
    uint8_t digits[ndigits];
    if (sig.decompose(exp, digits, ndigits))
        ++exp10;
    if (suppress) {
        // remove trailing '0'
        while (ndigits > 0 && digits[ndigits - 1] == 0)
            --ndigits;
    }
    // integral digit
    out.letter(digits[0] + '0');
    // fractional part
    for (uint_fast8_t i = 1; i <= prec && i < ndigits; ++i) {
        if (i == 1)
            out.letter('.');
        out.letter(digits[i] + '0');
    }
    if (exp10 || !suppress) {
        out.letter('e');
        if (exp10 >= 0)
            out.letter('+');
        out.int16(exp10, 2);
    }
}

void __float80_soft::fcvt(StrBuffer &out, uint_fast8_t prec, bool suppress) const {
    fixed64_t sig;
    int_fast16_t exp10;
    auto exp = decompose(sig, exp10);
    // number of digits = integral (exp10+1) + prec, at least 1 even
    // when exp10 < -prec.
    auto ndigits = min_digits(max_digits(exp10 + 1 + prec, 1), MAX_DECIMAL_DIG);
    uint8_t digits[ndigits];
    if (sig.decompose(exp, digits, ndigits))
        ++exp10;
    if (suppress) {
        // remove trailing '0'
        while (ndigits > 0 && digits[ndigits - 1] == 0)
            --ndigits;
    }
    uint_fast8_t index = 0;
    // integral part
    if (exp10 < 0) {
        out.letter('0');
    } else {
        for (int_fast16_t i = 0; i <= exp10; ++i)
            out.letter((index < ndigits ? digits[index++] : 0) + '0');
    }
    // fractional part
    if (prec && (index < ndigits || !suppress)) {
        out.letter('.');
        uint_fast8_t frac = 0;
        if (exp10 < 0) {
            // place value '0'
            for (auto nzero = -exp10 - 1; frac < prec && nzero > 0; --nzero, ++frac)
                out.letter('0');
        }
        // significant digits
        while (frac++ < prec) {
            if (index < ndigits) {
                out.letter(digits[index++] + '0');
            } else if (suppress) {
                break;
            } else {
                out.letter('0');
            }
        }
    }
}

/**
 * Convert floating point number as printf(3) doing.
 *
 * Format 'e'/ecvt. Exponent style "[-]d.ddde[+-]dd", such as
 * "%.(prec)Le" in printf(3). The |prec| specifies the exact number of
 * digits to appear after the decimal point.
 *
 * Format 'f'/fcvt. Fixed style "[-]ddd.ddd" such as "%.(prec)Lf" in
 * printf(3). The |prec| specifies the exact number of digits to
 * appear after the decimal point.
 *
 * Format 'g'/gcvt. Exponent or Fixed style depending on the value and
 * the |prec|, such as "%.(prec)Lg" in printf(3).  Let |P| is |prec|
 * and X is exponent 10 of the value.  If P>X>=-4, then convert with
 * fixed style with P-1-X as |prec|.  Otherwise convert with exponent
 * style with P-1 as |prec|. The trailing zeroes are removed, also the
 * decimal point is removed if no fractional part is left.
 *
 */
int __float80_soft::write(char *buf, uint_fast8_t len, uint_fast8_t prec, char format) const {
    StrBuffer out{buf, len};
    if (isNegative())
        out.letter('-');
    if (isNan() || !isValid()) {
        out.text_P(TEXT_NAN);
    } else if (isInf()) {
        out.text_P(TEXT_INF);
    } else {
        fixed64_t sig;
        int_fast16_t exp10;
        if (format == 'e') {
            ecvt(out, prec);
        } else if (format == 'f') {
            fcvt(out, prec);
        } else {
            if (prec == 0)
                prec = 1;
            auto exp = decompose(sig, exp10);
            const auto ndigits = min_digits(1 + prec, MAX_DECIMAL_DIG);
            uint8_t digits[ndigits];
            if (sig.decompose(exp, digits, ndigits))
                ++exp10;
            if (static_cast<int_fast16_t>(prec) > exp10 && exp10 >= -4) {
                fcvt(out, prec - 1 - exp10, true);
            } else {
                ecvt(out, prec - 1, true);
            }
        }
    }
    return out.len();
}

__float80_soft __float80_soft::compose(bool negative, int_fast16_t exp, fixed64_t &sig) {
    exp = normalize(exp, sig);
    if (exp > EXP_BASE)
        return infinity(negative);
    const auto sigval = sig.value();
    if (sigval == 0)
        return zero(negative);
    if (exp < -EXP_BASE)
        return __float80_soft(negative ? SGN_MASK : 0, UINT64_C(1));
    return bits(negative, exp + EXP_BASE, sigval);
}

__float80_soft __float80_soft::bits(bool sign, uint16_t exp, uint64_t sig) {
    return __float80_soft((sign ? SGN_MASK : 0) | (exp & EXP_MASK), sig);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
