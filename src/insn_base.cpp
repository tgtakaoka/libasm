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

Error Insn::emitFloat32Be(double data, uint8_t pos) {
    union {
        float float32;
        uint32_t data32;
    } bytes;
    bytes.float32 = data;
    return emitUint32Be(bytes.data32, pos);
}

Error Insn::emitFloat32Le(double data, uint8_t pos) {
    union {
        float float32;
        uint32_t data32;
    } bytes;
    bytes.float32 = data;
    return emitUint32Le(bytes.data32, pos);
}

Error Insn::emitFloat64Be(double data, uint8_t pos) {
    union {
        double float64;
        uint64_t data64;
    } bytes;
    bytes.float64 = data;
    return emitUint64Be(bytes.data64, pos);
}

Error Insn::emitFloat64Le(double data, uint8_t pos) {
    union {
        double float64;
        uint64_t data64;
    } bytes;
    bytes.float64 = data;
    return emitUint64Le(bytes.data64, pos);
}

namespace {

uint64_t convert2pbcd(uint64_t bin, uint8_t digits) {
    uint64_t pbcd = 0;
    uint8_t shift = 0;
    while (digits-- > 0) {
        pbcd |= (bin % 10) << shift;
        bin /= 10;
        shift += 4;
    }
    return pbcd;
}

void swap(uint8_t &a, uint8_t &b) {
    auto t = a;
    a = b;
    b = t;
}

struct Float80 {
    uint16_t exponent;
    uint64_t significant;
    Float80(double value) {
        union {
            double float64;
            uint8_t buffer[8];
        };
        float64 = value;
        if (host::is_little_endian()) {
            swap(buffer[0], buffer[7]);
            swap(buffer[1], buffer[6]);
            swap(buffer[2], buffer[5]);
            swap(buffer[3], buffer[4]);
        }
        uint16_t exp11 = ((uint16_t)(buffer[0] & 0x7F) << 4) + (buffer[1] >> 4);
        const auto denormal = exp11 == 0;
        exponent = (exp11 == 0x07FF) ? 0x7FFF : exp11 + (0x3FFF - 0x03FF);
        if (buffer[0] & 0x80)
            exponent |= 0x8000;
        significant = buffer[1] & 0xF;
        if (!denormal)
            significant |= 0x10; // hidden integer 1
        for (auto i = 2; i < 8; ++i) {
            significant <<= 8;
            significant |= buffer[i];
        }
        significant <<= (63 - 52);
    }
};

}  // namespace

Error Insn::emitFloat80Le(double value) {
    Float80 v(value);
    emitUint64Le(v.significant);
    emitUint16Le(v.exponent);
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

Error Insn::emitFloat96Be(double value, uint8_t pos) {
    Float80 v(value);
    emitUint16Be(v.exponent, pos);
    emitUint16Be(0, pos + 2);
    emitUint64Be(v.significant, pos + 4);
    return getError();
}

Error Insn::emitPackedBcd96Be(double value, uint8_t pos) {
    Float80 v(value);
    if (isinf(value)) {
        emitUint16Be(v.exponent | INT16_MAX, pos);
        emitUint16Be(0, pos += 2);
        emitUint64Be(0, pos += 2);
    } else if (isnan(value)) {
        emitUint16Be(v.exponent | INT16_MAX, pos);
        emitUint16Be(0, pos += 2);
        emitUint64Be(INT64_MIN, pos += 2);
    } else {
        if (value < 0)
            value = -value;
        auto exponent = static_cast<int16_t>(log10(value));
        if (exponent < 0 || (exponent == 0 && value < 1.0))
            --exponent;
        uint8_t digit = value * pow(10.0, -exponent);
        v.significant = value * pow(10.0, 16 - exponent);
        auto sign = v.exponent & 0x8000;
        if (exponent < 0) {
            sign |= 0x4000;
            exponent = -exponent;
        }
        emitUint16Be(sign | convert2pbcd(exponent, 3), pos);
        emitUint16Be(digit, pos += 2);
        emitUint64Be(convert2pbcd(v.significant, 16), pos += 2);
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
    const uint8_t msb = readByte();
    const uint8_t lsb = readByte();
    return static_cast<uint16_t>(msb) << 8 | lsb;
}

uint16_t DisInsnBase::readUint16Le() {
    const uint8_t lsb = readByte();
    const uint8_t msb = readByte();
    return static_cast<uint16_t>(msb) << 8 | lsb;
}

uint32_t DisInsnBase::readUint32Be() {
    const uint16_t msw = readUint16Be();
    const uint16_t lsw = readUint16Be();
    return static_cast<uint32_t>(msw) << 16 | lsw;
}

uint32_t DisInsnBase::readUint32Le() {
    const uint16_t lsw = readUint16Le();
    const uint16_t msw = readUint16Le();
    return static_cast<uint32_t>(msw) << 16 | lsw;
}

uint64_t DisInsnBase::readUint64Be() {
    const uint32_t msw = readUint32Be();
    const uint32_t lsw = readUint32Be();
    return static_cast<uint64_t>(msw) << 32 | lsw;
}

uint64_t DisInsnBase::readUint64Le() {
    const uint32_t lsw = readUint32Le();
    const uint32_t msw = readUint32Le();
    return static_cast<uint64_t>(msw) << 32 | lsw;
}

double DisInsnBase::readFloat32Be() {
    union {
        float float32;
        uint32_t data32;
    } bytes;
    bytes.data32 = readUint32Be();
    return bytes.float32;
}

double DisInsnBase::readFloat32Le() {
    union {
        float float32;
        uint32_t data32;
    } bytes;
    bytes.data32 = readUint32Le();
    return bytes.float32;
}

double DisInsnBase::readFloat64Be() {
    union {
        double float64;
        uint64_t data64;
    } bytes;
    bytes.data64 = readUint64Be();
    return bytes.float64;
}

double DisInsnBase::readFloat64Le() {
    union {
        double float64;
        uint64_t data64;
    } bytes;
    bytes.data64 = readUint64Le();
    return bytes.float64;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
