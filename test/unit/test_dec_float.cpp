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
#include "test_unit_helper.h"

#define __STR__(v) #v
#define STR(v) __STR__(v)

namespace libasm {
namespace test {

void set_up() {}

void tear_down() {}

const char *gcvt(const dec_float32_t &f32) {
    static char buf[80];
    static_cast<float80_t>(f32).gcvt(buf, sizeof(buf), 9);
    return buf;
}

void test_dec_float32() {
    dec_float32_t f32;
    EQ("0.0", "000.000000", f32.set(0, 0).str());
    EQ("0.0", "0", gcvt(f32));
    EQ("1.0", "081.000000", f32.set(1 + dec_float32_t::EXP_BASE, 0).str());
    EQ("1.0", "1", gcvt(f32));
    EQ("-1.0", "181.000000",
            f32.set(1 + dec_float32_t::EXP_BASE + dec_float32_t::SGN_MASK, 0).str());
    EQ("-1.0", "-1", gcvt(f32));

    EQ("DECF32_MAX", "0FF.FFFFFE", f32.set(dec_float32_t::EXP_MASK, dec_float32_t::SIG_MASK).str());
    EQ("DECF32_MAX", STR(DECF32_MAX), gcvt(f32));
    EQ("DECF32_MIN", "001.000000", f32.set(1, 0).str());
    EQ("DECF32_MIN", STR(DECF32_MIN), gcvt(f32));
}

const char *gcvt(const dec_float64_t &f64) {
    static char buf[80];
    static_cast<float80_t>(f64).gcvt(buf, sizeof(buf), 21);
    return buf;
}

void test_dec_float64() {
    dec_float64_t f64;
    EQ("0.0", "000.00000-00000000", f64.set(0, 0).str());
    EQ("0.0", "0", gcvt(f64));
    EQ("1.0", "081.00000-00000000", f64.set(1 + dec_float64_t::EXP_BASE, 0).str());
    EQ("1.0", "1", gcvt(f64));
    EQ("-1.0", "181.00000-00000000",
            f64.set(1 + dec_float64_t::EXP_BASE + dec_float64_t::SGN_MASK, 0).str());
    EQ("-1.0", "-1", gcvt(f64));

    EQ("DECF64_MAX", "0FF.FFFFF-FFFFFFFF",
            f64.set(dec_float64_t::EXP_MASK, dec_float64_t::SIG_MASK).str());
    EQ("DECF64_MAX", STR(DECF64_MAX), gcvt(f64));
    EQ("DECF64_MIN", "001.00000-00000000", f64.set(1, 0).str());
    EQ("DECF64_MIN", STR(DECF64_MIN), gcvt(f64));
}

void run_tests() {
    RUN_TEST(test_dec_float32);
    RUN_TEST(test_dec_float64);
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
