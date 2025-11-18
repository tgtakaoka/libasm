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

#include "ibm_float.h"
#include "fixed64.h"
#include "str_buffer.h"

namespace libasm {

const char *__ibm_float32::str() const {
    static char buf[16];
    StrBuffer out{buf, sizeof(buf)};
    const auto sig = _u32 & SIG_MASK;
    out.hex(tag(), 2).letter('.').hex(sig, 6);
    return buf;
}

__ibm_float32 &__ibm_float32::set(uint8_t tag, uint32_t sig) {
    return set((static_cast<uint32_t>(tag) << TAG_POS) | (sig & SIG_MASK));
}

__ibm_float32 &__ibm_float32::set(uint32_t bits) {
    _u32 = bits;
    return *this;
}

Error __ibm_float32::set(const float80_t &f80) {
    auto error = OK;
    if (f80.isNan())
        error = NOT_AN_EXPECTED;
    if (f80.isInf())
        error = OVERFLOW_RANGE;
    if (f80.isZero() || error) {
        _u32 = bits(f80.isNegative(), 0, 0);
        return error;
    }
    fixed64_t sig;
    // IBM float fraction is [1/16 .. 1.0)
    // IEEE float fraction is [1.0 .. 2.0)
    auto ieee_exp = f80.decompose(sig) + 1;  // +1 for adjust
    ieee_exp += sig.normalize();
    while (ieee_exp % 4) {
        ieee_exp++;
        sig.shift_right(1);
    }
    sig.round_off(MANT_DIG);
    auto ibm_sig = sig.value(MANT_DIG);  // has explicit MSB
    auto ibm_exp = ieee_exp / 4;
    if (ibm_exp >= EXP_BASE) {
        error = OVERFLOW_RANGE;
        ibm_exp = -EXP_BASE;  // ibm_sig = 0
    }
    if (ibm_exp < -EXP_BASE) {
        ibm_exp = -EXP_BASE;
        ibm_sig = 0;
    }
    _u32 = bits(f80.isNegative(), ibm_exp + EXP_BASE, ibm_sig);
    return error;
}

uint32_t __ibm_float32::bits(bool sign, uint8_t exp, uint32_t sig) {
    const auto tag = (sign ? SGN_MASK : 0) | exp;
    return (static_cast<uint32_t>(tag) << TAG_POS) | (sig & SIG_MASK);
}

__ibm_float32::operator float80_t() const {
    const auto tag = static_cast<uint16_t>(_u32 >> TAG_POS);
    const auto sign = (tag & SGN_MASK) != 0;
    const auto bexp = tag & EXP_MASK;
    const auto ibm_exp = static_cast<int16_t>(bexp) - EXP_BASE;
    const auto ibm_sig = (_u32 & SIG_MASK);
    if (bexp == 0 && ibm_sig == 0)
        return float80_t::zero(sign);
    fixed64_t ieee_sig{static_cast<uint64_t>(ibm_sig) << (fixed64_t::DIG - MANT_DIG)};
    // IBM float fraction is [1/16 .. 1.0)
    // IEEE float fraction is [1.0 .. 2.0)
    auto ieee_exp = ibm_exp * 4 - 1;  // -1 for adjust
    ieee_exp += ieee_sig.normalize();
    return float80_t::compose(sign, ieee_exp, ieee_sig);
}

const char *__ibm_float64::str() const {
    static char buf[20];
    StrBuffer out{buf, sizeof(buf)};
    const auto sig = _u64 & SIG_MASK;
    out.hex(tag(), 2)
            .letter('.')
            .hex(static_cast<uint32_t>(sig >> 32), 6)
            .letter('-')
            .hex(static_cast<uint32_t>(sig), 8);
    return buf;
}

__ibm_float64 &__ibm_float64::set(uint8_t tag, uint64_t sig) {
    return set((static_cast<uint64_t>(tag) << TAG_POS) | (sig & SIG_MASK));
}

__ibm_float64 &__ibm_float64::set(uint64_t bits) {
    _u64 = bits;
    return *this;
}

Error __ibm_float64::set(const float80_t &f80) {
    auto error = OK;
    if (f80.isNan())
        error = NOT_AN_EXPECTED;
    if (f80.isInf())
        error = OVERFLOW_RANGE;
    if (f80.isZero() || error) {
        _u64 = bits(f80.isNegative(), 0, 0);
        return error;
    }
    fixed64_t sig;
    // IBM float fraction is [1/16 .. 1.0)
    // IEEE float fraction is [1.0 .. 2.0)
    auto ieee_exp = f80.decompose(sig) + 1;  // +1 for adjust
    while (ieee_exp % 4) {
        ieee_exp++;
        sig.shift_right(1);
    }
    sig.round_off(MANT_DIG);
    auto ibm_sig = sig.value(MANT_DIG);  // has explicit MSB
    auto ibm_exp = ieee_exp / 4;
    if (ibm_exp >= EXP_BASE) {
        error = OVERFLOW_RANGE;
        ibm_exp = -EXP_BASE;  // ibm_sig = 0
    }
    if (ibm_exp < -EXP_BASE) {
        ibm_exp = -EXP_BASE;
        ibm_sig = 0;
    }
    _u64 = bits(f80.isNegative(), ibm_exp + EXP_BASE, ibm_sig);
    return error;
}

uint64_t __ibm_float64::bits(bool sign, uint8_t exp, uint64_t sig) {
    const auto tag = (sign ? SGN_MASK : 0) | exp;
    return (static_cast<uint64_t>(tag) << TAG_POS) | (sig & SIG_MASK);
}

__ibm_float64::operator float80_t() const {
    const auto tag = static_cast<uint16_t>(_u64 >> TAG_POS);
    const auto sign = (tag & SGN_MASK) != 0;
    const auto bexp = tag & EXP_MASK;
    const auto ibm_exp = static_cast<int16_t>(bexp) - EXP_BASE;
    const auto ibm_sig = (_u64 & SIG_MASK);
    if (bexp == 0 && ibm_sig == 0)
        return float80_t::zero(sign);
    fixed64_t ieee_sig{static_cast<uint64_t>(ibm_sig) << (fixed64_t::DIG - MANT_DIG)};
    // IBM float fraction is [1/16 .. 1.0)
    // IEEE float fraction is [1.0 .. 2.0)
    auto ieee_exp = ibm_exp * 4 - 1;  // -1 for adjust
    ieee_exp += ieee_sig.normalize();
    return float80_t::compose(sign, ieee_exp, ieee_sig);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
