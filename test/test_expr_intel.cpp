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
ValueParser parser{number, comment, symbol, letter, location};
IntelValueFormatter formatter;

static void set_up() {
    formatter.setCStyle(false);
}

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
    parser.setCurrentOrigin(0x1000);
    E16("$",        0x1000);
    E16("$+2",      0x1002);
    E16("$-2",      0x0FFE);
    X16("$+0F000H", OVERFLOW_RANGE, "$+0F000H");
    E16("$-1001H",  0xFFFF);
    E32("$+0F000H", 0x00010000);
    E32("$-1001H",  0xFFFFFFFF);

    symtab.intern(0x1000, "table");
    parser.setCurrentOrigin(0x1100);
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

    HEX(0,    8, true, "0");
    HEX(32,   8, true, "32");
    HEX(33,   8, true, "21h");
    HEX(128,  8, true, "80h");
    HEX(255,  8, true, "0ffh");
    HEX(256,  8, true, "0");
    HEX(-32,  8, true, "0e0h");
    HEX(-33,  8, true, "0dfh");
    HEX(-128, 8, true, "80h");
    HEX(-255, 8, true, "1");
    HEX(-256, 8, true, "0");

    HEX(0,    -8, true, "0");
    HEX(32,   -8, true, "32");
    HEX(33,   -8, true, "21h");
    HEX(128,  -8, true, "-80h");
    HEX(255,  -8, true, "-1");
    HEX(256,  -8, true, "0");
    HEX(-32,  -8, true, "-32");
    HEX(-33,  -8, true, "-21h");
    HEX(-128, -8, true, "-80h");
    HEX(-255, -8, true, "1");
    HEX(-256, -8, true, "0");

    HEX(0,    8, false, "00h");
    HEX(32,   8, false, "20h");
    HEX(128,  8, false, "80h");
    HEX(255,  8, false, "0ffh");
    HEX(256,  8, false, "00h");
    HEX(-32,  8, false, "0e0h");
    HEX(-128, 8, false, "80h");
    HEX(-255, 8, false, "01h");
    HEX(-256, 8, false, "00h");

    HEX(0,    -8, false, "00h");
    HEX(32,   -8, false, "20h");
    HEX(128,  -8, false, "-80h");
    HEX(255,  -8, false, "-01h");
    HEX(256,  -8, false, "00h");
    HEX(-32,  -8, false, "-20h");
    HEX(-128, -8, false, "-80h");
    HEX(-255, -8, false, "01h");
    HEX(-256, -8, false, "00h");
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

    HEX(0,        16, true, "0");
    HEX(32,       16, true, "32");
    HEX(33,       16, true, "0021h");
    HEX(0x8000,   16, true, "8000h");
    HEX(0xffff,   16, true, "0ffffh");
    HEX(0x10000,  16, true, "0");
    HEX(-32,      16, true, "0ffe0h");
    HEX(-33,      16, true, "0ffdfh");
    HEX(-0x8000,  16, true, "8000h");
    HEX(-0xffff,  16, true, "1");
    HEX(-0x10000, 16, true, "0");

    HEX(0,        -16, true, "0");
    HEX(32,       -16, true, "32");
    HEX(33,       -16, true, "0021h");
    HEX(0x8000,   -16, true, "-8000h");
    HEX(0xffff,   -16, true, "-1");
    HEX(0x10000,  -16, true, "0");
    HEX(-32,      -16, true, "-32");
    HEX(-33,      -16, true, "-0021h");
    HEX(-0x8000,  -16, true, "-8000h");
    HEX(-0xffff,  -16, true, "1");
    HEX(-0x10000, -16, true, "0");

    HEX(0,        16, false, "0000h");
    HEX(32,       16, false, "0020h");
    HEX(0x8000,   16, false, "8000h");
    HEX(0xffff,   16, false, "0ffffh");
    HEX(0x10000,  16, false, "0000h");
    HEX(-32,      16, false, "0ffe0h");
    HEX(-0x8000,  16, false, "8000h");
    HEX(-0xffff,  16, false, "0001h");
    HEX(-0x10000, 16, false, "0000h");

    HEX(0,        -16, false, "0000h");
    HEX(32,       -16, false, "0020h");
    HEX(0x8000,   -16, false, "-8000h");
    HEX(0xffff,   -16, false, "-0001h");
    HEX(0x10000,  -16, false, "0000h");
    HEX(-32,      -16, false, "-0020h");
    HEX(-0x8000,  -16, false, "-8000h");
    HEX(-0xffff,  -16, false, "0001h");
    HEX(-0x10000, -16, false, "0000h");
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

    HEX(0,           24, true, "0");
    HEX(32,          24, true, "32");
    HEX(33,          24, true, "000021h");
    HEX(0x800000,    24, true, "800000h");
    HEX(0xffffff,    24, true, "0ffffffh");
    HEX(0x1000000,   24, true, "0");
    HEX(-32,         24, true, "0ffffe0h");
    HEX(-33,         24, true, "0ffffdfh");
    HEX(-0x800000,   24, true, "800000h");
    HEX(-0xffffff,   24, true, "1");
    HEX(-0x1000000,  24, true, "0");

    HEX(0,          -24, true, "0");
    HEX(32,         -24, true, "32");
    HEX(33,         -24, true, "000021h");
    HEX(0x800000,   -24, true, "-800000h");
    HEX(0xffffff,   -24, true, "-1");
    HEX(0x1000000,  -24, true, "0");
    HEX(-32,        -24, true, "-32");
    HEX(-33,        -24, true, "-000021h");
    HEX(-0x800000,  -24, true, "-800000h");
    HEX(-0xffffff,  -24, true, "1");
    HEX(-0x1000000, -24, true, "0");

    HEX(0,           24, false, "000000h");
    HEX(32,          24, false, "000020h");
    HEX(0x800000,    24, false, "800000h");
    HEX(0xffffff,    24, false, "0ffffffh");
    HEX(0x1000000,   24, false, "000000h");
    HEX(-32,         24, false, "0ffffe0h");
    HEX(-0x800000,   24, false, "800000h");
    HEX(-0xffffff,   24, false, "000001h");
    HEX(-0x1000000,  24, false, "000000h");

    HEX(0,          -24, false, "000000h");
    HEX(32,         -24, false, "000020h");
    HEX(0x800000,   -24, false, "-800000h");
    HEX(0xffffff,   -24, false, "-000001h");
    HEX(0x1000000,  -24, false, "000000h");
    HEX(-32,        -24, false, "-000020h");
    HEX(-0x800000,  -24, false, "-800000h");
    HEX(-0xffffff,  -24, false, "000001h");
    HEX(-0x1000000, -24, false, "000000h");
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

    HEX(0,           32, true, "0");
    HEX(32,          32, true, "32");
    HEX(33,          32, true, "00000021h");
    HEX(0x80000000,  32, true, "80000000h");
    HEX(0xffffffff,  32, true, "0ffffffffh");
    HEX(-32,         32, true, "0ffffffe0h");
    HEX(-33,         32, true, "0ffffffdfh");
    HEX(-0x80000000, 32, true, "80000000h");
    HEX(-0xffffffff, 32, true, "1");

    HEX(0,           -32, true, "0");
    HEX(32,          -32, true, "32");
    HEX(33,          -32, true, "00000021h");
    HEX(0x80000000,  -32, true, "-80000000h");
    HEX(0xffffffff,  -32, true, "-1");
    HEX(-32,         -32, true, "-32");
    HEX(-33,         -32, true, "-00000021h");
    HEX(-0x80000000, -32, true, "-80000000h");
    HEX(-0xffffffff, -32, true, "1");

    HEX(0,           32, false, "00000000h");
    HEX(32,          32, false, "00000020h");
    HEX(0x80000000,  32, false, "80000000h");
    HEX(0xffffffff,  32, false, "0ffffffffh");
    HEX(-32,         32, false, "0ffffffe0h");
    HEX(-0x80000000, 32, false, "80000000h");
    HEX(-0xffffffff, 32, false, "00000001h");

    HEX(0,           -32, false, "00000000h");
    HEX(32,          -32, false, "00000020h");
    HEX(0x80000000,  -32, false, "-80000000h");
    HEX(0xffffffff,  -32, false, "-00000001h");
    HEX(-32,         -32, false, "-00000020h");
    HEX(-0x80000000, -32, false, "-80000000h");
    HEX(-0xffffffff, -32, false, "00000001h");
}

static void test_formatter_cstyle() {
    formatter.setCStyle(true);

    HEX(0,    8, false, "0x00");
    HEX(32,   8, false, "0x20");
    HEX(128,  8, false, "0x80");
    HEX(255,  8, false, "0xff");
    HEX(256,  8, false, "0x00");
    HEX(-32,  8, false, "0xe0");
    HEX(-128, 8, false, "0x80");
    HEX(-255, 8, false, "0x01");
    HEX(-256, 8, false, "0x00");

    HEX(0,         16, false, "0x0000");
    HEX(32,        16, false, "0x0020");
    HEX(0x8000,    16, false, "0x8000");
    HEX(0xffff,    16, false, "0xffff");
    HEX(0x10000,   16, false, "0x0000");
    HEX(-32,       16, false, "0xffe0");
    HEX(-0x8000,   16, false, "0x8000");
    HEX(-0xffff,   16, false, "0x0001");
    HEX(-0x10000,  16, false, "0x0000");

    HEX(0,           24, false, "0x000000");
    HEX(32,          24, false, "0x000020");
    HEX(0x800000,    24, false, "0x800000");
    HEX(0xffffff,    24, false, "0xffffff");
    HEX(0x1000000,   24, false, "0x000000");
    HEX(-32,         24, false, "0xffffe0");
    HEX(-0x800000,   24, false, "0x800000");
    HEX(-0xffffff,   24, false, "0x000001");
    HEX(-0x1000000,  24, false, "0x000000");

    HEX(0,           32, false, "0x00000000");
    HEX(32,          32, false, "0x00000020");
    HEX(0x80000000,  32, false, "0x80000000");
    HEX(0xffffffff,  32, false, "0xffffffff");
    HEX(-32,         32, false, "0xffffffe0");
    HEX(-0x80000000, 32, false, "0x80000000");
    HEX(-0xffffffff, 32, false, "0x00000001");

    HEX(-128 *2, 9, false, "0x100");
    HEX(   0 *2, 9, false, "0x000");
    HEX(+127 *2, 9, false, "0x0fe");

    HEX(-2048 *2, 13, false, "0x1000");
    HEX(    0 *2, 13, false, "0x0000");
    HEX(+2047 *2, 13, false, "0x0ffe");
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
    RUN_TEST(test_formatter_cstyle);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
