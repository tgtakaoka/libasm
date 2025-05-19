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
#include "ieee_float.h"

namespace libasm {

Insn::Insn(uint32_t addr) : ErrorAt(), _address(addr), _length(0) {
    memset(_bytes, 0, sizeof(_bytes));
}

void Insn::reset(uint32_t addr, uint8_t length) {
    resetError();
    resetAt();
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

#if !defined(LIBASM_ASM_NOFLOAT)

Error Insn::emitFloat32Be(const float80_t &data, uint8_t pos) {
    float32_t f32{data};
    if (f32.isInf() && !data.isInf())
        setErrorIf(OVERFLOW_RANGE);
    if (f32.isZero() && !data.isZero())
        setErrorIf(UNDERFLOW_RANGE);
    return emitUint32Be(f32.bits(), pos);
}

Error Insn::emitFloat32Le(const float80_t &data, uint8_t pos) {
    float32_t f32{data};
    if (f32.isInf() && !data.isInf())
        setErrorIf(OVERFLOW_RANGE);
    if (f32.isZero() && !data.isZero())
        setErrorIf(UNDERFLOW_RANGE);
    return emitUint32Le(f32.bits(), pos);
}

Error Insn::emitFloat64Be(const float80_t &data, uint8_t pos) {
    float64_t f64{data};
    if (f64.isInf() && !data.isInf())
        setErrorIf(OVERFLOW_RANGE);
    if (f64.isZero() && !data.isZero())
        setErrorIf(UNDERFLOW_RANGE);
    return emitUint64Be(f64.bits(), pos);
}

Error Insn::emitFloat64Le(const float80_t &data, uint8_t pos) {
    float64_t f64{data};
    if (f64.isInf() && !data.isInf())
        setErrorIf(OVERFLOW_RANGE);
    if (f64.isZero() && !data.isZero())
        setErrorIf(UNDERFLOW_RANGE);
    return emitUint64Le(f64.bits(), pos);
}

#endif

void DisInsnBase::resetLength(uint8_t length) {
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

#if !defined(LIBASM_DIS_NOFLOAT)

float80_t DisInsnBase::readFloat32Be() {
    float32_t f32;
    return float80_t(f32.set(readUint32Be()));
}

float80_t DisInsnBase::readFloat32Le() {
    float32_t f32;
    return float80_t(f32.set(readUint32Le()));
}

float80_t DisInsnBase::readFloat64Be() {
    float64_t f64;
    return float80_t(f64.set(readUint64Be()));
}

float80_t DisInsnBase::readFloat64Le() {
    float64_t f64;
    return float80_t(f64.set(readUint64Le()));
}

#endif

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
