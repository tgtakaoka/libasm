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

#include "str_buffer.h"

#include "test_src_helper.h"

namespace libasm {
namespace test {

void set_up() {}

void tear_down() {}

void test_letter() {
    char buffer[9];
    StrBuffer out(buffer, sizeof(buffer));
    LowercaseBuffer lower(out);
    UppercaseBuffer upper(out);

    EQ("empty", "", buffer);
    EQ("ctor", sizeof(buffer) - 1, out.size());
    EQ("ctor", sizeof(buffer) - 1, lower.size());
    EQ("ctor", sizeof(buffer) - 1, upper.size());

    out.letter('a').over(lower).letter('B').over(upper).letter('c').over(out).letter('D');
    EQ("letter alpha", "abCD", buffer);
    EQ("letter alpha", sizeof(buffer) - 3, lower.size());
    EQ("letter alpha", sizeof(buffer) - 4, upper.size());
    EQ("letter alpha", sizeof(buffer) - 5, out.size());

    out.over(upper).letter('0').over(lower).letter('1').over(out).letter('2');
    EQ("letter digit", OK, out.getError());
    EQ("letter digit", "abCD012", buffer);
    EQ("letter digit", sizeof(buffer) - 8, out.size());

    out.letter('x').letter('Y');
    EQ("letter xY", BUFFER_OVERFLOW, out.getError());
    EQ("letter xY", "abCD012x", buffer);
    EQ("letter xY", 0, out.size());

    out.letter('z').letter('W');
    EQ("letter zW", BUFFER_OVERFLOW, out.getError());
    EQ("letter zW", "abCD012x", buffer);
    EQ("letter zW", 0, out.size());
}

void test_text() {
    char buffer[10];
    StrBuffer out(buffer, sizeof(buffer));
    LowercaseBuffer lower(out);
    UppercaseBuffer upper(out);

    out.text("aB").over(lower).text("cD").over(upper).text("eF").over(out).text("gH");
    EQ("text alpha", "aBcdEFgH", buffer);
    EQ("text alpha", sizeof(buffer) - 5, lower.size());
    EQ("text alpha", sizeof(buffer) - 7, upper.size());
    EQ("text alpha", sizeof(buffer) - 9, out.size());

    out.text("xY");
    EQ("text xY", BUFFER_OVERFLOW, out.getError());
    EQ("text xY", "aBcdEFgHx", buffer);
    EQ("text alpha", sizeof(buffer) - 10, out.size());

    out.text("zW");
    EQ("text zW", BUFFER_OVERFLOW, out.getError());
    EQ("text zW", "aBcdEFgHx", buffer);
}

void test_text_P() {
    char buffer[10];
    StrBuffer out(buffer, sizeof(buffer));
    LowercaseBuffer lower(out);
    UppercaseBuffer upper(out);

    out.text_P(PSTR("aB"))
            .over(lower)
            .text_P(PSTR("cD"))
            .over(upper)
            .text_P(PSTR("eF"))
            .over(out)
            .text_P(PSTR("gH"));

    EQ("text alpha", "aBcdEFgH", buffer);
    EQ("text alpha", sizeof(buffer) - 5, lower.size());
    EQ("text alpha", sizeof(buffer) - 7, upper.size());
    EQ("text alpha", sizeof(buffer) - 9, out.size());

    static const char xY[] PROGMEM = "xY";
    out.text_P(xY);
    EQ("text xY", BUFFER_OVERFLOW, out.getError());
    EQ("text xY", "aBcdEFgHx", buffer);
    EQ("text alpha", sizeof(buffer) - 10, out.size());

    static const char zW[] PROGMEM = "zW";
    out.text_P(zW);
    EQ("text zW", BUFFER_OVERFLOW, out.getError());
    EQ("text zW", "aBcdEFgHx", buffer);
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
    LowercaseBuffer lower(out);
    UppercaseBuffer upper(out);

    out.text(aB).over(lower).text(cD).over(upper).text(eF).over(out).text(gH);
    EQ("text alpha", "aBcdEFgH", buffer);
    EQ("text alpha", sizeof(buffer) - 5, lower.size());
    EQ("text alpha", sizeof(buffer) - 7, upper.size());
    EQ("text alpha", sizeof(buffer) - 9, out.size());

    out.text(xY);
    EQ("text xY", BUFFER_OVERFLOW, out.getError());
    EQ("text xY", "aBcdEFgHx", buffer);
    EQ("text alpha", sizeof(buffer) - 10, out.size());

    out.text(zW);
    EQ("text zW", BUFFER_OVERFLOW, out.getError());
    EQ("text zW", "aBcdEFgHx", buffer);
}

void test_rtext() {
    char buffer[10];
    StrBuffer out(buffer, sizeof(buffer));
    LowercaseBuffer lower(out);
    UppercaseBuffer upper(out);

    // rletter(char)
    {
        out.rletter('a').over(lower).rletter('B').over(upper).rletter('c').over(out).rletter('D');
        EQ("rletter alpha", "aBcD", buffer);
        EQ("rletter alpha", sizeof(buffer) - 3, lower.size());
        EQ("rletter alpha", sizeof(buffer) - 4, upper.size());
        EQ("rletter alpha", sizeof(buffer) - 5, out.size());

        out.over(upper).rletter('0').over(lower).rletter('1').over(out).rletter('2').letter('3');
        EQ("rletter digit", OK, out.getError());
        EQ("rletter digit", "aBcD0123", buffer);
        EQ("rletter digit", sizeof(buffer) - 9, out.size());

        out.rletter('x').rletter('Y');
        EQ("rletter xY", BUFFER_OVERFLOW, out.getError());
        EQ("rletter xY", "aBcD0123x", buffer);
        EQ("rletter xY", 0, out.size());

        out.rletter('z').rletter('W');
        EQ("rletter zW", BUFFER_OVERFLOW, out.getError());
        EQ("rletter zW", "aBcD0123x", buffer);
        EQ("rletter zW", 0, out.size());
    }

    out.reset(buffer, sizeof(buffer));
    out.over(lower).over(upper);
    EQ("over", sizeof(buffer) - 1, out.size());
    EQ("over", sizeof(buffer) - 1, lower.size());
    EQ("over", sizeof(buffer) - 1, upper.size());

    // rtext(const char *)
    {
        out.rtext("aB").over(lower).rtext("cD").over(upper).rtext("eF").over(out).rtext("gH");
        EQ("rtext alpha", "aBcDeFgH", buffer);
        EQ("rtext alpha", sizeof(buffer) - 5, lower.size());
        EQ("rtext alpha", sizeof(buffer) - 7, upper.size());
        EQ("rtext alpha", sizeof(buffer) - 9, out.size());

        out.rtext("xY");
        EQ("rtext xY", BUFFER_OVERFLOW, out.getError());
        EQ("rtext xY", "aBcDeFgHx", buffer);
        EQ("rtext alpha", sizeof(buffer) - 10, out.size());

        out.rtext("zW");
        EQ("rtext zW", BUFFER_OVERFLOW, out.getError());
        EQ("rtext zW", "aBcDeFgHx", buffer);
    }

    out.reset(buffer, sizeof(buffer));
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

        EQ("rtext alpha", "aBcDeFgH", buffer);
        EQ("rtext alpha", sizeof(buffer) - 5, lower.size());
        EQ("rtext alpha", sizeof(buffer) - 7, upper.size());
        EQ("rtext alpha", sizeof(buffer) - 9, out.size());

        out.rtext_P(xY);
        EQ("rtext xY", BUFFER_OVERFLOW, out.getError());
        EQ("rtext xY", "aBcDeFgHx", buffer);
        EQ("rtext xY", sizeof(buffer) - 10, out.size());

        out.rtext_P(zW);
        EQ("rtext zW", BUFFER_OVERFLOW, out.getError());
        EQ("rtext zW", "aBcDeFgHx", buffer);
    }

    out.reset(buffer, sizeof(buffer));
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
        EQ("rtext alpha", "aBcDeFgH", buffer);
        EQ("rtext alpha", sizeof(buffer) - 5, lower.size());
        EQ("rtext alpha", sizeof(buffer) - 7, upper.size());
        EQ("rtext alpha", sizeof(buffer) - 9, out.size());

        out.rtext(xY);
        EQ("rtext xY", BUFFER_OVERFLOW, out.getError());
        EQ("rtext xY", "aBcDeFgHx", buffer);
        EQ("rtext xY", sizeof(buffer) - 10, out.size());

        out.rtext(zW);
        EQ("rtext zW", BUFFER_OVERFLOW, out.getError());
        EQ("rtext zW", "aBcDeFgHx", buffer);
    }
}

void test_comma() {
    char buffer[8];
    StrBuffer out(buffer, sizeof(buffer));
    LowercaseBuffer lower(out);
    UppercaseBuffer upper(out);

    out.comma().over(lower).comma().over(upper).comma().over(out);
    EQ("comma", OK, out.getError());
    EQ("comma", ", , , ", buffer);

    out.comma();
    EQ("over", BUFFER_OVERFLOW, out.getError());
    EQ("comma", ", , , ,", buffer);
}

void test_reverse() {
    char buffer[10];
    StrBuffer out(buffer, sizeof(buffer));

    out.text("abc");
    char *m = out.mark();
    out.text("123");
    EQ("before", "abc123", buffer);
    out.reverse(m);
    EQ("after", "abc321", buffer);

    out.text("xY");
    out.reverse(buffer);
    EQ("whole", "Yx123cba", buffer);

    out.reverse(buffer + 8);
    EQ("overwrap", OVERWRAP_PAGE, out.getError());
    EQ("error", "Yx123cba", buffer);
}

void test_reset() {
    char buffer[8];
    StrBuffer out(buffer, sizeof(buffer));

    EQ("reset", sizeof(buffer) - 1, out.size());
    out.text("ABCDEFxY");
    EQ("reset", BUFFER_OVERFLOW, out.getError());
    EQ("reset", 0, out.size());
    out.reset(buffer + 4, sizeof(buffer) - 4);
    EQ("reset", OK, out.getError());
    EQ("reset", sizeof(buffer) - 5, out.size());
    EQ("reset", "ABCD", buffer);
}

void run_tests() {
    RUN_TEST(test_letter);
    RUN_TEST(test_text);
    RUN_TEST(test_text_P);
    RUN_TEST(test_scanner);
    RUN_TEST(test_rtext);
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
