/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __VALUE_H__
#define __VALUE_H__

#include "config_base.h"
#include "error_reporter.h"
#include "str_scanner.h"

#include <stdint.h>

namespace libasm {

enum Radix : uint8_t {
    RADIX_NONE = 0,
    RADIX_2 = 2,
    RADIX_8 = 8,
    RADIX_10 = 10,
    RADIX_16 = 16,
};

struct Value {
    Value() : _value(0), _type(UNDEFINED) {}

    /**
     * Parse text |scan| as a number of |radix| based.
     *
     * - Returns OK when |scan| is recognized as valid |radix| based number, and updates |scan| at
     *   the end of number.
     * - Returns OVERFLOW_RANGE when a number exceeds UINT_MAX, and updates |scan| at the end of a
     *   number.
     * - Returns ILLEGAL_CONSTANT when there is no valid digit found at |scan|, and |scan} is
     *   unchanged.
     */
    Error parseNumber(StrScanner &scan, Radix radix);

    bool isUndefined() const { return _type == UNDEFINED; }
    bool isSigned() const { return _type == NEGATIVE; }
    bool isUnsigned() const { return _type == UNSIGNED; }
    bool overflowUint8() const { return ConfigBase::overflowUint8(_value); }
    bool overflowUint16() const { return ConfigBase::overflowUint16(_value); }
    bool overflowUint(uint8_t bitw) const { return ConfigBase::overflowUint(_value, bitw); }

    int32_t getSigned() const { return static_cast<int32_t>(_value); }
    uint32_t getUnsigned() const { return _value; }

    Value &setSigned(int32_t value) {
        _value = value;
        _type = value < 0 ? NEGATIVE : UNSIGNED;
        return *this;
    }
    Value &setUnsigned(uint32_t value) {
        _value = value;
        _type = UNSIGNED;
        return *this;
    }
    Value &clear() {
        _value = 0;
        _type = UNDEFINED;
        return *this;
    }

private:
    enum ValueType : uint8_t {
        UNDEFINED = 0,
        NEGATIVE = 1,
        UNSIGNED = 2,
    };

    uint32_t _value;
    ValueType _type;
};

}  // namespace libasm
#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
