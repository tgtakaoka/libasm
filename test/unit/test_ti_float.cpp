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

const char *ecvt(const ti_float16_t &f16) {
    static char buf[80];
    f80 = static_cast<float80_t>(f16);
    f80.ecvt(buf, sizeof(buf), 4);
    return buf;
}

const char *gcvt(const ti_float32_t &f32) {
    static char buf[80];
    f80 = static_cast<float80_t>(f32);
    f80.gcvt(buf, sizeof(buf), 5);
    return buf;
}

const char *ecvt(const ti_float32_t &f32) {
    static char buf[80];
    f80 = static_cast<float80_t>(f32);
    f80.ecvt(buf, sizeof(buf), 9);
    return buf;
}

const char *gcvt(const ti_float40_t &f40) {
    static char buf[80];
    f80 = static_cast<float80_t>(f40);
    f80.gcvt(buf, sizeof(buf), 5);
    return buf;
}

const char *ecvt(const ti_float40_t &f40) {
    static char buf[80];
    f80 = static_cast<float80_t>(f40);
    f80.ecvt(buf, sizeof(buf), 10);
    return buf;
}

const char *gcvt(const ti_float16_external_t &f16) {
    static char buf[80];
    f80 = static_cast<float80_t>(f16);
    f80.gcvt(buf, sizeof(buf), 5);
    return buf;
}

const char *ecvt(const ti_float16_external_t &f16) {
    static char buf[80];
    f80 = static_cast<float80_t>(f16);
    f80.ecvt(buf, sizeof(buf), 7);
    return buf;
}

void test_ti_float16() {
    ti_float16_t f16;

    EQ("0.0", "8.000", f16.set(-8, 0x000).str());
    EQ("0.0", "0", gcvt(f16));
    EQ("0.0", OK, f16.set(f80));
    EQ("0.0", "8.000", f16.str());

    EQ("1.0", "0.000", f16.set(0, 0x000).str());
    EQ("1.0", "1", gcvt(f16));
    EQ("1.0", OK, f16.set(f80));
    EQ("1.0", "0.000", f16.str());

    EQ("-1.0", "F.800", f16.set(-1, ti_float16_t::SGN_MASK | 0x000).str());
    EQ("-1.0", "-1", gcvt(f16));
    EQ("-1.0", OK, f16.set(f80));
    EQ("-1.0", "F.800", f16.str());

    EQ("1.5", "0.400", f16.set(0, 0x400).str());
    EQ("1.5", "1.5", gcvt(f16));
    EQ("1.5", OK, f16.set(f80));
    EQ("1.5", "0.400", f16.str());

    EQ("-1.5", "0.C00", f16.set(0, ti_float16_t::SGN_MASK | 0x400).str());
    EQ("-1.5", "-1.5", gcvt(f16));
    EQ("-1.5", OK, f16.set(f80));
    EQ("-1.5", "0.C00", f16.str());

    EQ("2.0", "1.000", f16.set(1, 0x000).str());
    EQ("2.0", "2", gcvt(f16));
    EQ("2.0", OK, f16.set(f80));
    EQ("2.0", "1.000", f16.str());

    EQ("-2.0", "0.800", f16.set(0, ti_float16_t::SGN_MASK | 0x000).str());
    EQ("-2.0", "-2", gcvt(f16));
    EQ("-2.0", OK, f16.set(f80));
    EQ("-2.0", "0.800", f16.str());

    EQ("3.0", "1.400", f16.set(1, 0x400).str());
    EQ("3.0", "3", gcvt(f16));
    EQ("3.0", OK, f16.set(f80));
    EQ("3.0", "1.400", f16.str());

    EQ("-3.0", "1.C00", f16.set(1, ti_float16_t::SGN_MASK | 0x400).str());
    EQ("-3.0", "-3", gcvt(f16));
    EQ("-3.0", OK, f16.set(f80));
    EQ("-3.0", "1.C00", f16.str());

    EQ("TI_FLT16_MAX_POS", "7.7FF", f16.set(ti_float16_t::EXP_MAX, ti_float16_t::FRAC_MASK).str());
    EQ("TI_FLT16_MAX_POS", STR(TI_FLT16_MAX_POS), ecvt(f16));
    EQ("TI_FLT16_MAX_POS", OK, f16.set(f80));
    EQ("TI_FLT16_MAX_POS", "7.7FF", f16.str());

    EQ("TI_FLT16_MIN_POS", "9.000", f16.set(ti_float16_t::EXP_MIN, 0).str());
    EQ("TI_FLT16_MIN_POS", STR(TI_FLT16_MIN_POS), ecvt(f16));
    EQ("TI_FLT16_MIN_POS", OK, f16.set(f80));
    EQ("TI_FLT16_MIN_POS", "9.000", f16.str());

    EQ("TI_FLT16_MAX_NEG", "7.800", f16.set(ti_float16_t::EXP_MAX, ti_float16_t::SGN_MASK).str());
    EQ("TI_FLT16_MAX_NEG", STR(TI_FLT16_MAX_NEG), ecvt(f16));
    EQ("TI_FLT16_MAX_NEG", OK, f16.set(f80));
    EQ("TI_FLT16_MAX_NEG", "7.800", f16.str());

    EQ("TI_FLT16_MIN_NEG", "9.FFF",
            f16.set(ti_float16_t::EXP_MIN, ti_float16_t::SGN_MASK | ti_float16_t::FRAC_MASK).str());
    EQ("TI_FLT16_MIN_NEG", STR(TI_FLT16_MIN_NEG), ecvt(f16));
    EQ("TI_FLT16_MIN_NEG", OK, f16.set(f80));
    EQ("TI_FLT16_MIN_NEG", "9.FFF", f16.str());

    EQ("DBL_MAX", OVERFLOW_RANGE, f16.set(read_f80(STR_DBL_MAX)));
    EQ("DBL_MAX", STR(TI_FLT16_MAX_POS), ecvt(f16));
    EQ("DBL_MIN", UNDERFLOW_RANGE, f16.set(read_f80(STR_DBL_MIN)));
    EQ("DBL_MIN", "0", gcvt(f16));
}

void test_ti_float32() {
    ti_float32_t f32;

    EQ("0.0", "80.000000", f32.set(-128, 0x000000).str());
    EQ("0.0", "0", gcvt(f32));
    EQ("0.0", OK, f32.set(f80));
    EQ("0.0", "80.000000", f32.str());

    EQ("1.0", "00.000000", f32.set(0, 0x000000).str());
    EQ("1.0", "1", gcvt(f32));
    EQ("1.0", OK, f32.set(f80));
    EQ("1.0", "00.000000", f32.str());

    EQ("-1.0", "FF.800000", f32.set(-1, ti_float32_t::SGN_MASK | 0x0000000).str());
    EQ("-1.0", "-1", gcvt(f32));
    EQ("-1.0", OK, f32.set(f80));
    EQ("-1.0", "FF.800000", f32.str());

    EQ("1.5", "00.400000", f32.set(0, 0x400000).str());
    EQ("1.5", "1.5", gcvt(f32));
    EQ("1.5", OK, f32.set(f80));
    EQ("1.5", "00.400000", f32.str());

    EQ("-1.5", "00.C00000", f32.set(0, ti_float32_t::SGN_MASK | 0x400000).str());
    EQ("-1.5", "-1.5", gcvt(f32));
    EQ("-1.5", OK, f32.set(f80));
    EQ("-1.5", "00.C00000", f32.str());

    EQ("2.0", "01.000000", f32.set(1, 0x000000).str());
    EQ("2.0", "2", gcvt(f32));
    EQ("2.0", OK, f32.set(f80));
    EQ("2.0", "01.000000", f32.str());

    EQ("-2.0", "00.800000", f32.set(0, ti_float32_t::SGN_MASK | 0x000000).str());
    EQ("-2.0", "-2", gcvt(f32));
    EQ("-2.0", OK, f32.set(f80));
    EQ("-2.0", "00.800000", f32.str());

    EQ("3.0", "01.400000", f32.set(1, 0x400000).str());
    EQ("3.0", "3", gcvt(f32));
    EQ("3.0", OK, f32.set(f80));
    EQ("3.0", "01.400000", f32.str());

    EQ("-3.0", "01.C00000", f32.set(1, ti_float32_t::SGN_MASK | 0x400000).str());
    EQ("-3.0", "-3", gcvt(f32));
    EQ("-3.0", OK, f32.set(f80));
    EQ("-3.0", "01.C00000", f32.str());

    EQ("TI_FLT32_MAX_POS", "7F.7FFFFF",
            f32.set(ti_float32_t::EXP_MAX, ti_float32_t::FRAC_MASK).str());
    EQ("TI_FLT32_MAX_POS", STR(TI_FLT32_MAX_POS), ecvt(f32));
    EQ("TI_FLT32_MAX_POS", OK, f32.set(f80));
    EQ("TI_FLT32_MAX_POS", "7F.7FFFFF", f32.str());

    EQ("TI_FLT32_MIN_POS", "81.000000", f32.set(ti_float32_t::EXP_MIN, 0).str());
    EQ("TI_FLT32_MIN_POS", STR(TI_FLT32_MIN_POS), ecvt(f32));
    EQ("TI_FLT32_MIN_POS", OK, f32.set(f80));
    EQ("TI_FLT32_MIN_POS", "81.000000", f32.str());

    EQ("TI_FLT32_MAX_NEG", "7F.800000", f32.set(ti_float32_t::EXP_MAX, ti_float32_t::SGN_MASK).str());
    EQ("TI_FLT32_MAX_NEG", STR(TI_FLT32_MAX_NEG), ecvt(f32));
    EQ("TI_FLT32_MAX_NEG", OK, f32.set(f80));
    EQ("TI_FLT32_MAX_NEG", "7F.800000", f32.str());

    EQ("TI_FLT32_MIN_NEG", "81.FFFFFF",
            f32.set(ti_float32_t::EXP_MIN, ti_float32_t::SGN_MASK | ti_float32_t::FRAC_MASK).str());
    EQ("TI_FLT32_MIN_NEG", STR(TI_FLT32_MIN_NEG), ecvt(f32));
    EQ("TI_FLT32_MIN_NEG", OK, f32.set(f80));
    EQ("TI_FLT32_MIN_NEG", "81.FFFFFF", f32.str());

    EQ("DBL_MAX", OVERFLOW_RANGE, f32.set(read_f80(STR_DBL_MAX)));
    EQ("DBL_MAX", STR(TI_FLT32_MAX_POS), ecvt(f32));
    EQ("DBL_MIN", UNDERFLOW_RANGE, f32.set(read_f80(STR_DBL_MIN)));
    EQ("DBL_MIN", "0", gcvt(f32));
}

void test_ti_float40() {
    ti_float40_t f40;

    EQ("0.0", "80.00000000", f40.set(-128, 0x00000000).str());
    EQ("0.0", "0", gcvt(f40));
    EQ("0.0", OK, f40.set(f80));
    EQ("0.0", "80.00000000", f40.str());

    EQ("1.0", "00.00000000", f40.set(0, 0x00000000).str());
    EQ("1.0", "1", gcvt(f40));
    EQ("1.0", OK, f40.set(f80));
    EQ("1.0", "00.00000000", f40.str());

    EQ("-1.0", "FF.80000000", f40.set(-1, ti_float40_t::SGN_MASK | 0x000000000).str());
    EQ("-1.0", "-1", gcvt(f40));
    EQ("-1.0", OK, f40.set(f80));
    EQ("-1.0", "FF.80000000", f40.str());

    EQ("1.5", "00.40000000", f40.set(0, 0x40000000).str());
    EQ("1.5", "1.5", gcvt(f40));
    EQ("1.5", OK, f40.set(f80));
    EQ("1.5", "00.40000000", f40.str());

    EQ("-1.5", "00.C0000000", f40.set(0, ti_float40_t::SGN_MASK | 0x40000000).str());
    EQ("-1.5", "-1.5", gcvt(f40));
    EQ("-1.5", OK, f40.set(f80));
    EQ("-1.5", "00.C0000000", f40.str());

    EQ("2.0", "01.00000000", f40.set(1, 0x00000000).str());
    EQ("2.0", "2", gcvt(f40));
    EQ("2.0", OK, f40.set(f80));
    EQ("2.0", "01.00000000", f40.str());

    EQ("-2.0", "00.80000000", f40.set(0, ti_float40_t::SGN_MASK | 0x00000000).str());
    EQ("-2.0", "-2", gcvt(f40));
    EQ("-2.0", OK, f40.set(f80));
    EQ("-2.0", "00.80000000", f40.str());

    EQ("3.0", "01.40000000", f40.set(1, 0x40000000).str());
    EQ("3.0", "3", gcvt(f40));
    EQ("3.0", OK, f40.set(f80));
    EQ("3.0", "01.40000000", f40.str());

    EQ("-3.0", "01.C0000000", f40.set(1, ti_float40_t::SGN_MASK | 0x40000000).str());
    EQ("-3.0", "-3", gcvt(f40));
    EQ("-3.0", OK, f40.set(f80));
    EQ("-3.0", "01.C0000000", f40.str());

    EQ("TI_FLT40_MAX_POS", "7F.7FFFFFFF",
            f40.set(ti_float40_t::EXP_MAX, ti_float40_t::FRAC_MASK).str());
    EQ("TI_FLT40_MAX_POS", STR(TI_FLT40_MAX_POS), ecvt(f40));
    EQ("TI_FLT40_MAX_POS", OK, f40.set(f80));
    EQ("TI_FLT40_MAX_POS", "7F.7FFFFFFF", f40.str());

    EQ("TI_FLT40_MIN_POS", "81.00000000", f40.set(ti_float40_t::EXP_MIN, 0).str());
    EQ("TI_FLT40_MIN_POS", STR(TI_FLT40_MIN_POS), ecvt(f40));
    EQ("TI_FLT40_MIN_POS", OK, f40.set(f80));
    EQ("TI_FLT40_MIN_POS", "81.00000000", f40.str());

    EQ("TI_FLT40_MAX_NEG", "7F.80000000", f40.set(ti_float40_t::EXP_MAX, ti_float40_t::SGN_MASK).str());
    EQ("TI_FLT40_MAX_NEG", STR(TI_FLT40_MAX_NEG), ecvt(f40));
    EQ("TI_FLT40_MAX_NEG", OK, f40.set(f80));
    EQ("TI_FLT40_MAX_NEG", "7F.80000000", f40.str());

    EQ("TI_FLT40_MIN_NEG", "81.FFFFFFFF",
            f40.set(ti_float40_t::EXP_MIN, ti_float40_t::SGN_MASK | ti_float40_t::FRAC_MASK).str());
    EQ("TI_FLT40_MIN_NEG", STR(TI_FLT40_MIN_NEG), ecvt(f40));
    EQ("TI_FLT40_MIN_NEG", OK, f40.set(f80));
    EQ("TI_FLT40_MIN_NEG", "81.FFFFFFFF", f40.str());

    EQ("DBL_MAX", OVERFLOW_RANGE, f40.set(read_f80(STR_DBL_MAX)));
    EQ("DBL_MAX", STR(TI_FLT40_MAX_POS), ecvt(f40));
    EQ("DBL_MIN", UNDERFLOW_RANGE, f40.set(read_f80(STR_DBL_MIN)));
    EQ("DBL_MIN", "0", gcvt(f40));
}

void test_ti_float16_external() {
    ti_float16_external_t f16;

    EQ("0.0", "80.00", f16.set(-128, 0x00).str());
    EQ("0.0", "0", gcvt(f16));
    EQ("0.0", OK, f16.set(f80));
    EQ("0.0", "80.00", f16.str());

    EQ("1.0", "00.00", f16.set(0, 0x00).str());
    EQ("1.0", "1", gcvt(f16));
    EQ("1.0", OK, f16.set(f80));
    EQ("1.0", "00.00", f16.str());

    EQ("-1.0", "FF.80", f16.set(-1, ti_float16_external_t::SGN_MASK | 0x00).str());
    EQ("-1.0", "-1", gcvt(f16));
    EQ("-1.0", OK, f16.set(f80));
    EQ("-1.0", "FF.80", f16.str());

    EQ("1.5", "00.40", f16.set(0, 0x40).str());
    EQ("1.5", "1.5", gcvt(f16));
    EQ("1.5", OK, f16.set(f80));
    EQ("1.5", "00.40", f16.str());

    EQ("-1.5", "00.C0", f16.set(0, ti_float16_external_t::SGN_MASK | 0x40).str());
    EQ("-1.5", "-1.5", gcvt(f16));
    EQ("-1.5", OK, f16.set(f80));
    EQ("-1.5", "00.C0", f16.str());

    EQ("2.0", "01.00", f16.set(1, 0x00).str());
    EQ("2.0", "2", gcvt(f16));
    EQ("2.0", OK, f16.set(f80));
    EQ("2.0", "01.00", f16.str());

    EQ("-2.0", "00.80", f16.set(0, ti_float16_external_t::SGN_MASK | 0x00).str());
    EQ("-2.0", "-2", gcvt(f16));
    EQ("-2.0", OK, f16.set(f80));
    EQ("-2.0", "00.80", f16.str());

    EQ("3.0", "01.40", f16.set(1, 0x40).str());
    EQ("3.0", "3", gcvt(f16));
    EQ("3.0", OK, f16.set(f80));
    EQ("3.0", "01.40", f16.str());

    EQ("-3.0", "01.C0", f16.set(1, ti_float16_external_t::SGN_MASK | 0x40).str());
    EQ("-3.0", "-3", gcvt(f16));
    EQ("-3.0", OK, f16.set(f80));
    EQ("-3.0", "01.C0", f16.str());

    EQ("TI_FLT16EXT_MAX_POS", "7F.7F",
            f16.set(ti_float16_external_t::EXP_MAX, ti_float16_external_t::FRAC_MASK).str());
    EQ("TI_FLT16EXT_MAX_POS", STR(TI_FLT16EXT_MAX_POS), ecvt(f16));
    EQ("TI_FLT16EXT_MAX_POS", OK, f16.set(f80));
    EQ("TI_FLT16EXT_MAX_POS", "7F.7F", f16.str());

    EQ("TI_FLT16EXT_MIN_POS", "81.00", f16.set(ti_float16_external_t::EXP_MIN, 0).str());
    EQ("TI_FLT16EXT_MIN_POS", STR(TI_FLT16EXT_MIN_POS), ecvt(f16));
    EQ("TI_FLT16EXT_MIN_POS", OK, f16.set(f80));
    EQ("TI_FLT16EXT_MIN_POS", "81.00", f16.str());

    EQ("TI_FLT16EXT_MAX_NEG", "7F.80",
            f16.set(ti_float16_external_t::EXP_MAX, ti_float16_external_t::SGN_MASK).str());
    EQ("TI_FLT16EXT_MAX_NEG", STR(TI_FLT16EXT_MAX_NEG), ecvt(f16));
    EQ("TI_FLT16EXT_MAX_NEG", OK, f16.set(f80));
    EQ("TI_FLT16EXT_MAX_NEG", "7F.80", f16.str());

    EQ("TI_FLT16EXT_MIN_NEG", "81.FF",
            f16.set(ti_float16_external_t::EXP_MIN,
                       ti_float16_external_t::SGN_MASK | ti_float16_external_t::FRAC_MASK)
                    .str());
    EQ("TI_FLT16EXT_MIN_NEG", STR(TI_FLT16EXT_MIN_NEG), ecvt(f16));
    EQ("TI_FLT16EXT_MIN_NEG", OK, f16.set(f80));
    EQ("TI_FLT16EXT_MIN_NEG", "81.FF", f16.str());

    EQ("DBL_MAX", OVERFLOW_RANGE, f16.set(read_f80(STR_DBL_MAX)));
    EQ("DBL_MAX", STR(TI_FLT16EXT_MAX_POS), ecvt(f16));
    EQ("DBL_MIN", UNDERFLOW_RANGE, f16.set(read_f80(STR_DBL_MIN)));
    EQ("DBL_MIN", "0", gcvt(f16));
}

void run_tests() {
    RUN_TEST(test_ti_float16);
    RUN_TEST(test_ti_float32);
    RUN_TEST(test_ti_float40);
    RUN_TEST(test_ti_float16_external);
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
