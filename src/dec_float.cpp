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

#include "dec_float.h"
#include "fixed64.h"
#include "str_buffer.h"

namespace libasm {

const char *__dec_float32::str() const {
    static char buf[16];
    StrBuffer out{buf, sizeof(buf)};
    const auto sig = _u32 & SIG_MASK;
    out.hex(tag(), 3).letter('.').hex(sig << 1, 6);
    return buf;
}

__dec_float32 &__dec_float32::set(uint16_t tag, uint32_t sig) {
    return set((static_cast<uint32_t>(tag) << TAG_POS) | (sig & SIG_MASK));
}

__dec_float32 &__dec_float32::set(uint32_t bits) {
    _u32 = bits;
    return *this;
}

__dec_float32::operator float80_t() const {
    const auto tag = static_cast<uint16_t>(_u32 >> TAG_POS);
    const auto sign = (tag & SGN_MASK) != 0;
    const auto bexp = tag & EXP_MASK;
    if (bexp == 0)
        return float80_t::zero(sign);
    const auto frac = (_u32 & SIG_MASK) | HIDDEN_MSB;
    const auto exp = static_cast<int16_t>(bexp) - EXP_BASE - 1;  // -1 for HIDDEN_MSB
    fixed64_t sig{static_cast<uint64_t>(frac) << (64 - MANT_DIG)};
    return float80_t::compose(sign, exp, sig);
}

const char *__dec_float64::str() const {
    static char buf[40];
    StrBuffer out{buf, sizeof(buf)};
    const auto sig = _u64 & SIG_MASK;
    out.hex(tag(), 3)
            .letter('.')
            .hex(static_cast<uint32_t>(sig >> 32), 5)
            .letter('-')
            .hex(static_cast<uint32_t>(sig));
    return buf;
}

__dec_float64 &__dec_float64::set(uint16_t tag, uint64_t sig) {
    return set((static_cast<uint64_t>(tag) << TAG_POS) | (sig & SIG_MASK));
}

__dec_float64 &__dec_float64::set(uint64_t bits) {
    _u64 = bits;
    return *this;
}

__dec_float64::operator float80_t() const {
    const auto tag = static_cast<uint16_t>(_u64 >> TAG_POS);
    const auto sign = (tag & SGN_MASK) != 0;
    const auto bexp = tag & EXP_MASK;
    if (bexp == 0)
        return float80_t::zero(sign);
    const auto frac = (_u64 & SIG_MASK) | HIDDEN_MSB;
    const auto exp = static_cast<int16_t>(bexp) - EXP_BASE - 1;  // -1 for HIDDEN_MSB
    fixed64_t sig{static_cast<uint64_t>(frac) << (64 - MANT_DIG)};
    return float80_t::compose(sign, exp, sig);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
