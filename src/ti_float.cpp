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
    fixed64_t sig;
    auto exp = f80.decompose(sig);
    sig.round_off(MANT_DIG);
    exp += sig.normalize();
    auto frac = sig.value(MANT_DIG);
    if (f80.isNegative()) {
        frac = (SGN_MASK << 1) - frac;  // 2-frac
        if ((frac & FRAC_MASK) == 0)
            exp--;
    } else {
        frac &= ~SGN_MASK;  // clear explicit MSB
    }
    if (exp > EXP_MAX) {
        _u16 = bits(false, EXP_MAX, FRAC_MASK);
        error = OVERFLOW_RANGE;
    } else if (exp < EXP_MIN) {
        _u16 = bits(false, EXP_ZERO, 0);
        error = UNDERFLOW_RANGE;
    } else {
        _u16 = bits(f80.isNegative(), exp, frac);
    }
    return error;
}

uint16_t __ti_float16::bits(bool sign, int8_t exp, uint16_t frac) {
    return (exp << EXP_POS) | (sign ? SGN_MASK : 0) | (frac & FRAC_MASK);
}

__ti_float16::operator float80_t() const {
    if (_u16 == bits(false, EXP_ZERO, 0))
        return float80_t::zero(false);
    const auto sign = (_u16 & SGN_MASK) != 0;
    uint16_t frac = _u16 & FRAC_MASK;
    if (sign) {
        // fraction's absolute value by negating frac via 2's complememt
        frac = -static_cast<int16_t>(frac | EXP_MASK);
    } else {
        frac |= SGN_MASK;  // set hidden MSB
    }
    fixed64_t sig{frac};
    auto bexp = exp() + fixed64_t::DIG - MANT_DIG;
    bexp += sig.normalize();
    return float80_t::compose(sign, bexp, sig);
}

const char *__ti_float32::str() const {
    static char buf[16];
    StrBuffer out{buf, sizeof(buf)};
    const auto frac = _u32 & (SGN_MASK | FRAC_MASK);
    const auto exp = (_u32 & EXP_MASK) >> EXP_POS;
    out.hex(exp, 2).letter('.').hex(frac, 6);
    return buf;
}

__ti_float32 &__ti_float32::set(int8_t exp, uint32_t frac) {
    return set((exp << EXP_POS) | (frac & (SGN_MASK | FRAC_MASK)));
}

__ti_float32 &__ti_float32::set(uint32_t bits) {
    _u32 = bits;
    return *this;
}

Error __ti_float32::set(const float80_t &f80) {
    auto error = OK;
    if (f80.isNan())
        error = NOT_AN_EXPECTED;
    if (f80.isInf())
        error = OVERFLOW_RANGE;
    if (f80.isZero() || error) {
        _u32 = bits(false, EXP_ZERO, 0);
        return error;
    }
    fixed64_t sig;
    auto exp = f80.decompose(sig);
    sig.round_off(MANT_DIG);
    exp += sig.normalize();
    auto frac = sig.value(MANT_DIG);
    if (f80.isNegative()) {
        frac = (SGN_MASK << 1) - frac;  // 2-frac
        if ((frac & FRAC_MASK) == 0)
            exp--;
    } else {
        frac &= ~SGN_MASK;  // clear explicit MSB
    }
    if (exp > EXP_MAX) {
        _u32 = bits(false, EXP_MAX, FRAC_MASK);
        error = OVERFLOW_RANGE;
    } else if (exp < EXP_MIN) {
        _u32 = bits(false, EXP_ZERO, 0);
        error = UNDERFLOW_RANGE;
    } else {
        _u32 = bits(f80.isNegative(), exp, frac);
    }
    return error;
}

uint32_t __ti_float32::bits(bool sign, int8_t exp, uint32_t frac) {
    return (exp << EXP_POS) | (sign ? SGN_MASK : 0) | (frac & FRAC_MASK);
}

__ti_float32::operator float80_t() const {
    if (_u32 == bits(false, EXP_ZERO, 0))
        return float80_t::zero(false);
    const auto sign = (_u32 & SGN_MASK) != 0;
    uint32_t frac = _u32 & FRAC_MASK;
    if (sign) {
        // fraction's absolute value by negating frac via 2's complememt
        frac = -static_cast<int32_t>(frac | EXP_MASK);
    } else {
        frac |= SGN_MASK;  // set hidden MSB
    }
    fixed64_t sig{frac};
    auto bexp = exp() + fixed64_t::DIG - MANT_DIG;
    bexp += sig.normalize();
    return float80_t::compose(sign, bexp, sig);
}

const char *__ti_float40::str() const {
    static char buf[16];
    StrBuffer out{buf, sizeof(buf)};
    const auto frac = _u40 & (SGN_MASK | FRAC_MASK);
    const auto exp = (_u40 & EXP_MASK) >> EXP_POS;
    out.hex(exp, 2).letter('.').hex(frac, 8);
    return buf;
}

__ti_float40 &__ti_float40::set(int8_t exp, uint32_t frac) {
    return set((static_cast<uint64_t>(exp) << EXP_POS) | (frac & (SGN_MASK | FRAC_MASK)));
}

__ti_float40 &__ti_float40::set(uint64_t bits) {
    _u40 = bits;
    return *this;
}

Error __ti_float40::set(const float80_t &f80) {
    auto error = OK;
    if (f80.isNan())
        error = NOT_AN_EXPECTED;
    if (f80.isInf())
        error = OVERFLOW_RANGE;
    if (f80.isZero() || error) {
        _u40 = bits(false, EXP_ZERO, 0);
        return error;
    }
    fixed64_t sig;
    auto exp = f80.decompose(sig);
    sig.round_off(MANT_DIG);
    exp += sig.normalize();
    auto frac = sig.value(MANT_DIG);
    if (f80.isNegative()) {
        frac = (SGN_MASK << 1) - frac;  // 2-frac
        if ((frac & FRAC_MASK) == 0)
            exp--;
    } else {
        frac &= ~SGN_MASK;  // clear explicit MSB
    }
    if (exp > EXP_MAX) {
        _u40 = bits(false, EXP_MAX, FRAC_MASK);
        error = OVERFLOW_RANGE;
    } else if (exp < EXP_MIN) {
        _u40 = bits(false, EXP_ZERO, 0);
        error = UNDERFLOW_RANGE;
    } else {
        _u40 = bits(f80.isNegative(), exp, frac);
    }
    return error;
}

uint64_t __ti_float40::bits(bool sign, int8_t exp, uint32_t frac) {
    return (static_cast<uint64_t>(exp) << EXP_POS) | (sign ? SGN_MASK : 0) | (frac & FRAC_MASK);
}

__ti_float40::operator float80_t() const {
    if (_u40 == bits(false, EXP_ZERO, 0))
        return float80_t::zero(false);
    const auto sign = (_u40 & SGN_MASK) != 0;
    uint64_t frac = _u40 & FRAC_MASK;
    if (sign) {
        // fraction's absolute value by negating frac via 2's complememt
        frac |= UINT64_C(0xFFFFFFFF) << 32;
        frac = -static_cast<int64_t>(frac);
    } else {
        frac |= SGN_MASK;  // set hidden MSB
    }
    fixed64_t sig{frac};
    auto bexp = exp() + fixed64_t::DIG - MANT_DIG;
    bexp += sig.normalize();
    return float80_t::compose(sign, bexp, sig);
}

const char *__ti_float16_external::str() const {
    static char buf[16];
    StrBuffer out{buf, sizeof(buf)};
    const auto frac = _u16 & (SGN_MASK | FRAC_MASK);
    const auto exp = (_u16 & EXP_MASK) >> EXP_POS;
    out.hex(exp, 2).letter('.').hex(frac, 2);
    return buf;
}

__ti_float16_external &__ti_float16_external::set(int8_t exp, uint8_t frac) {
    return set((exp << EXP_POS) | (frac & (SGN_MASK | FRAC_MASK)));
}

__ti_float16_external &__ti_float16_external::set(uint16_t bits) {
    _u16 = bits;
    return *this;
}

Error __ti_float16_external::set(const float80_t &f80) {
    auto error = OK;
    if (f80.isNan())
        error = NOT_AN_EXPECTED;
    if (f80.isInf())
        error = OVERFLOW_RANGE;
    if (f80.isZero() || error) {
        _u16 = bits(false, EXP_ZERO, 0);
        return error;
    }
    fixed64_t sig;
    auto exp = f80.decompose(sig);
    sig.round_off(MANT_DIG);
    exp += sig.normalize();
    auto frac = sig.value(MANT_DIG);
    if (f80.isNegative()) {
        frac = (SGN_MASK << 1) - frac;  // 2-frac
        if ((frac & FRAC_MASK) == 0)
            exp--;
    } else {
        frac &= ~SGN_MASK;  // clear explicit MSB
    }
    if (exp > EXP_MAX) {
        _u16 = bits(false, EXP_MAX, FRAC_MASK);
        error = OVERFLOW_RANGE;
    } else if (exp < EXP_MIN) {
        _u16 = bits(false, EXP_ZERO, 0);
        error = UNDERFLOW_RANGE;
    } else {
        _u16 = bits(f80.isNegative(), exp, frac);
    }
    return error;
}

uint16_t __ti_float16_external::bits(bool sign, int8_t exp, uint8_t frac) {
    return (exp << EXP_POS) | (sign ? SGN_MASK : 0) | (frac & FRAC_MASK);
}

__ti_float16_external::operator float80_t() const {
    if (_u16 == bits(false, EXP_ZERO, 0))
        return float80_t::zero(false);
    const auto sign = (_u16 & SGN_MASK) != 0;
    uint16_t frac = _u16 & FRAC_MASK;
    if (sign) {
        // fraction's absolute value by negating frac via 2's complememt
        frac = -static_cast<int16_t>(frac | EXP_MASK);
    } else {
        frac |= SGN_MASK;  // set hidden MSB
    }
    fixed64_t sig{frac};
    auto bexp = exp() + fixed64_t::DIG - MANT_DIG;
    bexp += sig.normalize();
    return float80_t::compose(sign, bexp, sig);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
