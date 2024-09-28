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
#include <math.h>
#include "ieee_float.h"
#include "test_unit_helper.h"

namespace libasm {
namespace test {

using f64 = float64_t;

constexpr auto INT53_MIN = -INT64_C(0x00010000000000000);
constexpr auto INT53_MAX = INT64_C(0x0000FFFFFFFFFFFFF);
constexpr auto UINT53_MAX = UINT64_C(0x0001FFFFFFFFFFFFF);
constexpr auto UINT54_MIN = UINT64_C(0x00020000000000000);
// String representation
constexpr char tINT53_MIN[] = "-4503599627370496";           // -0x1.0000000000000p+52
constexpr char tINT53_MAX[] = "4503599627370495";            // +0x1.0000000000000p+52
constexpr char tUINT53_MAX[] = "9007199254740991";           // +0x1.FFFFFFFFFFFFFp+53
constexpr char tUINT54_MIN[] = "9007199254740992";           // +0x1.0000000000000p+54
constexpr char tDBL_MAX[] = "1.7976931348623157e+308";       // +0x1.FFFFFFFFFFFFFp+1023
constexpr char tDBL_MIN[] = "2.2250738585072014e-308";       // +0x1.0000000000000p-1022
constexpr char tDBL_SUBN_MAX[] = "2.2250738585072009e-308";  // +0x0.FFFFFFFFFFFFFp-1022
constexpr char tDBL_TRUE_MIN[] = "4.9406564584124654e-324";  // +0x0.0000000000001p-1022

void set_up() {}

void tear_down() {}

void test_decompose() {
    fixed64_t sig;

    EQ("+0", 0, f64(0, UINT64_C(0)).decompose(sig));
    EQ("+0", "0000-00000000-00000000-0000", sig.str());

    EQ("-0", 0, f64(0x800, UINT64_C(0)).decompose(sig));
    EQ("-0", "0000-00000000-00000000-0000", sig.str());

    EQ("+1", 0, f64(0x3ff, UINT64_C(0)).decompose(sig));
    EQ("+1", "0000-80000000-00000000-0000", sig.str());

    EQ("-1", 0, f64(0xbff, UINT64_C(0)).decompose(sig));
    EQ("-1", "0000-80000000-00000000-0000", sig.str());

    EQ("DBL_MAX", 1023, f64(0x7fe, UINT64_C(0xFFFFFFFFFFFFFFF)).decompose(sig));
    EQ("DBL_MAX", "0000-FFFFFFFF-FFFFF800-0000", sig.str());

    EQ("DBL_MIN", -1022, f64(0x001, UINT64_C(0)).decompose(sig));
    EQ("DBL_MIN", "0000-80000000-00000000-0000", sig.str());

    EQ("DBL_SUBN_MAX", -1022 - 1, f64(0, UINT64_C(0x7FFFFFFFFFFFFFF)).decompose(sig));
    EQ("DBL_SUBN_MAX", "0000-FFFFFFFF-FFFFF000-0000", sig.str());

    EQ("DBL_TRUE_MIN", -1022 - 52, f64(0, UINT64_C(1)).decompose(sig));
    EQ("DBL_TRUE_MIN", "0000-80000000-00000000-0000", sig.str());
}

void test_compose() {
    fixed64_t sig;

    EQ("+0", "000.00000-00000000", f64::compose(false, 0, sig).str());
    EQ("-0", "800.00000-00000000", f64::compose(true, 0, sig).str());

    EQ("+1", "0000-80000000-00000000-0000", sig.set(UINT64_C(0x8000000000000000)).str());
    EQ("+1", "3FF.00000-00000000", f64::compose(false, 0, sig).str());
    EQ("-1", "BFF.00000-00000000", f64::compose(true, 0, sig).str());

    EQ("DBL_MAX", "0000-FFFFFFFF-FFFFF800-0000", sig.set(UINT64_C(0xFFFFFFFFFFFFF800)).str());
    EQ("DBL_MAX", "7FE.FFFFF-FFFFFFFF", f64::compose(false, 1023, sig).str());

    EQ("DBL_MIN", "0000-80000000-00000000-0000", sig.set(UINT64_C(0x8000000000000000)).str());
    EQ("DBL_MIN", "001.00000-00000000", f64::compose(false, -1022, sig).str());

    EQ("DBL_SUBN_MAX", "0000-FFFFFFFF-FFFFF000-0000", sig.set(UINT64_C(0xFFFFFFFFFFFFF000)).str());
    EQ("DBL_SUBN_MAX", "000.FFFFF-FFFFFFFF", f64::compose(false, -1022 - 1, sig).str());

    EQ("DBL_TRUE_MIN", "0000-80000000-00000000-0000", sig.set(UINT64_C(0x8000000000000000)).str());
    EQ("DBL_TRUE_MIN", "000.00000-00000001", f64::compose(false, -1022 - 52, sig).str());

    // normal significand
    EQ("round=", "0000-80000000-00000400-0000", sig.set(UINT64_C(0x8000000000000400)).str());
    EQ("round=", "3FF.00000-00000000", f64::compose(false, 0, sig).str());
    EQ("round+", "0000-80000000-00000401-0000", sig.set(UINT64_C(0x8000000000000401)).str());
    EQ("round+", "3FF.00000-00000001", f64::compose(false, 0, sig).str());
    EQ("round-", "0000-80000000-00000BFF-0000", sig.set(UINT64_C(0x8000000000000BFF)).str());
    EQ("round-", "3FF.00000-00000001", f64::compose(false, 0, sig).str());
    EQ("round+", "0000-80000000-00000C00-0000", sig.set(UINT64_C(0x8000000000000C00)).str());
    EQ("round+", "3FF.00000-00000002", f64::compose(false, 0, sig).str());
    EQ("round-", "0000-FFFFFFFF-FFFFFBFF-0000", sig.set(UINT64_C(0xFFFFFFFFFFFFFBFF)).str());
    EQ("round-", "3FF.FFFFF-FFFFFFFF", f64::compose(false, 0, sig).str());
    EQ("round+", "0000-FFFFFFFF-FFFFFC00-0000", sig.set(UINT64_C(0xFFFFFFFFFFFFFC00)).str());
    EQ("round+", "400.00000-00000000", f64::compose(false, 0, sig).str());

    // subnormal significand
    EQ("round=", "0000-87F00000-00000000-0000", sig.set(UINT64_C(0x87F0000000000000)).str());
    EQ("round=", "000.00000-00000008", f64::compose(false, -1022 - 49, sig).str());
    EQ("round=", "0000-88000000-00000000-0000", sig.set(UINT64_C(0x8800000000000000)).str());
    EQ("round=", "000.00000-00000008", f64::compose(false, -1022 - 49, sig).str());
    EQ("round+", "0000-88100000-00000000-0000", sig.set(UINT64_C(0x8810000000000000)).str());
    EQ("round+", "000.00000-00000009", f64::compose(false, -1022 - 49, sig).str());
    EQ("round-", "0000-97F00000-00000000-0000", sig.set(UINT64_C(0x97F0000000000000)).str());
    EQ("round-", "000.00000-00000009", f64::compose(false, -1022 - 49, sig).str());
    EQ("round+", "0000-98000000-00000000-0000", sig.set(UINT64_C(0x9800000000000000)).str());
    EQ("round+", "000.00000-0000000A", f64::compose(false, -1022 - 49, sig).str());
    EQ("round-", "0000-FFFFFFFF-FFFFF7FF-0000", sig.set(UINT64_C(0xFFFFFFFFFFFFF7FF)).str());
    EQ("round-", "000.FFFFF-FFFFFFFF", f64::compose(false, -1022 - 1, sig).str());
    EQ("round+", "0000-FFFFFFFF-FFFFF800-0000", sig.set(UINT64_C(0xFFFFFFFFFFFFF800)).str());
    EQ("round+", "001.00000-00000000", f64::compose(false, -1022 - 1, sig).str());
}

void test_ctor() {
    const auto p0 = f64{UINT64_C(0)};  // +0
    EQ("+0", 0, p0.tag());
    EQ("+0", 0, p0.significand());
    TRUE("+0", p0.isZero());
    FALSE("+0", p0.isNegative());
    TRUE("+0", p0.isFractional());
    FALSE("+0", p0.isInf());
    FALSE("+0", p0.isNan());

    const auto m0 = f64::zero(true);  // -0
    EQ("-0", 0x800, m0.tag());
    EQ("-0", 0, m0.significand());
    TRUE("-0", m0.isZero());
    TRUE("-0", m0.isNegative());
    FALSE("-0", m0.isFractional());
    FALSE("-0", m0.isInf());
    FALSE("-0", m0.isNan());

    const f64 p1{0x3FF, UINT64_C(0)};  // +1
    EQ("+1", 0x3FF, p1.tag());
    EQ("+1", UINT64_C(0x8000000000000000), p1.significand());
    FALSE("+1", p1.isZero());
    FALSE("+1", p1.isNegative());
    FALSE("+1", p1.isFractional());
    FALSE("+1", p1.isInf());
    FALSE("+1", p1.isNan());

    const f64 m1{0x800 | 0x3FF, UINT64_C(0)};  // -1
    EQ("-1", 0xBFF, m1.tag());
    EQ("-1", UINT64_C(0x8000000000000000), m1.significand());
    FALSE("-1", m1.isZero());
    TRUE("-1", m1.isNegative());
    FALSE("-1", m1.isFractional());
    FALSE("-1", m1.isInf());
    FALSE("-1", m1.isNan());

    const f64 p099{-1 + 0x3FF, UINT64_C(0xFFFFFFFFFFFFF)};
    EQ("0.99..", -1 + 0x3FF, p099.tag());
    EQ("0.99..", UINT64_C(0xFFFFFFFFFFFFF800), p099.significand());
    FALSE("0.99..", p099.isZero());
    FALSE("0.99..", p099.isNegative());
    TRUE("0.99..", p099.isFractional());
    FALSE("0.99..", p099.isInf());
    FALSE("0.99..", p099.isNan());

    const f64 I53_MAX{51 + 0x3FF, UINT64_C(0x000FFFFFFFFFFFFF)};
    EQ("INT53_MAX", 51 + 0x3FF, I53_MAX.tag());
    EQ("INT53_MAX", UINT64_C(0xFFFFFFFFFFFFF800), I53_MAX.significand());

    const f64 I53_MIN{0x800 | (52 + 0x3FF), UINT64_C(0x0010000000000000)};
    EQ("INT53_MIN", 0x800 | (52 + 0x3FF), I53_MIN.tag());
    EQ("INT53_MIN", UINT64_C(0x8000000000000000), I53_MIN.significand());

    const f64 U53_MAX{52 + 0x3FF, UINT64_C(0x001FFFFFFFFFFFFF)};
    EQ("UINT53_MAX", 52 + 0x3FF, U53_MAX.tag());
    EQ("UINT53_MAX", UINT64_C(0xFFFFFFFFFFFFF800), U53_MAX.significand());

    const f64 F64_MAX{1023 + 0x3FF, UINT64_C(0xFFFFFFFFFFFFF)};  // DBL_MAX
    EQ("DBL_MAX", 1023 + 0x3FF, F64_MAX.tag());
    EQ("DBL_MAX", UINT64_C(0xFFFFFFFFFFFFF800), F64_MAX.significand());

    const f64 F64_MIN{1, UINT64_C(0)};  // DBL_MIN
    EQ("DBL_MIN", -1022 + 0x3FF, F64_MIN.tag());
    EQ("DBL_MIN", UINT64_C(0x8000000000000000), F64_MIN.significand());

    const f64 F64_SUBN_MAX{0, UINT64_C(0xFFFFFFFFFFFFF)};  // DBL_SUBN_MAX
    EQ("DBL_SUBN_MAX", 0, F64_SUBN_MAX.tag());
    EQ("DBL_SUBN_MAX", UINT64_C(0x7FFFFFFFFFFFF800), F64_SUBN_MAX.significand());

    const f64 F64_TRUE_MIN{0, UINT64_C(1)};  // DBL_TRUE_MIN
    EQ("DBL_TRUE_MIN", 0, F64_TRUE_MIN.tag());
    EQ("DBL_TRUE_MIN", UINT64_C(0x0000000000000800), F64_TRUE_MIN.significand());

    const auto pinf = f64::infinity(false);  // +INF
    EQ("+INF", 0x7FF, pinf.tag());
    EQ("+INF", 0, pinf.significand());
    FALSE("+INF", pinf.isZero());
    FALSE("+INF", pinf.isNegative());
    FALSE("+INF", pinf.isFractional());
    TRUE("+INF", pinf.isInf());
    FALSE("+INF", pinf.isNan());

    const auto minf = f64::infinity(true);  // -INF
    EQ("-INF", 0xFFF, minf.tag());
    EQ("-INF", 0, minf.significand());
    FALSE("-INF", minf.isZero());
    TRUE("-INF", minf.isNegative());
    FALSE("-INF", minf.isFractional());
    TRUE("-INF", minf.isInf());
    FALSE("-INF", minf.isNan());

    const auto pnan = f64::not_a_number(false);  // +NAN
    EQ("+NAN", 0x7FF, pnan.tag());
    FALSE("+NAN", pnan.significand() == 0);
    FALSE("+NAN", pnan.isZero());
    FALSE("+NAN", pnan.isNegative());
    FALSE("+NAN", pnan.isFractional());
    TRUE("+NAN", pnan.isNan());
    FALSE("+NAN", pnan.isInf());

    const auto mnan = f64::not_a_number(true);  // -NAN
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
    RUN_TEST(test_ctor);
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
