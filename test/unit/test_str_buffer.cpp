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

void test_int16() {
    char buffer[8];
    StrBuffer out(buffer, sizeof(buffer));

    out.int16(0);
    EQ("int16-0", "0", out.str());

    out.reset();
    out.int16(0, 2);
    EQ("int16-0", "00", out.str());

    out.reset();
    out.int16(INT16_MIN);
    EQ("int16-min", "-32768", out.str());

    out.reset();
    out.int16(INT16_MAX);
    EQ("int16-max", "32767", out.str());

    out.reset();
    out.int16(INT16_MAX, 6);
    EQ("int16-max", "032767", out.str());
}

void test_float32() {
    char buffer[16];
    StrCaseBuffer out(buffer, sizeof(buffer), false);

    out.float32(0);
    EQ("float32-0", "0", out.str());

    out.reset();
    out.float32(FLT_EPSILON, 10);
    EQ("float32-epsilon", "1.192092896e-07", out.str());

    out.reset();
    out.float32(FLT_MIN, 10);
    EQ("float32-min", "1.175494351e-38", out.str());

    out.reset();
    out.float32(FLT_MAX, 10);
    EQ("float32-max", "3.402823466e+38", out.str());

    out.reset();
    out.float32(-INFINITY);
    EQ("float32-inf", "-inf", out.str());

    out.reset();
    out.float32(-NAN);
    EQ("float32-nan", "-nan", out.str());

    char sbuf[10];
    StrCaseBuffer sout(sbuf, sizeof(sbuf), false);
    sout.float32(FLT_EPSILON);
    EQ("float32-overflow", BUFFER_OVERFLOW, sout.getError());
    EQ("float32-overflow", "1.1920929", sout.str());
}

void test_float64() {
    char buffer[32];
    StrCaseBuffer out(buffer, sizeof(buffer), true);

    out.float64(0);
    EQ("float64-0", "0", out.str());

    out.reset();
    out.float64(DBL_EPSILON);
    EQ("float64-epsilon", "2.2204460492503131E-16", out.str());

    out.reset();
    out.float64(DBL_MIN);
    EQ("float64-min", "2.2250738585072014E-308", out.str());

    out.reset();
    out.float64(DBL_MAX);
    EQ("float64-max", "1.7976931348623157E+308", out.str());

    out.reset();
    out.float64(-INFINITY);
    EQ("float64-inf", "-INF", out.str());

    out.reset();
    out.float64(-NAN);
    EQ("float64-nan", "-NAN", out.str());

    char sbuf[20];
    StrCaseBuffer sout(sbuf, sizeof(sbuf), false);
    sout.float64(DBL_EPSILON);
    EQ("float64-overflow", BUFFER_OVERFLOW, sout.getError());
    EQ("float64-overflow", "2.2204460492503131e", sout.str());
}

void test_float80() {
    char buffer[40];
    StrCaseBuffer out(buffer, sizeof(buffer), true);

    out.float80(0, UINT64_C(0), 17);
    EQ("float80-0", "0", out.str());

    out.reset();
    out.float80(-52, UINT64_C(1), 17); // DBL_EPSILON
    EQ("float80-epsilon", "2.2204460492503131E-16", out.str());

    out.reset();
    out.float80(-1022, UINT64_C(1), 17); // DBL_MIN
    EQ("float80-min", "2.2250738585072014E-308", out.str());

    out.reset();
    out.float80(+1023-52, UINT64_C(0x1F'FFFF'FFFF'FFFF), 17); // DBL_MAX
    EQ("float80-max", "1.7976931348623157E+308", out.str());

    out.reset();
    out.float80(INT16_MAX, UINT64_C(0xFFFF'FFFF'FFFF'FFFF));
    EQ("float80-inf", "INF", out.str());

    char sbuf[20];
    StrCaseBuffer sout(sbuf, sizeof(sbuf), false);
    sout.float80(-52, UINT64_C(1), 17); // DBL_EPSILON
    EQ("float80-overflow", BUFFER_OVERFLOW, sout.getError());
    EQ("float64-overflow", "2.2204460492503131e", sout.str());
}

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
    out.text("123");
    EQ("before", "abc123", out.str());
    out.reverse(m);
    EQ("after", "abc321", out.str());

    out.text("xY");
    out.reverse(buffer);
    EQ("whole", "Yx123cba", out.str());

    out.reverse(buffer + 8);
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
    RUN_TEST(test_int16);
    RUN_TEST(test_float32);
    RUN_TEST(test_float64);
    RUN_TEST(test_float80);
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
