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

Error Value::parseNumber(StrScanner &scan, Radix radix, uint32_t &value) {
    return strToNumber<uint32_t, UINT32_MAX>(scan, radix, value);
}

#ifndef ASM_NOFLOAT
Error Value::parseNumber(StrScanner &scan, Radix radix, uint64_t &value) {
    return strToNumber<uint64_t, UINT64_MAX>(scan, radix, value);
}
#endif

Error Value::parseNumber(StrScanner &scan, Radix radix) {
#ifdef ASM_NOFLOAT
    _type = V_UINT32;
    return parseNumber(scan, radix, _uint32);
#else
    const auto save = scan;
    auto type = V_UINT32;
    auto error = parseNumber(scan, radix, _uint32);
    if (error == OVERFLOW_RANGE) {
        scan = save;
        type = V_UINT64;
        error = parseNumber(scan, radix, _uint64);
    }
    if (error == OK) {
        _type = type;
        return OK;
    }
    return NOT_AN_EXPECTED;
#endif
}

#ifndef ASM_NOFLOAT
int64_t Value::getInt64() const {
    if (isSigned())
        return getSigned();
    if (isUnsigned())
        return getUnsigned();
    return static_cast<int64_t>(_uint64);
}

double Value::getFloat() const {
    if (isInt32() || _type == V_INT64)
        return static_cast<double>(getInt64());
    if (_type == V_UINT64)
        return static_cast<double>(_uint64);
    return _float64;
}
#endif

bool Value::isZero() const {
    if (isUndefined())
        return false;
#ifdef ASM_NOFLOAT
    return getUnsigned() == 0;
#else
    if (isInt32())
        return getUnsigned() == 0;
    if (isInt64())
        return getInt64() == 0;
    return _float64 == 0;
#endif
}

bool Value::negateOverflow() const {
    return isUnsigned() && getUnsigned() > static_cast<uint32_t>(INT32_MIN);
}

bool Value::operator==(const Value &rhs) const {
    return getUnsigned() == rhs.getUnsigned();
}

bool Value::operator<(const Value &rhs) const {
    if (isUnsigned() && rhs.isUnsigned()) {
        return getUnsigned() < rhs.getUnsigned();
    } else if (isSigned() && rhs.isSigned()) {
        return getSigned() < rhs.getSigned();
    } else if (isSigned()) {
        const auto v = getSigned();
        return v < 0 || static_cast<uint32_t>(v) < rhs.getUnsigned();
    } else {
        const auto v = rhs.getSigned();
        return v >= 0 && getUnsigned() < static_cast<uint32_t>(v);
    }
}

Value Value::operator-() const {
    Value v;
    if (isSigned()) {
        const auto i32 = getSigned();
        if (i32 == INT32_MIN) {
            v.setUnsigned(static_cast<uint32_t>(INT32_MIN));
        } else {
            v.setSigned(-i32);
        }
    } else if (isUnsigned()) {
        const auto u32 = getUnsigned();
        if (u32 <= static_cast<uint32_t>(INT32_MIN)) {
            v.setSigned(-static_cast<int32_t>(u32));
#ifndef ASM_NOFLOAT
        } else {
            v.setInt64(-static_cast<int64_t>(u32));
#endif
        }
#ifndef ASM_NOFLOAT
    } else if (isInt64()) {
        v.setInt64(-getInt64());
    } else {
        v.setFloat(-getFloat());
#endif
    }
    return v;
}

Value Value::operator+(const Value &rhs) const {
    Value v;
    if (isInt32() && rhs.isInt32()) {
        if (isSigned() || rhs.isSigned()) {
            v.setSigned(getSigned() + rhs.getSigned());
        } else {
            v.setUnsigned(getUnsigned() + rhs.getUnsigned());
        }
#ifndef ASM_NOFLOAT
    } else if (isInt64() && rhs.isInt64()) {
        v.setInt64(getInt64() + rhs.getInt64());
    } else {
        v.setFloat(getFloat() + rhs.getFloat());
#endif
    }
    return v;
}

Value Value::operator-(const Value &rhs) const {
    Value v;
    if (isInt32() && rhs.isInt32()) {
        if (isSigned() || rhs.isSigned()) {
            v.setSigned(getSigned() - rhs.getSigned());
        } else if (getUnsigned() < rhs.getUnsigned()) {
            v.setSigned(getUnsigned() - rhs.getUnsigned());
        } else {
            v.setUnsigned(getUnsigned() - rhs.getUnsigned());
        }
#ifndef ASM_NOFLOAT
    } else if (isInt64() && rhs.isInt64()) {
        v.setInt64(getInt64() - rhs.getInt64());
    } else {
        v.setFloat(getFloat() - rhs.getFloat());
#endif
    }
    return v;
}

Value Value::operator*(const Value &rhs) const {
    Value v;
    if (isInt32() && rhs.isInt32()) {
        if (isSigned() || rhs.isSigned()) {
            v.setSigned(getSigned() * rhs.getSigned());
        } else {
            v.setUnsigned(getUnsigned() * rhs.getUnsigned());
        }
#ifndef ASM_NOFLOAT
    } else if (isInt64() && rhs.isInt64()) {
        v.setInt64(getInt64() * rhs.getInt64());
    } else {
        v.setFloat(getFloat() * rhs.getFloat());
#endif
    }
    return v;
}

Value Value::operator/(const Value &rhs) const {
    Value v;
    if (rhs.isUndefined()) {
        v = *this;
    } else if (isInt32() && rhs.isInt32()) {
        if (isSigned() || rhs.isSigned()) {
            v.setSigned(getSigned() / rhs.getSigned());
        } else {
            v.setUnsigned(getUnsigned() / rhs.getUnsigned());
        }
#ifndef ASM_NOFLOAT
    } else if (isInt64() && rhs.isInt64()) {
        v.setInt64(getInt64() / rhs.getInt64());
    } else {
        v.setFloat(getFloat() / rhs.getFloat());
#endif
    }
    return v;
}

Value Value::operator%(const Value &rhs) const {
    Value v;
    if (rhs.isUndefined()) {
        v = *this;
    } else if (isInt32() && rhs.isInt32()) {
        if (isSigned() || rhs.isSigned()) {
            v.setSigned(getSigned() % rhs.getSigned());
        } else {
            v.setUnsigned(getUnsigned() % rhs.getUnsigned());
        }
#ifndef ASM_NOFLOAT
    } else if (isInt64() && rhs.isInt64()) {
        v.setInt64(getInt64() % rhs.getInt64());
    } else {
        v.setFloat(fmod(getFloat(), rhs.getFloat()));
#endif
    }
    return v;
}

Value Value::operator<<(const Value &rhs) const {
    const auto count = rhs.getUnsigned();
    Value v;
    v.setUnsigned(count < 32 ? getUnsigned() << count : 0);
    return v;
}

Value Value::operator>>(const Value &rhs) const {
    const auto count = rhs.getUnsigned();
    Value v;
    if (isSigned()) {
        const auto msb = static_cast<uint32_t>(INT32_MIN);
        auto value = getUnsigned();
        for (uint32_t i = 0; i < count && i < 32; ++i) {
            value >>= 1;
            value |= msb;
        }
        v.setUnsigned(value);
    } else {
        v.setUnsigned(count < 32 ? getUnsigned() >> count : 0);
    }
    return v;
}

template <typename BASE>
static BASE power(BASE base, uint32_t exp) {
    BASE prod = 1;
    while (true) {
        if (exp & 1)
            prod *= base;
        if ((exp >>= 1) == 0)
            return prod;
        base *= base;
    }
}

Value Value::exponential(const Value &rhs) const {
    Value v;
    if (isInt32() && rhs.isInt32()) {
        if (isSigned() && getSigned() < 0) {
            v.setSigned(power<int32_t>(getSigned(), rhs.getUnsigned()));
        } else {
            v.setUnsigned(power<uint32_t>(getUnsigned(), rhs.getUnsigned()));
        }
#ifndef ASM_NOFLOAT
    } else {
        v.setFloat(pow(getFloat(), rhs.getFloat()));
#endif
    }
    return v;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
