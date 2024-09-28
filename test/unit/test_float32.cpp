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
#include "fixed64.h"
#include "ieee_float.h"
#include "test_unit_helper.h"

namespace libasm {
namespace test {

using f32 = float32_t;

constexpr auto INT24_MAX = INT32_C(0x7FFFFF);
constexpr auto INT24_MIN = -INT32_C(0x800000);
constexpr auto UINT24_MAX = UINT32_C(0xFFFFFF);
constexpr auto UINT25_MIN = UINT32_C(0x1000000);
// String representation
constexpr char tINT24_MAX[] = "8388507";            // +0x1.FFFFFCp+22
constexpr char tINT24_MIN[] = "-8388608";           // -0x1.000000p+23
constexpr char tUINT24_MAX[] = "16777215";          // +0x1.FFFFFEp+23
constexpr char tUINT25_MIN[] = "16777216";          // +0x1.000000p+24
constexpr char tFLT_MAX[] = "3.40282347e+38";       // +0x1.FFFFFEp+127
constexpr char tFLT_MIN[] = "1.17549435e-38";       // +0x1.000000p-126
constexpr char tFLT_SUBN_MAX[] = "1.17549421e-38";  // +0x0.FFFFFCp-127
constexpr char tFLT_TRUE_MIN[] = "1.40129846e-45";  // +0x0.000000p-149

void set_up() {}

void tear_down() {}

void test_decompose() {
    fixed64_t sig;

    EQ("+0", 0, f32(0x000, UINT32_C(0)).decompose(sig));
    EQ("+0", "0000-00000000-00000000-0000", sig.str());

    EQ("-0", 0, f32(0x100, UINT32_C(0)).decompose(sig));
    EQ("-0", "0000-00000000-00000000-0000", sig.str());

    EQ("+1", 0, f32(0x07f, UINT32_C(0)).decompose(sig));
    EQ("+1", "0000-80000000-00000000-0000", sig.str());

    EQ("-1", 0, f32(0x17f, UINT32_C(0)).decompose(sig));
    EQ("-1", "0000-80000000-00000000-0000", sig.str());

    EQ("FLT_MAX", 127, f32(0x0fe, UINT32_C(0x7FFFFF)).decompose(sig));
    EQ("FLT_MAX", "0000-FFFFFF00-00000000-0000", sig.str());

    EQ("FLT_MIN", -126, f32(0x001, UINT32_C(0)).decompose(sig));
    EQ("FLT_MIN", "0000-80000000-00000000-0000", sig.str());

    EQ("FLT_SUBN_MAX", -126 - 1, f32(0, UINT32_C(0x7FFFFF)).decompose(sig));
    EQ("FLT_SUBN_MAX", "0000-FFFFFE00-00000000-0000", sig.str());

    EQ("FLT_TRUE_MIN", -126 - 23, f32(0, UINT32_C(1)).decompose(sig));
    EQ("FLT_TRUE_MIN", "0000-80000000-00000000-0000", sig.str());
}

void test_compose() {
    fixed64_t sig;

    EQ("+0", "000.000000", f32::compose(false, 0, sig).str());
    EQ("-0", "100.000000", f32::compose(true, 0, sig).str());

    EQ("+1", "0000-80000000-00000000-0000", sig.set(UINT32_C(0x8000000000000000)).str());
    EQ("+1", "07F.000000", f32::compose(false, 0, sig).str());
    EQ("-1", "17F.000000", f32::compose(true, 0, sig).str());

    EQ("FLT_MAX", "0000-FFFFFF00-00000000-0000", sig.set(UINT32_C(0xFFFFFF0000000000)).str());
    EQ("FLT_MAX", "0FE.FFFFFE", f32::compose(false, 127, sig).str());

    EQ("FLT_MIN", "0000-80000000-00000000-0000", sig.set(UINT32_C(0x8000000000000000)).str());
    EQ("FLT_MIN", "001.000000", f32::compose(false, -126, sig).str());

    EQ("FLT_SUBN_MAX", "0000-FFFFFE00-00000000-0000", sig.set(UINT32_C(0xFFFFFE0000000000)).str());
    EQ("FLT_SUBN_MAX", "000.FFFFFE", f32::compose(false, -126 - 1, sig).str());

    EQ("FLT_TRUE_MIN", "0000-80000000-00000000-0000", sig.set(UINT32_C(0x8000000000000000)).str());
    EQ("FLT_TRUE_MIN", "000.000002", f32::compose(false, -126 - 23, sig).str());

    // normal significand
    EQ("round=", "0000-80000080-00000000-0000", sig.set(UINT32_C(0x8000008000000000)).str());
    EQ("round=", "07F.000000", f32::compose(false, 0, sig).str());
    EQ("round+", "0000-80000080-00000001-0000", sig.set(UINT32_C(0x8000008000000001)).str());
    EQ("round+", "07F.000002", f32::compose(false, 0, sig).str());
    EQ("round-", "0000-8000007F-FFFFFFFF-0000", sig.set(UINT32_C(0x8000007FFFFFFFFF)).str());
    EQ("round-", "07F.000000", f32::compose(false, 0, sig).str());
    EQ("round+", "0000-80000180-00000000-0000", sig.set(UINT32_C(0x8000018000000000)).str());
    EQ("round+", "07F.000004", f32::compose(false, 0, sig).str());
    EQ("round-", "0000-FFFFFF7F-FFFFFFFF-0000", sig.set(UINT32_C(0xFFFFFF7FFFFFFFFF)).str());
    EQ("round-", "07F.FFFFFE", f32::compose(false, 0, sig).str());
    EQ("round+", "0000-FFFFFF80-00000000-0000", sig.set(UINT32_C(0xFFFFFF8000000000)).str());
    EQ("round+", "080.000000", f32::compose(false, 0, sig).str());

    // subnormal significand
    EQ("round=", "0000-87F00000-00000000-0000", sig.set(UINT32_C(0x87F0000000000000)).str());
    EQ("round=", "000.000010", f32::compose(false, -126 - 20, sig).str());
    EQ("round=", "0000-88000000-00000000-0000", sig.set(UINT32_C(0x8800000000000000)).str());
    EQ("round=", "000.000010", f32::compose(false, -126 - 20, sig).str());
    EQ("round+", "0000-88100000-00000000-0000", sig.set(UINT32_C(0x8810000000000000)).str());
    EQ("round+", "000.000012", f32::compose(false, -126 - 20, sig).str());
    EQ("round-", "0000-97F00000-00000000-0000", sig.set(UINT32_C(0x97F0000000000000)).str());
    EQ("round-", "000.000012", f32::compose(false, -126 - 20, sig).str());
    EQ("round+", "0000-98000000-00000000-0000", sig.set(UINT32_C(0x9800000000000000)).str());
    EQ("round+", "000.000014", f32::compose(false, -126 - 20, sig).str());
    EQ("round-", "0000-FFFFFEFF-FFFFFFFF-0000", sig.set(UINT32_C(0xFFFFFEFFFFFFFFFF)).str());
    EQ("round-", "000.FFFFFE", f32::compose(false, -126 - 1, sig).str());
    EQ("round+", "0000-FFFFFF00-00000000-0000", sig.set(UINT32_C(0xFFFFFF0000000000)).str());
    EQ("round+", "001.000000", f32::compose(false, -126 - 1, sig).str());
}

void test_ctor() {
    const f32 p099{-1 + 0x7F, UINT32_C(0x7FFFFF)};
    EQ("0.99..", 0x17E, p099.tag());
    EQ("0.99..", UINT32_C(0xFFFFFFFFFFFFF800), p099.significand());
    FALSE("0.99..", p099.isZero());
    FALSE("0.99..", p099.isNegative());
    TRUE("0.99..", p099.isFractional());
    FALSE("0.99..", p099.isInf());
    FALSE("0.99..", p099.isNan());

    const f32 I24_MAX{23 + 0x7F, UINT32_C(0x7FFFFF)};
    EQ("INT24_MAX", 23 + 0x7F, I24_MAX.tag());
    EQ("INT24_MAX", UINT32_C(0xFFFFFF00), I24_MAX.significand());

    const f32 I24_MIN{0x100 | (23 + 0x7F), 0};
    EQ("INT24_MIN", 0x100 | (23 + 0x7F), I24_MIN.tag());
    EQ("INT24_MIN", UINT32_C(0x80000000), I24_MIN.significand());

    const f32 U24_MAX{24 + 0x7F, UINT32_C(0x7FFFFF)};
    EQ("UINT24_MAX", 23 + 0x7F, U24_MAX.tag());
    EQ("UINT24_MAX", UINT32_C(0xFFFFFE00), U24_MAX.significand());

    const f32 F32_MAX{127 + 0x7F, UINT32_C(0x7FFFFFF)};  // FLT_MAX
    EQ("FLT_MAX", 127 + 0x7F, F32_MAX.tag());
    EQ("FLT_MAX", UINT32_C(0xFFFFFFE00), F32_MAX.significand());

    const f32 F32_MIN{-126 + 0x7F, UINT32_C(0)};  // FLT_MIN
    EQ("FLT_MIN", -126 + 0x7F, F32_MIN.tag());
    EQ("FLT_MIN", UINT32_C(0x800000000), F32_MIN.significand());

    const f32 F32_SUBN_MAX{0, UINT32_C(0x7FFFFFF)};  // FLT_SUBN_MAX
    EQ("FLT_SUBN_MAX", 0, F32_SUBN_MAX.tag());
    EQ("FLT_SUBN_MAX", UINT32_C(0x7FFFFF00), F32_SUBN_MAX.significand());

    const f32 F32_TRUE_MIN{0, UINT32_C(1)};  // FLT_TRUE_MIN
    EQ("FLT_TRUE_MIN", 0, F32_TRUE_MIN.tag());
    EQ("FLT_TRUE_MIN", UINT32_C(0x00000200), F32_TRUE_MIN.significand());

    const auto pinf = f32::infinity(false);  // +INF
    EQ("+INF", 0x7FF, pinf.tag());
    EQ("+INF", 0, pinf.significand());
    FALSE("+INF", pinf.isZero());
    FALSE("+INF", pinf.isNegative());
    FALSE("+INF", pinf.isFractional());
    TRUE("+INF", pinf.isInf());
    FALSE("+INF", pinf.isNan());

    const auto minf = f32::infinity(true);  // -INF
    EQ("-INF", 0xFFF, minf.tag());
    EQ("-INF", 0, minf.significand());
    FALSE("-INF", minf.isZero());
    TRUE("-INF", minf.isNegative());
    FALSE("-INF", minf.isFractional());
    TRUE("-INF", minf.isInf());
    FALSE("-INF", minf.isNan());

    const auto pnan = f32::not_a_number(false);  // +NAN
    EQ("+NAN", 0x7FF, pnan.tag());
    FALSE("+NAN", pnan.significand() == 0);
    FALSE("+NAN", pnan.isZero());
    FALSE("+NAN", pnan.isNegative());
    FALSE("+NAN", pnan.isFractional());
    TRUE("+NAN", pnan.isNan());
    FALSE("+NAN", pnan.isInf());

    const auto mnan = f32::not_a_number(true);  // -NAN
    EQ("-NAN", 0xFFF, mnan.tag());
    FALSE("-NAN", mnan.significand() == 0);
    FALSE("-NAN", mnan.isZero());
    TRUE("-NAN", mnan.isNegative());
    FALSE("-NAN", mnan.isFractional());
    TRUE("-NAN", mnan.isNan());
    FALSE("-NAN", mnan.isInf());
}

void run_tests() {
    RUN_TEST(test_decompose);
    RUN_TEST(test_compose);
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
