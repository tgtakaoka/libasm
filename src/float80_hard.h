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

#ifndef __LIBASM_FLOAT80_HARD_H__
#define __LIBASM_FLOAT80_HARD_H__

#include <float.h>

#if defined(LDBL_MANT_DIG) && LDBL_MANT_DIG == 64
#define LIBASM_HAS_LONG_DOUBLE
#endif

#ifdef LIBASM_HAS_LONG_DOUBLE

#include <stdlib.h>
#include "float80_base.h"

namespace libasm {

struct __float64;

/** float80_t implementation using hardware 80-bit long double */
struct __float80_hard : __float80_base {
    __float80_hard(uint16_t tag = 0, uint64_t sig = 0) { set(tag, sig); }
    __float80_hard(const __float80_hard &f80) = default;
    __float80_hard &operator=(const __float80_hard &f80) = default;
    __float80_hard(const __float64 &f64) { set(f64); }
    __float80_hard &operator=(const __float64 &f64) { return set(f64); }

    __float80_hard &set(uint16_t tag, uint64_t sig);
    __float80_hard &set(int64_t i64);
    __float80_hard &set(uint64_t u64);
    __float80_hard &set(const __float64 &f64);
    __float64 &get(__float64 &f64) const;

    uint16_t tag() const { return _tag; }
    uint64_t significand() const { return _sig; }
    /** Returns exponent of 2 */
    int_fast16_t exponent() const { return __float80_base::exponent(_tag); }

    __float80_hard operator-() const { return __float80_hard(-_f80); }
    __float80_hard operator+(const __float80_hard &rhs) const {
        return __float80_hard(_f80 + rhs._f80);
    }
    __float80_hard operator-(const __float80_hard &rhs) const {
        return __float80_hard(_f80 - rhs._f80);
    }
    __float80_hard operator*(const __float80_hard &rhs) const {
        return __float80_hard(_f80 * rhs._f80);
    }
    __float80_hard operator/(const __float80_hard &rhs) const {
        return __float80_hard(_f80 / rhs._f80);
    }
    __float80_hard pow10(int_fast16_t n) const;

    bool operator==(const __float80_hard &rhs) const { return _f80 == rhs._f80; }
    bool operator<(const __float80_hard &rhs) const { return _f80 < rhs._f80; }

    bool isValid() const { return __float80_base::isValid(_tag, _sig); }
    bool isZero() const { return __float80_base::isZero(_tag, _sig); }
    bool isSubnormal() const { return __float80_base::isSubnormal(_tag, _sig); }
    bool isInf() const { return __float80_base::isInf(_tag, _sig); }
    bool isNan() const { return __float80_base::isNan(_tag, _sig); }
    bool isNegative() const { return __float80_base::isNegative(_tag); }
    bool isFractional() const { return __float80_base::isFractional(_tag); }

    /**
     * Read floatng point string as |strtold| doing. If the correct
     * |str| would cause overflow, makes it an infinity and returns
     * ERANGE. If the correct |str| would cause underflow, makes it a
     * zero and returns ERANGE.
     */
    int read(const char *str, char **end = nullptr);
    int ecvt(char *buf, uint_fast8_t len, uint_fast8_t prec = 6) const;
    int fcvt(char *buf, uint_fast8_t len, uint_fast8_t prec = 6) const;
    int gcvt(char *buf, uint_fast8_t len, uint_fast8_t prec = 6) const;

    /** Compose zero */
    static __float80_hard zero(bool sign) { return bits(sign, 0, 0); }
    /** Compose infinity */
    static __float80_hard infinity(bool sign = false) { return bits(sign, EXP_MASK, SIG_NORM); }
    /** Compose not a number */
    static __float80_hard not_a_number(bool sign = false, uint64_t sig = 0) {
        return bits(sign, EXP_MASK, UINT64_C(0xC000000000000000));
    }

    /** For testing */
    const char *str() const { return __float80_base::str(_tag, _sig); }

    /**
     * Decompose |this| floating point number into exponent and
     * significand. |sig| is normalized as 1.0 <= |sig| < 2.0.
     */
    int_fast16_t decompose(fixed64_t &sig) const {
        return __float80_base::decompose(_tag, _sig, sig);
    }
    /**
     * Decompose |this| floating point number into exponent and
     * significand.  |sig| is normalized as 1.0 <= |sig | < 10.0, and
     * returns 10 based exponent to |exp10|.
     */
    int_fast16_t decompose(fixed64_t &sig, int_fast16_t &exp10) const {
        return sig.normalize(decompose(sig), exp10);
    }

    /** Normalize and compose __float80_hard */
    static __float80_hard compose(bool negative, int_fast16_t exp, fixed64_t &sig);

private:
    union {
        long double _f80;
        struct {
#if defined(__BYTE_ORDER)
#if __BYTE_ORDER == __BIG_ENDIAN
            uint16_t _tag;
            uint64_t _sig;
#elif __BYTE_ORDER == __LITTLE_ENDIAN
            uint64_t _sig;
            uint16_t _tag;
#else
#error "Unknown __BYTE_ORDER" ## __BYTE_ORDER
#endif
#else
#error "__BYTE_ORDER is undefined"
#endif
        };
    };

    explicit __float80_hard(long double v) : _f80(v) {}

    friend struct __float80_base;

    /** Compose __float80_hard */
    static __float80_hard bits(bool sign, uint16_t exp, uint64_t sig);
};

}  // namespace libasm

#endif

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
