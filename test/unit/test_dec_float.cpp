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

#include <float.h>
#include "dec_float.h"
#include "test_unit_helper.h"

#define __STR__(v) #v
#define STR(v) __STR__(v)
#define STR_DBL_MAX "1.79769313486231570814527423731704357e+308"
#define STR_DBL_MIN "2.22507385850720138309023271733240406e-308"

namespace libasm {
namespace test {

void set_up() {}

void tear_down() {}


float80_t f80;

const float80_t &read_f80(const char *str) {
    char *end;
    f80.read(str, &end);
    return f80;
}

const char *gcvt(const dec_float16_t &f16) {
    static char buf[80];
    f80 = static_cast<float80_t>(f16);
    f80.gcvt(buf, sizeof(buf), 3);
    return buf;
}

void test_dec_float16() {
    dec_float16_t f16;

    EQ("0.0", "000.00", f16.set(0, 0).str());
    EQ("0.0", "0", gcvt(f16));
    EQ("0.0", OK, f16.set(f80));
    EQ("0.0", "000.00", f16.str());

    EQ("1.0", "081.00", f16.set(1 + dec_float16_t::EXP_BASE, 0).str());
    EQ("1.0", "1", gcvt(f16));
    EQ("1.0", OK, f16.set(f80));
    EQ("1.0", "081.00", f16.str());

    EQ("-1.0", "181.00", f16.set(1 + dec_float16_t::EXP_BASE + dec_float16_t::SGN_MASK, 0).str());
    EQ("-1.0", "-1", gcvt(f16));
    EQ("-1.0", OK, f16.set(f80));
    EQ("-1.0", "181.00", f16.str());

    EQ("DECF16_MAX", "0FF.FE", f16.set(dec_float16_t::EXP_MASK, dec_float16_t::SIG_MASK).str());
    EQ("DECF16_MAX", STR(DECF16_MAX), gcvt(f16));
    EQ("DECF16_MAX", OK, f16.set(f80));
    EQ("DECF16_MAX", "0FF.FE", f16.str());

    EQ("DECF16_MIN", "001.00", f16.set(1, 0).str());
    EQ("DECF16_MIN", STR(DECF16_MIN), gcvt(f16));
    EQ("DECF16_MIN", OK, f16.set(f80));
    EQ("DECF16_MIN", "001.00", f16.str());

    EQ("DBL_MAX", OVERFLOW_RANGE, f16.set(read_f80(STR_DBL_MAX)));
    EQ("DBL_MIN", OK, f16.set(read_f80(STR_DBL_MIN)));
    EQ("DBL_MIN", "0", gcvt(f16));
}

const char *gcvt(const dec_float32_t &f32) {
    static char buf[80];
    f80 = static_cast<float80_t>(f32);
    f80.gcvt(buf, sizeof(buf), 9);
    return buf;
}

void test_dec_float32() {
    dec_float32_t f32;
    EQ("0.0", "000.000000", f32.set(0, 0).str());
    EQ("0.0", "0", gcvt(f32));
    EQ("0.0", OK, f32.set(f80));
    EQ("0.0", "000.000000", f32.str());

    EQ("1.0", "081.000000", f32.set(1 + dec_float32_t::EXP_BASE, 0).str());
    EQ("1.0", "1", gcvt(f32));
    EQ("1.0", OK, f32.set(f80));
    EQ("1.0", "081.000000", f32.str());

    EQ("-1.0", "181.000000",
            f32.set(1 + dec_float32_t::EXP_BASE + dec_float32_t::SGN_MASK, 0).str());
    EQ("-1.0", "-1", gcvt(f32));
    EQ("-1.0", OK, f32.set(f80));
    EQ("-1.0", "181.000000", f32.str());

    EQ("DECF32_MAX", "0FF.FFFFFE", f32.set(dec_float32_t::EXP_MASK, dec_float32_t::SIG_MASK).str());
    EQ("DECF32_MAX", STR(DECF32_MAX), gcvt(f32));
    EQ("DECF32_MAX", OK, f32.set(f80));
    EQ("DECF32_MAX", "0FF.FFFFFE", f32.str());

    EQ("DECF32_MIN", "001.000000", f32.set(1, 0).str());
    EQ("DECF32_MIN", STR(DECF32_MIN), gcvt(f32));
    EQ("DECF32_MIN", OK, f32.set(f80));
    EQ("DECF32_MIN", "001.000000", f32.str());

    EQ("DBL_MAX", OVERFLOW_RANGE, f32.set(read_f80(STR_DBL_MAX)));
    EQ("DBL_MIN", OK, f32.set(read_f80(STR_DBL_MIN)));
    EQ("DBL_MIN", "0", gcvt(f32));
}

const char *gcvt(const dec_float64_t &f64) {
    static char buf[80];
    f80 = static_cast<float80_t>(f64);
    f80.gcvt(buf, sizeof(buf), 21);
    return buf;
}

void test_dec_float64() {
    dec_float64_t f64;
    EQ("0.0", "000.00000-00000000", f64.set(0, 0).str());
    EQ("0.0", "0", gcvt(f64));
    EQ("0.0", OK, f64.set(f80));
    EQ("0.0", "000.00000-00000000", f64.str());

    EQ("1.0", "081.00000-00000000", f64.set(1 + dec_float64_t::EXP_BASE, 0).str());
    EQ("1.0", "1", gcvt(f64));
    EQ("1.0", OK, f64.set(f80));
    EQ("1.0", "081.00000-00000000", f64.str());

    EQ("-1.0", "181.00000-00000000",
            f64.set(1 + dec_float64_t::EXP_BASE + dec_float64_t::SGN_MASK, 0).str());
    EQ("-1.0", "-1", gcvt(f64));
    EQ("-1.0", OK, f64.set(f80));
    EQ("-1.0", "181.00000-00000000", f64.str());

    EQ("DECF64_MAX", "0FF.FFFFF-FFFFFFFF",
            f64.set(dec_float64_t::EXP_MASK, dec_float64_t::SIG_MASK).str());
    EQ("DECF64_MAX", STR(DECF64_MAX), gcvt(f64));
    EQ("DECF64_MAX", OK, f64.set(f80));
    EQ("DECF64_MAX", "0FF.FFFFF-FFFFFFFF", f64.str());

    EQ("DECF64_MIN", "001.00000-00000000", f64.set(1, 0).str());
    EQ("DECF64_MIN", STR(DECF64_MIN), gcvt(f64));
    EQ("DECF64_MIN", OK, f64.set(f80));
    EQ("DECF64_MIN", "001.00000-00000000", f64.str());

    EQ("DBL_MAX", OVERFLOW_RANGE, f64.set(read_f80(STR_DBL_MAX)));
    EQ("DBL_MIN", OK, f64.set(read_f80(STR_DBL_MIN)));
    EQ("DBL_MIN", "0", gcvt(f64));
}

void run_tests() {
    RUN_TEST(test_dec_float16);
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
