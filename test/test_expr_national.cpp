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

#include "test_expr_helper.h"

using namespace libasm;
using namespace libasm::test;

NationalValueParser national_parser;
ValueParser &parser = national_parser;
NationalValueFormatter formatter;

static void set_up() {}

static void tear_down() {
    symtab.reset();
}

// clang-format off
static void test_dec_constant() {
    E32("2147483647",    0x7fffffff, OK);
    E32("d'2147483647",  0x7fffffff, OK);
    E32("D'2147483647",  0x7fffffff, OK);
    E32("d'2147483647'", 0x7fffffff, OK);
    E32("D'2147483647'", 0x7fffffff, OK);
}

static void test_hex_constant() {
    E8("x'0",   0x00, OK);
    E8("x'7f",  0x7f, OK);
    E8("x'80",  0x80, OK);
    E8("x'FF'", 0xff, OK);
    E8("x'100", 0,    OVERFLOW_RANGE);

    E16("x'0",     0x0000, OK);
    E16("x'7fff",  0x7fff, OK);
    E16("x'8000",  0x8000, OK);
    E16("x'ffff",  0xffff, OK);
    E16("x'10000", 0,      OVERFLOW_RANGE);

    E32("x'0",         0x00000000, OK);
    E32("x'7FFFFFFF",  0x7fffffff, OK);
    E32("h'7FFFFFFF",  0x7fffffff, OK);
    E32("H'7FFFFFFF",  0x7fffffff, OK);
    E32("X'7FFFFFFF",  0x7fffffff, OK);
    E32("x'7FFFFFFF'", 0x7fffffff, OK);
    E32("h'7FFFFFFF'", 0x7fffffff, OK);
    E32("H'7FFFFFFF'", 0x7fffffff, OK);
    E32("X'7FFFFFFF'", 0x7fffffff, OK);
    E32("x'80000000",  0x80000000, OK);
    E32("x'FFFFffff",  0xffffffff, OK);
    E32("x'100000000", 0,          OVERFLOW_RANGE);

    E32("0x0",         0x00000000, OK);
    E32("0x7FFFFFFF",  0x7fffffff, OK);
    E32("0x80000000",  0x80000000, OK);
    E32("0xFFFFffff",  0xffffffff, OK);
    E32("0x100000000", 0,          OVERFLOW_RANGE);
}

static void test_oct_constant() {
    E8("o'0",   0x00, OK);
    E8("o'177", 0x7f, OK);
    E8("o'200", 0x80, OK);
    E8("o'377", 0xff, OK);
    E8("o'400", 0,    OVERFLOW_RANGE);

    E16("o'0",      0x0000, OK);
    E16("o'077777", 0x7fff, OK);
    E16("o'100000", 0x8000, OK);
    E16("o'177777", 0xffff, OK);
    E16("o'200000", 0,      OVERFLOW_RANGE);

    E32("o'0",            0x00000000, OK);
    E32("o'17777777777",  0x7fffffff, OK);
    E32("o'20000000000",  0x80000000, OK);
    E32("o'37777777777",  0xffffffff, OK);
    E32("q'37777777777",  0xffffffff, OK);
    E32("O'37777777777",  0xffffffff, OK);
    E32("Q'37777777777",  0xffffffff, OK);
    E32("o'37777777777'", 0xffffffff, OK);
    E32("q'37777777777'", 0xffffffff, OK);
    E32("O'37777777777'", 0xffffffff, OK);
    E32("Q'37777777777'", 0xffffffff, OK);
    E32("o'40000000000",  0,          OVERFLOW_RANGE);

    E32("00",           0x00000000, OK);
    E32("017777777777", 0x7fffffff, OK);
    E32("020000000000", 0x80000000, OK);
    E32("037777777777", 0xffffffff, OK);
    E32("040000000000", 0,          OVERFLOW_RANGE);
}

static void test_bin_constant() {
    E8("b'0",        0x00, OK);
    E8("b'01111111", 0x7f, OK);
    E8("b'10000000", 0x80, OK);
    E8("b'11111111", 0xff, OK);
    E8("100000000", 0,    OVERFLOW_RANGE);

    E16("b'0",                0x0000, OK);
    E16("b'0111111111111111", 0x7fff, OK);
    E16("b'1000000000000000", 0x8000, OK);
    E16("b'1111111111111111", 0xffff, OK);
    E16("b'10000000000000000", 0,      OVERFLOW_RANGE);

    E32("b'0",                                 0x00000000, OK);
    E32("b'01111111111111111111111111111111",  0x7fffffff, OK);
    E32("b'10000000000000000000000000000000",  0x80000000, OK);
    E32("b'11111111111111111111111111111111",  0xffffffff, OK);
    E32("B'11111111111111111111111111111111",  0xffffffff, OK);
    E32("b'11111111111111111111111111111111'", 0xffffffff, OK);
    E32("B'11111111111111111111111111111111'", 0xffffffff, OK);
    E32("b'100000000000000000000000000000000", 0,          OVERFLOW_RANGE);

    E32("0b0",                                 0x00000000, OK);
    E32("0b01111111111111111111111111111111",  0x7fffffff, OK);
    E32("0b10000000000000000000000000000000",  0x80000000, OK);
    E32("0b11111111111111111111111111111111",  0xffffffff, OK);
    E32("0b100000000000000000000000000000000", 0,          OVERFLOW_RANGE);
}

static void test_current_address() {
    parser.setCurrentOrigin(0x1000);
    E16(".",       0x1000, OK);
    E16(".+2",     0x1002, OK);
    E16(".-2",     0x0FFE, OK);
    E16(".+x'F000", 0,      OVERFLOW_RANGE);
    E16(".-x'1001", 0xFFFF, OK);
    E32(".+x'F000", 0x00010000, OK);
    E32(".-x'1001", 0xFFFFFFFF, OK);
    E16(".",       0x1000, OK);
    E16(".+2",     0x1002, OK);
    E16(".-2",     0x0FFE, OK);
    E16(".+0xF000", 0,      OVERFLOW_RANGE);
    E16(".-0x1001", 0xFFFF, OK);
    E32(".+0xF000", 0x00010000, OK);
    E32(".-0x1001", 0xFFFFFFFF, OK);

    symtab.intern(0x1000, "table");
    parser.setCurrentOrigin(0x1100);
    E16(".-table",     0x100, OK);
    E16("(.-table)/2", 0x80,  OK);
}

static void test_scan() {
    SCAN('|', "|a+b|c+d",     "");
    SCAN('|', "a+b|c+d",      "a+b");
    SCAN('|', "a+(b|c)+d|e",  "a+(b|c)+d");
    SCAN('|', "a+'|'+d|e",    "a+'|'+d");
    SCAN(',', "|a+b|c+d",     "");
    SCAN(',', "','+'\'',abc", "','+'\''");
    SCAN('x', "0x1230xG",     "0x1230");
    SCAN('b', "0b1010b0",     "0b1010");
    SCAN('H', "1AB0HHX",      "1AB0H");
    SCAN('O', "1230OOX",      "1230O");
    SCAN('O', "1239OOX",      "1239");
    SCAN('B', "1010BBX",      "1010B");
    SCAN('B', "1012BBX",      "1012");
    SCAN(',', "X'1230,",      "X'1230");
    SCAN(',', "X'1230',",     "X'1230'");
}

static void test_errors() {
    E32("x'bcdefg", 0, ILLEGAL_CONSTANT);
    E32("o'345678", 0, ILLEGAL_CONSTANT);
    E32("b'101012", 0, ILLEGAL_CONSTANT);
    E32("456789a", 0, ILLEGAL_CONSTANT);
}

static void test_formatter_8bit() {
    DEC(0,     8, "0");
    DEC(32,    8, "32");
    DEC(128,   8, "128");
    DEC(255,   8, "255");
    DEC(256,   8, "0");
    DEC(-32,   8, "224");
    DEC(-128,  8, "128");
    DEC(-255,  8, "1");
    DEC(-256,  8, "0");

    DEC(0,    -8, "0");
    DEC(32,   -8, "32");
    DEC(128,  -8, "-128");
    DEC(255,  -8, "-1");
    DEC(256,  -8, "0");
    DEC(-32,  -8, "-32");
    DEC(-128, -8, "-128");
    DEC(-255, -8, "1");
    DEC(-256, -8, "0");

    DEC(0,     8, "0");
    DEC(32,    8, "32");
    DEC(128,   8, "128");
    DEC(255,   8, "255");
    DEC(256,   8, "0");
    DEC(-32,   8, "224");
    DEC(-128,  8, "128");
    DEC(-255,  8, "1");
    DEC(-256,  8, "0");

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
    HEX(33,   8, true, "x'21");
    HEX(128,  8, true, "x'80");
    HEX(255,  8, true, "x'ff");
    HEX(256,  8, true, "0");
    HEX(-32,  8, true, "x'e0");
    HEX(-33,  8, true, "x'df");
    HEX(-128, 8, true, "x'80");
    HEX(-255, 8, true, "1");
    HEX(-256, 8, true, "0");

    HEX(0,   -8, true, "0");
    HEX(32,  -8, true, "32");
    HEX(33,  -8, true, "x'21");
    HEX(128, -8, true, "-x'80");
    HEX(255, -8, true, "-1");
    HEX(256, -8, true, "0");
    HEX(-32, -8, true, "-32");
    HEX(-33, -8, true, "-x'21");
    HEX(-128,-8, true, "-x'80");
    HEX(-255,-8, true, "1");
    HEX(-256,-8, true, "0");

    HEX(0,    8, false, "x'00");
    HEX(32,   8, false, "x'20");
    HEX(128,  8, false, "x'80");
    HEX(255,  8, false, "x'ff");
    HEX(256,  8, false, "x'00");
    HEX(-32,  8, false, "x'e0");
    HEX(-128, 8, false, "x'80");
    HEX(-255, 8, false, "x'01");
    HEX(-256, 8, false, "x'00");

    HEX(0,   -8, false, "x'00");
    HEX(32,  -8, false, "x'20");
    HEX(128, -8, false, "-x'80");
    HEX(255, -8, false, "-x'01");
    HEX(256, -8, false, "x'00");
    HEX(-32, -8, false, "-x'20");
    HEX(-128,-8, false, "-x'80");
    HEX(-255,-8, false, "x'01");
    HEX(-256,-8, false, "x'00");
}

static void test_formatter_16bit() {
    DEC(0,         16, "0");
    DEC(32,        16, "32");
    DEC(0x8000,    16, "32768");
    DEC(0xffff,    16, "65535");
    DEC(0x10000,   16, "0");
    DEC(-32,       16, "65504");
    DEC(-0x8000,   16, "32768");
    DEC(-0xffff,   16, "1");
    DEC(-0x10000,  16, "0");

    DEC(0,        -16, "0");
    DEC(32,       -16, "32");
    DEC(0x8000,   -16, "-32768");
    DEC(0xffff,   -16, "-1");
    DEC(0x10000,  -16, "0");
    DEC(-32,      -16, "-32");
    DEC(-0x8000,  -16, "-32768");
    DEC(-0xffff,  -16, "1");
    DEC(-0x10000, -16, "0");

    DEC(0,         16, "0");
    DEC(32,        16, "32");
    DEC(0x8000,    16, "32768");
    DEC(0xffff,    16, "65535");
    DEC(0x10000,   16, "0");
    DEC(-32,       16, "65504");
    DEC(-0x8000,   16, "32768");
    DEC(-0xffff,   16, "1");
    DEC(-0x10000,  16, "0");

    DEC(0,        -16, "0");
    DEC(32,       -16, "32");
    DEC(0x8000,   -16, "-32768");
    DEC(0xffff,   -16, "-1");
    DEC(0x10000,  -16, "0");
    DEC(-32,      -16, "-32");
    DEC(-0x8000,  -16, "-32768");
    DEC(-0xffff,  -16, "1");
    DEC(-0x10000, -16, "0");

    HEX(0,         16, true, "0");
    HEX(32,        16, true, "32");
    HEX(33,        16, true, "x'0021");
    HEX(0x8000,    16, true, "x'8000");
    HEX(0xffff,    16, true, "x'ffff");
    HEX(0x10000,   16, true, "0");
    HEX(-32,       16, true, "x'ffe0");
    HEX(-33,       16, true, "x'ffdf");
    HEX(-0x8000,   16, true, "x'8000");
    HEX(-0xffff,   16, true, "1");
    HEX(-0x10000,  16, true, "0");

    HEX(0,        -16, true, "0");
    HEX(32,       -16, true, "32");
    HEX(33,       -16, true, "x'0021");
    HEX(0x8000,   -16, true, "-x'8000");
    HEX(0xffff,   -16, true, "-1");
    HEX(0x10000,  -16, true, "0");
    HEX(-32,      -16, true, "-32");
    HEX(-33,      -16, true, "-x'0021");
    HEX(-0x8000,  -16, true, "-x'8000");
    HEX(-0xffff,  -16, true, "1");
    HEX(-0x10000, -16, true, "0");

    HEX(0,         16, false, "x'0000");
    HEX(32,        16, false, "x'0020");
    HEX(0x8000,    16, false, "x'8000");
    HEX(0xffff,    16, false, "x'ffff");
    HEX(0x10000,   16, false, "x'0000");
    HEX(-32,       16, false, "x'ffe0");
    HEX(-0x8000,   16, false, "x'8000");
    HEX(-0xffff,   16, false, "x'0001");
    HEX(-0x10000,  16, false, "x'0000");

    HEX(0,        -16, false, "x'0000");
    HEX(32,       -16, false, "x'0020");
    HEX(0x8000,   -16, false, "-x'8000");
    HEX(0xffff,   -16, false, "-x'0001");
    HEX(0x10000,  -16, false, "x'0000");
    HEX(-32,      -16, false, "-x'0020");
    HEX(-0x8000,  -16, false, "-x'8000");
    HEX(-0xffff,  -16, false, "x'0001");
    HEX(-0x10000, -16, false, "x'0000");
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
    HEX(33,          24, true, "x'000021");
    HEX(0x800000,    24, true, "x'800000");
    HEX(0xffffff,    24, true, "x'ffffff");
    HEX(0x1000000,   24, true, "0");
    HEX(-32,         24, true, "x'ffffe0");
    HEX(-33,         24, true, "x'ffffdf");
    HEX(-0x800000,   24, true, "x'800000");
    HEX(-0xffffff,   24, true, "1");
    HEX(-0x1000000,  24, true, "0");

    HEX(0,          -24, true, "0");
    HEX(32,         -24, true, "32");
    HEX(33,         -24, true, "x'000021");
    HEX(0x800000,   -24, true, "-x'800000");
    HEX(0xffffff,   -24, true, "-1");
    HEX(0x1000000,  -24, true, "0");
    HEX(-32,        -24, true, "-32");
    HEX(-33,        -24, true, "-x'000021");
    HEX(-0x800000,  -24, true, "-x'800000");
    HEX(-0xffffff,  -24, true, "1");
    HEX(-0x1000000, -24, true, "0");

    HEX(0,           24, false, "x'000000");
    HEX(32,          24, false, "x'000020");
    HEX(0x800000,    24, false, "x'800000");
    HEX(0xffffff,    24, false, "x'ffffff");
    HEX(0x1000000,   24, false, "x'000000");
    HEX(-32,         24, false, "x'ffffe0");
    HEX(-0x800000,   24, false, "x'800000");
    HEX(-0xffffff,   24, false, "x'000001");
    HEX(-0x1000000,  24, false, "x'000000");

    HEX(0,          -24, false, "x'000000");
    HEX(32,         -24, false, "x'000020");
    HEX(0x800000,   -24, false, "-x'800000");
    HEX(0xffffff,   -24, false, "-x'000001");
    HEX(0x1000000,  -24, false, "x'000000");
    HEX(-32,        -24, false, "-x'000020");
    HEX(-0x800000,  -24, false, "-x'800000");
    HEX(-0xffffff,  -24, false, "x'000001");
    HEX(-0x1000000, -24, false, "x'000000");
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
    HEX(33,          32, true, "x'00000021");
    HEX(0x80000000,  32, true, "x'80000000");
    HEX(0xffffffff,  32, true, "x'ffffffff");
    HEX(-32,         32, true, "x'ffffffe0");
    HEX(-33,         32, true, "x'ffffffdf");
    HEX(-0x80000000, 32, true, "x'80000000");
    HEX(-0xffffffff, 32, true, "1");

    HEX(0,           -32, true, "0");
    HEX(32,          -32, true, "32");
    HEX(33,          -32, true, "x'00000021");
    HEX(0x80000000,  -32, true, "-x'80000000");
    HEX(0xffffffff,  -32, true, "-1");
    HEX(-32,         -32, true, "-32");
    HEX(-33,         -32, true, "-x'00000021");
    HEX(-0x80000000, -32, true, "-x'80000000");
    HEX(-0xffffffff, -32, true, "1");

    HEX(0,           32, false, "x'00000000");
    HEX(32,          32, false, "x'00000020");
    HEX(0x80000000,  32, false, "x'80000000");
    HEX(0xffffffff,  32, false, "x'ffffffff");
    HEX(-32,         32, false, "x'ffffffe0");
    HEX(-0x80000000, 32, false, "x'80000000");
    HEX(-0xffffffff, 32, false, "x'00000001");

    HEX(0,           -32, false, "x'00000000");
    HEX(32,          -32, false, "x'00000020");
    HEX(0x80000000,  -32, false, "-x'80000000");
    HEX(0xffffffff,  -32, false, "-x'00000001");
    HEX(-32,         -32, false, "-x'00000020");
    HEX(-0x80000000, -32, false, "-x'80000000");
    HEX(-0xffffffff, -32, false, "x'00000001");
}

static void test_formatter_suffix() {
    formatter.setSuffix(true);
    HEX(255,  8, false, "x'ff'");
    HEX(0xffffff,    24, true, "x'ffffff'");
}

// clang-format on

void run_tests() {
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
    RUN_TEST(test_formatter_suffix);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4: