/*
 * Copyright 2020 Tadashi G. Takaoka
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

#include "test_expr_helper.h"

using namespace libasm;
using namespace libasm::test;

const IntelNumberParser number;
const SemicolonCommentParser comment;
const SimpleSymbolParser symbol{SymbolParser::ATMARK_QUESTION, SymbolParser::NONE};
const DefaultLetterParser letter;
const DollarLocationParser location;
struct : ValueParser::Locator {
    uint32_t location = 0;
    uint32_t currentLocation() const { return location; }
} locator;
const ValueParser parser{number, comment, symbol, letter, location, locator};

const SuffixHexFormatter hexFormatter{'h'};
const ValueFormatter formatter{hexFormatter};

static void set_up() {}

static void tear_down() {
    symtab.reset();
}

// clang-format off
static void test_char_constant() {
    E8("'a'",   0x61);
    E8("'a'+5", 0x66);
    E8("5+'a'", 0x66);
    X8("'a",    MISSING_CLOSING_QUOTE, "'a");
    X8("'a+5",  MISSING_CLOSING_QUOTE, "'a+5");
    X8("5+'a",  MISSING_CLOSING_QUOTE, "'a");
    X8("' ",    MISSING_CLOSING_QUOTE, "' ");
    X8("''",    MISSING_CLOSING_QUOTE, "''");
    X8("'''",   MISSING_CLOSING_QUOTE, "'''");
    E8("''''",  0x27);

    E16("'a'", 0x61);
    E32("'a'", 0x61);
}

static void test_dec_constant() {
    E8("0",   0x00);
    E8("127", 0x7f);
    E8("128", 0x80);
    E8("255", 0xff);
    X8("256", OVERFLOW_RANGE, "256");

    E16("0D",     0x0000);
    E16("32767D", 0x7fff);
    E16("32768d", 0x8000);
    E16("65535D", 0xffff);
    X16("65536D", OVERFLOW_RANGE, "65536D");

    E32("0d",          0x00000000);
    E32("2147483647D", 0x7fffffff);
    E32("2147483648",  0x80000000);
    E32("4294967295D", 0xffffffff);
    X32("4294967296",  OVERFLOW_RANGE, "4294967296");
    X32("9999999999D", OVERFLOW_RANGE, "9999999999D");
}

static void test_hex_constant() {
    E8("0H",   0x00);
    E8("7fH",  0x7f);
    E8("80H",  0x80);
    E8("0FFH", 0xff);
    X8("100H", OVERFLOW_RANGE, "100H");

    E16("0H",     0x0000);
    E16("7FFFH",  0x7fff);
    E16("8000H",  0x8000);
    E16("0ffffh", 0xffff);
    X16("10000H", OVERFLOW_RANGE, "10000H");

    E32("0H",         0x00000000);
    E32("7FFFFFFFH",  0x7fffffff);
    E32("80000000H",  0x80000000);
    E32("0FFFFffffH", 0xffffffff);
    X32("100000000H", OVERFLOW_RANGE, "100000000H");

    E32("0x0",         0x00000000);
    E32("0x7FFFFFFF",  0x7fffffff);
    E32("0x80000000",  0x80000000);
    E32("0xFFFFffff",  0xffffffff);
    X32("0x100000000", OVERFLOW_RANGE, "0x100000000");
}

static void test_oct_constant() {
    E8("0O",   0x00);
    E8("177O", 0x7f);
    E8("200Q", 0x80);
    E8("377O", 0xff);
    X8("400Q", OVERFLOW_RANGE, "400Q");

    E16("0Q",      0x0000);
    E16("77777O",  0x7fff);
    E16("100000o", 0x8000);
    E16("177777q", 0xffff);
    X16("200000O", OVERFLOW_RANGE, "200000O");

    E32("0O",           0x00000000);
    E32("17777777777O", 0x7fffffff);
    E32("20000000000o", 0x80000000);
    E32("37777777777O", 0xffffffff);
    X32("40000000000Q", OVERFLOW_RANGE, "40000000000Q");

    E32("00",           0x00000000);
    E32("017777777777", 0x7fffffff);
    E32("020000000000", 0x80000000);
    E32("037777777777", 0xffffffff);
    X32("040000000000", OVERFLOW_RANGE, "040000000000");
}

static void test_bin_constant() {
    E8("0B",         0x00);
    E8("01111111b",  0x7f);
    E8("10000000B",  0x80);
    E8("11111111B",  0xff);
    X8("100000000B", OVERFLOW_RANGE, "100000000B");

    E16("0B",                 0x0000);
    E16("0111111111111111B",  0x7fff);
    E16("1000000000000000B",  0x8000);
    E16("1111111111111111b",  0xffff);
    X16("10000000000000000B", OVERFLOW_RANGE, "10000000000000000B");

    E32("0B",                                 0x00000000);
    E32("01111111111111111111111111111111B",  0x7fffffff);
    E32("10000000000000000000000000000000B",  0x80000000);
    E32("11111111111111111111111111111111B",  0xffffffff);
    X32("100000000000000000000000000000000b", OVERFLOW_RANGE, "100000000000000000000000000000000b");

    E32("0b0",                                 0x00000000);
    E32("0b01111111111111111111111111111111",  0x7fffffff);
    E32("0b10000000000000000000000000000000",  0x80000000);
    E32("0b11111111111111111111111111111111",  0xffffffff);
    X32("0b100000000000000000000000000000000", OVERFLOW_RANGE, "0b100000000000000000000000000000000");
}

static void test_current_address() {
    locator.location = 0x1000;
    E16("$",        0x1000);
    E16("$+2",      0x1002);
    E16("$-2",      0x0FFE);
    X16("$+0F000H", OVERFLOW_RANGE, "$+0F000H");
    E16("$-1001H",  0xFFFF);
    E32("$+0F000H", 0x00010000);
    E32("$-1001H",  0xFFFFFFFF);

    symtab.intern(0x1000, "table");
    locator.location = 0x1100;
    E16("$-table",     0x100);
    E16("($-table)/2", 0x080);
}

static void test_scan() {
    SCAN('|', "|a+b|c+d",     "");
    SCAN('|', "a+b|c+d",      "a+b");
    SCAN('|', "a+(b|c)+d|e",  "a+(b|c)+d");
    SCAN('|', "a+'|'+d|e",    "a+'|'+d");
    SCAN(',', "|a+b|c+d",     "");
    SCAN(',', "','+'''',abc", "','+''''");
    SCAN('x', "0x1230xG",     "0x1230");
    SCAN('b', "0b1010b0",     "0b1010");
    SCAN('H', "1AB0HHX",      "1AB0H");
    SCAN('O', "1230OOX",      "1230O");
    SCAN('O', "1239OOX",      "1239");
    SCAN('B', "1010BBX",      "1010B");
    SCAN('B', "1012BBX",      "1012");
}

static void test_errors() {
    X32("0ABCGH", ILLEGAL_CONSTANT, "0ABCGH");
    X32("01778O", GARBAGE_AT_END, "8O");
    X32("01012B", GARBAGE_AT_END, "B");
    X32("56789A", GARBAGE_AT_END, "A");
}

static void test_formatter_8bit() {
    DEC(0,    8, "0");
    DEC(32,   8, "32");
    DEC(128,  8, "128");
    DEC(255,  8, "255");
    DEC(256,  8, "0");
    DEC(-32,  8, "224");
    DEC(-128, 8, "128");
    DEC(-255, 8, "1");
    DEC(-256, 8, "0");

    DEC(0,    -8, "0");
    DEC(32,   -8, "32");
    DEC(128,  -8, "-128");
    DEC(255,  -8, "-1");
    DEC(256,  -8, "0");
    DEC(-32,  -8, "-32");
    DEC(-128, -8, "-128");
    DEC(-255, -8, "1");
    DEC(-256, -8, "0");

    DEC(0,    8, "0");
    DEC(32,   8, "32");
    DEC(128,  8, "128");
    DEC(255,  8, "255");
    DEC(256,  8, "0");
    DEC(-32,  8, "224");
    DEC(-128, 8, "128");
    DEC(-255, 8, "1");
    DEC(-256, 8, "0");

    DEC(0,    -8, "0");
    DEC(32,   -8, "32");
    DEC(128,  -8, "-128");
    DEC(255,  -8, "-1");
    DEC(256,  -8, "0");
    DEC(-32,  -8, "-32");
    DEC(-128, -8, "-128");
    DEC(-255, -8, "1");
    DEC(-256, -8, "0");

    HEX(0,    8, "00h");
    HEX(32,   8, "20h");
    HEX(128,  8, "80h");
    HEX(255,  8, "0ffh");
    HEX(256,  8, "00h");
    HEX(-32,  8, "0e0h");
    HEX(-128, 8, "80h");
    HEX(-255, 8, "01h");
    HEX(-256, 8, "00h");

    HEX(0,    -8, "00h");
    HEX(32,   -8, "20h");
    HEX(128,  -8, "-80h");
    HEX(255,  -8, "-01h");
    HEX(256,  -8, "00h");
    HEX(-32,  -8, "-20h");
    HEX(-128, -8, "-80h");
    HEX(-255, -8, "01h");
    HEX(-256, -8, "00h");

    // Relax

    RHEX(0,    8, "0");
    RHEX(32,   8, "32");
    RHEX(33,   8, "21h");
    RHEX(128,  8, "80h");
    RHEX(255,  8, "0ffh");
    RHEX(256,  8, "0");
    RHEX(-32,  8, "0e0h");
    RHEX(-33,  8, "0dfh");
    RHEX(-128, 8, "80h");
    RHEX(-255, 8, "1");
    RHEX(-256, 8, "0");

    RHEX(0,    -8, "0");
    RHEX(32,   -8, "32");
    RHEX(33,   -8, "21h");
    RHEX(128,  -8, "-80h");
    RHEX(255,  -8, "-1");
    RHEX(256,  -8, "0");
    RHEX(-32,  -8, "-32");
    RHEX(-33,  -8, "-21h");
    RHEX(-128, -8, "-80h");
    RHEX(-255, -8, "1");
    RHEX(-256, -8, "0");
}

static void test_formatter_16bit() {
    DEC(0,        16, "0");
    DEC(32,       16, "32");
    DEC(0x8000,   16, "32768");
    DEC(0xffff,   16, "65535");
    DEC(0x10000,  16, "0");
    DEC(-32,      16, "65504");
    DEC(-0x8000,  16, "32768");
    DEC(-0xffff,  16, "1");
    DEC(-0x10000, 16, "0");

    DEC(0,        -16, "0");
    DEC(32,       -16, "32");
    DEC(0x8000,   -16, "-32768");
    DEC(0xffff,   -16, "-1");
    DEC(0x10000,  -16, "0");
    DEC(-32,      -16, "-32");
    DEC(-0x8000,  -16, "-32768");
    DEC(-0xffff,  -16, "1");
    DEC(-0x10000, -16, "0");

    DEC(0,        16, "0");
    DEC(32,       16, "32");
    DEC(0x8000,   16, "32768");
    DEC(0xffff,   16, "65535");
    DEC(0x10000,  16, "0");
    DEC(-32,      16, "65504");
    DEC(-0x8000,  16, "32768");
    DEC(-0xffff,  16, "1");
    DEC(-0x10000, 16, "0");

    DEC(0,        -16, "0");
    DEC(32,       -16, "32");
    DEC(0x8000,   -16, "-32768");
    DEC(0xffff,   -16, "-1");
    DEC(0x10000,  -16, "0");
    DEC(-32,      -16, "-32");
    DEC(-0x8000,  -16, "-32768");
    DEC(-0xffff,  -16, "1");
    DEC(-0x10000, -16, "0");

    HEX(0,        16, "0000h");
    HEX(32,       16, "0020h");
    HEX(0x8000,   16, "8000h");
    HEX(0xffff,   16, "0ffffh");
    HEX(0x10000,  16, "0000h");
    HEX(-32,      16, "0ffe0h");
    HEX(-0x8000,  16, "8000h");
    HEX(-0xffff,  16, "0001h");
    HEX(-0x10000, 16, "0000h");

    HEX(0,        -16, "0000h");
    HEX(32,       -16, "0020h");
    HEX(0x8000,   -16, "-8000h");
    HEX(0xffff,   -16, "-0001h");
    HEX(0x10000,  -16, "0000h");
    HEX(-32,      -16, "-0020h");
    HEX(-0x8000,  -16, "-8000h");
    HEX(-0xffff,  -16, "0001h");
    HEX(-0x10000, -16, "0000h");

    // Relax

    RHEX(0,        16, "0");
    RHEX(32,       16, "32");
    RHEX(33,       16, "0021h");
    RHEX(0x8000,   16, "8000h");
    RHEX(0xffff,   16, "0ffffh");
    RHEX(0x10000,  16, "0");
    RHEX(-32,      16, "0ffe0h");
    RHEX(-33,      16, "0ffdfh");
    RHEX(-0x8000,  16, "8000h");
    RHEX(-0xffff,  16, "1");
    RHEX(-0x10000, 16, "0");

    RHEX(0,        -16, "0");
    RHEX(32,       -16, "32");
    RHEX(33,       -16, "0021h");
    RHEX(0x8000,   -16, "-8000h");
    RHEX(0xffff,   -16, "-1");
    RHEX(0x10000,  -16, "0");
    RHEX(-32,      -16, "-32");
    RHEX(-33,      -16, "-0021h");
    RHEX(-0x8000,  -16, "-8000h");
    RHEX(-0xffff,  -16, "1");
    RHEX(-0x10000, -16, "0");
}

static void test_formatter_24bit() {
    DEC(0,           24, "0");
    DEC(32,          24, "32");
    DEC(0x800000,    24, "8388608");
    DEC(0xffffff,    24, "16777215");
    DEC(0x1000000,   24, "0");
    DEC(-32,         24, "16777184");
    DEC(-0x800000,   24, "8388608");
    DEC(-0xffffff,   24, "1");
    DEC(-0x1000000,  24, "0");

    DEC(0,          -24, "0");
    DEC(32,         -24, "32");
    DEC(0x800000,   -24, "-8388608");
    DEC(0xffffff,   -24, "-1");
    DEC(0x1000000,  -24, "0");
    DEC(-32,        -24, "-32");
    DEC(-0x800000,  -24, "-8388608");
    DEC(-0xffffff,  -24, "1");
    DEC(-0x1000000, -24, "0");

    DEC(0,           24, "0");
    DEC(32,          24, "32");
    DEC(0x800000,    24, "8388608");
    DEC(0xffffff,    24, "16777215");
    DEC(0x1000000,   24, "0");
    DEC(-32,         24, "16777184");
    DEC(-0x800000,   24, "8388608");
    DEC(-0xffffff,   24, "1");
    DEC(-0x1000000,  24, "0");

    DEC(0,          -24, "0");
    DEC(32,         -24, "32");
    DEC(0x800000,   -24, "-8388608");
    DEC(0xffffff,   -24, "-1");
    DEC(0x1000000,  -24, "0");
    DEC(-32,        -24, "-32");
    DEC(-0x800000,  -24, "-8388608");
    DEC(-0xffffff,  -24, "1");
    DEC(-0x1000000, -24, "0");

    HEX(0,           24, "000000h");
    HEX(32,          24, "000020h");
    HEX(0x800000,    24, "800000h");
    HEX(0xffffff,    24, "0ffffffh");
    HEX(0x1000000,   24, "000000h");
    HEX(-32,         24, "0ffffe0h");
    HEX(-0x800000,   24, "800000h");
    HEX(-0xffffff,   24, "000001h");
    HEX(-0x1000000,  24, "000000h");

    HEX(0,          -24, "000000h");
    HEX(32,         -24, "000020h");
    HEX(0x800000,   -24, "-800000h");
    HEX(0xffffff,   -24, "-000001h");
    HEX(0x1000000,  -24, "000000h");
    HEX(-32,        -24, "-000020h");
    HEX(-0x800000,  -24, "-800000h");
    HEX(-0xffffff,  -24, "000001h");
    HEX(-0x1000000, -24, "000000h");

    // Relax

    RHEX(0,           24, "0");
    RHEX(32,          24, "32");
    RHEX(33,          24, "000021h");
    RHEX(0x800000,    24, "800000h");
    RHEX(0xffffff,    24, "0ffffffh");
    RHEX(0x1000000,   24, "0");
    RHEX(-32,         24, "0ffffe0h");
    RHEX(-33,         24, "0ffffdfh");
    RHEX(-0x800000,   24, "800000h");
    RHEX(-0xffffff,   24, "1");
    RHEX(-0x1000000,  24, "0");

    RHEX(0,          -24, "0");
    RHEX(32,         -24, "32");
    RHEX(33,         -24, "000021h");
    RHEX(0x800000,   -24, "-800000h");
    RHEX(0xffffff,   -24, "-1");
    RHEX(0x1000000,  -24, "0");
    RHEX(-32,        -24, "-32");
    RHEX(-33,        -24, "-000021h");
    RHEX(-0x800000,  -24, "-800000h");
    RHEX(-0xffffff,  -24, "1");
    RHEX(-0x1000000, -24, "0");
}

static void test_formatter_32bit() {
    DEC(0,            32, "0");
    DEC(32,           32, "32");
    DEC(0x80000000,   32, "2147483648");
    DEC(0xffffffff,   32, "4294967295");
    DEC(-32,          32, "4294967264");
    DEC(-0x80000000,  32, "2147483648");
    DEC(-0xffffffff,  32, "1");

    DEC(0,           -32, "0");
    DEC(32,          -32, "32");
    DEC(0x80000000,  -32, "-2147483648");
    DEC(0xffffffff,  -32, "-1");
    DEC(-32,         -32, "-32");
    DEC(-0x80000000, -32, "-2147483648");
    DEC(-0xffffffff, -32, "1");

    DEC(0,            32, "0");
    DEC(32,           32, "32");
    DEC(0x80000000,   32, "2147483648");
    DEC(0xffffffff,   32, "4294967295");
    DEC(-32,          32, "4294967264");
    DEC(-0x80000000,  32, "2147483648");
    DEC(-0xffffffff,  32, "1");

    DEC(0,           -32, "0");
    DEC(32,          -32, "32");
    DEC(0x80000000,  -32, "-2147483648");
    DEC(0xffffffff,  -32, "-1");
    DEC(-32,         -32, "-32");
    DEC(-0x80000000, -32, "-2147483648");
    DEC(-0xffffffff, -32, "1");

    HEX(0,           32, "00000000h");
    HEX(32,          32, "00000020h");
    HEX(33,          32, "00000021h");
    HEX(0x80000000,  32, "80000000h");
    HEX(0xffffffff,  32, "0ffffffffh");
    HEX(-32,         32, "0ffffffe0h");
    HEX(-33,         32, "0ffffffdfh");
    HEX(-0x80000000, 32, "80000000h");
    HEX(-0xffffffff, 32, "00000001h");

    HEX(0,           -32, "00000000h");
    HEX(32,          -32, "00000020h");
    HEX(33,          -32, "00000021h");
    HEX(0x80000000,  -32, "-80000000h");
    HEX(0xffffffff,  -32, "-00000001h");
    HEX(-32,         -32, "-00000020h");
    HEX(-33,         -32, "-00000021h");
    HEX(-0x80000000, -32, "-80000000h");
    HEX(-0xffffffff, -32, "00000001h");

    // Relax

    RHEX(0,           32, "0");
    RHEX(32,          32, "32");
    RHEX(33,          32, "00000021h");
    RHEX(0x80000000,  32, "80000000h");
    RHEX(0xffffffff,  32, "0ffffffffh");
    RHEX(-32,         32, "0ffffffe0h");
    RHEX(-33,         32, "0ffffffdfh");
    RHEX(-0x80000000, 32, "80000000h");
    RHEX(-0xffffffff, 32, "1");

    RHEX(0,           -32, "0");
    RHEX(32,          -32, "32");
    RHEX(33,          -32, "00000021h");
    RHEX(0x80000000,  -32, "-80000000h");
    RHEX(0xffffffff,  -32, "-1");
    RHEX(-32,         -32, "-32");
    RHEX(-33,         -32, "-00000021h");
    RHEX(-0x80000000, -32, "-80000000h");
    RHEX(-0xffffffff, -32, "1");

    // Uppercase

    UHEX(0,           32, "00000000H");
    UHEX(32,          32, "00000020H");
    UHEX(0x80000000,  32, "80000000H");
    UHEX(0xffffffff,  32, "0FFFFFFFFH");
    UHEX(-32,         32, "0FFFFFFE0H");
    UHEX(-0x80000000, 32, "80000000H");
    UHEX(-0xffffffff, 32, "00000001H");

    UHEX(0,           -32, "00000000H");
    UHEX(32,          -32, "00000020H");
    UHEX(0x80000000,  -32, "-80000000H");
    UHEX(0xffffffff,  -32, "-00000001H");
    UHEX(-32,         -32, "-00000020H");
    UHEX(-0x80000000, -32, "-80000000H");
    UHEX(-0xffffffff, -32, "00000001H");

    // Uppercase and Relax

    URHEX(0,           32, "0");
    URHEX(32,          32, "32");
    URHEX(33,          32, "00000021H");
    URHEX(0x80000000,  32, "80000000H");
    URHEX(0xffffffff,  32, "0FFFFFFFFH");
    URHEX(-32,         32, "0FFFFFFE0H");
    URHEX(-33,         32, "0FFFFFFDFH");
    URHEX(-0x80000000, 32, "80000000H");
    URHEX(-0xffffffff, 32, "1");

    URHEX(0,           -32, "0");
    URHEX(32,          -32, "32");
    URHEX(33,          -32, "00000021H");
    URHEX(0x80000000,  -32, "-80000000H");
    URHEX(0xffffffff,  -32, "-1");
    URHEX(-32,         -32, "-32");
    URHEX(-33,         -32, "-00000021H");
    URHEX(-0x80000000, -32, "-80000000H");
    URHEX(-0xffffffff, -32, "1");

}
// clang-format on

void run_tests() {
    RUN_TEST(test_char_constant);
    RUN_TEST(test_dec_constant);
    RUN_TEST(test_hex_constant);
    RUN_TEST(test_oct_constant);
    RUN_TEST(test_bin_constant);
    RUN_TEST(test_current_address);
    RUN_TEST(test_scan);
    RUN_TEST(test_errors);
    RUN_TEST(test_formatter_8bit);
    RUN_TEST(test_formatter_16bit);
    RUN_TEST(test_formatter_24bit);
    RUN_TEST(test_formatter_32bit);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
