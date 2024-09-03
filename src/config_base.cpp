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

bool ConfigBase::overflowDelta(int32_t s32, uint_fast8_t bitw) {
    const auto sign = INT32_C(1) << (bitw - 1);
    return s32 < -sign || s32 >= sign;
}

int32_t ConfigBase::signExtend(uint32_t u32, uint_fast8_t width) {
    const auto sign = UINT32_C(1) << (width - 1);
    const auto mask = (sign << 1) - 1;
    return static_cast<int32_t>(((u32 + sign) & mask) - sign);
}

Error ConfigBase::checkAddr(uint32_t addr, bool align) const {
    const auto width = static_cast<uint8_t>(addressWidth());
    const uint32_t max = 1UL << width;
    if (addr & ~(max - 1))
        return OVERFLOW_RANGE;
    if (align && addr % 2)
        return OPERAND_NOT_ALIGNED;
    return OK;
}

Error ConfigBase::checkAddr(uint32_t addr, uint32_t base, uint8_t width, bool align) const {
    const auto error = checkAddr(addr, align);
    if (error)
        return error;
    const uint32_t mask = ~((1UL << width) - 1);
    return (addr & mask) == (base & mask) ? OK : OVERWRAP_SEGMENT;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
