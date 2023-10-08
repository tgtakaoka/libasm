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

#include "str_scanner.h"

#include "test_src_helper.h"

#include <cctype>

namespace libasm {
namespace test {

void set_up() {}

void tear_down() {}

void test_empty() {
    StrScanner empty;
    const char *const str = empty.str();
    EQ("empty.size", 0, empty.size());
    EQ("empty.str", "", empty.str());
    EQ("*empty", 0, *empty);
    EQ("empty[]", 0, empty[4]);

    StrScanner next = empty++;
    EQ("next.size", 0, next.size());
    EQ("next.str", str, next.str());
    ++empty;
    EQ("++empty", 0, empty.size());
    EQ("++empty.str", str, empty.str());
    empty += 4;
    EQ("empty+=", 0, empty.size());
    EQ("empty+=.str", str, empty.str());

    const auto eos = [](char c) { return c == 0; };
    empty.trimStart(eos);
    EQ("empty.trimStart", 0, empty.size());
    EQ("empty.trimStart.str", str, empty.str());
    empty.trimEnd(eos);
    EQ("empty.trimEnd", 0, empty.size());
    EQ("empty.expect(eos)", 0, empty.expect(eos));
    EQ("empty.expect(eos)", 0, empty.size());
    EQ("empty.expect(eos).str", str, empty.str());
}

void test_constructor() {
    const char text1[] = "text1";
    StrScanner scan1(text1);
    EQ("scan1.size", 5, scan1.size());
    EQ("scan1.str", text1, scan1.str());

    StrScanner scan2(scan1);
    EQ("scan2.size", scan1.size(), scan2.size());
    EQ("scan2.str", text1, scan2.str());

    const char text3[] = "text3";
    StrScanner scan3(text3, text3 + 4);
    EQ("scan3.size", 4, scan3.size());
    EQ("scan3.str", text3, scan3.str());

    StrScanner scan4 = scan3;
    EQ("scan4.size", scan3.size(), scan4.size());
    EQ("scan4.str", text3, scan4.str());
}

void test_reference() {
    const char text1[] = "text1";
    StrScanner scan1(text1);

    EQ("*scan1", 't', *scan1);
    EQ("scan1[1]", 'e', scan1[1]);
    EQ("scan1[4]", '1', scan1[4]);
    EQ("scan1[5]", 0, scan1[5]);

    EQ("*++scan1", 'e', *++scan1);
    EQ("++scan1.str", "ext1", scan1.str());
    EQ("*++scan1", 'x', *++scan1);
    EQ("*++scan1", 't', *++scan1);
    EQ("*--scan1", 'x', *--scan1);
    EQ("--scan1.str", "xt1", scan1.str());
    EQ("scan[-1]", 'e', scan1[-1]);
    EQ("*(scan1+=2)", '1', *(scan1 += 2));
    EQ("*++scan1", 0, *++scan1);
    EQ("*++scan1", 0, *++scan1);

    const char text2[] = "text2";
    StrScanner scan2(text2);
    EQ("*scan2++", 't', *scan2++);
    EQ("scan2++.str", "ext2", scan2.str());
    EQ("*scan2--", 'e', *scan2--);
    EQ("scan2--.str", "text2", scan2.str());
    EQ("*(scan2+=5)", 0, *(scan2 += 5));
    EQ("*scan2++", 0, *scan2++);
    EQ("scan2++.str", "", scan2);
    EQ("*(scan2+=-5)", 't', *(scan2 += -5));
    EQ("scan2+=-5.str", text2, scan2.str());
}

void test_trim() {
    const char text1[] = "  texT123abCDef";
    StrScanner scan1(text1);

    EQ("skipSpaces", &text1[2], scan1.skipSpaces());
    EQ("trimStart", &text1[5], scan1.trimStart(islower).str());
    EQ("trimStart.size", 10, scan1.size());
    EQ("trimEnd", &text1[5], scan1.trimEnd(isxdigit).str());
    EQ("trimEnd.size", 1, scan1.size());
}

void test_take() {
    const char text1[] = "  text1";
    StrScanner scan1(text1);
    auto cont = scan1.takeWhile(isspace);
    EQ("scan1", text1, scan1.str());
    EQ("scan1", 2, scan1.size());
    EQ("scan1", "text1", cont.str());
    EQ("scan1", 5, cont.size());

    const char text2[] = "text123ab_cdf";
    StrScanner scan2(text2);
    cont = scan2.takeWhile(isalpha);
    EQ("scan2", text2, scan2.str());
    EQ("scan2", 4, scan2.size());
    EQ("scan2", "123ab_cdf", cont.str());
    EQ("scan2", 9, cont.size());

    const char text3[] = "text123ab_cdf, x";
    StrScanner scan3(text3);
    cont = scan3.takeWhile([](char c) { return isalnum(c) || c == '_'; });
    EQ("scan3", text3, scan3.str());
    EQ("scan3", 13, scan3.size());
    EQ("scan3", ", x", cont.str());
    EQ("scan3", 3, cont.size());
}

void test_expect() {
    const char text2[] = " \tteXT21:   ";
    StrScanner scan3(text2);
    EQ("scan3.size", 12, scan3.size());
    EQ("expect", ' ', scan3.expect(' '));
    EQ("scan3.str", &text2[1], scan3.str());
    EQ("scan3.size", 11, scan3.size());
    EQ("expect", '\t', scan3.expect(isspace));
    EQ("scan3.str", &text2[2], scan3.str());
    EQ("scan3.size", 10, scan3.size());
    EQ("scan3.iexpect", 't', scan3.iexpect('T'));
    EQ("scan3.iexpect", 'e', scan3.iexpect('e'));
    EQ("scan3.iexpect", 'X', scan3.iexpect('X'));
    EQ("scan3.iexpect", 'T', scan3.iexpect('t'));
    EQ("scan3.expect", 0, scan3.expect(isalpha));
    EQ("scan3.expect", '2', scan3.expect(isdigit));
    EQ("scan3.iexpect", '1', scan3.expect('1'));
    EQ("scan3.expect", 0, scan3.expect([](char c) { return c != ':'; }));
    EQ("scan3.expect", ':', scan3.expect(':'));
    EQ("scan3.skipSpaces.str", "", scan3.skipSpaces().str());
}

void test_expectText() {
    StrScanner scan1("text1");
    TRUE("scan1", scan1.iexpectText_P(PSTR("TEXT")));
    EQ("scan1", "1", scan1.str());

    StrScanner scan2("text2");
    TRUE("scan2", scan2.iexpectText_P(PSTR("TEXT1"), 4));
    EQ("scan2", "2", scan2.str());

    StrScanner scan3("text3");
    FALSE("scan3", scan3.iexpectText_P(PSTR("TEXT345")));
    EQ("scan3", "text3", scan3.str());
}

void test_expectWord() {
    StrScanner scan1("text1.text2");
    TRUE("scan1", scan1.iexpectWord_P(PSTR("TEXT1")));
    EQ("scan1", ".text2", scan1.str());

    StrScanner scan3("text2");
    TRUE("scan3", scan3.iexpectWord_P(PSTR("TEXT2")));
    EQ("scan3", "", scan3.str());

    StrScanner scan2("text345");
    FALSE("scan2", scan2.iexpectWord_P(PSTR("TEXT3")));
    EQ("scan2", "text345", scan2.str());

    StrScanner scan4("text4text5");
    FALSE("scan4", scan4.iexpectWord_P(PSTR("TEXT4")));
    EQ("scan4", "text4text5", scan4.str());

    StrScanner scan5("text5_text6");
    FALSE("scan5", scan5.iexpectWord_P(PSTR("TEXT3")));
    EQ("scan5", "text5_text6", scan5.str());
}

void test_iequals() {
    const StrScanner textA("textA");
    const StrScanner scanA("textA");
    const StrScanner scanB("textB");

    TRUE("textA == scanA", textA.iequals(scanA));
    TRUE("scanA == textA", scanA.iequals(textA));
    FALSE("textA != scanB", textA.iequals(scanB));
    FALSE("scanB != textA", scanB.iequals(textA));
}

void run_tests() {
    RUN_TEST(test_empty);
    RUN_TEST(test_constructor);
    RUN_TEST(test_reference);
    RUN_TEST(test_trim);
    RUN_TEST(test_take);
    RUN_TEST(test_expect);
    RUN_TEST(test_expectText);
    RUN_TEST(test_expectWord);
    RUN_TEST(test_iequals);
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
