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

#include "test_expr_helper.h"

using namespace libasm;
using namespace libasm::test;

FairchildValueParser parser;
FairchildValueFormatter formatter;

static void set_up() {
    formatter.setCStyle(false);
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
static void test_char_constant() {
    E8("'a'",   0x61, OK);
    E8("'a'+5", 0x66, OK);
    E8("5+'a'", 0x66, OK);
    E8("'a",    0x61, OK);
    E8("'a+5",  0x66, OK);
    E8("5+'a",  0x66, OK);
    E8("#a",    0x61, OK);
    E8("#a+5",  0x66, OK);
    E8("5+#a",  0x66, OK);
    E8("# ",    0x20, OK);
    E8("#'",    0x27, OK);
    E8("' ",    0x20, OK);
    E8("''",    0x27, OK);
    E8("'';",   0x27, OK);
    E8("''''",  0,    GARBAGE_AT_END);

    E8("c'a'",   0x61, OK);
    E8("C'a'+5", 0x66, OK);
    E8("5+c'a'", 0x66, OK);
    E8("c'a",    0,    MISSING_CLOSING_QUOTE);
    E8("C'a+5",  0,    MISSING_CLOSING_QUOTE);
    E8("5+c'a",  0,    MISSING_CLOSING_QUOTE);
    E8("c' ",    0,    MISSING_CLOSING_QUOTE);
    E8("c''",    0,    MISSING_CLOSING_QUOTE);
    E8("c'''",   0,    MISSING_CLOSING_QUOTE);
    E8("c''''",  0x27, OK);

    E16("'a'", 0x61, OK);
    E32("'a'", 0x61, OK);
}

static void test_dec_constant() {
    E32("2147483647",    0x7fffffff, OK);
    E32("d'2147483647",  0x7fffffff, OK);
    E32("D'2147483647",  0x7fffffff, OK);
    E32("d'2147483647'", 0x7fffffff, OK);
    E32("D'2147483647'", 0x7fffffff, OK);
}

static void test_hex_constant() {
    E8("h'0",   0x00, OK);
    E8("h'7f",  0x7f, OK);
    E8("h'80",  0x80, OK);
    E8("h'FF'", 0xff, OK);
    E8("h'100", 0,    OVERFLOW_RANGE);

    E16("h'0",     0x0000, OK);
    E16("h'7fff",  0x7fff, OK);
    E16("h'8000",  0x8000, OK);
    E16("h'ffff",  0xffff, OK);
    E16("h'10000", 0,      OVERFLOW_RANGE);

    E32("h'0",         0x00000000, OK);
    E32("h'7FFFFFFF",  0x7fffffff, OK);
    E32("H'7FFFFFFF",  0x7fffffff, OK);
    E32("h'80000000",  0x80000000, OK);
    E32("h'FFFFffff",  0xffffffff, OK);
    E32("h'100000000", 0,          OVERFLOW_RANGE);

    E8("$0",   0x00, OK);
    E8("$7f",  0x7f, OK);
    E8("$80",  0x80, OK);
    E8("$FF",  0xff, OK);
    E8("$100", 0,    OVERFLOW_RANGE);

    E16("$0",     0x0000, OK);
    E16("$7fff",  0x7fff, OK);
    E16("$8000",  0x8000, OK);
    E16("$ffff",  0xffff, OK);
    E16("$10000", 0,      OVERFLOW_RANGE);

    E32("$0",         0x00000000, OK);
    E32("$7FFFFFFF",  0x7fffffff, OK);
    E32("$80000000",  0x80000000, OK);
    E32("$FFFFffff",  0xffffffff, OK);
    E32("$100000000", 0,          OVERFLOW_RANGE);

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
    E32("O'37777777777",  0xffffffff, OK);
    E32("o'37777777777'", 0xffffffff, OK);
    E32("O'37777777777'", 0xffffffff, OK);
    E32("o'40000000000",  0,          OVERFLOW_RANGE);

    E8("@0",   0x00, OK);
    E8("@177", 0x7f, OK);
    E8("@200", 0x80, OK);
    E8("@377", 0xff, OK);
    E8("@400", 0,    OVERFLOW_RANGE);

    E16("@0",      0x0000, OK);
    E16("@077777", 0x7fff, OK);
    E16("@100000", 0x8000, OK);
    E16("@177777", 0xffff, OK);
    E16("@200000", 0,      OVERFLOW_RANGE);

    E32("@0",           0x00000000, OK);
    E32("@17777777777", 0x7fffffff, OK);
    E32("@20000000000", 0x80000000, OK);
    E32("@37777777777", 0xffffffff, OK);
    E32("@40000000000", 0,          OVERFLOW_RANGE);

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

    E8("%0",        0x00, OK);
    E8("%01111111", 0x7f, OK);
    E8("%10000000", 0x80, OK);
    E8("%11111111", 0xff, OK);
    E8("100000000", 0,    OVERFLOW_RANGE);

    E16("%0",                0x0000, OK);
    E16("%0111111111111111", 0x7fff, OK);
    E16("%1000000000000000", 0x8000, OK);
    E16("%1111111111111111", 0xffff, OK);
    E16("%10000000000000000", 0,      OVERFLOW_RANGE);

    E32("%0",                                0x00000000, OK);
    E32("%01111111111111111111111111111111", 0x7fffffff, OK);
    E32("%10000000000000000000000000000000", 0x80000000, OK);
    E32("%11111111111111111111111111111111", 0xffffffff, OK);
    E32("%100000000000000000000000000000000", 0,          OVERFLOW_RANGE);

    E32("0b0",                                 0x00000000, OK);
    E32("0b01111111111111111111111111111111",  0x7fffffff, OK);
    E32("0b10000000000000000000000000000000",  0x80000000, OK);
    E32("0b11111111111111111111111111111111",  0xffffffff, OK);
    E32("0b100000000000000000000000000000000", 0,          OVERFLOW_RANGE);
}

static void test_current_address() {
    parser.setCurrentOrigin(0x1000);

    E16("*",         0x1000, OK);
    E16("*+2",       0x1002, OK);
    E16("*-2",       0x0FFE, OK);
    E16("*+h'F000'", 0,      OVERFLOW_RANGE);
    E16("*-h'1001'", 0xFFFF, OK);
    E16("*+$F000",   0,      OVERFLOW_RANGE);
    E16("*-$1001",   0xFFFF, OK);
    E16("*+0xF000",  0,      OVERFLOW_RANGE);
    E16("*-$1001",   0xFFFF, OK);
    E32("*+h'F000'", 0x00010000, OK);
    E32("*-h'1001'", 0xFFFFFFFF, OK);
    E32("*+$F000",   0x00010000, OK);
    E32("*-$1001",   0xFFFFFFFF, OK);
    E32("*+0xF000",  0x00010000, OK);
    E32("*-0x1001",  0xFFFFFFFF, OK);

    E16("$",         0x1000, OK);
    E16("$+2",       0x1002, OK);
    E16("$-2",       0x0FFE, OK);
    E16("$+h'F000'", 0,      OVERFLOW_RANGE);
    E16("$-h'1001'", 0xFFFF, OK);
    E16("$+$F000",   0,      OVERFLOW_RANGE);
    E16("$-$1001",   0xFFFF, OK);
    E16("$+0xF000",  0,      OVERFLOW_RANGE);
    E16("$-$1001",   0xFFFF, OK);
    E32("$+h'F000'", 0x00010000, OK);
    E32("$-h'1001'", 0xFFFFFFFF, OK);
    E32("$+$F000",   0x00010000, OK);
    E32("$-$1001",   0xFFFFFFFF, OK);
    E32("$+0xF000",  0x00010000, OK);
    E32("$-0x1001",  0xFFFFFFFF, OK);

    symtab.intern(0x1000, "table");
    parser.setCurrentOrigin(0x1100);
    E16("*-table",     0x100, OK);
    E16("(*-table)/2", 0x80,  OK);
    E16("$-table",     0x100, OK);
    E16("($-table)/2", 0x80,  OK);
}

static void test_scan() {
    SCAN('|', "|a+b|c+d",     "");
    SCAN('|', "a+b|c+d",      "a+b");
    SCAN('|', "a+(b|c)+d|e",  "a+(b|c)+d");
    SCAN('|', "a+c'|'+d|e",   "a+c'|'+d");
    SCAN(',', "|a+b|c+d",     "");
    SCAN(',', "','+'\'',abc", "','+'\''");
    SCAN('h', "0x1230hG",     "0x1230");
    SCAN('b', "0b1010b0",     "0b1010");
    SCAN('H', "1AB0HHX",      "1AB0");
    SCAN('O', "1230OOX",      "1230");
    SCAN('O', "1239OOX",      "1239");
    SCAN('B', "1010BBX",      "1010");
    SCAN('B', "1012BBX",      "1012");
    SCAN(',', "h'1230,",      "h'1230");
    SCAN(',', "h'1230',",     "h'1230'");
    SCAN('$', "$1230$X230",   "$1230");
    SCAN('@', "@1230@8230",   "@1230");
    SCAN('%', "%1010%2010",   "%1010");
}

static void test_errors() {
    E32("h'bcdefg", 0, ILLEGAL_CONSTANT);
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
    HEX(33,   8, true, "h'21'");
    HEX(128,  8, true, "h'80'");
    HEX(255,  8, true, "h'ff'");
    HEX(256,  8, true, "0");
    HEX(-32,  8, true, "h'e0'");
    HEX(-33,  8, true, "h'df'");
    HEX(-128, 8, true, "h'80'");
    HEX(-255, 8, true, "1");
    HEX(-256, 8, true, "0");

    HEX(0,   -8, true, "0");
    HEX(32,  -8, true, "32");
    HEX(33,  -8, true, "h'21'");
    HEX(128, -8, true, "-h'80'");
    HEX(255, -8, true, "-1");
    HEX(256, -8, true, "0");
    HEX(-32, -8, true, "-32");
    HEX(-33, -8, true, "-h'21'");
    HEX(-128,-8, true, "-h'80'");
    HEX(-255,-8, true, "1");
    HEX(-256,-8, true, "0");

    HEX(0,    8, false, "h'00'");
    HEX(32,   8, false, "h'20'");
    HEX(128,  8, false, "h'80'");
    HEX(255,  8, false, "h'ff'");
    HEX(256,  8, false, "h'00'");
    HEX(-32,  8, false, "h'e0'");
    HEX(-128, 8, false, "h'80'");
    HEX(-255, 8, false, "h'01'");
    HEX(-256, 8, false, "h'00'");

    HEX(0,   -8, false, "h'00'");
    HEX(32,  -8, false, "h'20'");
    HEX(128, -8, false, "-h'80'");
    HEX(255, -8, false, "-h'01'");
    HEX(256, -8, false, "h'00'");
    HEX(-32, -8, false, "-h'20'");
    HEX(-128,-8, false, "-h'80'");
    HEX(-255,-8, false, "h'01'");
    HEX(-256,-8, false, "h'00'");
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
    HEX(33,        16, true, "h'0021'");
    HEX(0x8000,    16, true, "h'8000'");
    HEX(0xffff,    16, true, "h'ffff'");
    HEX(0x10000,   16, true, "0");
    HEX(-32,       16, true, "h'ffe0'");
    HEX(-33,       16, true, "h'ffdf'");
    HEX(-0x8000,   16, true, "h'8000'");
    HEX(-0xffff,   16, true, "1");
    HEX(-0x10000,  16, true, "0");

    HEX(0,        -16, true, "0");
    HEX(32,       -16, true, "32");
    HEX(33,       -16, true, "h'0021'");
    HEX(0x8000,   -16, true, "-h'8000'");
    HEX(0xffff,   -16, true, "-1");
    HEX(0x10000,  -16, true, "0");
    HEX(-32,      -16, true, "-32");
    HEX(-33,      -16, true, "-h'0021'");
    HEX(-0x8000,  -16, true, "-h'8000'");
    HEX(-0xffff,  -16, true, "1");
    HEX(-0x10000, -16, true, "0");

    HEX(0,         16, false, "h'0000'");
    HEX(32,        16, false, "h'0020'");
    HEX(0x8000,    16, false, "h'8000'");
    HEX(0xffff,    16, false, "h'ffff'");
    HEX(0x10000,   16, false, "h'0000'");
    HEX(-32,       16, false, "h'ffe0'");
    HEX(-0x8000,   16, false, "h'8000'");
    HEX(-0xffff,   16, false, "h'0001'");
    HEX(-0x10000,  16, false, "h'0000'");

    HEX(0,        -16, false, "h'0000'");
    HEX(32,       -16, false, "h'0020'");
    HEX(0x8000,   -16, false, "-h'8000'");
    HEX(0xffff,   -16, false, "-h'0001'");
    HEX(0x10000,  -16, false, "h'0000'");
    HEX(-32,      -16, false, "-h'0020'");
    HEX(-0x8000,  -16, false, "-h'8000'");
    HEX(-0xffff,  -16, false, "h'0001'");
    HEX(-0x10000, -16, false, "h'0000'");
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
    HEX(33,          24, true, "h'000021'");
    HEX(0x800000,    24, true, "h'800000'");
    HEX(0xffffff,    24, true, "h'ffffff'");
    HEX(0x1000000,   24, true, "0");
    HEX(-32,         24, true, "h'ffffe0'");
    HEX(-33,         24, true, "h'ffffdf'");
    HEX(-0x800000,   24, true, "h'800000'");
    HEX(-0xffffff,   24, true, "1");
    HEX(-0x1000000,  24, true, "0");

    HEX(0,          -24, true, "0");
    HEX(32,         -24, true, "32");
    HEX(33,         -24, true, "h'000021'");
    HEX(0x800000,   -24, true, "-h'800000'");
    HEX(0xffffff,   -24, true, "-1");
    HEX(0x1000000,  -24, true, "0");
    HEX(-32,        -24, true, "-32");
    HEX(-33,        -24, true, "-h'000021'");
    HEX(-0x800000,  -24, true, "-h'800000'");
    HEX(-0xffffff,  -24, true, "1");
    HEX(-0x1000000, -24, true, "0");

    HEX(0,           24, false, "h'000000'");
    HEX(32,          24, false, "h'000020'");
    HEX(0x800000,    24, false, "h'800000'");
    HEX(0xffffff,    24, false, "h'ffffff'");
    HEX(0x1000000,   24, false, "h'000000'");
    HEX(-32,         24, false, "h'ffffe0'");
    HEX(-0x800000,   24, false, "h'800000'");
    HEX(-0xffffff,   24, false, "h'000001'");
    HEX(-0x1000000,  24, false, "h'000000'");

    HEX(0,          -24, false, "h'000000'");
    HEX(32,         -24, false, "h'000020'");
    HEX(0x800000,   -24, false, "-h'800000'");
    HEX(0xffffff,   -24, false, "-h'000001'");
    HEX(0x1000000,  -24, false, "h'000000'");
    HEX(-32,        -24, false, "-h'000020'");
    HEX(-0x800000,  -24, false, "-h'800000'");
    HEX(-0xffffff,  -24, false, "h'000001'");
    HEX(-0x1000000, -24, false, "h'000000'");
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
    HEX(33,          32, true, "h'00000021'");
    HEX(0x80000000,  32, true, "h'80000000'");
    HEX(0xffffffff,  32, true, "h'ffffffff'");
    HEX(-32,         32, true, "h'ffffffe0'");
    HEX(-33,         32, true, "h'ffffffdf'");
    HEX(-0x80000000, 32, true, "h'80000000'");
    HEX(-0xffffffff, 32, true, "1");

    HEX(0,           -32, true, "0");
    HEX(32,          -32, true, "32");
    HEX(33,          -32, true, "h'00000021'");
    HEX(0x80000000,  -32, true, "-h'80000000'");
    HEX(0xffffffff,  -32, true, "-1");
    HEX(-32,         -32, true, "-32");
    HEX(-33,         -32, true, "-h'00000021'");
    HEX(-0x80000000, -32, true, "-h'80000000'");
    HEX(-0xffffffff, -32, true, "1");

    HEX(0,           32, false, "h'00000000'");
    HEX(32,          32, false, "h'00000020'");
    HEX(0x80000000,  32, false, "h'80000000'");
    HEX(0xffffffff,  32, false, "h'ffffffff'");
    HEX(-32,         32, false, "h'ffffffe0'");
    HEX(-0x80000000, 32, false, "h'80000000'");
    HEX(-0xffffffff, 32, false, "h'00000001'");

    HEX(0,           -32, false, "h'00000000'");
    HEX(32,          -32, false, "h'00000020'");
    HEX(0x80000000,  -32, false, "-h'80000000'");
    HEX(0xffffffff,  -32, false, "-h'00000001'");
    HEX(-32,         -32, false, "-h'00000020'");
    HEX(-0x80000000, -32, false, "-h'80000000'");
    HEX(-0xffffffff, -32, false, "h'00000001'");
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
