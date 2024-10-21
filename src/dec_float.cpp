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

#include "dec_float.h"
#include "fixed64.h"
#include "str_buffer.h"

namespace libasm {

const char *__dec_float16::str() const {
    static char buf[16];
    StrBuffer out{buf, sizeof(buf)};
    const auto sig = _u16 & SIG_MASK;
    out.hex(tag(), 3).letter('.').hex(sig << 1, 2);
    return buf;
}

__dec_float16 &__dec_float16::set(uint16_t tag, uint8_t sig) {
    return set((tag << TAG_POS) | (sig & SIG_MASK));
}

__dec_float16 &__dec_float16::set(uint16_t bits) {
    _u16 = bits;
    return *this;
}

Error __dec_float16::set(const float80_t &f80) {
    auto error = OK;
    if (f80.isNan())
        error = NOT_AN_EXPECTED;
    if (f80.isInf())
        error = OVERFLOW_RANGE;
    if (f80.isZero() || error) {
        _u16 = bits(f80.isNegative(), 0, 0);
        return error;
    }
    fixed64_t sig;
    auto ieee_exp = f80.decompose(sig);
    sig.round_off(MANT_DIG);
    ieee_exp += sig.normalize();
    auto dec_sig = sig.value(MANT_DIG);  // has explicit MSB
    // DEC float fraction is [0.5 .. 1.0)
    // IEEE float fraction is [1.0 .. 2.0)
    auto dec_exp = ieee_exp + 1;
    if (dec_exp >= EXP_BASE) {
        error = OVERFLOW_RANGE;
        dec_exp = -EXP_BASE;  // dec_sig = 0
    }
    if (dec_exp <= -EXP_BASE) {
        dec_exp = -EXP_BASE;
        dec_sig = 0;
    }
    _u16 = bits(f80.isNegative(), dec_exp + EXP_BASE, dec_sig);
    return error;
}

uint16_t __dec_float16::bits(bool sign, uint8_t exp, uint8_t sig) {
    const auto tag = (sign ? SGN_MASK : 0) | exp;
    return (tag << TAG_POS) | (sig & SIG_MASK);
}

__dec_float16::operator float80_t() const {
    const auto tag = static_cast<uint16_t>(_u16 >> TAG_POS);
    const auto sign = (tag & SGN_MASK) != 0;
    const auto bexp = tag & EXP_MASK;
    if (bexp == 0)
        return float80_t::zero(sign);
    const auto frac = (_u16 & SIG_MASK) | HIDDEN_MSB;
    const auto exp = static_cast<int16_t>(bexp) - EXP_BASE - 1;  // -1 for HIDDEN_MSB
    fixed64_t sig{static_cast<uint64_t>(frac) << (64 - MANT_DIG)};
    return float80_t::compose(sign, exp, sig);
}

const char *__dec_float32::str() const {
    static char buf[16];
    StrBuffer out{buf, sizeof(buf)};
    const auto sig = _u32 & SIG_MASK;
    out.hex(tag(), 3).letter('.').hex(sig << 1, 6);
    return buf;
}

__dec_float32 &__dec_float32::set(uint16_t tag, uint32_t sig) {
    return set((static_cast<uint32_t>(tag) << TAG_POS) | (sig & SIG_MASK));
}

__dec_float32 &__dec_float32::set(uint32_t bits) {
    _u32 = bits;
    return *this;
}

Error __dec_float32::set(const float80_t &f80) {
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
    auto ieee_exp = f80.decompose(sig);
    sig.round_off(MANT_DIG);
    ieee_exp += sig.normalize();
    auto dec_sig = sig.value(MANT_DIG);  // has explicit MSB
    // DEC float fraction is [0.5 .. 1.0)
    // IEEE float fraction is [1.0 .. 2.0)
    auto dec_exp = ieee_exp + 1;
    if (dec_exp >= EXP_BASE) {
        error = OVERFLOW_RANGE;
        dec_exp = -EXP_BASE;  // dec_sig = 0
    }
    if (dec_exp <= -EXP_BASE) {
        dec_exp = -EXP_BASE;
        dec_sig = 0;
    }
    _u32 = bits(f80.isNegative(), dec_exp + EXP_BASE, dec_sig);
    return error;
}

uint32_t __dec_float32::bits(bool sign, uint8_t exp, uint32_t sig) {
    const auto tag = (sign ? SGN_MASK : 0) | exp;
    return (static_cast<uint32_t>(tag) << TAG_POS) | (sig & SIG_MASK);
}

__dec_float32::operator float80_t() const {
    const auto tag = static_cast<uint16_t>(_u32 >> TAG_POS);
    const auto sign = (tag & SGN_MASK) != 0;
    const auto bexp = tag & EXP_MASK;
    if (bexp == 0)
        return float80_t::zero(sign);
    const auto frac = (_u32 & SIG_MASK) | HIDDEN_MSB;
    const auto exp = static_cast<int16_t>(bexp) - EXP_BASE - 1;  // -1 for HIDDEN_MSB
    fixed64_t sig{static_cast<uint64_t>(frac) << (64 - MANT_DIG)};
    return float80_t::compose(sign, exp, sig);
}

const char *__dec_float64::str() const {
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

__dec_float64 &__dec_float64::set(uint16_t tag, uint64_t sig) {
    return set((static_cast<uint64_t>(tag) << TAG_POS) | (sig & SIG_MASK));
}

__dec_float64 &__dec_float64::set(uint64_t bits) {
    _u64 = bits;
    return *this;
}

Error __dec_float64::set(const float80_t &f80) {
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
    auto ieee_exp = f80.decompose(sig);
    sig.round_off(MANT_DIG);
    ieee_exp += sig.normalize();
    auto dec_sig = sig.value(MANT_DIG);  // has explicit MSB
    // DEC float fraction is [0.5 .. 1.0)
    // IEEE float fraction is [1.0 .. 2.0)
    auto dec_exp = ieee_exp + 1;
    if (dec_exp >= EXP_BASE) {
        error = OVERFLOW_RANGE;
        dec_exp = -EXP_BASE;  // dec_sig = 0
    }
    if (dec_exp <= -EXP_BASE) {
        dec_exp = -EXP_BASE;
        dec_sig = 0;
    }
    _u64 = bits(f80.isNegative(), dec_exp + EXP_BASE, dec_sig);
    return error;
}

uint64_t __dec_float64::bits(bool sign, uint8_t exp, uint64_t sig) {
    const auto tag = (sign ? SGN_MASK : 0) | exp;
    return (static_cast<uint64_t>(tag)) << TAG_POS | (sig & SIG_MASK);
}

__dec_float64::operator float80_t() const {
    const auto tag = static_cast<uint16_t>(_u64 >> TAG_POS);
    const auto sign = (tag & SGN_MASK) != 0;
    const auto bexp = tag & EXP_MASK;
    if (bexp == 0)
        return float80_t::zero(sign);
    const auto frac = (_u64 & SIG_MASK) | HIDDEN_MSB;
    const auto exp = static_cast<int16_t>(bexp) - EXP_BASE - 1;  // -1 for HIDDEN_MSB
    fixed64_t sig{static_cast<uint64_t>(frac) << (64 - MANT_DIG)};
    return float80_t::compose(sign, exp, sig);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
