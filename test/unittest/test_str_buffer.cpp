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
    char buffer[8];
    StrBuffer buf(buffer, sizeof(buffer));

    EQ("empty", "", buffer);

    buf.letter('a').letter('B', false).letter('c', true);
    EQ("letter alpha", "abC", buffer);

    buf.letter('0', true).letter('1', false).letter('2');
    EQ("letter digit", "abC012", buffer);

    buf.letter('X');
    EQ("letter 7", OK, buf.getError());
    EQ("letter X", "abC012X", buffer);

    buf.letter('Y');
    EQ("letter 8", BUFFER_OVERFLOW, buf.getError());
    EQ("letter XY", "abC012X", buffer);

    buf.letter('Z');
    EQ("letter 9", BUFFER_OVERFLOW, buf.getError());
    EQ("letter XYZ", "abC012X", buffer);
}

void test_text() {
    char buffer[10];
    StrBuffer buf(buffer, sizeof(buffer));

    buf.text("ab").text("cD", false).text("eF", true);
    EQ("text alpha", "abcdEF", buffer);

    buf.text("XY");
    EQ("text XY", OK, buf.getError());
    EQ("text XY", "abcdEFXY", buffer);

    buf.text("ZW");
    EQ("text ZW", BUFFER_OVERFLOW, buf.getError());
    EQ("text ZW", "abcdEFXYZ", buffer);
}

void test_text_P() {
    char buffer[10];
    StrBuffer buf(buffer, sizeof(buffer));

    buf.text(PSTR("ab")).text(PSTR("cD"), false).text(PSTR("eF"), true);
    EQ("text alpha", "abcdEF", buffer);

    static const char XY[] PROGMEM = "XY";
    buf.text(XY);
    EQ("text XY", OK, buf.getError());
    EQ("text XY", "abcdEFXY", buffer);

    static const char ZW[] PROGMEM = "ZW";
    buf.text(ZW);
    EQ("text ZW", BUFFER_OVERFLOW, buf.getError());
    EQ("text ZW", "abcdEFXYZ", buffer);
}

void test_scanner() {
    static const char text[] = "abcDeFXYZW";
    const StrScanner ab(text + 0, text + 2);
    const StrScanner cD(text + 2, text + 4);
    const StrScanner eF(text + 4, text + 6);
    const StrScanner XY(text + 6, text + 8);
    const StrScanner ZW(text + 8);

    char buffer[10];
    StrBuffer buf(buffer, sizeof(buffer));

    buf.text(ab).text(cD, false).text(eF, true);
    EQ("text alpha", "abcdEF", buffer);

    buf.text(XY);
    EQ("text XY", OK, buf.getError());
    EQ("text XY", "abcdEFXY", buffer);

    buf.text(ZW);
    EQ("text ZW", BUFFER_OVERFLOW, buf.getError());
    EQ("text ZW", "abcdEFXYZ", buffer);
}

void test_comma() {
    char buffer[8];
    StrBuffer buf(buffer, sizeof(buffer));

    buf.comma().comma().comma();
    EQ("comma", OK, buf.getError());
    EQ("comma", ", , , ", buffer);

    buf.comma();
    EQ("over", BUFFER_OVERFLOW, buf.getError());
    EQ("comma", ", , , ,", buffer);
}

void test_reverse() {
    char buffer[10];
    StrBuffer buf(buffer, sizeof(buffer));

    buf.text("abc");
    char *m = buf.mark();
    buf.text("123");
    EQ("before", "abc123", buffer);
    buf.reverse(m);
    EQ("after", "abc321", buffer);

    buf.text("XY");
    buf.reverse(buffer);
    EQ("whole", "YX123cba", buffer);

    buf.reverse(buffer + 8);
    EQ("overwrap", OVERWRAP_PAGE, buf.getError());
    EQ("error", "YX123cba", buffer);
}

void test_reset() {
    char buffer[8];
    StrBuffer buf(buffer, sizeof(buffer));

    buf.text("ABCDEFXY");
    EQ("reset", BUFFER_OVERFLOW, buf.getError());
    buf.reset(buffer + 4, sizeof(buffer) - 4);
    EQ("reset", OK, buf.getError());
    EQ("reset", "ABCD", buffer);
}

void run_tests() {
    RUN_TEST(test_letter);
    RUN_TEST(test_text);
    RUN_TEST(test_text_P);
    RUN_TEST(test_scanner);
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
