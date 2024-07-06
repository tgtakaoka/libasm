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

#include "insn_base.h"
#include <math.h>

namespace libasm {

Insn::Insn(uint32_t addr) : ErrorAt(), _address(addr), _length(0) {
    memset(_bytes, 0, sizeof(_bytes));
}

void Insn::reset(uint32_t addr, uint8_t length) {
    resetError();
    setAt(StrScanner::EMPTY);
    _address = addr;
    if (length >= sizeof(_bytes))
        length = sizeof(_bytes);
    _length = length;
    memset(_bytes + length, 0, sizeof(_bytes) - length);
    _buffer.reset();
}

uint32_t Insn::align(uint8_t step) {
    const uint8_t rem = _address % step;
    if (rem)
        _address += step - rem;
    return _address;
}

Error Insn::emitByte(uint8_t val, uint8_t pos) {
    if (pos >= MAX_CODE)
        return setErrorIf(NO_MEMORY);
    _bytes[pos++] = val;
    if (_length < pos)
        _length = pos;
    return OK;
}

Error Insn::emitUint16Be(uint16_t data, uint8_t pos) {
    emitByte(data >> 8, pos + 0);
    return emitByte(data >> 0, pos + 1);
}

Error Insn::emitUint16Le(uint16_t data, uint8_t pos) {
    emitByte(data >> 0, pos + 0);
    return emitByte(data >> 8, pos + 1);
}

Error Insn::emitUint32Be(uint32_t data, uint8_t pos) {
    emitUint16Be(data >> 16, pos + 0);
    return emitUint16Be(data >> 0, pos + 2);
}

Error Insn::emitUint32Le(uint32_t data, uint8_t pos) {
    emitUint16Le(data >> 0, pos + 0);
    return emitUint16Le(data >> 16, pos + 2);
}

Error Insn::emitUint64Be(uint64_t data, uint8_t pos) {
    emitUint32Be(data >> 32, pos + 0);
    return emitUint32Be(data >> 0, pos + 4);
}

Error Insn::emitUint64Le(uint64_t data, uint8_t pos) {
    emitUint32Le(data >> 0, pos + 0);
    return emitUint32Le(data >> 32, pos + 4);
}

#ifndef ASM_NOFLOAT

Error Insn::emitFloat32Be(float80_t data, uint8_t pos) {
    union {
        float f32;
        uint32_t u32;
    } bytes;
    bytes.f32 = static_cast<double>(data);
    return emitUint32Be(bytes.u32, pos);
}

Error Insn::emitFloat32Le(float80_t data, uint8_t pos) {
    union {
        float f32;
        uint32_t u32;
    } bytes;
    bytes.f32 = static_cast<double>(data);
    return emitUint32Le(bytes.u32, pos);
}

Error Insn::emitFloat64Be(float80_t data, uint8_t pos) {
    union {
        double f64;
        uint64_t u64;
    } bytes;
    bytes.f64 = static_cast<double>(data);
    return emitUint64Be(bytes.u64, pos);
}

Error Insn::emitFloat64Le(float80_t data, uint8_t pos) {
    union {
        double f64;
        uint64_t u64;
    } bytes;
    bytes.f64 = static_cast<double>(data);
    return emitUint64Le(bytes.u64, pos);
}

namespace {

uint64_t convert2pbcd(uint64_t bin, uint_fast8_t digits) {
    uint64_t pbcd = 0;
    uint_fast8_t shift = 0;
    while (digits-- > 0) {
        pbcd |= (bin % 10) << shift;
        bin /= 10;
        shift += 4;
    }
    return pbcd;
}

}  // namespace

Error Insn::emitFloat80Le(float80_t value) {
    emitUint64Le(value.significand());
    emitUint16Le(value.tag());
    return getError();
}

Error Insn::emitPackedBcd80Le(int64_t value) {
    const auto sign = value < 0 ? INT8_MIN : 0;
    auto u64 = static_cast<uint64_t>(sign ? -value : value);
    for (auto i = 0; i < 9; ++i) {
        uint8_t d = u64 % 10;
        u64 /= 10;
        d |= (u64 % 10) << 4;
        u64 /= 10;
        emitByte(d);
    }
    return emitByte(sign);
}

Error Insn::emitFloat96Be(float80_t value, uint8_t pos) {
    emitUint16Be(value.tag(), pos);
    emitUint16Be(0, pos + 2);
    emitUint64Be(value.significand(), pos + 4);
    return getError();
}

Error Insn::emitPackedBcd96Be(float80_t value, uint8_t pos) {
    if (value.isInf()) {
        emitUint16Be(value.tag() | 0x7FFF, pos);
        emitUint16Be(0, pos += 2);
        emitUint64Be(0, pos += 2);
    } else if (value.isNan()) {
        emitUint16Be(value.tag() | 0x7FFF, pos);
        emitUint16Be(0, pos += 2);
        emitUint64Be(INT64_MIN, pos += 2);
    } else {
        const auto negative = value.isNegative();
        if (negative)
            value = -value;
        constexpr auto log10_2 = 0.3010299956639812;
        // Estimate approximate exponent of 10.
        int_fast16_t exp = floor(value.exponent() * log10_2);
        auto v = static_cast<uint64_t>(value.pow10(16 - exp));
        constexpr auto ten17 = UINT64_C(100'000'000'000'000'000);
        while (v >= ten17) {
            v /= 10;
            exp++;
        }
        // v < 10^17
        constexpr auto ten16 = UINT64_C(10'000'000'000'000'000);
        while (v < ten16) {
            v *= 10;
            exp--;
        }
        // v >= 10^16
        uint16_t tag = negative ? 0x8000 : 0;
        if (exp < 0) {
            tag |= 0x4000;
            exp = -exp;
        }
        emitUint16Be(tag | convert2pbcd(exp, 3), pos);
        emitUint16Be(convert2pbcd(v / ten16, 2), pos += 2);
        emitUint64Be(convert2pbcd(v % ten16, 16), pos += 2);
    }
    return getError();
}
#endif

void DisInsnBase::reset(uint8_t length) {
    resetError();
    _insn.reset(_insn.address(), length);
}

uint8_t DisInsnBase::readByte() {
    if (!_memory.hasNext()) {
        setErrorIf(_out, NO_MEMORY);
        return 0;
    }
    const auto data = _memory.readByte();
    if (_insn.emitByte(data))
        setErrorIf(_out, NO_MEMORY);
    return data;
}

uint16_t DisInsnBase::readUint16Be() {
    const auto msb = readByte();
    const auto lsb = readByte();
    return static_cast<uint16_t>(msb) << 8 | lsb;
}

uint16_t DisInsnBase::readUint16Le() {
    const auto lsb = readByte();
    const auto msb = readByte();
    return static_cast<uint16_t>(msb) << 8 | lsb;
}

uint32_t DisInsnBase::readUint32Be() {
    const auto msw = readUint16Be();
    const auto lsw = readUint16Be();
    return static_cast<uint32_t>(msw) << 16 | lsw;
}

uint32_t DisInsnBase::readUint32Le() {
    const auto lsw = readUint16Le();
    const auto msw = readUint16Le();
    return static_cast<uint32_t>(msw) << 16 | lsw;
}

uint64_t DisInsnBase::readUint64Be() {
    const auto msw = readUint32Be();
    const auto lsw = readUint32Be();
    return static_cast<uint64_t>(msw) << 32 | lsw;
}

uint64_t DisInsnBase::readUint64Le() {
    const auto lsw = readUint32Le();
    const auto msw = readUint32Le();
    return static_cast<uint64_t>(msw) << 32 | lsw;
}

double DisInsnBase::readFloat32Be() {
    union {
        float f32;
        uint32_t u32;
    } bytes;
    bytes.u32 = readUint32Be();
    return bytes.f32;
}

double DisInsnBase::readFloat32Le() {
    union {
        float f32;
        uint32_t u32;
    } bytes;
    bytes.u32 = readUint32Le();
    return bytes.f32;
}

double DisInsnBase::readFloat64Be() {
    union {
        double f64;
        uint64_t u64;
    } bytes;
    bytes.u64 = readUint64Be();
    return bytes.f64;
}

double DisInsnBase::readFloat64Le() {
    union {
        double f64;
        uint64_t u64;
    } bytes;
    bytes.u64 = readUint64Le();
    return bytes.f64;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
