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

#include "float80_base.h"
#include "str_buffer.h"

namespace libasm {

const char *__float80_base::str(uint16_t tag, uint64_t sig) {
    static char buf[24];
    StrBuffer out{buf, sizeof(buf)};
    out.hex(tag)
            .letter('.')
            .hex(static_cast<uint32_t>(sig >> 32))
            .letter('-')
            .hex(static_cast<uint32_t>(sig));
    return buf;
}

int_fast16_t __float80_base::normalize(int_fast16_t exp, fixed64_t &sig) {
    exp += sig.normalize();
    if (exp <= -EXP_BASE) {
        const auto shifts = -(exp + EXP_BASE);
        sig.shift_right(shifts + 1, true);  // 1 for subnromal mark
        exp = -EXP_BASE;                    // for subnormal mark
        sig.round_off(MANT_DIG);
        if (sig.normal())
            exp++;  // Overflow subnormal, increase exponent
    } else if (sig.round_off(MANT_DIG)) {
        exp += sig.normalize();  // Overflowe, increase exponent
    }
    return exp;
}

int_fast16_t __float80_base::decompose(uint16_t tag, uint64_t significand, fixed64_t &sig) {
    sig.set(significand);
    if (isZero(tag, significand))
        return 0;
    if (sig.normal())
        return exponent(tag);
    return sig.normalize() + (-EXP_BASE + 1);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
