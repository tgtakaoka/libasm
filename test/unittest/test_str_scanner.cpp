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

#include <ctype.h>

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

    const char text2[] = " \tteXT21:   ";
    StrScanner scan2(text2);
    EQ("scan2.size", 12, scan2.size());
    EQ("expect", ' ', scan2.expect(' '));
    EQ("scan2.str", &text2[1], scan2.str());
    EQ("scan2.size", 11, scan2.size());
    EQ("expect", '\t', scan2.expect(isspace));
    EQ("scan2.str", &text2[2], scan2.str());
    EQ("scan2.size", 10, scan2.size());
    EQ("scan2.iexpect", 't', scan2.iexpect('T'));
    EQ("scan2.iexpect", 'e', scan2.iexpect('e'));
    EQ("scan2.iexpect", 'X', scan2.iexpect('X'));
    EQ("scan2.iexpect", 'T', scan2.iexpect('t'));
    EQ("scan2.expect", 0, scan2.expect(isalpha));
    EQ("scan2.expect", '2', scan2.expect(isdigit));
    EQ("scan2.iexpect", '1', scan2.expect('1'));
    EQ("scan2.expect", 0, scan2.expect([](char c) { return c != ':'; }));
    EQ("scan2.expect", ':', scan2.expect(':'));
    EQ("scan2.skipSpaces.str", "", scan2.skipSpaces().str());
}

void test_compare() {
    const char text1[] = "text1";
    StrScanner scan1(text1);
    const char text2[] = "text2";
    StrScanner scan2(text2);

    TRUE("iequals_P", scan1.iequals_P(PSTR("TEXT1")));
    FALSE("iequals_P", scan2.iequals_P(PSTR("TEXT1")));

    TRUE("istarts_P", scan1.istarts_P(PSTR("TEXT")));
    TRUE("istarts_P", scan2.istarts_P(PSTR("TEXT1"), 4));
    FALSE("istarts_P", scan2.istarts_P(PSTR("TEXT123"), 5));
}

void run_tests() {
    RUN_TEST(test_empty);
    RUN_TEST(test_constructor);
    RUN_TEST(test_reference);
    RUN_TEST(test_trim);
    RUN_TEST(test_compare);
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
