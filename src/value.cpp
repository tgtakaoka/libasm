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

#include "value.h"
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include "str_buffer.h"

namespace libasm {

namespace {
bool isValidDigit(char c, Radix radix) {
    if (radix == RADIX_16)
        return isxdigit(c);
    return c >= '0' && c < '0' + uint8_t(radix);
}

uint8_t toNumber(char c, Radix radix) {
    if (radix == RADIX_16 && c >= 'A')
        return (c & ~0x20) - 'A' + 10;
    return c - '0';
}

template <typename VAL_T, VAL_T VAL_MAX>
Error strToNumber(StrScanner &scan, Radix radix, VAL_T &value) {
    auto p = scan;
    if (!isValidDigit(*p, radix))
        return NOT_AN_EXPECTED;
    const VAL_T limit = VAL_MAX / uint8_t(radix);
    const uint8_t limit_digit = VAL_MAX % uint8_t(radix);
    Error error = OK;
    VAL_T v = 0;
    while (isValidDigit(*p, radix)) {
        const auto n = toNumber(*p, radix);
        if (v > limit || (v == limit && n > limit_digit))
            error = OVERFLOW_RANGE;
        v *= uint8_t(radix);
        v += n;
        ++p;
    }
    scan = p;
    value = v;
    return error;
}
}  // namespace

Error Value::read(StrScanner &scan, Radix radix, unsigned_t &value) {
    return strToNumber<unsigned_t, UNSIGNED_MAX>(scan, radix, value);
}

Error Value::read(StrScanner &scan, Radix radix) {
    const auto error = read(scan, radix, _unsigned);
    _type = V_UNSIGNED;
    if (error)
        clear();
    return error;
}

const char *Value::str() const {
#ifdef LIBASM_DEBUG_VALUE
    static char buf[80];
    StrBuffer out{buf, sizeof(buf)};
    if (_type == V_UNSIGNED) {
        out.text("<u>").hex(_unsigned).letter('(').dec(_unsigned).letter(')');
    } else if (_type == V_SIGNED) {
        out.text("<s>").hex(_signed).letter('(').dec(_signed).letter(')');
#ifndef ASM_NOFLOAT
    } else if (_type == V_FLOAT) {
        out.text("<f>").hex(_unsigned).letter('(').float80(_float).letter(')');
#endif
    } else {
        return "<undef>";
    }
    return out.str();
#else
    return "";
#endif
}

void Value::setS(signed_t s) {
    if (s >= 0) {
        setU(static_cast<unsigned_t>(s));
    } else {
        _signed = s;
        _type = V_SIGNED;
    }
}

void Value::setU(unsigned_t u) {
    _unsigned = u;
    _type = V_UNSIGNED;
}

Value &Value::setSigned(int32_t s) {
    if (s >= 0) {
        setU(static_cast<uint32_t>(s));
    } else {
        setS(s);
    }
    return *this;
}

Value &Value::setUnsigned(uint32_t u) {
    setU(u);
    return *this;
}

Value &Value::setInteger(signed_t s) {
    setS(s);
    return *this;
}

Value &Value::setUinteger(unsigned_t u) {
    setU(u);
    return *this;
}

#ifdef ASM_NOFLOAT

bool Value::isFloat() const {
    return false;
}

#else

bool Value::isFloat() const {
    return _type == V_FLOAT;
}

Value::float_t Value::getFloat() const {
    if (isUnsigned()) {
        float_t v;
        return v.set(_unsigned);
    }
    if (isSigned()) {
        float_t v;
        return v.set(_signed);
    }
    return _float;
}

Value &Value::setFloat(const float_t &f) {
    _float = f;
    _type = V_FLOAT;
    return *this;
}

#endif

Value &Value::clear() {
    _unsigned = 0;
    _type = V_UNDEF;
    return *this;
}

bool Value::overflowInt8() const {
    return overflow(INT8_MAX, INT8_MIN);
}

bool Value::overflowUint8() const {
    return overflow(UINT8_MAX, INT8_MIN);
}

bool Value::overflowInt16() const {
    return overflow(INT16_MAX, INT16_MIN);
}

bool Value::overflowUint16() const {
    return overflow(UINT16_MAX, INT16_MIN);
}

bool Value::overflowInt32() const {
    if (sizeof(_signed) == sizeof(int32_t)) {
        return false;
    } else if (isUnsigned()) {
        return _unsigned > INT32_MAX;
    } else {
        return _signed > INT32_MAX || _signed < INT32_MIN;
    }
}

bool Value::overflowUint32() const {
    if (sizeof(_unsigned) == sizeof(uint32_t)) {
        return false;
    } else if (isUnsigned()) {
        return _unsigned > UINT32_MAX;
    } else {
        return _signed > UINT32_MAX || _signed < INT32_MIN;
    }
}

bool Value::overflow(uint32_t max, int32_t min) const {
    if (overflowUint32()) {
        return true;
    } else if (isUnsigned()) {
        const auto u = static_cast<uint32_t>(_unsigned);
        return u > max || (min > 0 && u < static_cast<uint32_t>(min));
    } else if (_signed >= 0) {
        const auto u = static_cast<uint32_t>(_signed);
        return u > max || (min > 0 && u < static_cast<uint32_t>(min));
    } else {
        const auto s = static_cast<int32_t>(_signed);
        return (max <= INT32_MAX && s > static_cast<int32_t>(max)) || s < min;
    }
}

bool Value::overflowInteger(unsigned_t max, signed_t min) const {
    if (isUnsigned())
        return _unsigned > max;
    if (isSigned())
        return (max <= SIGNED_MAX && _signed > static_cast<signed_t>(max)) || _signed < min;
    return false;
}

bool Value::isZero() const {
    if (isUndefined())
        return false;
#ifdef LIBASM_ASM_NOFLOAT
    return _unsigned == 0;
#else
    if (isInteger())
        return _unsigned == 0;
    return _float == 0.0;
#endif
}

bool Value::isNegative() const {
#ifndef ASM_NOFLOAT
    return isSigned() && _signed < 0;
#else
    return (isSigned() && _signed < 0) || (isFloat() && getFloat() < 0);
#endif
}

bool Value::negateOverflow() const {
#ifdef ASM_NOFLOAT
    if (isUnsigned())
        return _unsigned >= static_cast<unsigned_t>(SIGNED_MAX) + 1;
    if (isSigned())
        return _signed == SIGNED_MIN;
#endif
    return false;
}

bool Value::operator==(const Value &rhs) const {
    if (isInteger() && rhs.isInteger())
        return _unsigned == rhs._unsigned;
#ifdef ASM_NOFLOAT
    return false;
#else
    return getFloat() == rhs.getFloat();
#endif
}

bool Value::operator<(const Value &rhs) const {
    if (isInteger() && rhs.isInteger()) {
        if (isUnsigned()) {
            if (rhs.isUnsigned())
                return _unsigned < rhs._unsigned;
            const auto s = rhs._signed;
            return s >= 0 && _unsigned < static_cast<unsigned_t>(s);
        } else {
            if (rhs.isSigned())
                return _signed < rhs._signed;
            const auto u = rhs._unsigned;
            return _signed < 0 || static_cast<unsigned_t>(_signed) < u;
        }
#ifndef ASM_NOFLOAT
    } else {
        return getFloat() < rhs.getFloat();
#endif
    }
}

Value::operator bool() const {
    return isInteger() && _unsigned;
};

bool Value::operator!() const {
    return isInteger() && _unsigned == 0;
};

bool Value::operator&&(const Value &rhs) const {
    return !!*this && !!rhs;
};

bool Value::operator||(const Value &rhs) const {
    return !!*this || !!rhs;
};

Value Value::negate() const {
    Value v;
    if (isInteger()) {
        if (isNegative()) {
            v.setU(static_cast<unsigned_t>(-_signed));
        } else {
            v.setS(-static_cast<signed_t>(_unsigned));
        }
#ifndef ASM_NOFLOAT
    } else {
        v.setFloat(-getFloat());
#endif
    }
    return v;
}

Value::unsigned_t Value::absolute() const {
    return isNegative() ? static_cast<unsigned_t>(-_signed) : _unsigned;
}

Value Value::operator+(const Value &rhs) const {
    Value v;
    if (isInteger() && rhs.isInteger()) {
        const auto l = absolute();
        const auto r = rhs.absolute();
        if (isNegative() == rhs.isNegative()) {
            const auto a = l + r;
            if (isNegative()) {
                v.setS(-static_cast<signed_t>(a));
            } else {
                v.setU(a);
            }
        } else if (l >= r) {
            const auto u = l - r;
            if (isNegative()) {
                v.setS(-static_cast<signed_t>(u));
            } else {
                v.setU(u);
            }
        } else {
            const auto u = r - l;
            if (isNegative()) {
                v.setU(u);
            } else {
                v.setS(-static_cast<signed_t>(u));
            }
        }
#ifndef ASM_NOFLOAT
    } else {
        v.setFloat(getFloat() + rhs.getFloat());
#endif
    }
    return v;
}

Value Value::operator-(const Value &rhs) const {
    return *this + rhs.negate();
}

Value Value::operator*(const Value &rhs) const {
    Value v;
    if (isInteger() && rhs.isInteger()) {
        const auto p = absolute() * rhs.absolute();
        if (isNegative() == rhs.isNegative()) {
            v.setU(p);
        } else {
            v.setS(-static_cast<signed_t>(p));
        }
#ifndef ASM_NOFLOAT
    } else {
        v.setFloat(getFloat() * rhs.getFloat());
#endif
    }
    return v;
}

Value Value::operator/(const Value &rhs) const {
    Value v;
    if (rhs.isZero() || rhs.isUndefined()) {
        ;  // divide by zero
    } else if (isInteger() && rhs.isInteger()) {
        const auto q = absolute() / rhs.absolute();
        if (isNegative() == rhs.isNegative()) {
            v.setU(q);
        } else {
            v.setS(-static_cast<signed_t>(q));
        }
#ifndef ASM_NOFLOAT
    } else {
        v.setFloat(getFloat() / rhs.getFloat());
#endif
    }
    return v;
}

Value Value::operator%(const Value &rhs) const {
    Value v;
    if (rhs.isZero() || rhs.isUndefined()) {
        ;  // divide by zero
    } else if (isInteger() && rhs.isInteger()) {
        const auto r = absolute() % rhs.absolute();
        if (isNegative()) {
            v.setS(-static_cast<signed_t>(r));
        } else {
            v.setU(r);
        }
    }
    // not defined for floating point number
    return v;
}

Value Value::operator~() const {
    Value v;
    if (isInteger()) {
        // logical operator set signed value for spurious overflow on lower precision.
        v.setS(~_unsigned);
    }
    // not defined for floating point number
    return v;
}

Value Value::operator&(const Value &rhs) const {
    Value v;
    if (isInteger() && rhs.isInteger()) {
        // logical operator set signed value for spurious overflow on lower precision.
        v.setS(_unsigned & rhs._unsigned);
    }
    return v;
}

Value Value::operator|(const Value &rhs) const {
    Value v;
    if (isInteger() && rhs.isInteger()) {
        // logical operator set signed value for spurious overflow on lower precision.
        v.setS(_unsigned | rhs._unsigned);
    }
    return v;
}

Value Value::operator^(const Value &rhs) const {
    Value v;
    if (isInteger() && rhs.isInteger()) {
        // logical operator set signed value for spurious overflow on lower precision.
        v.setS(_unsigned ^ rhs._unsigned);
    }
    return v;
}

Value Value::operator<<(const Value &rhs) const {
    Value v;
    if (isInteger() && rhs.isInteger()) {
        const auto count = rhs.getUnsigned();
        if (count >= INTEGER_BITS) {
            v.setU(0);
        } else {
            v.setU(_unsigned << count);
        }
    }
    return v;
}

Value Value::operator>>(const Value &rhs) const {
    Value v;
    if (isInteger() && rhs.isInteger()) {
        const auto count = rhs.getUnsigned();
        if (count >= INTEGER_BITS) {
            v.setS(0);
        } else if (isNegative()) {
            v.setS(_signed >> count);
        } else {
            v.setU(_unsigned >> count);
        }
    }
    return v;
}

namespace {
template <typename BASE>
BASE power(BASE base, Value::unsigned_t exp) {
    BASE prod = 1;
    while (true) {
        if (exp & 1)
            prod *= base;
        if ((exp >>= 1) == 0)
            return prod;
        base *= base;
    }
}
}  // namespace

Value Value::exponential(const Value &rhs) const {
    Value v;
    if (isInteger() && rhs.isInteger()) {
        if (isNegative()) {
            v.setS(power(_signed, rhs._unsigned));
        } else {
            v.setU(power(_unsigned, rhs._unsigned));
        }
    }
    // not defined for floating point number
    return v;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
