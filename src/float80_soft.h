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

#ifndef __LIBASM_FLOAT80_SOFT_H__
#define __LIBASM_FLOAT80_SOFT_H__

#include <stdint.h>
#include "float80_base.h"

namespace libasm {

struct __float64;
struct StrBuffer;
struct StrScanner;

/** float80_t implementation using software emulation */
struct __float80_soft : __float80_base {
    __float80_soft(uint16_t tag = 0, uint64_t sig = 0) { set(tag, sig); }
    __float80_soft(const __float80_soft &f80) = default;
    __float80_soft &operator=(const __float80_soft &f80) = default;
    __float80_soft(const __float64 &f64) { set(f64); }
    __float80_soft &operator=(const __float64 &f64) { return set(f64); }

    __float80_soft &set(uint16_t tag, uint64_t sig);
    __float80_soft &set(int64_t i64);
    __float80_soft &set(uint64_t u64);
    __float80_soft &set(const __float64 &f64);
    __float64 &get(__float64 &f64) const;
    
    uint16_t tag() const { return _tag; }
    uint64_t significand() const { return _sig; }
    int_fast16_t exponent() const { return __float80_base::exponent(_tag); }

    __float80_soft operator-() const { return neg(*this); }
    __float80_soft operator+(const __float80_soft &rhs) const { return add(*this, rhs); }
    __float80_soft operator-(const __float80_soft &rhs) const { return add(*this, -rhs); }
    __float80_soft operator*(const __float80_soft &rhs) const { return mul(*this, rhs); }
    __float80_soft operator/(const __float80_soft &rhs) const { return div(*this, rhs); }
    __float80_soft pow10(int_fast16_t n) const;

    bool operator==(const __float80_soft &rhs) const;
    bool operator<(const __float80_soft &rhs) const;

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
    int ecvt(char *buf, uint_fast8_t len, uint_fast8_t prec = 6) const {
        return write(buf, len, prec, 'e');
    }
    int fcvt(char *buf, uint_fast8_t len, uint_fast8_t prec = 6) const {
        return write(buf, len, prec, 'f');
    }
    int gcvt(char *buf, uint_fast8_t len, uint_fast8_t prec = 6) const {
        return write(buf, len, prec, 'g');
    }

    /** Compose zero */
    static __float80_soft zero(bool sign) { return bits(sign, 0, 0); }
    /** Compose infinity */
    static __float80_soft infinity(bool sign = false) { return bits(sign, EXP_MASK, SIG_NORM); }
    /** Compose not a number */
    static __float80_soft not_a_number(bool sign = false, uint64_t sig = 0) {
        return bits(sign, EXP_MASK, UINT64_C(0xC000000000000000) | sig);
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

    /** Normalize and compose floating point number. */
    static __float80_soft compose(bool negative, int_fast16_t exp, fixed64_t &sig);

private:
    uint16_t _tag;
    uint64_t _sig;

    int write(char *buf, uint_fast8_t len, uint_fast8_t prec, char format) const;
    void ecvt(StrBuffer &out, uint_fast8_t prec, bool suppress = false) const;
    void fcvt(StrBuffer &out, uint_fast8_t prec, bool suppress = false) const;

    friend struct __float80_base;

    /** Compose __float80_soft */
    static __float80_soft bits(bool sign, uint16_t exp, uint64_t sig);
    /** Negate |v| */
    static __float80_soft neg(const __float80_soft &v);
    /** Add |a| and |b| */
    static __float80_soft add(const __float80_soft &a, const __float80_soft &b);
    /** Multiply non-zero |a| and |b| */
    static __float80_soft mul(const __float80_soft &a, const __float80_soft &b);
    /** Divide non-zero |a| by |b| */
    static __float80_soft div(const __float80_soft &a, const __float80_soft &b);
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
