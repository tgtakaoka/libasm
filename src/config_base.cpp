/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "config_base.h"

namespace libasm {

bool ConfigBase::overflowInt8(int16_t s16) {
    return s16 < -128 || s16 >= 128;
}

bool ConfigBase::overflowInt8(int32_t s32) {
    return s32 < -128 || s32 >= 128;
}

bool ConfigBase::overflowInt16(int32_t s32) {
    return s32 < -32768L || s32 >= 32768L;
}

bool ConfigBase::overflowInt(int32_t s32, uint8_t bitw) {
    const int32_t sign = 1UL << (bitw - 1);
    return s32 < -sign || s32 >= sign;
}

bool ConfigBase::overflowUint8(uint16_t u16) {
    const auto s16 = static_cast<int16_t>(u16);
    return s16 < -128 || (s16 >= 0 && u16 >= 256);
}

bool ConfigBase::overflowUint8(uint32_t u32) {
    const auto s32 = static_cast<int32_t>(u32);
    return s32 < -128 || (s32 >= 0 && u32 >= 0x100);
}

bool ConfigBase::overflowUint16(uint32_t u32) {
    const auto s32 = static_cast<int32_t>(u32);
    return s32 < -32768L || (s32 >= 0 && u32 >= 0x10000L);
}

bool ConfigBase::overflowUint(uint32_t u32, uint8_t bitw) {
    const auto s32 = static_cast<int32_t>(u32);
    if (bitw >= 32)
        return false;
    const int32_t sign = 1UL << (bitw - 1);
    const uint32_t max = sign << 1;
    return s32 < -sign || (s32 >= 0 && u32 >= max);
}

int32_t ConfigBase::signExtend(uint32_t u32, uint8_t width) {
    const uint32_t sign = 1UL << (width - 1);
    const uint32_t mask = (sign << 1) - 1;
    return static_cast<int32_t>(((u32 + sign) & mask) - sign);
}

Error ConfigBase::checkAddr(uint32_t addr, uint8_t width) const {
    if (width == 0)
        width = static_cast<uint8_t>(addressWidth());
    const uint32_t max = 1UL << width;
    if (addr & ~(max - 1))
        return OVERFLOW_RANGE;
    if (opCodeWidth() == OPCODE_16BIT && addressUnit() == ADDRESS_BYTE && addr % 2)
        return OPERAND_NOT_ALIGNED;
    return OK;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
