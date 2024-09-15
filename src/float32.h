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

#ifndef __LIBASM_FLOAT32_H__
#define __LIBASM_FLOAT32_H__

#include <stdint.h>
#include "fixed64.h"
#include "float80.h"

namespace libasm {

/** float32_t data struct */
struct __float32 {
    __float32(uint16_t tag = 0, uint32_t sig = 0) { set(tag, sig); }
    __float32(const __float32 &f32) = default;
    __float32 &operator=(const __float32 &f32) = default;
#ifdef LIBASM_HAS_LONG_DOUBLE
    __float32(const __float80_hard &f80) { set(f80); }
    __float32 &operator=(const __float80_hard &f80) { return set(f80); }
    __float32 &set(const __float80_hard &f80) { return f80.get(*this); }
#endif
    __float32(const __float80_soft &f80) { set(f80); }
    __float32 &operator=(const __float80_soft &f80) { return set(f80); }
    __float32 &set(const __float80_soft &f80) { return f80.get(*this); }

    __float32 &set(uint16_t tag, uint32_t sig);
    __float32 &set(uint32_t bits);

    uint32_t bits() const { return _u32; }
    uint16_t tag() const { return _u32 >> TAG_POS; }
    uint32_t significand() const;
    int_fast16_t exponent() const;

    bool isZero() const;        // +0/-0
    bool isNegative() const;    // < 0
    bool isFractional() const;  // < 1
    bool isInf() const;
    bool isNan() const;
    bool isSubnormal() const;

    /** Compose zero */
    static __float32 zero(bool sign) { return bits(sign, 0, 0); }
    /** Compose infinity */
    static __float32 infinity(bool sign = false) { return bits(sign, EXP_MASK, 0); }
    /** Compose not a number */
    static __float32 not_a_number(bool sign = false, uint32_t sig = 0) {
        return bits(sign, EXP_MASK, UINT32_C(1) << (TAG_POS - 1) | sig);
    }

    /** For testing */
    const char *str() const;

    /** Decompose __float64_soft into exponent and significand */
    int_fast16_t decompose(fixed64_t &sig) const;
    /** Normalize and compose __float64_soft */
    static __float32 compose(bool negative, int_fast16_t exp, fixed64_t &sig);

private:
    uint32_t _u32;

    static __float32 bits(bool sign, uint16_t exp, uint32_t sig);
    static int_fast16_t normalize(int_fast16_t exp, fixed64_t &sig);

    static constexpr auto MANT_DIG = 24;
    static constexpr auto HIDDEN_MSB = UINT32_C(1) << (MANT_DIG - 1);
    static constexpr auto TAG_POS = MANT_DIG - 1;  // 1 for hidden MSB
    static constexpr auto SGN_MASK = UINT16_C(0x100);
    static constexpr auto EXP_MASK = UINT16_C(0x0FF);
    static constexpr auto EXP_BASE = INT16_C(0x07F);
    static constexpr auto SIG_MASK = (UINT32_C(1) << TAG_POS) - 1;
};

using float32_t = __float32;

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
