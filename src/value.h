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

#include <stdint.h>

namespace libasm {

class Value {
public:
    Value() : _value(0), _type(UNDEFINED) {}

    bool isUndefined() const { return _type == UNDEFINED; }
    bool isSigned() const { return _type == SIGNED; }
    bool isUnsigned() const { return _type == UNSIGNED; }
    bool overflowUint8() const { return overflowUint8(_value); }
    bool overflowUint16() const { return overflowUint16(_value); }
    int32_t getSigned() const { return static_cast<int32_t>(_value); }
    uint32_t getUnsigned() const { return _value; }

    Value &setSign(bool sign) {
        _type = sign ? SIGNED : UNSIGNED;
        return *this;
    }
    Value &setValue(uint32_t value) { return setSign(static_cast<int32_t>(_value = value) >= 0); }
    Value &clear() {
        _value = 0;
        _type = UNDEFINED;
        return *this;
    }

    static bool overflowRel8(int16_t s16);
    static bool overflowRel8(int32_t s32);
    static bool overflowRel16(int32_t s32);
    static bool overflowUint8(uint16_t u16);
    static bool overflowUint8(uint32_t u32);
    static bool overflowUint16(uint32_t u32);
    static bool overflowUint(uint32_t, uint8_t bitw);
    static Value makeSigned(int32_t value) { return Value(value, SIGNED); }
    static Value makeUnsigned(uint32_t value) { return Value(value, UNSIGNED); }

private:
    enum ValueType : uint8_t {
        UNDEFINED = 0,
        SIGNED = 1,
        UNSIGNED = 2,
    };

    uint32_t _value;
    ValueType _type;

    Value(uint32_t value, ValueType type) : _value(value), _type(type) {}
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
