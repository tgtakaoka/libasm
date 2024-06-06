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

const struct final : ValueParser::Plugins {
    const NumberParser &number() const override { return FairchildNumberParser::singleton(); }
    const CommentParser &comment() const override { return AsteriskCommentParser::singleton(); }
    const LetterParser &letter() const override { return FairchildLetterParser::singleton(); }
    const LocationParser &location() const override { return AsteriskLocationParser::singleton(); }
} plugins{};
TestLocator locator{};
const ValueParser parser{plugins, locator};

const ValueFormatter formatter{ValueFormatter::Plugins::fairchild()};

static void set_up() {}

static void tear_down() {
    symtab.reset();
}

// clang-format off
static void test_char_constant() {
    E8("'a'",   0x61);
    E8("'a'+5", 0x66);
    E8("5+'a'", 0x66);
    E8("'a",    0x61);
    E8("'a+5",  0x66);
    E8("5+'a",  0x66);
    E8("#a",    0x61);
    E8("#a+5",  0x66);
    E8("5+#a",  0x66);
    E8("# ",    0x20);
    E8("#'",    0x27);
    E8("' ",    0x20);
    X8("''",    MISSING_CLOSING_QUOTE, "''", "''");
    E8("'''",   0x27);
    E8("''''",  0x27);

    E8("c'a'",   0x61);
    E8("C'a'+5", 0x66);
    E8("5+c'a'", 0x66);
    X8("c'a",    MISSING_CLOSING_QUOTE, "c'a", "c'a");
    X8("C'a+5",  MISSING_CLOSING_QUOTE, "C'a+5", "C'a+5");
    X8("5+c'a",  MISSING_CLOSING_QUOTE, "c'a", "c'a");
    X8("c' ",    MISSING_CLOSING_QUOTE, "c' ", "c' ");
    X8("c''",    MISSING_CLOSING_QUOTE, "c''", "c''");
    X8("c'''",   MISSING_CLOSING_QUOTE, "c'''", "c'''");
    E8("c''''",  0x27);

    E16("'a'", 0x61);
    E32("'a'", 0x61);
}

static void test_dec_constant() {
    E32("2147483647",    0x7fffffff);
    X32("d'2147483647",  MISSING_CLOSING_QUOTE, "d'2147483647", "d'2147483647");
    X32("D'2147483647",  MISSING_CLOSING_QUOTE, "D'2147483647", "D'2147483647");
    E32("d'2147483647'", 0x7fffffff);
    E32("D'2147483647'", 0x7fffffff);
}

static void test_hex_constant() {
    E8("h'0'",   0x00);
    E8("h'7f'",  0x7f);
    E8("h'80'",  0x80);
    X8("h'FF",   MISSING_CLOSING_QUOTE, "h'FF", "h'FF");
    X8("h'100'", OVERFLOW_RANGE, "h'100'", "");

    E16("h'0'",     0x0000);
    E16("h'7fff'",  0x7fff);
    E16("h'8000'",  0x8000);
    E16("h'ffff'",  0xffff);
    X16("h'10000'", OVERFLOW_RANGE, "h'10000'", "");

    E32("h'0'",         0x00000000);
    E32("h'7FFFFFFF'",  0x7fffffff);
    E32("H'7FFFFFFF'",  0x7fffffff);
    E32("h'80000000'",  0x80000000);
    E32("h'FFFFffff'",  0xffffffff);
    X32("h'100000000'", OVERFLOW_RANGE, "h'100000000'", "h'100000000'");

    E8("$0",   0x00);
    E8("$7f",  0x7f);
    E8("$80",  0x80);
    E8("$FF",  0xff);
    X8("$100", OVERFLOW_RANGE, "$100", "");

    E16("$0",     0x0000);
    E16("$7fff",  0x7fff);
    E16("$8000",  0x8000);
    E16("$ffff",  0xffff);
    X16("$10000", OVERFLOW_RANGE, "$10000", "");

    E32("$0",         0x00000000);
    E32("$7FFFFFFF",  0x7fffffff);
    E32("$80000000",  0x80000000);
    E32("$FFFFffff",  0xffffffff);
    X32("$100000000", OVERFLOW_RANGE, "$100000000", "$100000000");

    E32("0x0",         0x00000000);
    E32("0x7FFFFFFF",  0x7fffffff);
    E32("0x80000000",  0x80000000);
    E32("0xFFFFffff",  0xffffffff);
    X32("0x100000000", OVERFLOW_RANGE, "0x100000000", "0x100000000");
}

static void test_oct_constant() {
    E8("o'0'",   0x00);
    E8("o'177'", 0x7f);
    X8("o'200",  MISSING_CLOSING_QUOTE, "o'200", "o'200");
    E8("o'377'", 0xff);
    X8("o'400'", OVERFLOW_RANGE, "o'400'", "");

    E16("o'0'",      0x0000);
    E16("o'077777'", 0x7fff);
    E16("o'100000'", 0x8000);
    E16("o'177777'", 0xffff);
    X16("o'200000'", OVERFLOW_RANGE, "o'200000'", "");

    E32("o'0'",           0x00000000);
    E32("o'17777777777'", 0x7fffffff);
    E32("o'20000000000'", 0x80000000);
    E32("o'37777777777'", 0xffffffff);
    E32("O'37777777777'", 0xffffffff);
    E32("o'37777777777'", 0xffffffff);
    E32("O'37777777777'", 0xffffffff);
    X32("o'40000000000'", OVERFLOW_RANGE, "o'40000000000'", "o'40000000000'");

    E32("00",           0x00000000);
    E32("017777777777", 0x7fffffff);
    E32("020000000000", 0x80000000);
    E32("037777777777", 0xffffffff);
    X32("040000000000", OVERFLOW_RANGE, "040000000000", "040000000000");
}

static void test_bin_constant() {
    E8("b'0'",         0x00);
    E8("b'01111111'",  0x7f);
    X8("b'10000000",   MISSING_CLOSING_QUOTE, "b'10000000", "b'10000000");
    E8("b'11111111'",  0xff);
    X8("b'100000000'", OVERFLOW_RANGE, "b'100000000'", "");

    E16("b'0'",                 0x0000);
    E16("b'0111111111111111'",  0x7fff);
    E16("b'1000000000000000'",  0x8000);
    E16("b'1111111111111111'",  0xffff);
    X16("b'10000000000000000'", OVERFLOW_RANGE, "b'10000000000000000'", "");

    E32("b'0'",                                 0x00000000);
    E32("b'01111111111111111111111111111111'",  0x7fffffff);
    E32("b'10000000000000000000000000000000'",  0x80000000);
    E32("b'11111111111111111111111111111111'",  0xffffffff);
    E32("B'11111111111111111111111111111111'",  0xffffffff);
    E32("b'11111111111111111111111111111111'",  0xffffffff);
    E32("B'11111111111111111111111111111111'",  0xffffffff);
    X32("b'100000000000000000000000000000000'", OVERFLOW_RANGE, "b'100000000000000000000000000000000'", "b'100000000000000000000000000000000'");

    E32("0b0",                                 0x00000000);
    E32("0b01111111111111111111111111111111",  0x7fffffff);
    E32("0b10000000000000000000000000000000",  0x80000000);
    E32("0b11111111111111111111111111111111",  0xffffffff);
    X32("0b100000000000000000000000000000000", OVERFLOW_RANGE, "0b100000000000000000000000000000000", "0b100000000000000000000000000000000");
}

static void test_current_address() {
    locator.location = 0x1000;
    E16("*",         0x1000);
    E16("*+2",       0x1002);
    E16("*-2",       0x0FFE);
    X16("*+h'F000'", OVERFLOW_RANGE, "*+h'F000'", "");
    E16("*-h'1001'", 0xFFFF);
    X16("*+$F000",   OVERFLOW_RANGE, "*+$F000", "");
    E16("*-$1001",   0xFFFF);
    X16("*+0xF000",  OVERFLOW_RANGE, "*+0xF000", "");
    E16("*-$1001",   0xFFFF);
    E32("*+h'F000'", 0x00010000);
    E32("*-h'1001'", 0xFFFFFFFF);
    E32("*+$F000",   0x00010000);
    E32("*-$1001",   0xFFFFFFFF);
    E32("*+0xF000",  0x00010000);
    E32("*-0x1001",  0xFFFFFFFF);

    symtab.intern(0x1000, "table");
    locator.location = 0x1100;
    E16("*-table",     0x100);
    E16("(*-table)/2", 0x080);
}

static void test_scan() {
    SERR('|', "|1+2|",        0,        "|1+2|", NOT_AN_EXPECTED, "|1+2|");
    SCAN('|', "1+2|3+4",      3,        "|3+4");
    SCAN('|', "1+(2|3)+4|5",  8,        "|5");
    SCAN('|', "1+c'|'+2|3",   '|'+3,    "|3");
    SCAN(',', "1+2|3+4",      7,        "");
    SCAN(',', R"(','+''',abc)", ','+'\'', ",abc");
    SCAN('h', "0x1230hG",     0x1230,   "hG");
    SCAN('b', "0b1010b0",     0b1010,   "b0");
    SERR('H', "H'1AB0HHX",    0x1AB0,   "H'1AB0HHX", MISSING_CLOSING_QUOTE, "H'1AB0HHX");
    SCAN('O', "1230OOX",      01230,    "OX");
    SCAN('O', "1239OOX",      1239,     "OOX");
    SCAN('B', "1010BBX",      0b1010,   "BX");
    SCAN('B', "1012BBX",      1012,     "BBX");
    SERR(',', "h'1230,",      0x1230,   "h'1230,", MISSING_CLOSING_QUOTE, "h'1230,");
    SCAN(',', "h'1230',",     0x1230,   ",");
    SCAN('$', "$1230$X230",   0x1230,   "$X230");
}

static void test_errors() {
    X32("h'bcdefg'", MISSING_CLOSING_QUOTE, "h'bcdefg'", "h'bcdefg'");
    X32("o'345678'", MISSING_CLOSING_QUOTE, "o'345678'", "o'345678'");
    X32("b'101012'", MISSING_CLOSING_QUOTE, "b'101012'", "b'101012'");
    EXPR("456789a",  456789, "a");
}

static void test_comment() {
    EXPR(" - 1 ", -1, "");
    EXPR(" + 1 ",  1, "");
    EXPR(" ~ 0 ", -1, "");
    EXPR(" 1 + 2 ; + 3 ", 3, "; + 3 ");
    EXPR(" ( 1 + 2 ) * 3 ", 9, "");
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

    UDEC(0,     8, "0");
    UDEC(32,    8, "32");
    UDEC(128,   8, "128");
    UDEC(255,   8, "255");
    UDEC(256,   8, "0");
    UDEC(-32,   8, "224");
    UDEC(-128,  8, "128");
    UDEC(-255,  8, "1");
    UDEC(-256,  8, "0");

    UDEC(0,    -8, "0");
    UDEC(32,   -8, "32");
    UDEC(128,  -8, "-128");
    UDEC(255,  -8, "-1");
    UDEC(256,  -8, "0");
    UDEC(-32,  -8, "-32");
    UDEC(-128, -8, "-128");
    UDEC(-255, -8, "1");
    UDEC(-256, -8, "0");

    BIN(0,    8, "b'00000000'");
    BIN(32,   8, "b'00100000'");
    BIN(128,  8, "b'10000000'");
    BIN(255,  8, "b'11111111'");
    BIN(256,  8, "b'00000000'");
    BIN(-32,  8, "b'11100000'");
    BIN(-128, 8, "b'10000000'");
    BIN(-255, 8, "b'00000001'");
    BIN(-256, 8, "b'00000000'");

    BIN(0,    -8, "b'00000000'");
    BIN(32,   -8, "b'00100000'");
    BIN(128,  -8, "-b'10000000'");
    BIN(255,  -8, "-b'00000001'");
    BIN(256,  -8, "b'00000000'");
    BIN(-32,  -8, "-b'00100000'");
    BIN(-128, -8, "-b'10000000'");
    BIN(-255, -8, "b'00000001'");
    BIN(-256, -8, "b'00000000'");

    UBIN(0,    8, "B'00000000'");
    UBIN(32,   8, "B'00100000'");
    UBIN(128,  8, "B'10000000'");
    UBIN(255,  8, "B'11111111'");
    UBIN(256,  8, "B'00000000'");
    UBIN(-32,  8, "B'11100000'");
    UBIN(-128, 8, "B'10000000'");
    UBIN(-255, 8, "B'00000001'");
    UBIN(-256, 8, "B'00000000'");

    UBIN(0,    -8, "B'00000000'");
    UBIN(32,   -8, "B'00100000'");
    UBIN(128,  -8, "-B'10000000'");
    UBIN(255,  -8, "-B'00000001'");
    UBIN(256,  -8, "B'00000000'");
    UBIN(-32,  -8, "-B'00100000'");
    UBIN(-128, -8, "-B'10000000'");
    UBIN(-255, -8, "B'00000001'");
    UBIN(-256, -8, "B'00000000'");

    OCT(0,    8, "o'000'");
    OCT(32,   8, "o'040'");
    OCT(128,  8, "o'200'");
    OCT(255,  8, "o'377'");
    OCT(256,  8, "o'000'");
    OCT(-32,  8, "o'340'");
    OCT(-128, 8, "o'200'");
    OCT(-255, 8, "o'001'");
    OCT(-256, 8, "o'000'");

    OCT(0,    -8, "o'000'");
    OCT(32,   -8, "o'040'");
    OCT(128,  -8, "-o'200'");
    OCT(255,  -8, "-o'001'");
    OCT(256,  -8, "o'000'");
    OCT(-32,  -8, "-o'040'");
    OCT(-128, -8, "-o'200'");
    OCT(-255, -8, "o'001'");
    OCT(-256, -8, "o'000'");

    UOCT(0,    8, "O'000'");
    UOCT(32,   8, "O'040'");
    UOCT(128,  8, "O'200'");
    UOCT(255,  8, "O'377'");
    UOCT(256,  8, "O'000'");
    UOCT(-32,  8, "O'340'");
    UOCT(-128, 8, "O'200'");
    UOCT(-255, 8, "O'001'");
    UOCT(-256, 8, "O'000'");

    UOCT(0,    -8, "O'000'");
    UOCT(32,   -8, "O'040'");
    UOCT(128,  -8, "-O'200'");
    UOCT(255,  -8, "-O'001'");
    UOCT(256,  -8, "O'000'");
    UOCT(-32,  -8, "-O'040'");
    UOCT(-128, -8, "-O'200'");
    UOCT(-255, -8, "O'001'");
    UOCT(-256, -8, "O'000'");

    HEX(0,    8, "h'00'");
    HEX(32,   8, "h'20'");
    HEX(128,  8, "h'80'");
    HEX(255,  8, "h'ff'");
    HEX(256,  8, "h'00'");
    HEX(-32,  8, "h'e0'");
    HEX(-128, 8, "h'80'");
    HEX(-255, 8, "h'01'");
    HEX(-256, 8, "h'00'");

    HEX(0,   -8, "h'00'");
    HEX(32,  -8, "h'20'");
    HEX(128, -8, "-h'80'");
    HEX(255, -8, "-h'01'");
    HEX(256, -8, "h'00'");
    HEX(-32, -8, "-h'20'");
    HEX(-128,-8, "-h'80'");
    HEX(-255,-8, "h'01'");
    HEX(-256,-8, "h'00'");

    UHEX(0,    8, "H'00'");
    UHEX(32,   8, "H'20'");
    UHEX(128,  8, "H'80'");
    UHEX(255,  8, "H'FF'");
    UHEX(256,  8, "H'00'");
    UHEX(-32,  8, "H'E0'");
    UHEX(-128, 8, "H'80'");
    UHEX(-255, 8, "H'01'");
    UHEX(-256, 8, "H'00'");

    UHEX(0,   -8, "H'00'");
    UHEX(32,  -8, "H'20'");
    UHEX(128, -8, "-H'80'");
    UHEX(241, -8, "-H'0F'");
    UHEX(256, -8, "H'00'");
    UHEX(-32, -8, "-H'20'");
    UHEX(-128,-8, "-H'80'");
    UHEX(-255,-8, "H'01'");
    UHEX(-256,-8, "H'00'");

    // Relax

    RHEX(0,    8, "0");
    RHEX(32,   8, "32");
    RHEX(33,   8, "h'21'");
    RHEX(128,  8, "h'80'");
    RHEX(255,  8, "h'ff'");
    RHEX(256,  8, "0");
    RHEX(-32,  8, "h'e0'");
    RHEX(-33,  8, "h'df'");
    RHEX(-128, 8, "h'80'");
    RHEX(-255, 8, "1");
    RHEX(-256, 8, "0");

    RHEX(0,   -8, "0");
    RHEX(32,  -8, "32");
    RHEX(33,  -8, "h'21'");
    RHEX(128, -8, "-h'80'");
    RHEX(255, -8, "-1");
    RHEX(256, -8, "0");
    RHEX(-32, -8, "-32");
    RHEX(-33, -8, "-h'21'");
    RHEX(-128,-8, "-h'80'");
    RHEX(-255,-8, "1");
    RHEX(-256,-8, "0");

    URHEX(0,    8, "0");
    URHEX(32,   8, "32");
    URHEX(33,   8, "H'21'");
    URHEX(128,  8, "H'80'");
    URHEX(255,  8, "H'FF'");
    URHEX(256,  8, "0");
    URHEX(-32,  8, "H'E0'");
    URHEX(-33,  8, "H'DF'");
    URHEX(-128, 8, "H'80'");
    URHEX(-255, 8, "1");
    URHEX(-256, 8, "0");

    URHEX(0,   -8, "0");
    URHEX(32,  -8, "32");
    URHEX(33,  -8, "H'21'");
    URHEX(128, -8, "-H'80'");
    URHEX(255, -8, "-1");
    URHEX(256, -8, "0");
    URHEX(-32, -8, "-32");
    URHEX(-47, -8, "-H'2F'");
    URHEX(-128,-8, "-H'80'");
    URHEX(-255,-8, "1");
    URHEX(-256,-8, "0");
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

    BIN(0,         16, "b'0000000000000000'");
    BIN(32,        16, "b'0000000000100000'");
    BIN(0x8000,    16, "b'1000000000000000'");
    BIN(0xffff,    16, "b'1111111111111111'");
    BIN(0x10000,   16, "b'0000000000000000'");
    BIN(-32,       16, "b'1111111111100000'");
    BIN(-0x8000,   16, "b'1000000000000000'");
    BIN(-0xffff,   16, "b'0000000000000001'");
    BIN(-0x10000,  16, "b'0000000000000000'");

    BIN(0,        -16, "b'0000000000000000'");
    BIN(32,       -16, "b'0000000000100000'");
    BIN(0x8000,   -16, "-b'1000000000000000'");
    BIN(0xffff,   -16, "-b'0000000000000001'");
    BIN(0x10000,  -16, "b'0000000000000000'");
    BIN(-32,      -16, "-b'0000000000100000'");
    BIN(-0x8000,  -16, "-b'1000000000000000'");
    BIN(-0xffff,  -16, "b'0000000000000001'");
    BIN(-0x10000, -16, "b'0000000000000000'");

    OCT(0,         16, "o'000000'");
    OCT(32,        16, "o'000040'");
    OCT(0x8000,    16, "o'100000'");
    OCT(0xffff,    16, "o'177777'");
    OCT(0x10000,   16, "o'000000'");
    OCT(-32,       16, "o'177740'");
    OCT(-0x8000,   16, "o'100000'");
    OCT(-0xffff,   16, "o'000001'");
    OCT(-0x10000,  16, "o'000000'");

    OCT(0,        -16, "o'000000'");
    OCT(32,       -16, "o'000040'");
    OCT(0x8000,   -16, "-o'100000'");
    OCT(0xffff,   -16, "-o'000001'");
    OCT(0x10000,  -16, "o'000000'");
    OCT(-32,      -16, "-o'000040'");
    OCT(-0x8000,  -16, "-o'100000'");
    OCT(-0xffff,  -16, "o'000001'");
    OCT(-0x10000, -16, "o'000000'");

    HEX(0,         16, "h'0000'");
    HEX(32,        16, "h'0020'");
    HEX(0x8000,    16, "h'8000'");
    HEX(0xffff,    16, "h'ffff'");
    HEX(0x10000,   16, "h'0000'");
    HEX(-32,       16, "h'ffe0'");
    HEX(-0x8000,   16, "h'8000'");
    HEX(-0xffff,   16, "h'0001'");
    HEX(-0x10000,  16, "h'0000'");

    HEX(0,        -16, "h'0000'");
    HEX(32,       -16, "h'0020'");
    HEX(0x8000,   -16, "-h'8000'");
    HEX(0xffff,   -16, "-h'0001'");
    HEX(0x10000,  -16, "h'0000'");
    HEX(-32,      -16, "-h'0020'");
    HEX(-0x8000,  -16, "-h'8000'");
    HEX(-0xffff,  -16, "h'0001'");
    HEX(-0x10000, -16, "h'0000'");

    // Relax

    RHEX(0,         16, "0");
    RHEX(32,        16, "32");
    RHEX(33,        16, "h'0021'");
    RHEX(0x8000,    16, "h'8000'");
    RHEX(0xffff,    16, "h'ffff'");
    RHEX(0x10000,   16, "0");
    RHEX(-32,       16, "h'ffe0'");
    RHEX(-33,       16, "h'ffdf'");
    RHEX(-0x8000,   16, "h'8000'");
    RHEX(-0xffff,   16, "1");
    RHEX(-0x10000,  16, "0");

    RHEX(0,        -16, "0");
    RHEX(32,       -16, "32");
    RHEX(33,       -16, "h'0021'");
    RHEX(0x8000,   -16, "-h'8000'");
    RHEX(0xffff,   -16, "-1");
    RHEX(0x10000,  -16, "0");
    RHEX(-32,      -16, "-32");
    RHEX(-33,      -16, "-h'0021'");
    RHEX(-0x8000,  -16, "-h'8000'");
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

    BIN(0,           24, "b'000000000000000000000000'");
    BIN(32,          24, "b'000000000000000000100000'");
    BIN(0x800000,    24, "b'100000000000000000000000'");
    BIN(0xffffff,    24, "b'111111111111111111111111'");
    BIN(0x1000000,   24, "b'000000000000000000000000'");
    BIN(-32,         24, "b'111111111111111111100000'");
    BIN(-0x800000,   24, "b'100000000000000000000000'");
    BIN(-0xffffff,   24, "b'000000000000000000000001'");
    BIN(-0x1000000,  24, "b'000000000000000000000000'");

    BIN(0,          -24, "b'000000000000000000000000'");
    BIN(32,         -24, "b'000000000000000000100000'");
    BIN(0x800000,   -24, "-b'100000000000000000000000'");
    BIN(0xffffff,   -24, "-b'000000000000000000000001'");
    BIN(0x1000000,  -24, "b'000000000000000000000000'");
    BIN(-32,        -24, "-b'000000000000000000100000'");
    BIN(-0x800000,  -24, "-b'100000000000000000000000'");
    BIN(-0xffffff,  -24, "b'000000000000000000000001'");
    BIN(-0x1000000, -24, "b'000000000000000000000000'");

    OCT(0,           24, "o'00000000'");
    OCT(32,          24, "o'00000040'");
    OCT(0x800000,    24, "o'40000000'");
    OCT(0xffffff,    24, "o'77777777'");
    OCT(0x1000000,   24, "o'00000000'");
    OCT(-32,         24, "o'77777740'");
    OCT(-0x800000,   24, "o'40000000'");
    OCT(-0xffffff,   24, "o'00000001'");
    OCT(-0x1000000,  24, "o'00000000'");

    OCT(0,          -24, "o'00000000'");
    OCT(32,         -24, "o'00000040'");
    OCT(0x800000,   -24, "-o'40000000'");
    OCT(0xffffff,   -24, "-o'00000001'");
    OCT(0x1000000,  -24, "o'00000000'");
    OCT(-32,        -24, "-o'00000040'");
    OCT(-0x800000,  -24, "-o'40000000'");
    OCT(-0xffffff,  -24, "o'00000001'");
    OCT(-0x1000000, -24, "o'00000000'");

    HEX(0,           24, "h'000000'");
    HEX(32,          24, "h'000020'");
    HEX(0x800000,    24, "h'800000'");
    HEX(0xffffff,    24, "h'ffffff'");
    HEX(0x1000000,   24, "h'000000'");
    HEX(-32,         24, "h'ffffe0'");
    HEX(-0x800000,   24, "h'800000'");
    HEX(-0xffffff,   24, "h'000001'");
    HEX(-0x1000000,  24, "h'000000'");

    HEX(0,          -24, "h'000000'");
    HEX(32,         -24, "h'000020'");
    HEX(0x800000,   -24, "-h'800000'");
    HEX(0xffffff,   -24, "-h'000001'");
    HEX(0x1000000,  -24, "h'000000'");
    HEX(-32,        -24, "-h'000020'");
    HEX(-0x800000,  -24, "-h'800000'");
    HEX(-0xffffff,  -24, "h'000001'");
    HEX(-0x1000000, -24, "h'000000'");

    // Relax

    RHEX(0,           24, "0");
    RHEX(32,          24, "32");
    RHEX(33,          24, "h'000021'");
    RHEX(0x800000,    24, "h'800000'");
    RHEX(0xffffff,    24, "h'ffffff'");
    RHEX(0x1000000,   24, "0");
    RHEX(-32,         24, "h'ffffe0'");
    RHEX(-33,         24, "h'ffffdf'");
    RHEX(-0x800000,   24, "h'800000'");
    RHEX(-0xffffff,   24, "1");
    RHEX(-0x1000000,  24, "0");

    RHEX(0,          -24, "0");
    RHEX(32,         -24, "32");
    RHEX(33,         -24, "h'000021'");
    RHEX(0x800000,   -24, "-h'800000'");
    RHEX(0xffffff,   -24, "-1");
    RHEX(0x1000000,  -24, "0");
    RHEX(-32,        -24, "-32");
    RHEX(-33,        -24, "-h'000021'");
    RHEX(-0x800000,  -24, "-h'800000'");
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

    BIN(0,           32, "b'00000000000000000000000000000000'");
    BIN(32,          32, "b'00000000000000000000000000100000'");
    BIN(0x80000000,  32, "b'10000000000000000000000000000000'");
    BIN(0xffffffff,  32, "b'11111111111111111111111111111111'");
    BIN(-32,         32, "b'11111111111111111111111111100000'");
    BIN(-0x80000000, 32, "b'10000000000000000000000000000000'");
    BIN(-0xffffffff, 32, "b'00000000000000000000000000000001'");

    BIN(0,           -32, "b'00000000000000000000000000000000'");
    BIN(32,          -32, "b'00000000000000000000000000100000'");
    BIN(0x80000000,  -32, "-b'10000000000000000000000000000000'");
    BIN(0xffffffff,  -32, "-b'00000000000000000000000000000001'");
    BIN(-32,         -32, "-b'00000000000000000000000000100000'");
    BIN(-0x80000000, -32, "-b'10000000000000000000000000000000'");
    BIN(-0xffffffff, -32, "b'00000000000000000000000000000001'");

    OCT(0,           32, "o'00000000000'");
    OCT(32,          32, "o'00000000040'");
    OCT(0x80000000,  32, "o'20000000000'");
    OCT(0xffffffff,  32, "o'37777777777'");
    OCT(-32,         32, "o'37777777740'");
    OCT(-0x80000000, 32, "o'20000000000'");
    OCT(-0xffffffff, 32, "o'00000000001'");

    OCT(0,           -32, "o'00000000000'");
    OCT(32,          -32, "o'00000000040'");
    OCT(0x80000000,  -32, "-o'20000000000'");
    OCT(0xffffffff,  -32, "-o'00000000001'");
    OCT(-32,         -32, "-o'00000000040'");
    OCT(-0x80000000, -32, "-o'20000000000'");
    OCT(-0xffffffff, -32, "o'00000000001'");

    HEX(0,           32, "h'00000000'");
    HEX(32,          32, "h'00000020'");
    HEX(33,          32, "h'00000021'");
    HEX(0x80000000,  32, "h'80000000'");
    HEX(0xffffffff,  32, "h'ffffffff'");
    HEX(-32,         32, "h'ffffffe0'");
    HEX(-33,         32, "h'ffffffdf'");
    HEX(-0x80000000, 32, "h'80000000'");
    HEX(-0xffffffff, 32, "h'00000001'");

    HEX(0,           -32, "h'00000000'");
    HEX(32,          -32, "h'00000020'");
    HEX(33,          -32, "h'00000021'");
    HEX(0x80000000,  -32, "-h'80000000'");
    HEX(0xffffffff,  -32, "-h'00000001'");
    HEX(-32,         -32, "-h'00000020'");
    HEX(-33,         -32, "-h'00000021'");
    HEX(-0x80000000, -32, "-h'80000000'");
    HEX(-0xffffffff, -32, "h'00000001'");

    // Relax

    RHEX(0,           32, "0");
    RHEX(32,          32, "32");
    RHEX(33,          32, "h'00000021'");
    RHEX(0x80000000,  32, "h'80000000'");
    RHEX(0xffffffff,  32, "h'ffffffff'");
    RHEX(-32,         32, "h'ffffffe0'");
    RHEX(-33,         32, "h'ffffffdf'");
    RHEX(-0x80000000, 32, "h'80000000'");
    RHEX(-0xffffffff, 32, "1");

    RHEX(0,           -32, "0");
    RHEX(32,          -32, "32");
    RHEX(33,          -32, "h'00000021'");
    RHEX(0x80000000,  -32, "-h'80000000'");
    RHEX(0xffffffff,  -32, "-1");
    RHEX(-32,         -32, "-32");
    RHEX(-33,         -32, "-h'00000021'");
    RHEX(-0x80000000, -32, "-h'80000000'");
    RHEX(-0xffffffff, -32, "1");

    // Uppercase

    UHEX(0,           32, "H'00000000'");
    UHEX(32,          32, "H'00000020'");
    UHEX(0x80000000,  32, "H'80000000'");
    UHEX(0xffffffff,  32, "H'FFFFFFFF'");
    UHEX(-32,         32, "H'FFFFFFE0'");
    UHEX(-0x80000000, 32, "H'80000000'");
    UHEX(-0xffffffff, 32, "H'00000001'");

    UHEX(0,           -32, "H'00000000'");
    UHEX(32,          -32, "H'00000020'");
    UHEX(0x80000000,  -32, "-H'80000000'");
    UHEX(0xffffffff,  -32, "-H'00000001'");
    UHEX(-32,         -32, "-H'00000020'");
    UHEX(-0x80000000, -32, "-H'80000000'");
    UHEX(-0xffffffff, -32, "H'00000001'");


    // Uppercase and Relax

    URHEX(0,           32, "0");
    URHEX(32,          32, "32");
    URHEX(33,          32, "H'00000021'");
    URHEX(0x80000000,  32, "H'80000000'");
    URHEX(0xffffffff,  32, "H'FFFFFFFF'");
    URHEX(-32,         32, "H'FFFFFFE0'");
    URHEX(-33,         32, "H'FFFFFFDF'");
    URHEX(-0x80000000, 32, "H'80000000'");
    URHEX(-0xffffffff, 32, "1");

    URHEX(0,           -32, "0");
    URHEX(32,          -32, "32");
    URHEX(33,          -32, "H'00000021'");
    URHEX(0x80000000,  -32, "-H'80000000'");
    URHEX(0xffffffff,  -32, "-1");
    URHEX(-32,         -32, "-32");
    URHEX(-33,         -32, "-H'00000021'");
    URHEX(-0x80000000, -32, "-H'80000000'");
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
    RUN_TEST(test_comment);
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
