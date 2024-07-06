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
#include "float80.h"
#include "test_unit_helper.h"

using f80 = libasm::__float80<false>;

namespace libasm {
namespace test {

void set_up() {}

void tear_down() {}

void test_constructor() {
    f80 zero;
    EQ("zero exp", 0, zero.tag());
    EQ("zero sig", 0, zero.significand());

    f80 m1{INT64_C(-1)};
    EQ("m1 exp", 0x8000 | 0x3FFF, m1.tag());
    EQ("m1 sig", UINT64_C(0x8000'0000'0000'0000), m1.significand());

    f80 p1{UINT64_C(1)};
    EQ("p1 exp", 0x3FFF, p1.tag());
    EQ("p1 sig", UINT64_C(0x8000'0000'0000'0000), p1.significand());
    FALSE("p1 frac", p1.isFractional());

    f80 p09{0x3FFE, UINT64_C(0xFFFF'FFFF'FFFF'FFFF)};
    TRUE("p09 frac", p09.isFractional());

    f80 mini64{INT64_MIN};
    EQ("p1 mini64", 0x8000 | (0x3FFF + 63), mini64.tag());
    EQ("p1 mini64", UINT64_C(0x8000'0000'0000'0000), mini64.significand());

    f80 maxi64{INT64_MAX};
    EQ("p1 maxi64", 0x3FFF + 62, maxi64.tag());
    EQ("p1 maxi64", UINT64_C(0xFFFF'FFFF'FFFF'FFFE), maxi64.significand());

    f80 maxu64{UINT64_MAX};
    EQ("p1 maxu64", 0x3FFF + 63, maxu64.tag());
    EQ("p1 maxu64", UINT64_C(0xFFFF'FFFF'FFFF'FFFF), maxu64.significand());
}

void test_cast_operator() {
    f80 zero;
    EQ("zero cast", 0, static_cast<int64_t>(zero));
    EQ("zero cast", 0, static_cast<uint64_t>(zero));

    f80 m1{INT64_C(-1)};
    EQ("m1 cast", -1, static_cast<int64_t>(m1));
    EQ("m1 cast", 0, static_cast<uint64_t>(m1));

    f80 p5{UINT16_C(0x4001), UINT64_C(0xA000'0000'0000'0000)};
    EQ("p5 cast", 5, static_cast<int64_t>(p5));

    f80 p9{UINT16_C(0x4002), UINT64_C(0x9000'0000'0000'0000)};
    EQ("p9 cast", 9, static_cast<int64_t>(p9));

    f80 p50{UINT16_C(0x4004), UINT64_C(0xC800'0000'0000'0000)};
    EQ("p50 cast", 50, static_cast<int64_t>(p50));

    f80 p11{UINT64_C(11)};
    EQ("p11 cast", 11, static_cast<int64_t>(p11));
    EQ("p11 cast", 11, static_cast<uint64_t>(p11));

    f80 mini64{INT64_MIN};
    EQ("mini64", INT64_MIN, static_cast<int64_t>(mini64));
    EQ("mini64", 0, static_cast<uint64_t>(mini64));
    EQ("-mini64", INT64_MAX, static_cast<int64_t>(-mini64));

    f80 maxi64{INT64_MAX};
    EQ("maxi64", INT64_MAX, static_cast<int64_t>(maxi64));
    EQ("maxi64", INT64_MAX, static_cast<uint64_t>(maxi64));
    EQ("-maxi64", INT64_MIN + 1, static_cast<int64_t>(-maxi64));

    f80 maxu64{UINT64_MAX};
    EQ("maxu64", INT64_MAX, static_cast<int64_t>(maxu64));
    EQ("maxu64", UINT64_MAX, static_cast<uint64_t>(maxu64));
    EQ("-maxu64", INT64_MIN, static_cast<int64_t>(-maxu64));

    f80 maxf64{UINT16_C(1023 + 0x3FFF), UINT64_C(0xFFFF'FFFF'FFFF'F800)};
    EQ("maxf64", FP_NORMAL, fpclassify(static_cast<double>(maxf64)));

    f80 minf64{UINT16_C(-1022 + 0x3FFF), UINT64_C(0x8000'0000'0000'0000)};
    EQ("minf64", FP_NORMAL, fpclassify(static_cast<double>(minf64)));

    f80 trueminf64{UINT16_C(-1022 + 0x3FFF), UINT64_C(0x0000'0000'0000'0800)};
    EQ("trueminf64", FP_SUBNORMAL, fpclassify(static_cast<double>(trueminf64)));

    f80 zerof64{UINT16_C(-1023 - 53 + 0x3FFF), UINT64_C(0xFFFF'FFFF'FFFF'F800)};
    EQ("zerof64", FP_ZERO, fpclassify(static_cast<double>(zerof64)));

    f80 p10o3{UINT16_C(0x4000), UINT64_C(0xD555'5555'5555'5555)};
    TRUE("p10o3 cast", 3.33333333 - static_cast<double>(p10o3) < 1e-10);
}

void test_add() {
    const f80 zero{UINT64_C(0)};
    const f80 p1{UINT64_C(1)};
    const f80 p10{UINT64_C(10)};
    const f80 m1{INT64_C(-1)};
    const f80 m10{INT64_C(-10)};

    // Addition
    TRUE("0+0", (zero + zero).isZero());
    TRUE("1+(-1)", (p1 + m1).isZero());
    TRUE("-1+1", (m1 + p1).isZero());
    EQ("1+0", 1, static_cast<int64_t>(zero + p1));
    EQ("0+1", 1, static_cast<int64_t>(p1 + zero));
    EQ("(-1)+0", -1, static_cast<int64_t>(m1 + zero));
    EQ("0+(-1)", -1, static_cast<int64_t>(zero + m1));
    EQ("1+10", 11, static_cast<int64_t>(p1 + p10));
    EQ("10+1", 11, static_cast<int64_t>(p10 + p1));
    EQ("(-1)+(-10)", -11, static_cast<int64_t>(m1 + m10));
    EQ("(-10)+(-1)", -11, static_cast<int64_t>(m10 + m1));

    const f80 x{1, UINT64_C(0x9000'0000'0000'0000)};  // 3.7823660360011051945e-4932
    const f80 y{1, UINT64_C(0x8000'0000'0000'0000)};  // 3.3621031431120935061e-4932
    EQ("x-y", 0, (x - y).tag());
    EQ("x-y", UINT64_C(0x0000'0000'0000'0001), (x - y).significand());
    EQ("y-x", 0x8000, (y - x).tag());
    EQ("y-x", UINT64_C(0x0000'0000'0000'0001), (y - x).significand());
}

void test_sub() {
    const f80 zero{UINT64_C(0)};
    const f80 p1{UINT64_C(1)};
    const f80 p10{UINT64_C(10)};
    const f80 m1{INT64_C(-1)};
    const f80 m10{INT64_C(-10)};

    // Negate
    EQ("-10", -10, static_cast<int64_t>(-p10));
    EQ("-(-10)", 10, static_cast<int64_t>(-m10));
    // Subtraction
    TRUE("0-0", (zero - zero).isZero());
    TRUE("1-1", (p1 - p1).isZero());
    TRUE("(-1)-(-1)", (m1 - m1).isZero());
    EQ("1-0", 1, static_cast<int64_t>(p1 - zero));
    EQ("0-1", -1, static_cast<int64_t>(zero - p1));
    EQ("1-(-1)", 2, static_cast<int64_t>(p1 - m1));
    EQ("-1-1", -2, static_cast<int64_t>(m1 - p1));
    EQ("1-10", -9, static_cast<int64_t>(p1 - p10));
    EQ("10-1", 9, static_cast<int64_t>(p10 - p1));
    EQ("-1-(-10)", 9, static_cast<int64_t>(m1 - m10));
    EQ("-10-(-1)", -9, static_cast<int64_t>(m10 - m1));
}

void test_mul() {
    const f80 zero{UINT64_C(0)};
    const f80 p1{UINT64_C(1)};
    const f80 p10{UINT64_C(10)};
    const f80 m1{INT64_C(-1)};
    const f80 m10{INT64_C(-10)};
    const f80 max{0x7FFE, UINT64_C(0xFFFF'FFFF'FFFF'FFFF)};  // 1.189731495357231765E4932
    const f80 min{1, UINT64_C(0x8000'0000'0000'0000)};       // 3.3621031431120935061e-4932

    // Multiplication
    EQ("0*0", 0, static_cast<int64_t>(zero * zero));
    EQ("0*1", 0, static_cast<int64_t>(zero * p1));
    EQ("1*0", 0, static_cast<int64_t>(p1 * zero));
    EQ("1*1", 1, static_cast<int64_t>(p1 * p1));
    EQ("2*2", 4, static_cast<int64_t>((p1 + p1) * (p1 + p1)));
    EQ("3*2", 6, static_cast<int64_t>((p1 + p1 + p1) * (p1 + p1)));
    EQ("-10*(-10)", 100, static_cast<int64_t>(m10 * m10));
    TRUE("max*max", (max * max).isInf());
    TRUE("min*min", (min * min).isZero());
    EQ("min*max", "3.999999999999999999", (min * max).str());
}

void test_div() {
    const f80 zero{UINT64_C(0)};
    const f80 p1{UINT64_C(1)};
    const f80 p10{UINT64_C(10)};
    const f80 m1{INT64_C(-1)};
    const f80 m10{INT64_C(-10)};

    // Division
    TRUE("0/0", (zero / zero).isNan());
    EQ("0/1", 0, static_cast<int64_t>(zero / p1));
    TRUE("1/0", (p1 / zero).isInf());
    EQ("10/2", 5, static_cast<int64_t>(p10 / (p1 + p1)));
    EQ("100/2", 50, static_cast<int64_t>((p10 * p10) / (p1 + p1)));
}

void test_infinity() {
    const f80 zero;
    const auto inf = f80::infinity();

    EQ("inf", INT64_MAX, static_cast<int64_t>(inf));
    EQ("-inf", INT64_MIN, static_cast<int64_t>(-inf));
    EQ("inf", UINT64_MAX, static_cast<uint64_t>(inf));
    EQ("-inf", 0, static_cast<uint64_t>(-inf));

    EQ("inf", FP_INFINITE, fpclassify(static_cast<double>(inf)));
    TRUE("inf", static_cast<double>(inf) > 0.0);
    EQ("-inf", FP_INFINITE, fpclassify(static_cast<double>(-inf)));
    TRUE("-inf", static_cast<double>(-inf) < 0.0);

    TRUE("0+inf", (zero + inf).isInf());
    TRUE("0+(-inf)", (zero + -inf).isInf());
    TRUE("0+(-inf)", (zero + -inf).isNegative());
    TRUE("inf+0", (inf + zero).isInf());
    TRUE("inf+inf", (inf + -inf).isNan());
    TRUE("inf+(-inf)", (inf + -inf).isNan());
    TRUE("0-inf", (zero - inf).isInf());
    TRUE("0-inf", (zero - inf).isNegative());
    TRUE("0-(-inf)", (zero - -inf).isInf());
    FALSE("0-(-inf)", (zero - -inf).isNegative());
    TRUE("inf-0", (inf - zero).isInf());
    TRUE("inf-inf", (inf - inf).isNan());
    TRUE("inf-(-inf)", (inf - -inf).isInf());

    TRUE("0*inf", (zero * inf).isNan());
    TRUE("inf*0", (inf * zero).isNan());
    TRUE("inf*inf", (inf * inf).isInf());
    FALSE("inf*inf", (inf * inf).isNegative());
    TRUE("-inf*(-inf)", (-inf * -inf).isInf());
    FALSE("-inf*(-inf)", (-inf * -inf).isNegative());
    TRUE("-inf*inf", (-inf * inf).isInf());
    TRUE("-inf*inf", (-inf * inf).isNegative());
    TRUE("inf*(-inf)", (inf * -inf).isInf());
    TRUE("inf*(-inf)", (inf * -inf).isNegative());

    TRUE("0/0", (zero / zero).isNan());
    TRUE("0/inf", (zero / inf).isZero());
    TRUE("0/(-inf)", (zero / -inf).isZero());
    TRUE("0/(-inf)", (zero / -inf).isNegative());
    TRUE("-0/inf", (-zero / inf).isZero());
    TRUE("-0/inf", (-zero / inf).isNegative());
    TRUE("-0/(-inf)", (-zero / -inf).isZero());
    FALSE("-0/(-inf)", (-zero / -inf).isNegative());
    TRUE("inf/0", (inf / zero).isNan());
    TRUE("inf/0", (inf / zero).isNan());
    TRUE("inf/inf", (inf / inf).isNan());
}

void test_not_a_number() {
    const f80 zero;
    const auto nan = f80::not_a_number();

    EQ("nan", INT64_MAX, static_cast<int64_t>(nan));
    EQ("nan", UINT64_MAX, static_cast<uint64_t>(nan));
    EQ("nan", FP_NAN, fpclassify(static_cast<double>(nan)));
    FALSE("nan", static_cast<double>(nan) > 0.0);
    FALSE("nan", static_cast<double>(nan) < 0.0);

    TRUE("0+nan", (zero + nan).isNan());
    TRUE("nan+0", (nan + zero).isNan());
    TRUE("nan+nan", (nan + nan).isNan());
    TRUE("0-nan", (zero - nan).isNan());
    TRUE("nan-0", (nan - zero).isNan());
    TRUE("nan-nan", (nan - nan).isNan());

    TRUE("0*nan", (zero * nan).isNan());
    TRUE("nan*0", (nan * zero).isNan());
    TRUE("nan*nan", (nan * nan).isNan());

    TRUE("0/nan", (zero / nan).isNan());
    TRUE("nan/0", (nan / zero).isNan());
    TRUE("nan/nan", (nan / nan).isNan());
}

constexpr char f80max[] = "1.189731495357231765E4932";
constexpr char f80min[] = "3.3621031431120935061E-4932";
constexpr char f80smin[] = "1.8225997659412373013E-4951";

void test_read() {
    EQ("0", 0, static_cast<int64_t>(f80::read("0")));
    EQ("1", 1, static_cast<int64_t>(f80::read("1")));
    EQ("-1", -1, static_cast<int64_t>(f80::read("-1")));
    EQ("INT64_MIN", INT64_MIN, static_cast<int64_t>(f80::read("-9223372036854775808")));
    EQ("INT64_MAX", INT64_MAX, static_cast<int64_t>(f80::read("9223372036854775807")));
    EQ("UINT64_MAX", UINT64_MAX, static_cast<uint64_t>(f80::read("18446744073709551615")));
    const auto ten_third = f80::read("3.3333333333333333333");
    EQ("1/3", 0x4000, ten_third.tag());
    EQ("1/3", UINT64_C(0xD555'5555'5555'5555), ten_third.significand());

    TRUE("nan", f80::read("nan").isNan());
    TRUE("-nan", f80::read("-nan").isNan());
    TRUE("inf", f80::read("inf").isInf());
    TRUE("-inf", f80::read("-inf").isInf());

    const auto max = f80::read(f80max);
    EQ("max", 0x7FFE, max.tag());
    EQ("max", UINT64_C(0xFFFF'FFFF'FFFF'FFFF), max.significand());
    const auto min = f80::read(f80min);
    EQ("min", 0x0001, min.tag());
    EQ("min", UINT64_C(0x8000'0000'0000'0000), min.significand());
    const auto smin = f80::read(f80smin);
    EQ("smin", 0x0000, smin.tag());
    EQ("smin", UINT64_C(0x0000'0000'0000'0001), smin.significand());
}

void test_write() {
    const f80 max{0x7FFE, UINT64_C(0xFFFF'FFFF'FFFF'FFFF)};
    EQ("max", f80max, max.str());
    const f80 min{0x0001, UINT64_C(0x8000'0000'0000'0000)};
    EQ("min", f80min, min.str());
    const f80 smin{0x0000, UINT64_C(0x0000'0000'0000'0001)};
    EQ("smin", f80smin, smin.str());
}

void run_tests() {
    RUN_TEST(test_constructor);
    RUN_TEST(test_cast_operator);
    RUN_TEST(test_add);
    RUN_TEST(test_sub);
    RUN_TEST(test_mul);
    RUN_TEST(test_div);
    RUN_TEST(test_infinity);
    RUN_TEST(test_not_a_number);
    RUN_TEST(test_read);
    RUN_TEST(test_write);
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
