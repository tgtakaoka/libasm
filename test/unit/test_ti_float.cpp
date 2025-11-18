/*
 * Copyright 2025 Tadashi G. Takaoka
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
#include "test_unit_helper.h"
#include "ti_float.h"

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

const char *gcvt(const ti_float16_t &f16) {
    static char buf[80];
    f80 = static_cast<float80_t>(f16);
    f80.gcvt(buf, sizeof(buf), 5);
    return buf;
}

void test_ti_float16() {
    ti_float16_t f16;

    EQ("0.0", "8.000", f16.set(-8, 0).str());
    EQ("0.0", "0", gcvt(f16));
    EQ("0.0", OK, f16.set(f80));
    EQ("0.0", "8.000", f16.str());

    EQ("1.0", "0.000", f16.set(0, 0).str());
    EQ("1.0", "1", gcvt(f16));
    EQ("1.0", OK, f16.set(f80));
    EQ("1.0", "0.000", f16.str());

    EQ("-1.0", "0.800", f16.set(0, ti_float16_t::SGN_MASK).str());
    EQ("-1.0", "-1", gcvt(f16));
    EQ("-1.0", OK, f16.set(f80));
    EQ("-1.0", "0.800", f16.str());

    EQ("2.0", "1.000", f16.set(1, 0).str());
    EQ("2.0", "2", gcvt(f16));
    EQ("2.0", OK, f16.set(f80));
    EQ("2.0", "1.000", f16.str());

    EQ("-2.0", "1.800", f16.set(1, ti_float16_t::SGN_MASK).str());
    EQ("-2.0", "-2", gcvt(f16));
    EQ("-2.0", OK, f16.set(f80));
    EQ("-2.0", "1.800", f16.str());

    EQ("3.0", "1.400", f16.set(1, 0x400).str());
    EQ("3.0", "3", gcvt(f16));
    EQ("3.0", OK, f16.set(f80));
    EQ("3.0", "1.400", f16.str());

    EQ("-3.0", "1.C00", f16.set(1, ti_float16_t::SGN_MASK|0x400).str());
    EQ("-3.0", "-3", gcvt(f16));
    EQ("-3.0", OK, f16.set(f80));
    EQ("-3.0", "1.C00", f16.str());

    EQ("TI_FLT16_MAX_POS", "7.7FF", f16.set(ti_float16_t::EXP_MAX, ti_float16_t::FRAC_MASK).str());
    EQ("TI_FLT16_MAX_POS", STR(TI_FLT16_MAX_POS), gcvt(f16));
    EQ("TI_FLT16_MAX_POS", OK, f16.set(f80));
    EQ("TI_FLT16_MAX_POS", "7.7FF", f16.str());

    EQ("TI_FLT16_MIN_POS", "9.000", f16.set(ti_float16_t::EXP_MIN, 0).str());
    EQ("TI_FLT16_MIN_POS", STR(TI_FLT16_MIN_POS), gcvt(f16));
    EQ("TI_FLT16_MIN_POS", OK, f16.set(f80));
    EQ("TI_FLT16_MIN_POS", "9.000", f16.str());

    EQ("TI_FLT16_MAX_NEG", "7.800", f16.set(ti_float16_t::EXP_MAX, ti_float16_t::SGN_MASK).str());
    EQ("TI_FLT16_MAX_NEG", STR(TI_FLT16_MAX_NEG), gcvt(f16));
    EQ("TI_FLT16_MAX_NEG", OK, f16.set(f80));
    EQ("TI_FLT16_MAX_NEG", "7.800", f16.str());

    EQ("TI_FLT16_MIN_NEG", "9.FFF",
            f16.set(ti_float16_t::EXP_MIN, ti_float16_t::SGN_MASK | ti_float16_t::FRAC_MASK).str());
    EQ("TI_FLT16_MIN_NEG", STR(TI_FLT16_MIN_NEG), gcvt(f16));
    EQ("TI_FLT16_MIN_NEG", OK, f16.set(f80));
    EQ("TI_FLT16_MIN_NEG", "9.FFF", f16.str());

    EQ("DBL_MAX", OVERFLOW_RANGE, f16.set(read_f80(STR_DBL_MAX)));
    EQ("DBL_MIN", OK, f16.set(read_f80(STR_DBL_MIN)));
    EQ("DBL_MIN", "0", gcvt(f16));
}

void run_tests() {
    RUN_TEST(test_ti_float16);
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
