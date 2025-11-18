/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "ti_float.h"
#include "fixed64.h"
#include "str_buffer.h"

#include <stdio.h>

namespace libasm {

const char *__ti_float16::str() const {
    static char buf[16];
    StrBuffer out{buf, sizeof(buf)};
    const auto frac = _u16 & (SGN_MASK | FRAC_MASK);
    const auto exp = (_u16 & EXP_MASK) >> EXP_POS;
    out.hex(exp, 1).letter('.').hex(frac, 3);
    return buf;
}

__ti_float16 &__ti_float16::set(int8_t exp, uint16_t frac) {
    return set((exp << EXP_POS) | (frac & (SGN_MASK | FRAC_MASK)));
}

__ti_float16 &__ti_float16::set(uint16_t bits) {
    _u16 = bits;
    return *this;
}

Error __ti_float16::set(const float80_t &f80) {
    auto error = OK;
    if (f80.isNan())
        error = NOT_AN_EXPECTED;
    if (f80.isInf())
        error = OVERFLOW_RANGE;
    if (f80.isZero() || error) {
        _u16 = bits(false, EXP_ZERO, 0);
        return error;
    }
    printf("## f80=%s\n", f80.str());
    fixed64_t sig;
    auto exp = f80.decompose(sig);
    printf("##  decompose=%s exp=%ld\n", sig.str(), exp);
    const auto sign = f80.isNegative();
    if (sign) {
        //sig.complement();
        // printf("## complement=%s\n", sig.str());
    }
    sig.round_off(MANT_DIG);
    printf("##      round=%s\n", sig.str());
    exp += sig.normalize();
    printf("##  normalize=%s exp=%ld\n", sig.str(), exp);
    const auto frac = sig.value(MANT_DIG);
    if (exp > EXP_MAX) {
        error = OVERFLOW_RANGE;
        exp = EXP_MAX;
    }
    if (exp < EXP_MIN) {
        exp = EXP_ZERO;
    }
    _u16 = bits(sign, exp, frac);
    return error;
}

uint16_t __ti_float16::bits(bool sign, int8_t exp, uint16_t frac) {
    return (exp << EXP_POS) | (sign ? SGN_MASK : 0) | (frac & FRAC_MASK);
}

__ti_float16::operator float80_t() const {
    if (_u16 == bits(false, EXP_ZERO, 0))
        return float80_t::zero(false);
    const auto sign = (_u16 & SGN_MASK) != 0;
    auto frac = _u16 & FRAC_MASK;
    if (sign)
        frac = (-frac & FRAC_MASK);
    frac |= SGN_MASK;  // hidden bit
    printf("@@ u16=%04X exp=%d sign=%d frac=%04X\n", _u16, exp(), sign, frac);
    fixed64_t sig{static_cast<uint64_t>(frac)};
    auto bexp = exp() + 64 - MANT_DIG;
    printf("@@ bexp=%d sig=%s\n", bexp, sig.str());
    bexp += sig.normalize();
    printf("@@ bexp=%d sig=%s\n", bexp, sig.str());
    return float80_t::compose(sign, bexp, sig);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
