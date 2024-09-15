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

#include <string>

#define SEQ(msg, actual, expected)        \
    do {                                  \
        std::string a = (actual).str();   \
        std::string e = (expected).str(); \
        EQ(msg, e.c_str(), a.c_str());    \
    } while (0)

namespace libasm {
namespace test {

// String representation
constexpr char tINT64_MIN[] = "-9223372036854775808";              // -0x8.000000000000000p+60
constexpr char tINT64_MAX[] = "9223372036854775807";               // +0xF.FFFFFFFFFFFFFFEp+59
constexpr char tUINT64_MAX[] = "18446744073709551615";             // +0xF.FFFFFFFFFFFFFFFp+60
constexpr char tUINT65_MIN[] = "18446744073709551616";             // +0x8.000000000000000p+61
constexpr char tLDBL_MAX[] = "1.18973149535723176502e+4932";       // +0xF.FFFFFFFFFFFFFFFp+16380
constexpr char tLDBL_MIN[] = "3.36210314311209350626e-4932";       // +0x8.000000000000000p-16385
constexpr char tLDBL_SUBN_MAX[] = "3.3621031431120935059e-4932";   // +0x7.FFFFFFFFFFFFFFFp-16385
constexpr char tLDBL_TRUE_MIN[] = "3.64519953188247460253e-4951";  // +0x0.000000000000001p-16385

void test_decompose() {
    fixed64_t sig;

    EQ("+0", 0, f80(0, UINT64_C(0)).decompose(sig));
    EQ("+0", "0000-00000000-00000000-0000", sig.str());

    EQ("-0", 0, f80(0x8000, UINT64_C(0)).decompose(sig));
    EQ("-0", "0000-00000000-00000000-0000", sig.str());

    EQ("+1", 0, f80(0x3FFF, UINT64_C(0x8000000000000000)).decompose(sig));
    EQ("+1", "0000-80000000-00000000-0000", sig.str());

    EQ("-1", 0, f80(0xBFFF, UINT64_C(0x8000000000000000)).decompose(sig));
    EQ("-1", "0000-80000000-00000000-0000", sig.str());

    EQ("LDBL_MAX", 16383, f80(0x7FFE, UINT64_C(0xFFFFFFFFFFFFFFFF)).decompose(sig));
    EQ("LDBL_MAX", "0000-FFFFFFFF-FFFFFFFF-0000", sig.str());

    EQ("LDBL_MIN", -16382, f80(1, UINT64_C(0x8000000000000000)).decompose(sig));
    EQ("LDBL_MIN", "0000-80000000-00000000-0000", sig.str());

    EQ("LDBL_SUBN_MAX", -16382 - 1, f80(0, UINT64_C(0x7FFFFFFFFFFFFFFF)).decompose(sig));
    EQ("LDBL_SUBN_MAX", "0000-FFFFFFFF-FFFFFFFE-0000", sig.str());

    EQ("LDBL_TRUE_MIN", -16382 - 63, f80(0, UINT64_C(1)).decompose(sig));
    EQ("LDBL_TRUE_MIN", "0000-80000000-00000000-0000", sig.str());
}

void test_compose() {
    fixed64_t sig;

    EQ("+0", "0000.00000000-00000000", f80::compose(false, 0, sig).str());
    EQ("-0", "8000.00000000-00000000", f80::compose(true, 0, sig).str());

    EQ("+1", "0000-80000000-00000000-0000", sig.set(UINT64_C(0x8000000000000000)).str());
    EQ("+1", "3FFF.80000000-00000000", f80::compose(false, 0, sig).str());
    EQ("-1", "BFFF.80000000-00000000", f80::compose(true, 0, sig).str());

    EQ("LDBL_MAX", "0000-FFFFFFFF-FFFFFFFF-0000", sig.set(UINT64_C(0xFFFFFFFFFFFFFFFF)).str());
    EQ("LDBL_MAX", "7FFE.FFFFFFFF-FFFFFFFF", f80::compose(false, 16383, sig).str());

    EQ("LDBL_MIN", "0000-80000000-00000000-0000", sig.set(UINT64_C(0x8000000000000000)).str());
    EQ("LDBL_MIN", "0001.80000000-00000000", f80::compose(false, -16382, sig).str());

    EQ("LDBL_SUBN_MAX", "0000-FFFFFFFF-FFFFFFFE-0000", sig.set(UINT64_C(0xFFFFFFFFFFFFFFFE)).str());
    EQ("LDBL_SUBN_MAX", "0000.7FFFFFFF-FFFFFFFF", f80::compose(false, -16382 - 1, sig).str());

    EQ("LDBL_TRUE_MIN", "0000-80000000-00000000-0000", sig.set(UINT64_C(0x8000000000000000)).str());
    EQ("LDBL_TRUE_MIN", "0000.00000000-00000001", f80::compose(false, -16382 - 63, sig).str());

    // normal significand
    EQ("round=", "0000-80000000-00000000-8000",
            sig.set(UINT64_C(0x8000000000000000), 0x8000).str());
    EQ("round=", "3FFF.80000000-00000000", f80::compose(false, 0, sig).str());
    EQ("round+", "0000-80000000-00000000-8001",
            sig.set(UINT64_C(0x8000000000000000), 0x8001).str());
    EQ("round+", "3FFF.80000000-00000001", f80::compose(false, 0, sig).str());
    EQ("round-", "0000-80000000-00000001-7FFF",
            sig.set(UINT64_C(0x8000000000000001), 0x7FFF).str());
    EQ("round-", "3FFF.80000000-00000001", f80::compose(false, 0, sig).str());
    EQ("round+", "0000-80000000-00000001-8000",
            sig.set(UINT64_C(0x8000000000000001), 0x8000).str());
    EQ("round+", "3FFF.80000000-00000002", f80::compose(false, 0, sig).str());
    EQ("round-", "0000-FFFFFFFF-FFFFFFFF-7FFF",
            sig.set(UINT64_C(0xFFFFFFFFFFFFFFFF), 0x7FFF).str());
    EQ("round-", "3FFF.FFFFFFFF-FFFFFFFF", f80::compose(false, 0, sig).str());
    EQ("round+", "0000-FFFFFFFF-FFFFFFFF-8000",
            sig.set(UINT64_C(0xFFFFFFFFFFFFFFFF), 0x8000).str());
    EQ("round+", "4000.80000000-00000000", f80::compose(false, 0, sig).str());

    // subnormal significand
    EQ("round=", "0000-87F00000-00000000-0000", sig.set(UINT64_C(0x87F0000000000000)).str());
    EQ("round=", "0000.00000000-00000008", f80::compose(false, -16382 - 60, sig).str());
    EQ("round=", "0000-88000000-00000000-0000", sig.set(UINT64_C(0x8800000000000000)).str());
    EQ("round=", "0000.00000000-00000008", f80::compose(false, -16382 - 60, sig).str());
    EQ("round+", "0000-88100000-00000000-0000", sig.set(UINT64_C(0x8810000000000000)).str());
    EQ("round+", "0000.00000000-00000009", f80::compose(false, -16382 - 60, sig).str());
    EQ("round-", "0000-97F00000-00000000-0000", sig.set(UINT64_C(0x97F0000000000000)).str());
    EQ("round-", "0000.00000000-00000009", f80::compose(false, -16382 - 60, sig).str());
    EQ("round+", "0000-98000000-00000000-0000", sig.set(UINT64_C(0x9800000000000000)).str());
    EQ("round+", "0000.00000000-0000000A", f80::compose(false, -16382 - 60, sig).str());
    EQ("round-", "0000-FFFFFFFF-FFFFFFFE-7FFF",
            sig.set(UINT64_C(0xFFFFFFFFFFFFFFFE), 0x7FFF).str());
    EQ("round-", "0000.7FFFFFFF-FFFFFFFF", f80::compose(false, -16382 - 1, sig).str());
    EQ("round+", "0000-FFFFFFFF-FFFFFFFF-8000",
            sig.set(UINT64_C(0xFFFFFFFFFFFFFFFF), 0x8000).str());
    EQ("round+", "0001.80000000-00000000", f80::compose(false, -16382 - 1, sig).str());
}

void test_ctor() {
    const f80 p0{0, UINT64_C(0)};
    EQ("+0", 0, p0.tag());
    EQ("+0", 0, p0.significand());
    TRUE("+0", p0.isZero());
    FALSE("+0", p0.isNegative());
    TRUE("+0", p0.isFractional());
    FALSE("+0", p0.isInf());
    FALSE("+0", p0.isNan());

    const f80 m0{0x8000, UINT64_C(0)};
    EQ("-0", 0x8000, m0.tag());
    EQ("-0", 0, m0.significand());
    TRUE("-0", m0.isZero());
    TRUE("-0", m0.isNegative());
    FALSE("-0", m0.isFractional());
    FALSE("-0", m0.isInf());
    FALSE("-0", m0.isNan());

    const f80 p1{0 + 0x3FFF, UINT64_C(0x8000000000000000)};
    EQ("+1", 0x3FFF, p1.tag());
    EQ("+1", UINT64_C(0x8000000000000000), p1.significand());
    FALSE("+1", p1.isZero());
    FALSE("+1", p1.isNegative());
    FALSE("+1", p1.isFractional());
    FALSE("+1", p1.isInf());
    FALSE("+1", p1.isNan());

    const f80 m1{0x8000 | (0 + 0x3FFF), UINT64_C(0x8000000000000000)};
    EQ("-1", 0x8000 | 0x3FFF, m1.tag());
    EQ("-1", UINT64_C(0x8000000000000000), m1.significand());
    FALSE("-1", m1.isZero());
    TRUE("-1", m1.isNegative());
    FALSE("-1", m1.isFractional());
    FALSE("-1", m1.isInf());
    FALSE("-1", m1.isNan());

    const f80 p099{-1 + 0x3FFF, UINT64_C(0xFFFFFFFFFFFFFFFF)};
    FALSE("0.99..", p099.isZero());
    FALSE("0.99..", p099.isNegative());
    TRUE("0.99..", p099.isFractional());
    FALSE("0.99..", p099.isInf());
    FALSE("0.99..", p099.isNan());

    const f80 I64_MIN{0x8000 | (63 + 0x3FFF), UINT64_C(0x8000000000000000)};
    EQ("INT64_MIN", 0x8000 | (63 + 0x3FFF), I64_MIN.tag());
    EQ("INT64_MIN", UINT64_C(0x8000000000000000), I64_MIN.significand());

    const f80 I64_MAX{62 + 0x3FFF, UINT64_C(0xFFFFFFFFFFFFFFFE)};
    EQ("INT64_MAX", 62 + 0x3FFF, I64_MAX.tag());
    EQ("INT64_MAX", UINT64_C(0xFFFFFFFFFFFFFFFE), I64_MAX.significand());

    const f80 U64_MAX{63 + 0x3FFF, UINT64_C(0xFFFFFFFFFFFFFFFF)};
    EQ("UINT64_MAX", 63 + 0x3FFF, U64_MAX.tag());
    EQ("UINT64_MAX", UINT64_C(0xFFFFFFFFFFFFFFFF), U64_MAX.significand());
}

void test_set() {
    f80 v;
    EQ("+0", "0000.00000000-00000000", v.set(INT64_C(0)).str());
    EQ("+1", "3FFF.80000000-00000000", v.set(INT64_C(1)).str());
    EQ("-1", "BFFF.80000000-00000000", v.set(INT64_C(-1)).str());
    EQ("INT32_MIN", "C01E.80000000-00000000", v.set(static_cast<int64_t>(INT32_MIN)).str());
    EQ("INT32_MAX", "401D.FFFFFFFE-00000000", v.set(static_cast<int64_t>(INT32_MAX)).str());
    EQ("UINT32_MAX", "401E.FFFFFFFF-00000000", v.set(static_cast<int64_t>(UINT32_MAX)).str());
    EQ("INT64_MIN", "C03E.80000000-00000000", v.set(INT64_MIN).str());
    EQ("INT64_MAX", "403D.FFFFFFFF-FFFFFFFE", v.set(INT64_MAX).str());
    EQ("UINT64_MAX", "403E.FFFFFFFF-FFFFFFFF", v.set(UINT64_MAX).str());
}

void test_equals() {
    const auto p0 = f80::zero(false);  // +0
    const auto m0 = f80::zero(true);   // -0
    TRUE("(+0)==(+0)", p0 == p0);
    TRUE("(+0)==(-0)", p0 == m0);
    TRUE("(-0)==(+0)", m0 == p0);
    TRUE("(-0)==(-0)", m0 == m0);

    const auto pinf = f80::infinity(false);  // +INF
    const auto minf = f80::infinity(true);   // -INF
    TRUE("(+INF)==(+INF)", pinf == pinf);
    FALSE("(+INF)==(-INF)", pinf == minf);
    FALSE("(-INF)==(+INF)", minf == pinf);
    TRUE("(-INF)==(-INF)", minf == minf);

    const f80 p1{0 + 0x3FFF, UINT64_C(0x8000000000000000)};
    const f80 m1{0x8000 | (0 + 0x3FFF), UINT64_C(0x800000000000)};
    const auto pnan = f80::not_a_number(false);  // +NAN
    const auto mnan = f80::not_a_number(true);   // -NAN
    FALSE("(+0)==(+NAN)", p0 == pnan);
    FALSE("(+0)==(-NAN)", p0 == mnan);
    FALSE("(-0)==(+NAN)", m0 == pnan);
    FALSE("(-0)==(-NAN)", m0 == mnan);
    FALSE("(+1)==(+NAN)", p1 == pnan);
    FALSE("(+1)==(-NAN)", p1 == mnan);
    FALSE("(-1)==(+NAN)", m1 == pnan);
    FALSE("(-1)==(-NAN)", m1 == mnan);
    FALSE("(+INF)==(+NAN)", pinf == pnan);
    FALSE("(+INF)==(-NAN)", pinf == mnan);
    FALSE("(-INF)==(+NAN)", minf == pnan);
    FALSE("(-INF)==(-NAN)", minf == mnan);

    FALSE("(+NAN)==(+0)", pnan == p0);
    FALSE("(-NAN)==(+0)", mnan == p0);
    FALSE("(+NAN)==(-0)", pnan == m0);
    FALSE("(-NAN)==(-0)", mnan == m0);
    FALSE("(+NAN)==(+1)", pnan == p1);
    FALSE("(-NAN)==(+1)", mnan == p1);
    FALSE("(+NAN)==(-1)", pnan == m1);
    FALSE("(-NAN)==(-1)", mnan == m1);
    FALSE("(+NAN)==(+INF)", pnan == pinf);
    FALSE("(-NAN)==(+INF)", mnan == pinf);
    FALSE("(+NAN)==(-INF)", pnan == minf);
    FALSE("(-NAN)==(-INF)", mnan == minf);
}

void test_lessthan() {
    const auto p0 = f80::zero(false);  // +0
    const auto m0 = f80::zero(true);   // -0
    FALSE("(+0)<(+0)", p0 < p0);
    FALSE("(+0)<(-0)", p0 < m0);
    FALSE("(-0)<(+0)", m0 < p0);
    FALSE("(-0)<(-0)", m0 < m0);

    const auto pinf = f80::infinity(false);  // +INF
    const auto minf = f80::infinity(true);   // -INF
    const f80 p1{0 + 0x3FFF, UINT64_C(0x8000000000000000)};
    const f80 m1{0x8000 | (0 + 0x3FFF), UINT64_C(0x8000000000000000)};
    TRUE("(+1)<(+INF)", p1 < pinf);
    TRUE("(+0)<(+INF)", p0 < pinf);
    TRUE("(-0)<(+INF)", m0 < pinf);
    TRUE("(-INF)<(+0)", minf < p0);
    TRUE("(-INF)<(-0)", minf < m0);
    TRUE("(-INF)<(-1)", minf < m1);

    FALSE("(+INF)<(+INF)", pinf < pinf);
    FALSE("(+INF)<(-INF)", pinf < minf);
    TRUE("(-INF)<(+INF)", minf < pinf);
    FALSE("(-INF)<(-INF)", minf < minf);

    const auto pnan = f80::not_a_number(false);  // +NAN
    const auto mnan = f80::not_a_number(true);   // -NAN
    FALSE("(+0)<(+NAN)", p0 < pnan);
    FALSE("(+0)<(-NAN)", p0 < mnan);
    FALSE("(-0)<(+NAN)", m0 < pnan);
    FALSE("(-0)<(-NAN)", m0 < mnan);
    FALSE("(+1)<(+NAN)", p1 < pnan);
    FALSE("(+1)<(-NAN)", p1 < mnan);
    FALSE("(-1)<(+NAN)", m1 < pnan);
    FALSE("(-1)<(-NAN)", m1 < mnan);
    FALSE("(+INF)<(+NAN)", pinf < pnan);
    FALSE("(+INF)<(-NAN)", pinf < mnan);
    FALSE("(-INF)<(+NAN)", minf < pnan);
    FALSE("(-INF)<(-NAN)", minf < mnan);

    FALSE("(+NAN)<(+0)", pnan < p0);
    FALSE("(-NAN)<(+0)", mnan < p0);
    FALSE("(+NAN)<(-0)", pnan < m0);
    FALSE("(-NAN)<(-0)", mnan < m0);
    FALSE("(+NAN)<(+1)", pnan < p1);
    FALSE("(-NAN)<(+1)", mnan < p1);
    FALSE("(+NAN)<(-1)", pnan < m1);
    FALSE("(-NAN)<(-1)", mnan < m1);
    FALSE("(+NAN)<(+INF)", pnan < pinf);
    FALSE("(-NAN)<(+INF)", mnan < pinf);
    FALSE("(+NAN)<(-INF)", pnan < minf);
    FALSE("(-NAN)<(-INF)", mnan < minf);
}

void test_add() {
    const auto p0 = f80::zero(false);  // +0
    const auto m0 = f80::zero(true);   // -0
    SEQ("(+0)+(+0)", (p0 + p0), p0);
    FALSE("(+0)+(+0)", (p0 + p0).isNegative());  // (+0) + (+0) = (+0)
    SEQ("(+0)+(-0)", (p0 + m0), p0);
    FALSE("(+0)+(-0)", (p0 + m0).isNegative());  // round to +0
    SEQ("(-0)+(+0)", (m0 + p0), p0);
    FALSE("(-0)+(+0)", (m0 + p0).isNegative());  // round to +0
    SEQ("(-0)+(-0)", (m0 + m0), m0);
    TRUE("(-0)+(-0)", (m0 + m0).isNegative());  // (-0) + (-0) = (-0)

    const f80 p10{3 + 0x3FFF, UINT64_C(0xA000000000000000)};             // +10
    const f80 m10{0x8000 | (3 + 0x3FFF), UINT64_C(0xA000000000000000)};  // -10
    const f80 p20{4 + 0x3FFF, UINT64_C(0xA000000000000000)};             // +20
    SEQ("(+10)+(+0)", (p10 + p0), p10);
    SEQ("(+10)+(-0)", (p10 + m0), p10);
    SEQ("(+0)+(+10)", (p0 + p10), p10);
    SEQ("(-0)+(+10)", (m0 + p10), p10);
    SEQ("(+10)+(+10)", (p10 + p10), p20);
    SEQ("(+10)+(-10)", (p10 + m10), p0);
    FALSE("(+10)+(-10)", (p10 + m10).isNegative());  // round to +0
    SEQ("(-10)+(+10)", (m10 + p10), p0);
    FALSE("(-10)+(+10)", (m10 + p10).isNegative());  // round to +0
    SEQ("(+10)+(-10)", (p10 + m10), p0);
    FALSE("(+10)+(-10)", (p10 + m10).isNegative());  // round to +0

    const auto pinf = f80::infinity(false);  // +INF
    const auto minf = f80::infinity(true);   // -INF
    SEQ("(+0)+(+inf)", (p0 + pinf), pinf);
    SEQ("(+0)+(-inf)", (p0 + minf), minf);
    SEQ("(+inf)+(+0)", (pinf + p0), pinf);
    SEQ("(-inf)+(+0)", (minf + p0), minf);
    SEQ("(+inf)+(+inf)", (pinf + pinf), pinf);
    TRUE("(+inf)+(-inf)", (pinf + minf).isNan());
    TRUE("(-inf)+(+inf)", (minf + pinf).isNan());
    SEQ("(-inf)+(-inf)", (minf + minf), minf);
}

void test_sub() {
    const auto p0 = f80::zero(false);  // +0
    const auto m0 = f80::zero(true);   // -0
    SEQ("(-0)-(+0)", (p0 - p0), p0);
    FALSE("(+0)-(+0)", (p0 - p0).isNegative());
    SEQ("(+0)-(-0)", (p0 - m0), p0);
    FALSE("(+0)-(-0)", (p0 - m0).isNegative());  // round to +0
    SEQ("(-0)-(+0)", (m0 - p0), m0);
    TRUE("(-0)-(+0)", (m0 - p0).isNegative());  // (-0) - (+0) = (-0)
    SEQ("(-0)-(-0)", (m0 - m0), p0);
    FALSE("(-0)-(-0)", (m0 - m0).isNegative());  // round to +0

    const f80 p10{3 + 0x3FFF, UINT64_C(0xA000000000000000)};             // +10
    const f80 m10{0x8000 | (3 + 0x3FFF), UINT64_C(0xA000000000000000)};  // -10
    const f80 p20{4 + 0x3FFF, UINT64_C(0xA000000000000000)};             // +20
    const f80 m20{0x8000 | (4 + 0x3FFF), UINT64_C(0xA000000000000000)};  // -20
    SEQ("(+10)-(+0)", (p10 - p0), p10);
    SEQ("(+10)-(-0)", (p10 - m0), p10);
    SEQ("(+0)-(+10)", (p0 - p10), m10);
    SEQ("(-0)-(+10)", (m0 - p10), m10);
    SEQ("(+10)-(+10)", (p10 - p10), p0);
    FALSE("(+10)-(+10)", (p10 - p10).isNegative());  // round to +0
    SEQ("(+10)-(-10)", (p10 - m10), p20);
    SEQ("(-10)-(+10)", (m10 - p10), m20);
    SEQ("(-10)-(-10)", (m10 - m10), p0);
    FALSE("(-10)-(-10)", (m10 - m10).isNegative());  // round to +0

    const f80 F80_MIN{1, UINT64_C(0x8000000000000000)};       // DBL_MIN
    const f80 F80_SUBN_MAX{0, UINT64_C(0x7FFFFFFFFFFFFFFF)};  // DBL_SUBN_MAX
    const f80 F80_TRUE_MIN{0, UINT64_C(1)};                   // DBL_TRUE_MIN
    EQ("DBL_MIN-DBL_TRUE_MIN", F80_SUBN_MAX.tag(), (F80_MIN - F80_TRUE_MIN).tag());
    EQ("DBL_MIN-DBL_TRUE_MIN", F80_SUBN_MAX.significand(), (F80_MIN - F80_TRUE_MIN).significand());

    const auto pinf = f80::infinity(false);  // +INF
    const auto minf = f80::infinity(true);   // -INF
    SEQ("(+0)-(+INF)", (p0 - pinf), minf);
    SEQ("(+0)-(-INF)", (p0 - minf), pinf);
    TRUE("(+INF)-(+INF)", (pinf - pinf).isNan());
    SEQ("(+INF)-(-INF)", (pinf - minf), pinf);
    SEQ("(-INF)-(+INF)", (minf - pinf), minf);
    TRUE("(-INF)-(-INF)", (minf - minf).isNan());
}

void test_mul() {
    const f80 p10{3 + 0x3FFF, UINT64_C(0xA000000000000000)};             // +10
    const f80 m10{0x8000 | (3 + 0x3FFF), UINT64_C(0xA000000000000000)};  // -10
    const f80 p100{6 + 0x3FFF, UINT64_C(0xC800000000000000)};            // +100
    TRUE("(+10)*(+10)", (p10 * p10) == p100);
    TRUE("(+10)*(-10)", (p10 * m10) == -p100);
    TRUE("(-10)*(+10)", (m10 * p10) == -p100);
    SEQ("(-10)*(-10)", (m10 * m10), p100);

    const f80 F80_MAX{16383 + 0x3FFF, UINT64_C(0xFFFFFFFFFFFFFFFF)};  // DBL_MAX
    const f80 F80_MIN{1, UINT64_C(0x8000000000000000)};               // DBL_MIN
    TRUE("LDBL_MAX*LDBL_MAX", (F80_MAX * F80_MAX).isInf());
    TRUE("LDBL_MIN*LDBL_MIN", (F80_MIN * F80_MIN).isZero());

    const auto p0 = f80::zero(false);        // +0
    const auto m0 = f80::zero(true);         // -0
    const auto pinf = f80::infinity(false);  // +INF
    const auto minf = f80::infinity(true);   // -INF
    SEQ("(+0)*(+0)", (p0 * p0), p0);
    FALSE("(+0)*(+0)", (p0 * p0).isNegative());
    SEQ("(+0)*(-0)", (p0 * m0), m0);
    TRUE("(+0)*(-0)", (p0 * m0).isNegative());
    SEQ("(-0)*(+0)", (m0 * p0), m0);
    TRUE("(-0)*(+0)", (m0 * p0).isNegative());
    SEQ("(-0)*(-0)", (m0 * m0), p0);
    FALSE("(-0)*(-0)", (m0 * m0).isNegative());
    TRUE("(+0)*(+INF)", (p0 * pinf).isNan());
    TRUE("(+0)*(-INF)", (p0 * minf).isNan());
    TRUE("(+INF)*(-0)", (pinf * m0).isNan());
    TRUE("(-INF)*(-0)", (minf * m0).isNan());
    SEQ("(+INF)*(+INF)", (pinf * pinf), pinf);
    SEQ("(+INF)*(-INF)", (pinf * minf), minf);
    SEQ("(-INF)*(+INF)", (minf * pinf), minf);
    SEQ("(-INF)*(-INF)", (minf * minf), pinf);
}

void test_div() {
    const f80 p10{3 + 0x3FFF, UINT64_C(0xA000000000000000)};             // +10
    const f80 m10{0x8000 | (3 + 0x3FFF), UINT64_C(0xA000000000000000)};  // -10
    const f80 p100{6 + 0x3FFF, UINT64_C(0xC800000000000000)};            // +100
    SEQ("(+100)/(+10)", (p100 / p10), p10);
    SEQ("(+100)/(-10)", (p100 / m10), m10);
    SEQ("(-100)/(+10)", (-p100 / p10), m10);
    SEQ("(-100)/(-10)", (-p100 / m10), p10);

    const auto p0 = f80::zero(false);        // +0
    const auto m0 = f80::zero(true);         // -0
    const auto pinf = f80::infinity(false);  // +INF
    const auto minf = f80::infinity(true);   // -INF
    TRUE("(+0)/(+0)", (p0 / p0).isNan());
    TRUE("(+0)/(-0)", (p0 / m0).isNan());
    SEQ("(+0)/(+INF)", (p0 / pinf), p0);
    FALSE("(+0)/(+INF)", (p0 / pinf).isNegative());
    SEQ("(+0)/(-INF)", (p0 / minf), m0);
    TRUE("(+0)/(-INF)", (p0 / minf).isNegative());
    SEQ("(-0)/(+INF)", (m0 / pinf), m0);
    TRUE("(-0)/(+INF)", (m0 / pinf).isNegative());
    SEQ("(-0)/(-INF)", (m0 / minf), p0);
    FALSE("(-0)/(-INF)", (m0 / minf).isNegative());
    SEQ("(+INF)/(+0)", (pinf / p0), pinf);
    SEQ("(-INF)/(+0)", (minf / p0), minf);
    SEQ("(+INF)/(-0)", (pinf / m0), minf);
    SEQ("(-INF)/(-0)", (minf / m0), pinf);
    TRUE("(+INF)/(+INF)", (pinf / pinf).isNan());
    TRUE("(+INF)/(-INF)", (pinf / minf).isNan());
    TRUE("(-INF)/(+INF)", (minf / pinf).isNan());
    TRUE("(-INF)/(-INF)", (minf / minf).isNan());
}

#define read_f80(str)                                             \
    ({                                                            \
        char *end;                                                \
        const auto error = v.read(str, &end);                     \
        asserter.equals(__FILE__, __LINE__, "errno", 0, error);   \
        asserter.not_equals(__FILE__, __LINE__, "end", str, end); \
        v;                                                        \
    })

void test_pow10() {
    f80 v;
    const f80 p10{3 + 0x3FFF, UINT64_C(0xA000000000000000)};              // +10
    const f80 m10{0x8000 | (3 + 0x3FFF), UINT64_C(0xA000000000000000)};   // -10
    const f80 p100{6 + 0x3FFF, UINT64_C(0xC800000000000000)};             // +100
    const f80 m100{0x8000 | (6 + 0x3FFF), UINT64_C(0xC800000000000000)};  // -100
    SEQ("(+10).pow10(0)", p10.pow10(0), p10);
    SEQ("(-10).pow10(0)", m10.pow10(0), m10);
    SEQ("(+10).pow10(1)", p10.pow10(1), p100);
    SEQ("(-10).pow10(1)", m10.pow10(1), m100);
    SEQ("(+10).pow10(1024)", p10.pow10(1024), read_f80("1e+1025"));
    SEQ("(+10).pow10(-1023)", p10.pow10(-1023), read_f80("1e-1022"));

    const f80 p2{1 + 0x3FFF, UINT64_C(0x8000000000000000)};                   // +2
    const f80 m2{0x8000 | (1 + 0x3FFF), UINT64_C(0x8000000000000000)};        // -2
    const f80 p2ep10{34 + 0x3FFF, UINT64_C(0x9502F90000000000)};              // +2e+10
    const f80 m2em10{0x8000 | (-33 + 0x3FFF), UINT64_C(0xDBE6FECEBDEDD5BF)};  // +2e+10
    SEQ("(+2).pow10(10)", p2.pow10(10), p2ep10);
    SEQ("(-2E-10).pow10(10)", m2em10.pow10(10), m2);

    const f80 p3{1 + 0x3FFF, UINT64_C(0xC000000000000000)};                   // +3
    const f80 m3{0x8000 | (1 + 0x3FFF), UINT64_C(0xC000000000000000)};        // -3
    const f80 p3ep10{34 + 0x3FFF, UINT64_C(0xDF84758000000000)};              // +3e+10
    const f80 m3em10{0x8000 | (-32 + 0x3FFF), UINT64_C(0xA4ED3F1B0E72604F)};  // +3e-10
    SEQ("(+3E+10).pow10(-10)", p3ep10.pow10(-10), p3);
    SEQ("(-3).pow10(10)", m3em10.pow10(10), m3);

    SEQ("100/10==100.pow10(-1)", (p100 / p10), p100.pow10(-1));
    SEQ("(-10)*10==(-10).pow10(1)", (m10 * p10), m10.pow10(1));
}

void test_read() {
    f80 v;
    const auto pnan = read_f80("nan");
    EQ("+nan", "7FFF.C0000000-00000000", pnan.str());
    TRUE("nan", pnan.isNan());
    const auto mnan = read_f80("-NAN");
    EQ("-nan", "FFFF.C0000000-00000000", mnan.str());
    TRUE("-nan", mnan.isNan());
    const auto pinf = read_f80("inf");
    EQ("+inf", "7FFF.80000000-00000000", pinf.str());
    TRUE("+inf", pinf.isInf());
    const auto minf = read_f80("-INF");
    EQ("-inf", "FFFF.80000000-00000000", minf.str());
    TRUE("-inf", minf.isInf());

    const auto p0 = read_f80("0");
    EQ("+0", "0000.00000000-00000000", p0.str());
    TRUE("+0", p0.isZero());
    FALSE("+0", p0.isSubnormal());
    const auto p1 = read_f80("1");
    EQ("+1", "3FFF.80000000-00000000", p1.str());
    const auto m1 = read_f80("-1");
    EQ("-1", "BFFF.80000000-00000000", m1.str());

    const auto I64_MIN = read_f80(tINT64_MIN);
    EQ("INT64_MIN", "C03E.80000000-00000000", I64_MIN.str());
    const auto I64_MAX = read_f80(tINT64_MAX);
    EQ("INT64_MAX", "403D.FFFFFFFF-FFFFFFFE", I64_MAX.str());
    const auto U64_MAX = read_f80(tUINT64_MAX);
    EQ("UINT64_MAX", "403E.FFFFFFFF-FFFFFFFF", U64_MAX.str());
    const auto U65_MIN = read_f80(tUINT65_MIN);
    EQ("UINT54_MIN", "403F.80000000-00000000", U65_MIN.str());

    const auto ten_third = read_f80("3.333333333333333333333");
    EQ("10/3", "4000.D5555555-55555555", ten_third.str());
    const auto p22 = read_f80("1.00000e+22");
    EQ("1e+22", "4048.87867832-6EAC9000", p22.str());

    const auto F80_MAX = read_f80(tLDBL_MAX);
    EQ("LDBL_MAX", "7FFE.FFFFFFFF-FFFFFFFF", F80_MAX.str());
    FALSE("LDBL_MAX", F80_MAX.isSubnormal());
    const auto F80_MIN = read_f80(tLDBL_MIN);
    EQ("LDBL_MIN", "0001.80000000-00000000", F80_MIN.str());
    FALSE("LDBL_MIN", F80_MIN.isSubnormal());
    f80 F80_SUBN_MAX;
    EQ("LDBL_SUBN_MAX", ERANGE, F80_SUBN_MAX.read(tLDBL_SUBN_MAX));
    EQ("LDBL_SUBN_MAX", "0000.7FFFFFFF-FFFFFFFF", F80_SUBN_MAX.str());
    TRUE("LDBL_SUBN_MX", F80_SUBN_MAX.isSubnormal());
    f80 F80_TRUE_MIN;
    EQ("LDBL_TRUE_MIN", ERANGE, F80_TRUE_MIN.read(tLDBL_TRUE_MIN));
    EQ("LDBL_TRUE_MIN", "0000.00000000-00000001", F80_TRUE_MIN.str());
    TRUE("LDBL_TRUE_MIN", F80_TRUE_MIN.isSubnormal());
    EQ("overflow", ERANGE, v.read("1.2e+4932"));
    SEQ("overflow", f80::infinity(false), v);
    EQ("underflow", ERANGE, v.read("3.7e-4951"));
    SEQ("underflow", F80_TRUE_MIN, v);
}

const char *ecvt(const f80 &f, int prec = 21) {
    static char buf[80];
    f.ecvt(buf, sizeof(buf), prec);
    return buf;
}

const char *fcvt(const f80 &f, int prec = 21) {
    static char buf[80];
    f.fcvt(buf, sizeof(buf), prec);
    return buf;
}

const char *gcvt(const f80 &f, int prec = 21) {
    static char buf[80];
    f.gcvt(buf, sizeof(buf), prec);
    return buf;
}

#define CVT(msg, v, prec, efmt, ffmt, gfmt)   \
    do {                                      \
        EQ("ecvt " msg, efmt, ecvt(v, prec)); \
        EQ("fcvt " msg, ffmt, fcvt(v, prec)); \
        EQ("gcvt " msg, gfmt, gcvt(v, prec)); \
    } while (0)

void test_write() {
    f80 v;
    const auto pinf = f80::infinity(false);      // +INF
    const auto minf = f80::infinity(true);       // -INF
    const auto pnan = f80::not_a_number(false);  // +NAN
    const auto mnan = f80::not_a_number(true);   // -NAN
    EQ("+nan", "nan", gcvt(pnan));
    EQ("-nan", "-nan", gcvt(mnan));
    EQ("+inf", "inf", gcvt(pinf));
    EQ("-inf", "-inf", gcvt(minf));

    const auto p0 = f80::zero(false);                                   // +0
    const auto m0 = f80::zero(true);                                    // -0
    const f80 p1{0 + 0x3FFF, UINT64_C(0x8000000000000000)};             // +1
    const f80 m1{0x8000 | (0 + 0x3FFF), UINT64_C(0x8000000000000000)};  // -1
    CVT("+0", p0, 6, "0.000000e+00", "0.000000", "0");
    CVT("+0", p0, 0, "0e+00", "0", "0");
    CVT("-0", m0, 6, "-0.000000e+00", "-0.000000", "-0");
    CVT("+1", p1, 6, "1.000000e+00", "1.000000", "1");
    CVT("-1", m1, 6, "-1.000000e+00", "-1.000000", "-1");
    CVT("+1E+01", p1.pow10(1), 6, "1.000000e+01", "10.000000", "10");
    CVT("+1E+05", p1.pow10(5), 6, "1.000000e+05", "100000.000000", "100000");
    CVT("+1E+06", p1.pow10(6), 6, "1.000000e+06", "1000000.000000", "1e+06");
    CVT("+1E-04", p1.pow10(-4), 6, "1.000000e-04", "0.000100", "0.0001");
    CVT("+1E-05", p1.pow10(-5), 6, "1.000000e-05", "0.000010", "1e-05");

    const f80 p10o3 = read_f80("3.333333333333333333333");
    CVT("10/3", p10o3, 16, "3.3333333333333333e+00", "3.3333333333333333", "3.333333333333333");
    CVT("1e+22", p1.pow10(22), 6, "1.000000e+22", "10000000000000000000000.000000", "1e+22");

    const f80 p9_99_e09{33 + 0x3FFF, UINT64_C(0x9502F8FD80000000)};
    CVT("9.99..e+09", p9_99_e09, 9, "9.999999990e+09", "9999999990.000000000", "9.99999999e+09");
    CVT("9.99..e+09", p9_99_e09, 8, "9.99999999e+09", "9999999990.00000000", "1e+10");
    CVT("9.99..e+09", p9_99_e09, 7, "1.0000000e+10", "9999999990.0000000", "1e+10");
    CVT("9.99..e+09", p9_99_e09, 1, "1.0e+10", "9999999990.0", "1e+10");
    CVT("9.99..e+09", p9_99_e09, 0, "1e+10", "9999999990", "1e+10");

    const f80 p0_985{-1 + 0x3FFF, UINT64_C(0xFC28F5C28F5C28F6)};
    CVT("0.985", p0_985, 4, "9.8500e-01", "0.9850", "0.985");
    CVT("0.985", p0_985, 3, "9.850e-01", "0.985", "0.985");
    CVT("0.985", p0_985, 2, "9.85e-01", "0.99", "0.99");
    CVT("0.985", p0_985, 1, "9.9e-01", "1.0", "1");
    CVT("0.985", p0_985, 0, "1e+00", "1", "1");

    const f80 p0_000985{-10 + 0x3FFF, UINT64_C(0x811B1D92B7FE08AF)};
    CVT("0.000985", p0_000985, 6, "9.850000e-04", "0.000985", "0.000985");
    CVT("0.000985", p0_000985, 5, "9.85000e-04", "0.00099", "0.000985");
    CVT("0.000985", p0_000985, 4, "9.8500e-04", "0.0010", "0.000985");
    CVT("0.000985", p0_000985, 3, "9.850e-04", "0.001", "0.000985");
    CVT("0.000985", p0_000985, 2, "9.85e-04", "0.00", "0.00099");
    CVT("0.000985", p0_000985, 1, "9.9e-04", "0.0", "0.001");
    CVT("0.000985", p0_000985, 0, "1e-03", "0", "0.001");

    f80 I64_MIN{0x8000 | (63 + 0x3FFF), UINT64_C(0x8000000000000000)};
    f80 I64_MAX{62 + 0x3FFF, UINT64_C(0xFFFFFFFFFFFFFFFE)};
    f80 U64_MAX{63 + 0x3FFF, UINT64_C(0xFFFFFFFFFFFFFFFF)};
    f80 U65_MIN{64 + 0x3FFF, UINT64_C(0x8000000000000000)};
    CVT("INT64_MIN", I64_MIN, 0, "-9e+18", tINT64_MIN, "-9e+18");
    CVT("INT64_MAX", I64_MAX, 0, "9e+18", tINT64_MAX, "9e+18");
    CVT("UINT64_MAX", U64_MAX, 0, "2e+19", tUINT64_MAX, "2e+19");
    CVT("UINT65_MIN", U65_MIN, 0, "2e+19", tUINT65_MIN, "2e+19");

    const f80 F80_MAX{16383 + 0x3FFF, UINT64_C(0xFFFFFFFFFFFFFFFF)};  // LDBL_MAX
    const f80 F80_MIN{1, UINT64_C(0x8000000000000000)};               // LDBL_MIN
    const f80 F80_SUBN_MAX{0, UINT64_C(0x7FFFFFFFFFFFFFFF)};          // LDBL_SUBN_MAX
    const f80 F80_TRUE_MIN{0, UINT64_C(1)};                           // LDBL_TRUE_MIN
    EQ("LDBL_MAX", tLDBL_MAX, gcvt(F80_MAX, 21));
    EQ("LDBL_MIN", tLDBL_MIN, gcvt(F80_MIN, 21));
    EQ("LDBL_SUBN_MAX", tLDBL_SUBN_MAX, gcvt(F80_SUBN_MAX, 21));
    EQ("LDBL_TRUE_MIN", tLDBL_TRUE_MIN, gcvt(F80_TRUE_MIN, 21));

    const f80 ILLEGAL{0x3FFF, UINT64_C(0x4000000000000000)};
    FALSE("ILLEGAL", ILLEGAL.isValid());
    EQ("ILLEGAL", "nan", gcvt(ILLEGAL));
    FALSE("-ILLEGAL", (-ILLEGAL).isValid());
    EQ("-ILLEGAL", "-nan", gcvt(-ILLEGAL));
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
