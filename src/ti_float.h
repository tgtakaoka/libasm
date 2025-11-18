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

#ifndef __LIBASM_TI_FLOAT_H__
#define __LIBASM_TI_FLOAT_H__

#include <stdint.h>
#include "error_reporter.h"
#include "float80.h"

#define TI_FLT16_MAX_POS 2.5594e+02
#define TI_FLT16_MIN_POS 7.8125e-03
#define TI_FLT16_MIN_NEG -7.8163e-03
#define TI_FLT16_MAX_NEG -2.5600e+02
#define TI_FLT32_MAX_POS 3.402823466e+38
#define TI_FLT32_MIN_POS 5.877471754e-39
#define TI_FLT32_MAX_NEG -3.402823669e+38
#define TI_FLT32_MIN_NEG -5.877472455e-39
#define TI_FLT40_MAX_POS 3.4028236684e+38
#define TI_FLT40_MIN_POS 5.8774717541e-39
#define TI_FLT40_MAX_NEG -3.4028236692e+38
#define TI_FLT40_MIN_NEG -5.8774717568e-39
#define TI_FLT16EXT_MAX_POS 3.3895314e+38
#define TI_FLT16EXT_MIN_POS 5.8774718e-39
#define TI_FLT16EXT_MIN_NEG -5.9233895e-39
#define TI_FLT16EXT_MAX_NEG -3.4028237e+38

namespace libasm {

/**
 * TMS320C3x Short Floating-Point Format
 */
struct __ti_float16 {
    __ti_float16(int8_t exp = EXP_ZERO, uint16_t frac = 0) { set(exp, frac); }
    __ti_float16(const __ti_float16 &f16) = default;
    __ti_float16 &operator=(const __ti_float16 &f16) = default;

    __ti_float16 &set(int8_t exp, uint16_t frac);
    __ti_float16 &set(uint16_t bits);

    Error set(const float80_t &f80);
    explicit operator float80_t() const;

    uint16_t bits() const { return _u16; }
    int8_t exp() const { return static_cast<int16_t>(_u16) >> EXP_POS; }
    static uint16_t bits(bool sign, int8_t exp, uint16_t frac);

    const char *str() const;

    static constexpr auto EXP_POS = 12;
    static constexpr auto MANT_DIG = 12;
    static constexpr auto EXP_MAX = 7;
    static constexpr auto EXP_MIN = -7;
    static constexpr auto EXP_ZERO = -8;
    static constexpr auto EXP_MASK = UINT16_C(0xF000);
    static constexpr auto SGN_MASK = UINT16_C(0x0800);
    static constexpr auto FRAC_MASK = UINT16_C(0x07FF);

private:
    // | exp(4) | sign | frac(11) |
    // sign: 1=negative
    // exp: 2s-complement exponent, zero=-8
    // frac: MSB is hidden
    // value = ((-2)^sign + 0.frac) x 2^exp
    uint16_t _u16;
};

/**
 * TMS320C3x Single-Precision Floating-Point Format
 */
struct __ti_float32 {
    __ti_float32(int8_t exp = EXP_ZERO, uint32_t frac = 0) { set(exp, frac); }
    __ti_float32(const __ti_float32 &f32) = default;
    __ti_float32 &operator=(const __ti_float32 &f32) = default;

    __ti_float32 &set(int8_t exp, uint32_t frac);
    __ti_float32 &set(uint32_t bits);

    Error set(const float80_t &f80);
    explicit operator float80_t() const;

    uint32_t bits() const { return _u32; }
    int8_t exp() const { return static_cast<int32_t>(_u32) >> EXP_POS; }
    static uint32_t bits(bool sign, int8_t exp, uint32_t frac);

    const char *str() const;

    static constexpr auto EXP_POS = 24;
    static constexpr auto MANT_DIG = 24;
    static constexpr auto EXP_MAX = 127;
    static constexpr auto EXP_MIN = -127;
    static constexpr auto EXP_ZERO = -128;
    static constexpr auto EXP_MASK = UINT32_C(0xFF000000);
    static constexpr auto SGN_MASK = UINT32_C(0x00800000);
    static constexpr auto FRAC_MASK = UINT32_C(0x007FFFFF);

private:
    // | exp(8) | sign | frac(23) |
    // sign: 1=negative
    // exp: 2s-complement exponent, zero=-128
    // frac: MSB is hidden, sign=0: 01.frac x 2^exp, sign=1: 10.frac x 2^exp
    uint32_t _u32;
};

/**
 * TMS320C3x Extended-Precision Floating-Point Format
 */
struct __ti_float40 {
    __ti_float40(int8_t exp = EXP_ZERO, uint32_t frac = 0) { set(exp, frac); }
    __ti_float40(const __ti_float40 &f40) = default;
    __ti_float40 &operator=(const __ti_float40 &f40) = default;

    __ti_float40 &set(int8_t exp, uint32_t frac);
    __ti_float40 &set(uint64_t bits);

    Error set(const float80_t &f80);
    explicit operator float80_t() const;

    uint64_t bits() const { return _u40; }
    int8_t exp() const { return static_cast<int64_t>(_u40) >> EXP_POS; }
    static uint64_t bits(bool sign, int8_t exp, uint32_t frac);

    const char *str() const;

    static constexpr auto EXP_POS = 32;
    static constexpr auto MANT_DIG = 32;
    static constexpr auto EXP_MAX = 127;
    static constexpr auto EXP_MIN = -127;
    static constexpr auto EXP_ZERO = -128;
    static constexpr auto EXP_MASK = UINT64_C(0xFF00000000);
    static constexpr auto SGN_MASK = UINT64_C(0x0080000000);
    static constexpr auto FRAC_MASK = UINT64_C(0x007FFFFFFF);

private:
    // | exp(8) | sign | frac(31) |
    // sign: 1=negative
    // exp: 2s-complement exponent, zero=-128
    // frac: MSB is hidden, sign=0: 01.frac x 2^exp, sign=1: 10.frac x 2^exp
    uint64_t _u40;
};

/**
 * TMS320C32 Short Floating-Point Format for External 16-Bit Data
 */
struct __ti_float16_external {
    __ti_float16_external(int8_t exp = EXP_ZERO, uint8_t frac = 0) { set(exp, frac); }
    __ti_float16_external(const __ti_float16_external &f16) = default;
    __ti_float16_external &operator=(const __ti_float16_external &f16) = default;

    __ti_float16_external &set(int8_t exp, uint8_t frac);
    __ti_float16_external &set(uint16_t bits);

    Error set(const float80_t &f80);
    explicit operator float80_t() const;

    uint16_t bits() const { return _u16; }
    int8_t exp() const { return static_cast<int16_t>(_u16) >> EXP_POS; }
    static uint16_t bits(bool sign, int8_t exp, uint8_t frac);

    const char *str() const;

    static constexpr auto EXP_POS = 8;
    static constexpr auto MANT_DIG = 8;
    static constexpr auto EXP_MAX = 127;
    static constexpr auto EXP_MIN = -127;
    static constexpr auto EXP_ZERO = -128;
    static constexpr auto EXP_MASK = UINT16_C(0xFF00);
    static constexpr auto SGN_MASK = UINT16_C(0x0080);
    static constexpr auto FRAC_MASK = UINT16_C(0x007F);

private:
    // | exp(8) | sign | frac(7) |
    // sign: 1=negative
    // exp: 2s-complement exponent, zero=-128
    // frac: MSB is hidden, sign=0: 01.frac x 2^exp, sign=1: 10.frac x 2^exp
    uint16_t _u16;
};

using ti_float16_t = __ti_float16;
using ti_float32_t = __ti_float32;
using ti_float40_t = __ti_float40;
using ti_float16_external_t = __ti_float16_external;

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
