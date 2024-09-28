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

#ifndef __LIBASM_DEC_FLOAT_H__
#define __LIBASM_DEC_FLOAT_H__

#include <stdint.h>
#include "error_reporter.h"
#include "float80.h"

#define DECF16_MAX 1.69e+38
#define DECF16_MIN 2.94e-39
#define DECF32_MAX 1.70141173e+38
#define DECF32_MIN 2.93873588e-39
#define DECF64_MAX 1.70141183460469229371e+38
#define DECF64_MIN 2.93873587705571876992e-39

namespace libasm {

struct __dec_float16 {
    __dec_float16(uint16_t tag = 0, uint8_t sig = 0) { set(tag, sig); }
    __dec_float16(const __dec_float16 &f16) = default;
    __dec_float16 &operator=(const __dec_float16 &f16) = default;

    __dec_float16 &set(uint16_t tag, uint8_t sig);
    __dec_float16 &set(uint16_t bits);

    Error set(const float80_t &f80);
    explicit operator float80_t() const;

    uint16_t bits() const { return _u16; }
    uint16_t tag() const { return _u16 >> TAG_POS; }
    static uint16_t bits(bool sign, uint8_t exp, uint8_t sig);

    const char *str() const;

    static constexpr auto MANT_DIG = 8;
    static constexpr auto HIDDEN_MSB = UINT8_C(1) << (MANT_DIG - 1);
    static constexpr auto TAG_POS = MANT_DIG - 1;
    static constexpr auto SGN_MASK = UINT16_C(0400);
    static constexpr auto EXP_MASK = UINT16_C(0377);
    static constexpr auto EXP_BASE = INT16_C(0200);
    static constexpr auto SIG_MASK = (UINT8_C(1) << TAG_POS) - 1;

private:
    // |sign| bexp(8) | frac(7) |
    // sign: 1=negative
    // bexp: biased exponent, offset 0200, 0=zero
    // frac: fraction, 1/2 <= frac < 1, MSB is hidden
    uint16_t _u16;
};

struct __dec_float32 {
    __dec_float32(uint16_t tag = 0, uint32_t sig = 0) { set(tag, sig); }
    __dec_float32(const __dec_float32 &f32) = default;
    __dec_float32 &operator=(const __dec_float32 &f32) = default;

    __dec_float32 &set(uint16_t tag, uint32_t sig);
    __dec_float32 &set(uint32_t bits);

    Error set(const float80_t &f80);
    explicit operator float80_t() const;

    uint32_t bits() const { return _u32; }
    uint16_t tag() const { return _u32 >> TAG_POS; }
    static uint32_t bits(bool sign, uint8_t exp, uint32_t sig);

    const char *str() const;

    static constexpr auto MANT_DIG = 24;
    static constexpr auto HIDDEN_MSB = UINT32_C(1) << (MANT_DIG - 1);
    static constexpr auto TAG_POS = MANT_DIG - 1;
    static constexpr auto SGN_MASK = UINT16_C(0400);
    static constexpr auto EXP_MASK = UINT16_C(0377);
    static constexpr auto EXP_BASE = INT16_C(0200);
    static constexpr auto SIG_MASK = (UINT32_C(1) << TAG_POS) - 1;

private:
    // |sign| bexp(8) | frac(23) |
    // sign: 1=negative
    // bexp: biased exponent, offset 0200, 0=zero
    // frac: fraction, 1/2 <= frac < 1, MSB is hidden
    uint32_t _u32;
};

struct __dec_float64 {
    __dec_float64(uint16_t tag = 0, uint64_t sig = 0) { set(tag, sig); }
    __dec_float64(const __dec_float64 &f64) = default;
    __dec_float64 &operator=(const __dec_float64 &f64) = default;

    __dec_float64 &set(uint16_t tag, uint64_t sig);
    __dec_float64 &set(uint64_t bits);

    Error set(const float80_t &f80);
    explicit operator float80_t() const;

    uint64_t bits() const { return _u64; }
    uint16_t tag() const { return _u64 >> TAG_POS; }
    static uint64_t bits(bool sign, uint8_t exp, uint64_t sig);

    const char *str() const;

    static constexpr auto MANT_DIG = 56;
    static constexpr auto HIDDEN_MSB = UINT64_C(1) << (MANT_DIG - 1);
    static constexpr auto TAG_POS = MANT_DIG - 1;
    static constexpr auto SGN_MASK = UINT16_C(0400);
    static constexpr auto EXP_MASK = UINT16_C(0377);
    static constexpr auto EXP_BASE = INT16_C(0200);
    static constexpr auto SIG_MASK = (UINT64_C(1) << TAG_POS) - 1;

private:
    // |sign| bexp(8) | frac(55) |
    // sign: 1=negative
    // bexp: biased exponent, offset 0200, 0=zero
    // frac: fraction, 1/2 <= frac < 1, MSB is hidden
    uint64_t _u64;
};

using dec_float16_t = __dec_float16;
using dec_float32_t = __dec_float32;
using dec_float64_t = __dec_float64;

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
