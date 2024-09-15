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

#ifndef __LIBASM_FLOAT80_BASE_H__
#define __LIBASM_FLOAT80_BASE_H__

#include <stdint.h>
#include "fixed64.h"

namespace libasm {

/** Base class for float80_t */
struct __float80_base {
protected:
    static constexpr auto MANT_DIG = 64;
    static constexpr auto MAX_DECIMAL_DIG = 36;  // MANT_DIG=113
    static constexpr auto SGN_MASK = UINT16_C(0x8000);
    static constexpr auto EXP_MASK = UINT16_C(0x7FFF);
    static constexpr auto EXP_BASE = INT16_C(0x3FFF);
    static constexpr auto SIG_NORM = UINT64_C(0x8000000000000000);
    static constexpr auto SIG_BITS = UINT64_C(0x7FFFFFFFFFFFFFFF);

    static int_fast16_t exponent(uint16_t tag) { return (tag & EXP_MASK) - EXP_BASE; }

    static const char *str(uint16_t tag, uint64_t sig);
    static bool isValid(uint16_t tag, uint64_t sig) {
        // (Zero || Subnormal) || (Normal || Inf || Nan)
        return (tag & EXP_MASK) == 0 || (sig & SIG_NORM) != 0;
    }
    static bool isZero(uint16_t tag, uint64_t sig) { return (tag & EXP_MASK) == 0 && sig == 0; }
    static bool isSubnormal(uint16_t tag, uint64_t sig) {
        return (tag & EXP_MASK) == 0 && sig != 0;
    }
    static bool isInf(uint16_t tag, uint64_t sig) {
        return (~tag & EXP_MASK) == 0 && (sig & SIG_BITS) == 0;
    }
    static bool isNan(uint16_t tag, uint64_t sig) {
        return (~tag & EXP_MASK) == 0 && (sig & SIG_BITS) != 0;
    }
    static bool isNegative(uint16_t tag) { return tag & SGN_MASK; }
    static bool isFractional(uint16_t tag) { return !isNegative(tag) && exponent(tag) < 0; }

    /**
     * Normalize |exp| and |sig| floating point number, returns
     * normalized |exp|.
     */
    static int_fast16_t normalize(int_fast16_t exp, fixed64_t &sig);

    /**
     * Decompose |tag| and |significand| floating point number into
     * exponent and significand. |sig| is normalized as 1.0 <= |sig| <
     * 2.0.
     */
    static int_fast16_t decompose(uint16_t tag, uint64_t significand, fixed64_t &sig);
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
