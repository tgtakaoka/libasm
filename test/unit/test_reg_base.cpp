/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "reg_base.h"
#include "test_unit_helper.h"

using namespace libasm::reg;

namespace libasm {
namespace test {

#define NON_NAME(msg, name)                               \
    do {                                                  \
        const auto *actual = TABLE.searchName(name);      \
        asserter.isNull(__FILE__, __LINE__, msg, actual); \
    } while (0);

#define HAS_NAME(msg, name, expected)                                             \
    do {                                                                          \
        const auto *actual = TABLE.searchName(name);                              \
        asserter.isNotNull(__FILE__, __LINE__, msg, actual);                      \
        if (actual)                                                               \
            asserter.equals(__FILE__, __LINE__, msg, expected, actual->text_P()); \
    } while (0);

#define NON_TEXT(msg, line)                                         \
    do {                                                            \
        StrScanner scan{line};                                      \
        const auto *actual = TABLE.searchText(scan);                \
        asserter.isNull(__FILE__, __LINE__, msg, actual);           \
        asserter.equals(__FILE__, __LINE__, msg, line, scan.str()); \
    } while (0);

#define HAS_TEXT(msg, line, expected, expectedNext)                             \
    do {                                                                        \
        StrScanner scan(line);                                                  \
        const auto *actual = TABLE.searchText(scan);                            \
        asserter.isNotNull(__FILE__, __LINE__, msg, actual);                    \
        if (actual)                                                             \
            asserter.equals(__FILE__, __LINE__, msg, expected, actual->name()); \
        asserter.equals(__FILE__, __LINE__, msg, expectedNext, scan.str());     \
    } while (0);

void set_up() {}

void tear_down() {}

namespace {
constexpr NameEntry EMPTY_ENTRIES[] PROGMEM = {};
}

// clang-format off
void test_empty() {
    static PROGMEM constexpr NameTable TABLE{ARRAY_RANGE(EMPTY_ENTRIES)};

    NON_NAME("   0",    0);
    NON_NAME(" 127",  127);
    NON_NAME("  -1",   -1);
    NON_NAME("-128", -128);

    NON_TEXT("(empty)", "");
    NON_TEXT("text",    "text");
}
// clang-format on

namespace {
// clang-format off

constexpr char TEXT_REG_A[]   PROGMEM = "A";
constexpr char TEXT_REG_AF[]  PROGMEM = "AF";
constexpr char TEXT_REG_AFP[] PROGMEM = "AF'";
constexpr char TEXT_REG_CCR[] PROGMEM = "CCR";
constexpr char TEXT_REG_D0[]  PROGMEM = "D0";
constexpr char TEXT_REG_D1[]  PROGMEM = "D1";
constexpr char TEXT_REG_D2[]  PROGMEM = "D2";
constexpr char TEXT_REG_D3[]  PROGMEM = "D3";
constexpr char TEXT_REG_D4[]  PROGMEM = "D4";
constexpr char TEXT_REG_D5[]  PROGMEM = "D5";
constexpr char TEXT_REG_D6[]  PROGMEM = "D6";
constexpr char TEXT_REG_D7[]  PROGMEM = "D7";
constexpr char TEXT_REG_DR0[] PROGMEM = "DR0";
constexpr char TEXT_REG_P[]   PROGMEM = "P";
constexpr char TEXT_REG_P1[]  PROGMEM = "P1";
constexpr char TEXT_REG_P2[]  PROGMEM = "P2";
constexpr char TEXT_REG_SP[]  PROGMEM = "SP";

enum : int8_t {
    REG_A = 1,  // 1
    REG_AF,     // 2
    REG_AFP,    // 3
    REG_CCR,    // 4
    REG_D0,     // 5
    REG_D1,     // 6
    REG_D2,     // 7
    REG_D3,     // 8
    REG_D4,     // 9
    REG_D5,     // 10
    REG_D6,     // 11
    REG_D7,     // 12
    REG_DR0,    // 13
    REG_P,      // 14
    REG_P1,     // 15
    REG_P2,     // 16
    REG_SP,     // 17
};

constexpr NameEntry ENTRIES[] PROGMEM = {
    { TEXT_REG_A,   REG_A   },
    { TEXT_REG_AF,  REG_AF  },
    { TEXT_REG_AFP, REG_AFP },
    { TEXT_REG_CCR, REG_CCR },
    { TEXT_REG_D0,  REG_D0  },
    { TEXT_REG_D1,  REG_D1  },
    { TEXT_REG_D2,  REG_D2  },
    { TEXT_REG_D3,  REG_D3  },
    { TEXT_REG_D4,  REG_D4  },
    { TEXT_REG_D5,  REG_D5  },
    { TEXT_REG_D6,  REG_D6  },
    { TEXT_REG_D7,  REG_D7  },
    { TEXT_REG_DR0, REG_DR0 },
    { TEXT_REG_P,   REG_P   },
    { TEXT_REG_P1,  REG_P1  },
    { TEXT_REG_P2,  REG_P2  },
    { TEXT_REG_SP,  REG_SP  },
};

// clang-format on
}  // namespace

// clang-format off
void test_searchName() {
    static PROGMEM constexpr NameTable TABLE{ARRAY_RANGE(ENTRIES)};

    HAS_NAME("searchName A",   REG_A,   "A");
    HAS_NAME("searchName AF",  REG_AF,  "AF");
    HAS_NAME("searchName AF'", REG_AFP, "AF'");
    HAS_NAME("searchName CCR", REG_CCR, "CCR");
    HAS_NAME("searchName D0",  REG_D0,  "D0");
    HAS_NAME("searchName D1",  REG_D1,  "D1");
    HAS_NAME("searchName D2",  REG_D2,  "D2");
    HAS_NAME("searchName D3",  REG_D3,  "D3");
    HAS_NAME("searchName D4",  REG_D4,  "D4");
    HAS_NAME("searchName D5",  REG_D5,  "D5");
    HAS_NAME("searchName D6",  REG_D6,  "D6");
    HAS_NAME("searchName D7",  REG_D7,  "D7");
    HAS_NAME("searchName DR0", REG_DR0, "DR0");
    HAS_NAME("searchName P",   REG_P,   "P");
    HAS_NAME("searchName P1",  REG_P1,  "P1");
    HAS_NAME("searchName P2",  REG_P2,  "P2");
    HAS_NAME("searchName SP",  REG_SP,  "SP");

    NON_NAME("searchName   0" ,  0);
    NON_NAME("searchName 100", 100);
    NON_NAME("searchName 127", 127);
    NON_NAME("searchName  -1",  -1);
}

void test_searchText() {
    static PROGMEM constexpr NameTable TABLE{ARRAY_RANGE(ENTRIES)};

    HAS_TEXT("searchText A",   "A ",   REG_A,   " ");
    NON_TEXT("searchText AB",  "AB,X");
    HAS_TEXT("searchText af",  "af,",  REG_AF,  ",");
    HAS_TEXT("searchText AF'", "AF';", REG_AFP, ";");
    HAS_TEXT("searchText A,F", "A,F",  REG_A,   ",F");
    NON_TEXT("searchText A'F", "A'F");


    NON_TEXT("searchText C",   "C  ; comment");
    NON_TEXT("searchText CC",  "CC ; comment");
    HAS_TEXT("searchText CCR", "CCR; comment", REG_CCR, "; comment");

    NON_TEXT("searchText D",   "D,X");
    HAS_TEXT("searchText D0",  "D0,P",  REG_D0, ",P");
    HAS_TEXT("searchText D7",  "D7,D0", REG_D7, ",D0");
    NON_TEXT("searchText D8",  "D8,X");
    NON_TEXT("searchText D01", "D01");

    HAS_TEXT("searchText DR0", "DR0,SP", REG_DR0, ",SP");
    NON_TEXT("searchText DR1", "DR1,PC");

    HAS_TEXT("searchText P",   "P,P2", REG_P,  ",P2");
    HAS_TEXT("searchText P1",  "P1,X", REG_P1, ",X");
    HAS_TEXT("searchText P2",  "P2,Y", REG_P2, ",Y");
    NON_TEXT("searchText P3",  "P3,Z");
    NON_TEXT("searchText P12", "P12");
    NON_TEXT("searchText P2A", "P2A");
}
// clang-format on

void run_tests() {
    RUN_TEST(test_empty);
    RUN_TEST(test_searchName);
    RUN_TEST(test_searchText);
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
