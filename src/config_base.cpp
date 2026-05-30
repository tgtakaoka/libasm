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
    if (bitw >= 32)
        return false;
    const auto sign = INT32_C(1) << (bitw - 1);
    return s32 < -sign || s32 >= sign;
}

int32_t ConfigBase::signExtend(uint32_t u32, uint_fast8_t width) {
    // width 0 (degenerate) or >= 32 has no sign bit to extend within uint32_t.
    if (width == 0 || width >= 32)
        return static_cast<int32_t>(u32);
    const auto sign = UINT32_C(1) << (width - 1);
    const auto mask = (sign << 1) - 1;
    return static_cast<int32_t>(((u32 + sign) & mask) - sign);
}

Error ConfigBase::checkAddr(uint32_t addr, bool align) const {
    const auto width = static_cast<uint8_t>(addressWidth());
    // width 32 spans the whole uint32_t, so nothing can overflow; 1<<32 is UB
    // where long is 32-bit (e.g. AVR).
    if (width < 32) {
        const uint32_t max = UINT32_C(1) << width;
        if (addr & ~(max - 1))
            return OVERFLOW_RANGE;
    }
    if (align && addr % 2)
        return OPERAND_NOT_ALIGNED;
    return OK;
}

Error ConfigBase::checkAddr(uint32_t addr, uint32_t base, uint8_t width, bool align) const {
    const auto error = checkAddr(addr, align);
    if (error)
        return error;
    const uint32_t mask = width >= 32 ? 0 : ~((UINT32_C(1) << width) - 1);
    return (addr & mask) == (base & mask) ? OK : OVERWRAP_SEGMENT;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
