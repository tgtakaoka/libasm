#include <stdio.h>
/*
 * Copyright 2022 Tadashi G. Takaoka
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
#include "str_buffer.h"
#include "test_unit_helper.h"
#include "value.h"

namespace libasm {
namespace test {

void set_up() {}

void tear_down() {}

void test_letter() {
    char buffer[9];
    StrBuffer out(buffer, sizeof(buffer));
    StrCaseBuffer lower(out, false);
    StrCaseBuffer upper(out, true);

    EQ("empty", "", out.str());
    EQ("ctor", sizeof(buffer) - 1, out.size());
    EQ("ctor", sizeof(buffer) - 1, lower.size());
    EQ("ctor", sizeof(buffer) - 1, upper.size());
    EQ("ctor", 0, out.len());
    EQ("ctor", 0, lower.len());
    EQ("ctor", 0, upper.len());
    EQ("ctor", sizeof(buffer) - 1, out.capacity());
    EQ("ctor", sizeof(buffer) - 1, lower.capacity());
    EQ("ctor", sizeof(buffer) - 1, upper.capacity());

    out.letter('a').over(lower).letter('B').over(upper).letter('c').over(out).letter('D');
    EQ("letter alpha", "abCD", out);  // dereference
    EQ("letter alpha", 2, lower.len());
    EQ("letter alpha", 3, upper.len());
    EQ("letter alpha", 4, out.len());

    out.over(upper).letter('0').over(lower).letter('1').over(out).letter('2');
    EQ("letter digit", OK, out.getError());
    EQ("letter digit", "abCD012", out.str());
    EQ("letter digit", 7, out.len());

    out.letter('x').letter('Y');
    EQ("letter xY", BUFFER_OVERFLOW, out.getError());
    EQ("letter xY", "abCD012x", out.str());
    EQ("letter xY", 0, out.capacity());

    out.letter('z').letter('W');
    EQ("letter zW", BUFFER_OVERFLOW, out.getError());
    EQ("letter zW", "abCD012x", out.str());
    EQ("letter zW", 0, out.capacity());

    EQ("ctor", sizeof(buffer) - 1, out.size());
    EQ("ctor", sizeof(buffer) - 1, lower.size());
    EQ("ctor", sizeof(buffer) - 1, upper.size());
}

void test_text() {
    char buffer[10];
    StrBuffer out(buffer, sizeof(buffer));
    StrCaseBuffer lower(out, false);
    StrCaseBuffer upper(out, true);

    out.text("aB").over(lower).text("cD").over(upper).text("eF").over(out).text("gH");
    EQ("text alpha", "aBcdEFgH", out.str());
    EQ("text alpha", 8, out.len());
    EQ("text alpha", 4, lower.len());
    EQ("text alpha", 6, upper.len());

    out.text("xY");
    EQ("text xY", BUFFER_OVERFLOW, out.getError());
    EQ("text xY", "aBcdEFgHx", out.str());
    EQ("text alpha", 9, out.len());

    out.text("zW");
    EQ("text zW", BUFFER_OVERFLOW, out.getError());
    EQ("text zW", "aBcdEFgHx", out);
}

void test_text_P() {
    char buffer[10];
    StrBuffer out(buffer, sizeof(buffer));
    StrCaseBuffer lower(out, false);
    StrCaseBuffer upper(out, true);

    out.text_P(PSTR("aB"))
            .over(lower)
            .text_P(PSTR("cD"))
            .over(upper)
            .text_P(PSTR("eF"))
            .over(out)
            .text_P(PSTR("gH"));

    EQ("text alpha", "aBcdEFgH", out.str());
    EQ("text alpha", 8, out.len());
    EQ("text alpha", 6, upper.len());
    EQ("text alpha", 4, lower.len());

    static const char xY[] PROGMEM = "xY";
    out.text_P(xY);
    EQ("text xY", BUFFER_OVERFLOW, out.getError());
    EQ("text xY", "aBcdEFgHx", out.str());
    EQ("text alpha", 0, out.capacity());

    static const char zW[] PROGMEM = "zW";
    out.text_P(zW);
    EQ("text zW", BUFFER_OVERFLOW, out.getError());
    EQ("text zW", "aBcdEFgHx", out.str());
}

void test_scanner() {
    static const char text[] = "aBcDeFgHxYzW";
    const StrScanner aB(text + 0, text + 2);
    const StrScanner cD(text + 2, text + 4);
    const StrScanner eF(text + 4, text + 6);
    const StrScanner gH(text + 6, text + 8);
    const StrScanner xY(text + 8, text + 10);
    const StrScanner zW(text + 10);

    char buffer[10];
    StrBuffer out(buffer, sizeof(buffer));
    StrCaseBuffer lower(out, false);
    StrCaseBuffer upper(out, true);

    out.text(aB).over(lower).text(cD).over(upper).text(eF).over(out).text(gH);
    EQ("text alpha", "aBcdEFgH", out.str());
    EQ("text alpha", 8, out.len());
    EQ("text alpha", 6, upper.len());
    EQ("text alpha", 4, lower.len());

    out.text(xY);
    EQ("text xY", BUFFER_OVERFLOW, out.getError());
    EQ("text xY", "aBcdEFgHx", out.str());
    EQ("text alpha", 9, out.len());

    out.text(zW);
    EQ("text zW", BUFFER_OVERFLOW, out.getError());
    EQ("text zW", "aBcdEFgHx", out.str());
}

void test_rtext() {
    char buffer[10];
    StrBuffer out(buffer, sizeof(buffer));
    StrCaseBuffer lower(out, false);
    StrCaseBuffer upper(out, true);

    // rletter(char)
    {
        out.rletter('a').over(lower).rletter('B').over(upper).rletter('c').over(out).rletter('D');
        EQ("rletter alpha", "aBcD", out.str());
        EQ("rletter alpha", 4, out.len());
        EQ("rletter alpha", 3, upper.len());
        EQ("rletter alpha", 2, lower.len());

        out.over(upper).rletter('0').over(lower).rletter('1').over(out).rletter('2').letter('3');
        EQ("rletter digit", OK, out.getError());
        EQ("rletter digit", "aBcD0123", out.str());
        EQ("rletter digit", 8, out.len());
        EQ("rletter digit", 1, out.capacity());

        out.rletter('x').rletter('Y');
        EQ("rletter xY", BUFFER_OVERFLOW, out.getError());
        EQ("rletter xY", "aBcD0123x", out.str());
        EQ("rletter xY", 9, out.len());

        out.rletter('z').rletter('W');
        EQ("rletter zW", BUFFER_OVERFLOW, out.getError());
        EQ("rletter zW", "aBcD0123x", out.str());
        EQ("rletter zW", 0, out.capacity());
    }

    out.reset();
    out.over(lower).over(upper);
    EQ("over", 0, out.len());
    EQ("over", 0, lower.len());
    EQ("over", 0, upper.len());

    // rtext(const char *)
    {
        out.rtext("aB").over(lower).rtext("cD").over(upper).rtext("eF").over(out).rtext("gH");
        EQ("rtext alpha", "aBcDeFgH", out.str());
        EQ("rtext alpha", 8, out.len());
        EQ("rtext alpha", 6, upper.len());
        EQ("rtext alpha", 4, lower.len());

        out.rtext("xY");
        EQ("rtext xY", BUFFER_OVERFLOW, out.getError());
        EQ("rtext xY", "aBcDeFgHx", out.str());
        EQ("rtext alpha", 9, out.len());

        out.rtext("zW");
        EQ("rtext zW", BUFFER_OVERFLOW, out.getError());
        EQ("rtext zW", "aBcDeFgHx", out.str());
    }

    out.reset();
    out.over(lower).over(upper);

    // rtext_P(const StrScanner &)
    {
        static const char xY[] PROGMEM = "xY";
        static const char zW[] PROGMEM = "zW";
        out.rtext_P(PSTR("aB"))
                .over(lower)
                .rtext_P(PSTR("cD"))
                .over(upper)
                .rtext_P(PSTR("eF"))
                .over(out)
                .rtext_P(PSTR("gH"));

        EQ("rtext alpha", "aBcDeFgH", out.str());
        EQ("rtext alpha", 8, out.len());
        EQ("rtext alpha", 6, upper.len());
        EQ("rtext alpha", 4, lower.len());

        out.rtext_P(xY);
        EQ("rtext xY", BUFFER_OVERFLOW, out.getError());
        EQ("rtext xY", "aBcDeFgHx", out);
        EQ("rtext xY", 9, out.len());

        out.rtext_P(zW);
        EQ("rtext zW", BUFFER_OVERFLOW, out.getError());
        EQ("rtext zW", "aBcDeFgHx", out.str());
    }

    out.reset();
    out.over(lower).over(upper);

    // rtext(const StrScanner &)
    {
        static const char text[] = "aBcDeFgHxYzW";
        const StrScanner aB(text + 0, text + 2);
        const StrScanner cD(text + 2, text + 4);
        const StrScanner eF(text + 4, text + 6);
        const StrScanner gH(text + 6, text + 8);
        const StrScanner xY(text + 8, text + 10);
        const StrScanner zW(text + 10);

        out.rtext(aB).over(lower).rtext(cD).over(upper).rtext(eF).over(out).rtext(gH);
        EQ("rtext alpha", "aBcDeFgH", out.str());
        EQ("rtext alpha", 8, out.len());
        EQ("rtext alpha", 6, upper.len());
        EQ("rtext alpha", 4, lower.len());

        out.rtext(xY);
        EQ("rtext xY", BUFFER_OVERFLOW, out.getError());
        EQ("rtext xY", "aBcDeFgHx", out.str());
        EQ("rtext xY", 9, out.len());

        out.rtext(zW);
        EQ("rtext zW", BUFFER_OVERFLOW, out.getError());
        EQ("rtext zW", "aBcDeFgHx", out.str());
    }
}

void test_hex() {
    char buffer[20];
    StrBuffer out(buffer, sizeof(buffer));

    out.reset();
    out.hex(static_cast<int8_t>(INT8_MAX));
    EQ("INT8_MAX", "7F", out.str());

    out.reset();
    out.hex(static_cast<int16_t>(INT16_MAX));
    EQ("INT16_MAX", "7FFF", out.str());

    out.reset();
    out.hex(static_cast<int32_t>(INT32_MAX));
    EQ("INT32_MAX", "7FFFFFFF", out.str());

    out.reset();
    out.hex(static_cast<int64_t>(INT64_MAX));
    EQ("INT64_MAX", "7FFFFFFFFFFFFFFF", out.str());

    out.reset();
    out.hex(static_cast<int8_t>(INT8_MIN));
    EQ("INT8_MIN", "80", out.str());

    out.reset();
    out.hex(static_cast<int16_t>(INT16_MIN));
    EQ("INT16_MIN", "8000", out.str());

    out.reset();
    out.hex(static_cast<int32_t>(INT32_MIN));
    EQ("INT32_MIN", "80000000", out.str());

    out.reset();
    out.hex(static_cast<int64_t>(INT64_MIN));
    EQ("INT64_MIN", "8000000000000000", out.str());

    out.reset();
    out.hex(static_cast<uint8_t>(UINT8_MAX));
    EQ("UINT8_MAX", "FF", out.str());

    out.reset();
    out.hex(static_cast<uint16_t>(UINT16_MAX));
    EQ("UINT16_MAX", "FFFF", out.str());

    out.reset();
    out.hex(static_cast<uint32_t>(UINT32_MAX));
    EQ("UINT32_MAX", "FFFFFFFF", out.str());

    out.reset();
    out.hex(static_cast<uint64_t>(UINT64_MAX));
    EQ("UINT64_MAX", "FFFFFFFFFFFFFFFF", out.str());

    out.reset();
    out.hex(static_cast<uint8_t>(0x34), 4);
    EQ("0x34", "0034", out.str());

    out.reset();
    out.hex(static_cast<uint16_t>(0x1234), 3);
    EQ("0x234", "234", out.str());

    out.reset();
    out.hex(UINT64_C(0x123456789ABCDEF0), 13);
    EQ("0x456789ABCDEF0", "456789ABCDEF0", out.str());
}

void test_dec() {
    char buffer[30];
    StrBuffer out(buffer, sizeof(buffer));

    out.int16(0);
    EQ("0", "0", out.str());

    out.reset();
    out.int16(0, 2);
    EQ("00", "00", out.str());

    out.reset();
    out.int16(INT16_MIN);
    EQ("INT16_MIN", "-32768", out.str());

    out.reset();
    out.int16(INT16_MAX);
    EQ("INT16_MAX", "32767", out.str());

    out.reset();
    out.int16(INT16_MAX, 6);
    EQ("INT16_MAX", "032767", out.str());

    out.reset();
    out.dec(INT8_MAX);
    EQ("INT8_MAX", "127", out.str());

    out.reset();
    out.dec(INT16_MAX);
    EQ("INT16_MAX", "32767", out.str());

    out.reset();
    out.dec(INT32_MAX);
    EQ("INT32_MAX", "2147483647", out.str());

    out.reset();
    out.dec(INT64_MAX);
    EQ("INT64_MAX", "9223372036854775807", out.str());

    out.reset();
    out.dec(INT8_MIN);
    EQ("INT8_MIN", "-128", out.str());

    out.reset();
    out.dec(INT16_MIN);
    EQ("INT16_MIN", "-32768", out.str());

    out.reset();
    out.dec(INT32_MIN);
    EQ("INT32_MIN", "-2147483648", out.str());

    out.reset();
    out.dec(INT64_MIN);
    EQ("INT64_MIN", "-9223372036854775808", out.str());

    out.reset();
    out.dec(UINT8_MAX);
    EQ("UINT8_MAX", "255", out.str());

    out.reset();
    out.dec(UINT16_MAX);
    EQ("UINT16_MAX", "65535", out.str());

    out.reset();
    out.dec(UINT32_MAX);
    EQ("UINT32_MAX", "4294967295", out.str());

    out.reset();
    out.dec(UINT64_MAX);
    EQ("UINT64_MAX", "18446744073709551615", out.str());
}

#define read_float(str)                                             \
    ({                                                            \
        char *end;                                                \
        const auto error = v.read(str, &end);                     \
        asserter.equals(__FILE__, __LINE__, "errno", 0, error);   \
        asserter.not_equals(__FILE__, __LINE__, "end", str, end); \
        v;                                                        \
    })

#if !defined(LIBASM_DIS_NOFLOAT)
void test_float32() {
    char buffer[16];
    StrCaseBuffer out(buffer, sizeof(buffer), false);
    float80_t v;

    EQ("FLT_0", "0", out.float32(v.set(UINT64_C(0))).str());

    EQ("FLT_EPSILON", "1.1920929e-07", out.reset().float32(read_float("1.1920929e-07")).str());

    EQ("FLT_MIN", "1.17549435e-38",
            out.reset().float32(v.set(-126 + 0x3FFF, UINT64_C(0x8000000000000000))).str());

    EQ("FLT_MAX", "3.40282347e+38",
            out.reset().float32(v.set(127 + 0x3FFF, UINT32_C(0xFFFFFF0000000000))).str());

    EQ("-FLT_INF", "-inf", out.reset().float32(float80_t::infinity(true)).str());

    EQ("-FLT_NAN", "-nan", out.reset().float32(float80_t::not_a_number(true)).str());

    char sbuf[10];
    StrCaseBuffer sout(sbuf, sizeof(sbuf), false);
    sout.float32(read_float("1.1920929e-07"));
    EQ("float32-overflow", BUFFER_OVERFLOW, sout.getError());
    EQ("float32-overflow", "1.1920929", sout.str());
}

void test_float64() {
    char buffer[32];
    StrCaseBuffer out(buffer, sizeof(buffer), true);
    float80_t v;

    EQ("DBL_0", "0", out.float64(v.set(UINT64_C(0))).str());

    EQ("DBL_EPSILON", "2.2204460492503131E-16",
            out.reset().float64(read_float("2.2204460492503131e-16")).str());

    EQ("DBL_MIN", "2.2250738585072014E-308",
            out.reset().float64(v.set(-1022 + 0x3FFF, UINT64_C(0x8000000000000000))).str());

    EQ("DBL_MAX", "1.7976931348623157E+308",
            out.reset().float64(v.set(1023 + 0x3FFF, UINT64_C(0xFFFFFFFFFFFFF800))).str());

    EQ("-DBL_INF", "-INF", out.reset().float64(float80_t::infinity(true)).str());

    EQ("-DBL_NAN", "-NAN", out.reset().float64(float80_t::not_a_number(true)).str());

    char sbuf[20];
    StrCaseBuffer sout(sbuf, sizeof(sbuf), false);
    sout.float64(read_float("2.2204460492503131e-16"));
    EQ("float64-overflow", BUFFER_OVERFLOW, sout.getError());
    EQ("float64-overflow", "2.2204460492503131e", sout.str());
}

void test_float80() {
    char buffer[40];
    StrCaseBuffer out(buffer, sizeof(buffer), true);
    float80_t v;

    EQ("LDBL_0", "0", out.float80(v.set(UINT64_C(0))).str());

    EQ("LDBL_EPSILON", "1.08420217248550443401E-19",
            out.reset().float80(read_float("1.08420217248550443401e-19")).str());

    EQ("LDBL_MIN", "3.36210314311209350626E-4932",
            out.reset().float80(v.set(-16382 + 0x3FFF, UINT64_C(0x8000000000000000))).str());

    EQ("LDBL_MAX", "1.18973149535723176502E+4932",
            out.reset().float80(v.set(16383 + 0x3FFF, UINT64_C(0xFFFFFFFFFFFFFFFF))).str());

    EQ("LDBL_INF", "INF", out.reset().float80(float80_t::infinity(false)).str());

    EQ("-LDBL_NAN", "-NAN", out.reset().float80(float80_t::not_a_number(true)).str());

    char sbuf[20];
    StrCaseBuffer sout(sbuf, sizeof(sbuf), false);
    sout.float80(read_float("1.08420217248550443401e-19"));
    EQ("float80-overflow", BUFFER_OVERFLOW, sout.getError());
    EQ("float64-overflow", "1.08420217248550443", sout.str());
}
#endif

void test_comma() {
    char buffer[8];
    StrBuffer out(buffer, sizeof(buffer));
    StrCaseBuffer lower(out, false);
    StrCaseBuffer upper(out, true);

    out.comma().over(lower).comma().over(upper).comma().over(out);
    EQ("comma", OK, out.getError());
    EQ("comma", ", , , ", out.str());

    out.comma();
    EQ("over", BUFFER_OVERFLOW, out.getError());
    EQ("comma", ", , , ,", out.str());
}

void test_reverse() {
    char buffer[10];
    StrBuffer out(buffer, sizeof(buffer));

    out.text("abc");
    char *m = out.mark();
    out.reverse(m);
    EQ("empty", OK, out.getError());
    EQ("empty", "abc", out.str());

    out.text("123");
    EQ("before", "abc123", out.str());
    out.reverse(m);
    EQ("after", OK, out.getError());
    EQ("after", "abc321", out.str());

    out.text("xY");
    out.reverse(buffer);
    EQ("while", OK, out.getError());
    EQ("whole", "Yx123cba", out.str());

    out.reverse(buffer + 9);
    EQ("overwrap", OVERWRAP_SEGMENT, out.getError());
    EQ("error", "Yx123cba", out.str());
}

void test_reset() {
    char buffer[8];
    StrBuffer out(buffer, sizeof(buffer));

    EQ("reset", 0, out.len());
    out.text("ABCDEFxY");
    EQ("reset", BUFFER_OVERFLOW, out.getError());
    EQ("reset", 7, out.len());
    out.reset();
    EQ("reset", OK, out.getError());
    EQ("reset", 0, out.len());
    EQ("reset", "", out.str());
}

void run_tests() {
    RUN_TEST(test_letter);
    RUN_TEST(test_text);
    RUN_TEST(test_text_P);
    RUN_TEST(test_scanner);
    RUN_TEST(test_rtext);
    RUN_TEST(test_hex);
    RUN_TEST(test_dec);
#if !defined(LIBASM_DIS_NOFLOAT)
    RUN_TEST(test_float32);
    RUN_TEST(test_float64);
    RUN_TEST(test_float80);
#endif
    RUN_TEST(test_comma);
    RUN_TEST(test_reverse);
    RUN_TEST(test_reset);
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
