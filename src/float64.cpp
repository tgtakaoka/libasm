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

#include "float64.h"
#include "fixed64.h"
#include "str_buffer.h"

namespace libasm {

__float64 &__float64::set(uint64_t bits) {
    _u64 = bits;
    return *this;
}

__float64 &__float64::set(uint16_t tag, uint64_t sig) {
    return set((static_cast<uint64_t>(tag) << TAG_POS) | (sig & SIG_MASK));
}

const char *__float64::str() const {
    static char buf[40];
    StrBuffer out{buf, sizeof(buf)};
    const auto sig = _u64 & SIG_MASK;
    out.hex(tag(), 3)
            .letter('.')
            .hex(static_cast<uint32_t>(sig >> 32), 5)
            .letter('-')
            .hex(static_cast<uint32_t>(sig));
    return buf;
}

uint64_t __float64::significand() const {
    auto sig = _u64 & SIG_MASK;
    const auto t = tag();
    if ((~t & EXP_MASK) == 0)
        return sig;
    if (t & EXP_MASK)
        sig |= HIDDEN_MSB;
    sig <<= 64 - MANT_DIG;
    return sig;
}

int_fast16_t __float64::exponent() const {
    return (tag() & EXP_MASK) - EXP_BASE;
}

bool __float64::isZero() const {
    return (tag() & EXP_MASK) == 0 && significand() == 0;
}

bool __float64::isSubnormal() const {
    return (tag() & EXP_MASK) == 0 && significand() != 0;
}

bool __float64::isNegative() const {
    return tag() & SGN_MASK;
}

bool __float64::isFractional() const {
    return !isNegative() && exponent() < 0;
}

bool __float64::isInf() const {
    return (~tag() & EXP_MASK) == 0 && significand() == 0;
}

bool __float64::isNan() const {
    return (~tag() & EXP_MASK) == 0 && significand() != 0;
}

int_fast16_t __float64::decompose(fixed64_t &sig) const {
    sig.set(significand());
    if (isZero())
        return 0;
    if (sig.normal())
        return exponent();
    return sig.normalize() + (1 - EXP_BASE);
}

int_fast16_t __float64::normalize(int_fast16_t exp, fixed64_t &sig) {
    exp += sig.normalize();
    if (exp <= -EXP_BASE) {
        const auto shifts = -(exp + EXP_BASE) + 1;
        sig.shift_right(shifts, true);
        exp = -EXP_BASE;  // for subnormal mark
        sig.round_off(MANT_DIG);
        if (sig.normal())
            exp++;  // Overflow subnormal, increase exponent
    } else if (sig.round_off(MANT_DIG)) {
        exp += sig.normalize();  // Overflowe, increase exponent
    }
    return exp;
}

__float64 __float64::compose(bool negative, int_fast16_t exp, fixed64_t &sig) {
    exp = normalize(exp, sig);
    if (exp > EXP_BASE)
        return infinity(negative);
    auto sigval = sig.value();
    if (sigval == 0)
        return zero(negative);
    if (exp < -EXP_BASE)
        return __float64(negative ? SGN_MASK : 0, UINT64_C(1));
    sigval <<= 1;  // discard hidden MSB
    sigval >>= 64 - TAG_POS;
    return bits(negative, exp + EXP_BASE, sigval);
}

__float64 __float64::bits(bool sign, uint16_t exp, uint64_t sig) {
    return __float64((sign ? SGN_MASK : 0) | (exp & EXP_MASK), sig);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
