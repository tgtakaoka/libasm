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

#include "str_scanner.h"
#include "test_unit_helper.h"
#include "value.h"
#include <string>

#define SEQ(msg, expected, actual)        \
    do {                                  \
        std::string e = (expected).str(); \
        std::string a = (actual).str();   \
        EQ(msg, e.c_str(), a.c_str());    \
    } while (0)

namespace libasm {
namespace test {

using f80 = Value::float_t;

void set_up() {}

void tear_down() {}

void test_constructor() {
    Value undef;
    TRUE("undef", undef.isUndefined());
    TRUE("undef", undef.isUnsigned());
    FALSE("undef", undef.isSigned());
    TRUE("undef", undef.isInteger());
    FALSE("undef", undef.isZero());
    FALSE("undef", undef.isNegative());
    EQ("undef", 0, undef.getUnsigned());
    EQ("undef", 0, undef.getInteger());

    Value zero;
    zero.setSigned(0);
    FALSE("zero", zero.isUndefined());
    TRUE("zero", zero.isUnsigned());
    FALSE("zero", zero.isSigned());
    TRUE("zero", zero.isInteger());
    TRUE("zero", zero.isZero());
    FALSE("zero", zero.isNegative());
    EQ("zero", 0, zero.getUnsigned());
    EQ("zero", 0, zero.getInteger());
}

void test_read() {
    Value v;

    StrScanner zero{"0.0"};
    EQ("zero", OK, v.read(zero, RADIX_10));
    EQ("zero", 0, v.getInteger());
    EQ("zero", ".0", zero.str());

    StrScanner umax64{"18446744073709551615E20"};
    EQ("umax64", OK, v.read(umax64, RADIX_10));
    EQ("umax64", UINT64_MAX, v.getInteger());
    EQ("umax64", "E20", umax64.str());

    StrScanner umin65{"18446744073709551616H"};
    EQ("umin65", OVERFLOW_RANGE, v.read(umin65, RADIX_10));
    TRUE("umin65", v.isUndefined());
    EQ("umin65", "H", umin65.str());

    StrScanner nan{"-1"};
    EQ("nan", NOT_AN_EXPECTED, v.read(nan, RADIX_10));
    TRUE("nan", v.isUndefined());
    EQ("nan", "-1", nan.str());
}

void test_setget() {
    Value u;
    u.setUnsigned(UINT32_MAX);
    FALSE("u", u.isUndefined());
    TRUE("u", u.isUnsigned());
    FALSE("u", u.isSigned());
    TRUE("u", u.isInteger());
    FALSE("u", u.isZero());
    FALSE("u", u.isNegative());
    EQ("u", UINT32_MAX, u.getUnsigned());
    EQ("u", UINT32_MAX, u.getInteger());

    Value s;
    s.setSigned(INT32_MAX);
    FALSE("s", s.isUndefined());
    TRUE("s", s.isUnsigned());
    FALSE("s", s.isSigned());
    TRUE("s", s.isInteger());
    FALSE("s", s.isZero());
    FALSE("s", s.isNegative());
    EQ("s", INT32_MAX, s.getUnsigned());
    EQ("s", INT32_MAX, s.getInteger());

    s.setSigned(INT32_MIN);
    FALSE("s", s.isUndefined());
    FALSE("s", s.isUnsigned());
    TRUE("s", s.isSigned());
    TRUE("s", s.isInteger());
    FALSE("s", s.isZero());
    TRUE("s", s.isNegative());
    EQ("s", INT32_MIN, s.getSigned());
    EQ("s", INT32_MIN, s.getInteger());

    Value ui;
    ui.setUinteger(UINT64_MAX);
    TRUE("ui", ui.isUnsigned());
    FALSE("ui", ui.isSigned());
    TRUE("ui", ui.isInteger());
    EQ("ui", UINT32_MAX, ui.getUnsigned());
    EQ("ui", UINT64_MAX, ui.getInteger());

    Value si;
    si.setInteger(INT64_MAX);
    FALSE("si", si.isUndefined());
    TRUE("si", si.isUnsigned());
    FALSE("si", si.isSigned());
    TRUE("si", si.isInteger());
    FALSE("si", si.isZero());
    FALSE("si", si.isNegative());
    EQ("si", UINT32_MAX, si.getUnsigned());
    EQ("si", INT64_MAX, si.getInteger());

    si.setInteger(INT64_MIN);
    FALSE("si", si.isUndefined());
    FALSE("si", si.isUnsigned());
    TRUE("si", si.isSigned());
    TRUE("si", si.isInteger());
    FALSE("si", si.isZero());
    TRUE("si", si.isNegative());
    EQ("si", 0, si.getSigned());
    EQ("si", INT64_MIN, si.getInteger());

    si.clear();
    TRUE("clear", si.isUndefined());

#ifndef ASM_NOFLOAT
    Value::float_t v;
    Value f;
    f.setFloat(v.read("0.0"));
    FALSE("f", f.isUndefined());
    FALSE("f", f.isUnsigned());
    FALSE("f", f.isSigned());
    FALSE("f", f.isInteger());
    TRUE("f", f.isFloat());
    TRUE("f", f.isZero());
    FALSE("f", f.isNegative());
    TRUE("f", f.getFloat().isZero());

    f.setUinteger(UINT64_MAX);
    FALSE("f", f.isNegative());
    SEQ("f", v.set(UINT64_MAX), f.getFloat());
    f.setInteger(INT64_MIN);
    TRUE("f", f.isNegative());
    SEQ("f", v.set(INT64_MIN), f.getFloat());
#endif
}

void test_overflow() {
    Value v;

    v.setSigned(0);
    FALSE("0", v.overflowInt8());
    FALSE("0", v.overflowUint8());
    FALSE("0", v.overflowInt16());
    FALSE("0", v.overflowUint16());
    FALSE("0", v.overflowInt32());
    FALSE("0", v.overflowUint32());

    v.setSigned(-1);
    FALSE("-1", v.overflowInt8());
    FALSE("-1", v.overflowUint8());
    FALSE("-1", v.overflowInt16());
    FALSE("-1", v.overflowUint16());
    FALSE("-1", v.overflowInt32());
    FALSE("-1", v.overflowUint32());

    v.setSigned(0x7F);
    FALSE("0x7F", v.overflowInt8());
    FALSE("0x7F", v.overflowUint8());
    FALSE("0x7F", v.overflow(INT8_MAX, INT8_MIN));
    FALSE("0x7F", v.overflow(UINT8_MAX, INT8_MIN));

    v.setSigned(0x80);
    TRUE("0x80", v.overflowInt8());
    FALSE("0x80", v.overflowUint8());
    TRUE("0x80", v.overflow(INT8_MAX, INT8_MIN));
    FALSE("0x80", v.overflow(UINT8_MAX, INT8_MIN));

    v.setUnsigned(0xFF);
    TRUE("0xFF", v.overflowInt8());
    FALSE("0xFF", v.overflowUint8());
    TRUE("0xFF", v.overflow(INT8_MAX, INT8_MIN));
    FALSE("0xFF", v.overflow(UINT8_MAX, INT8_MIN));

    v.setUnsigned(0x100);
    TRUE("0x100", v.overflowInt8());
    TRUE("0x100", v.overflowUint8());
    TRUE("0x100", v.overflow(INT8_MAX, INT8_MIN));
    TRUE("0x100", v.overflow(UINT8_MAX, INT8_MIN));
    FALSE("0x100", v.overflowInt16());
    FALSE("0x100", v.overflowUint16());

    v.setSigned(-0x80);
    FALSE("-0x80", v.overflowInt8());
    FALSE("-0x80", v.overflowUint8());
    TRUE("-0x80", v.overflow(INT8_MAX));
    TRUE("-0x80", v.overflow(UINT8_MAX));

    v.setSigned(-0x81);
    TRUE("-0x81", v.overflowInt8());
    TRUE("-0x81", v.overflowUint8());
    TRUE("-0x81", v.overflow(INT8_MAX, INT8_MIN));
    TRUE("-0x81", v.overflow(UINT8_MAX, INT8_MIN));

    v.setSigned(0x7FFF);
    FALSE("0x7FFF", v.overflowInt16());
    FALSE("0x7FFF", v.overflowUint16());
    TRUE("0x7FFF", v.overflowInt8());
    TRUE("0x7FFF", v.overflowUint8());

    v.setSigned(0x8000);
    TRUE("0x8000", v.overflowInt16());
    FALSE("0x8000", v.overflowUint16());
    TRUE("0x8000", v.overflowInt8());
    TRUE("0x8000", v.overflowUint8());

    v.setUnsigned(0xFFFF);
    TRUE("0xFFFF", v.overflowInt16());
    FALSE("0xFFFF", v.overflowUint16());
    TRUE("0xFFFF", v.overflowInt8());
    TRUE("0xFFFF", v.overflowUint8());

    v.setUnsigned(0x10000);
    TRUE("0x10000", v.overflowInt16());
    TRUE("0x10000", v.overflowUint16());

    v.setSigned(-0x8000);
    FALSE("-0x8000", v.overflowInt16());
    FALSE("-0x8000", v.overflowUint16());
    TRUE("-0x8000", v.overflow(UINT16_MAX));
    TRUE("-0x8000", v.overflow(UINT16_MAX));
    TRUE("-0x8000", v.overflowInt8());
    TRUE("-0x8000", v.overflowUint8());

    v.setSigned(-0x8001);
    TRUE("-0x8001", v.overflowInt16());
    TRUE("-0x8001", v.overflowUint16());
    TRUE("-0x8001", v.overflow(INT16_MAX, INT16_MIN));
    TRUE("-0x8001", v.overflow(UINT16_MAX, INT16_MIN));
    FALSE("-0x8001", v.overflowInt32());
    FALSE("-0x8001", v.overflowUint32());

    v.setSigned(INT32_C(0x7FFFFFFF));
    FALSE("0x7FFFFFFF", v.overflowInt32());
    FALSE("0x7FFFFFFF", v.overflowUint32());

    v.setUnsigned(UINT32_C(0x80000000));
    TRUE("0x80000000", v.overflowInt32());
    FALSE("0x80000000", v.overflowUint32());

    v.setUnsigned(UINT32_C(0xFFFFFFFF));
    TRUE("0xFFFFFFFF", v.overflowInt32());
    FALSE("0xFFFFFFFF", v.overflowUint32());

    v.setInteger(INT64_C(0x100000000));
    TRUE("0x100000000", v.overflowInt32());
    TRUE("0x100000000", v.overflowUint32());

    v.setSigned(INT32_C(-0x80000000));
    FALSE("-0x80000000", v.overflowInt32());
    FALSE("-0x80000000", v.overflowUint32());

    v.setInteger(INT64_C(-0x80000001));
    TRUE("-0x80000001", v.overflowInt32());
    TRUE("-0x80000001", v.overflowUint32());

    v.setUnsigned(10);
    FALSE("10", v.overflowInt8());
    FALSE("10", v.overflowUint8());
    FALSE("10", v.overflow(10));
    TRUE("10", v.overflow(9));
    FALSE("10", v.overflow(15, 5));
    TRUE("10", v.overflow(30, 20));
    TRUE("10", v.overflow(5, -20));

    v.setSigned(-10);
    FALSE("-10", v.overflowInt8());
    FALSE("-10", v.overflowUint8());
    TRUE("-10", v.overflow(10));
    TRUE("-10", v.overflow(10, -9));
    FALSE("-10", v.overflow(10, -10));
    FALSE("-10", v.overflow(10, -20));
}

void test_bool() {
    Value undef, zero, p1, m1, p2, m2;
    zero.setSigned(0);
    p1.setSigned(1);
    p2.setSigned(2);
    m1.setSigned(-1);
    m2.setSigned(-2);

    TRUE("0==0", zero == zero);
    TRUE("1==1", p1 == p1);
    TRUE("-1==-1", m1 == m1);
    FALSE("0<0", zero < zero);
    TRUE("0<1", zero < p1);
    FALSE("0<-1", zero < m1);
    FALSE("1<0", p1 < zero);
    FALSE("1<1", p1 < p1);
    FALSE("1<-1", p1 < m1);
    TRUE("-1<0", m1 < zero);
    TRUE("-1<1", m1 < p1);
    FALSE("-1<-1", m1 < m1);
    FALSE("1==2", p1 == p2);
    TRUE("1<2", p1 < p2);
    FALSE("2==1", p2 == p1);
    FALSE("2<1", p2 < p1);
    FALSE("-1==-2", m1 == m2);
    FALSE("-1<-2", m1 < m2);
    FALSE("-2==-1", m2 == m1);
    TRUE("-2<-1", m2 < m1);

    FALSE("undef", static_cast<bool>(undef));
    FALSE("0", static_cast<bool>(zero));
    TRUE("1", static_cast<bool>(p1));
    TRUE("-1", static_cast<bool>(m1));
    TRUE("undef", !undef);
    TRUE("0", !zero);
    FALSE("1", !p1);
    FALSE("-1", !m1);

    Value t, f;
    t.setSigned(1);
    f.setSigned(0);
    TRUE("t&&t", t && t);
    FALSE("t&&f", t && f);
    FALSE("f&&t", f && t);
    FALSE("f&&f", f && f);
    TRUE("t||t", t || t);
    TRUE("t||f", t || f);
    TRUE("f||t", f || t);
    FALSE("f||f", f || f);
}

#define read_float(str)                                           \
    ({                                                            \
        char *end;                                                \
        const auto error = v.read(str, &end);                     \
        asserter.equals(__FILE__, __LINE__, "errno", 0, error);   \
        asserter.not_equals(__FILE__, __LINE__, "end", str, end); \
        v;                                                        \
    })

void test_arithmetic() {
    Value p1, m1, p2, m2, p6, m6, p10, m10;
    p1.setSigned(1);
    m1.setSigned(-1);
    p2.setSigned(2);
    m2.setSigned(-2);
    p6.setSigned(6);
    m6.setSigned(-6);
    p10.setSigned(10);
    m10.setSigned(-10);

    TRUE("-(1)", (-p1).isSigned());
    EQ("-(1)", -1, (-p1).getSigned());
    TRUE("-(-1)", (-m1).isUnsigned());
    EQ("-(-1)", 1, (-m1).getUnsigned());

    EQ("1+2", 3, (p1 + p2).getSigned());
    EQ("2+1", 3, (p2 + p1).getSigned());
    EQ("-1+2", 1, (m1 + p2).getSigned());
    EQ("2+(-1)", 1, (p2 + m1).getSigned());
    EQ("1+(-2)", -1, (p1 + m2).getSigned());
    EQ("-2+1", -1, (m2 + p1).getSigned());
    EQ("-1+(-2)", -3, (m1 + m2).getSigned());
    EQ("-2+(-1)", -3, (m2 + m1).getSigned());

    EQ("1-2", -1, (p1 - p2).getSigned());
    EQ("2-1", 1, (p2 - p1).getSigned());
    EQ("-1-2", -3, (m1 - p2).getSigned());
    EQ("2-(-1)", 3, (p2 - m1).getSigned());
    EQ("1-(-2)", 3, (p1 - m2).getSigned());
    EQ("-2-1", -3, (m2 - p1).getSigned());
    EQ("-1-(-2)", 1, (m1 - m2).getSigned());
    EQ("-2-(-1)", -1, (m2 - m1).getSigned());

    EQ("6*2", 12, (p6 * p2).getSigned());
    EQ("2*6", 12, (p2 * p6).getSigned());
    EQ("-6*2", -12, (m6 * p2).getSigned());
    EQ("2*(-6)", -12, (p2 * m6).getSigned());
    EQ("6*(-2)", -12, (p6 * m2).getSigned());
    EQ("-2*6", -12, (m2 * p6).getSigned());
    EQ("-6*(-2)", 12, (m6 * m2).getSigned());
    EQ("-2*(-6)", 12, (m2 * m6).getSigned());

    EQ("2/10", 0, (p2 / p10).getSigned());
    EQ("10/2", 5, (p10 / p2).getSigned());
    EQ("-2/10", 0, (m2 / p10).getSigned());
    EQ("10/(-2)", -5, (p10 / m2).getSigned());
    EQ("2/(-10)", 0, (p2 / m10).getSigned());
    EQ("-10/2", -5, (m10 / p2).getSigned());
    EQ("-2/(-10)", 0, (m2 / m10).getSigned());
    EQ("-10/(-2)", 5, (m10 / m2).getSigned());

    EQ("6%10", 6, (p6 % p10).getSigned());
    EQ("10%6", 4, (p10 % p6).getSigned());
    EQ("-6%10", -6, (m6 % p10).getSigned());
    EQ("10%(-6)", 4, (p10 % m6).getSigned());
    EQ("6%(-10)", 6, (p6 % m10).getSigned());
    EQ("-10%6", -4, (m10 % p6).getSigned());
    EQ("-6%(-10)", -6, (m6 % m10).getSigned());
    EQ("-10%(-6)", -4, (m10 % m6).getSigned());

#ifndef ASM_NOFLOAT
    Value::float_t v;
    Value p05, m05, p3, m3;
    p1.setSigned(1);
    m1.setSigned(-1);
    p05.setFloat(read_float("0.5"));
    m05.setFloat(read_float("-0.5"));
    p3.setSigned(3);
    m3.setSigned(-3);

    TRUE("1+0.5", read_float("1.5") == (p1 + p05).getFloat());
    TRUE("0.5+1", read_float("1.5") == (p05 + p1).getFloat());
    TRUE("-1+0.5", read_float("-0.5") == (m1 + p05).getFloat());
    TRUE("0.5+(-1)", read_float("-0.5") == (p05 + m1).getFloat());
    TRUE("1+(-0.5)", read_float("0.5") == (p1 + m05).getFloat());
    TRUE("-0.5+1", read_float("0.5") == (m05 + p1).getFloat());
    TRUE("-1+(-0.5)", read_float("-1.5") == (m1 + m05).getFloat());
    TRUE("-0.5+(-1)", read_float("-1.5") == (m05 + m1).getFloat());

    TRUE("1-0.5", read_float("0.5") == (p1 - p05).getFloat());
    TRUE("0.5-1", read_float("-0.5") == (p05 - p1).getFloat());
    TRUE("-1-0.5", read_float("-1.5") == (m1 - p05).getFloat());
    TRUE("0.5-(-1)", read_float("1.5") == (p05 - m1).getFloat());
    TRUE("1-(-0.5)", read_float("1.5") == (p1 - m05).getFloat());
    TRUE("-0.5-1", read_float("-1.5") == (m05 - p1).getFloat());
    TRUE("-1-(-0.5)", read_float("-0.5") == (m1 - m05).getFloat());
    TRUE("-0.5-(-1)", read_float("0.5") == (m05 - m1).getFloat());

    TRUE("3*0.5", read_float("1.5") == (p3 * p05).getFloat());
    TRUE("0.5*3", read_float("1.5") == (p05 * p3).getFloat());
    TRUE("-3*0.5", read_float("-1.5") == (m3 * p05).getFloat());
    TRUE("0.5*(-3)", read_float("-1.5") == (p05 * m3).getFloat());
    TRUE("3*(-0.5)", read_float("-1.5") == (p3 * m05).getFloat());
    TRUE("-0.5*3", read_float("-1.5") == (m05 * p3).getFloat());
    TRUE("-3*(-0.5)", read_float("1.5") == (m3 * m05).getFloat());
    TRUE("-0.5*(-3)", read_float("1.5") == (m05 * m3).getFloat());

    TRUE("2/0.5", read_float("4.0") == (p2 / p05).getFloat());
    TRUE("0.5/2", read_float("0.25") == (p05 / p2).getFloat());
    TRUE("-2/0.5", read_float("-4.0") == (m2 / p05).getFloat());
    TRUE("0.5/(-2)", read_float("-0.25") == (p05 / m2).getFloat());
    TRUE("2/(-0.5)", read_float("-4.0") == (p2 / m05).getFloat());
    TRUE("-0.5/2", read_float("-0.25") == (m05 / p2).getFloat());
    TRUE("-2/(-0.5)", read_float("4.0") == (m2 / m05).getFloat());
    TRUE("-0.5/(-2)", read_float("0.25") == (m05 / m2).getFloat());
#endif
}

void test_logical() {
    Value p1, p2, p4, p7, m7, p32, u32max, i32min;
    p1.setSigned(1);
    p2.setSigned(2);
    p4.setSigned(4);
    p7.setSigned(7);
    m7.setSigned(-7);
    p32.setSigned(32);
    u32max.setUnsigned(UINT32_MAX);
    i32min.setUnsigned(INT32_MIN);

    EQ("~1", 0xFFFFFFFE, ~p1.getUnsigned());
    EQ("1&7", 1, (p1 & p7).getUnsigned());
    EQ("1|4", 5, (p1 | p4).getUnsigned());
    EQ("(2|4)&7", 6, ((p2 | p4) & p7).getUnsigned());
    EQ("(2|4)^7", 1, ((p2 | p4) ^ p7).getUnsigned());

    EQ("7>>2", 1, (p7 >> p2).getUnsigned());
    EQ("7<<2", 28, (p7 << p2).getUnsigned());
    EQ("-7>>2", -2, (m7 >> p2).getInteger());
    EQ("-7<<2", -28, (m7 << p2).getInteger());
    EQ("UINT32_MAX>>32", 0, (u32max >> p32).getUnsigned());
    EQ("INT32_MIN>>32", 0, (i32min >> p32).getSigned());
    EQ("UINT32_MAX<<32", 0, (u32max << p32).getUnsigned());
    EQ("INT32_MIN<<32", 0, (i32min << p32).getSigned());
}

void run_tests() {
    RUN_TEST(test_constructor);
    RUN_TEST(test_read);
    RUN_TEST(test_setget);
    RUN_TEST(test_overflow);
    RUN_TEST(test_bool);
    RUN_TEST(test_arithmetic);
    RUN_TEST(test_logical);
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
