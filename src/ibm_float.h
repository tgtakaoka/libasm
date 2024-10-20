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

#ifndef __LIBASM_IBM_FLOAT_H__
#define __LIBASM_IBM_FLOAT_H__

#include <stdint.h>
#include "error_reporter.h"
#include "float80.h"

#define IBMF32_MAX 7.237005e+75
#define IBMF32_MIN 5.397605e-79
#define IBMF64_MAX 7.237005577332262114e+75
#define IBMF64_MIN 5.397605346934027891e-79

namespace libasm {

struct __ibm_float32 {
    __ibm_float32(uint8_t tag = 0, uint32_t sig = 0) { set(tag, sig); }
    __ibm_float32(const __ibm_float32 &f32) = default;
    __ibm_float32 &operator=(const __ibm_float32 &f32) = default;

    __ibm_float32 &set(uint8_t tag, uint32_t sig);
    __ibm_float32 &set(uint32_t bits);

    Error set(const float80_t &f80);
    explicit operator float80_t() const;

    uint32_t bits() const { return _u32; }
    uint8_t tag() const { return _u32 >> TAG_POS; }
    static uint32_t bits(bool sign, uint8_t exp, uint32_t sig);

    const char *str() const;

    static constexpr auto MANT_DIG = 24;
    static constexpr auto TAG_POS = MANT_DIG;
    static constexpr auto SGN_MASK = UINT8_C(0x80);
    static constexpr auto EXP_MASK = UINT8_C(0x7F);
    static constexpr auto EXP_BASE = INT8_C(0x40);
    static constexpr auto SIG_MASK = (UINT32_C(1) << TAG_POS) - 1;
    static constexpr auto MSD_POS = MANT_DIG - 4;

private:
    // |sign| bexp(7) | frac(24) |
    // sign: 1=negative
    // bexp: biased radix 16 exponent, offset 0x40, 0=zero
    // frac: fraction hexadecimal digits, 1/16 <= frac < 1
    uint32_t _u32;
};

struct __ibm_float64 {
    __ibm_float64(uint8_t tag = 0, uint64_t sig = 0) { set(tag, sig); }
    __ibm_float64(const __ibm_float64 &f64) = default;
    __ibm_float64 &operator=(const __ibm_float64 &f64) = default;

    __ibm_float64 &set(uint8_t tag, uint64_t sig);
    __ibm_float64 &set(uint64_t bits);

    Error set(const float80_t &f80);
    explicit operator float80_t() const;

    uint64_t bits() const { return _u64; }
    uint8_t tag() const { return _u64 >> TAG_POS; }
    static uint64_t bits(bool sign, uint8_t exp, uint64_t sig);

    const char *str() const;

    static constexpr auto MANT_DIG = 56;
    static constexpr auto TAG_POS = MANT_DIG;
    static constexpr auto SGN_MASK = UINT8_C(0x80);
    static constexpr auto EXP_MASK = UINT8_C(0x7F);
    static constexpr auto EXP_BASE = INT8_C(0x40);
    static constexpr auto SIG_MASK = (UINT64_C(1) << TAG_POS) - 1;
    static constexpr auto MSD_POS = MANT_DIG - 4;

private:
    // |sign| bexp(7) | frac(56) |
    // sign: 1=negative
    // bexp: biased radix 16 exponent, offset 0x40, 0=zero
    // frac: fraction hexadecimal digits, 1/16 <= frac < 1
    uint64_t _u64;
};

using ibm_float32_t = __ibm_float32;
using ibm_float64_t = __ibm_float64;

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
