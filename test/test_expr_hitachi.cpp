/*
 * Copyright 2026 Tadashi G. Takaoka
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

const ValueParser parser{ValueParser::Plugins::hitachi()};
const ValueFormatter formatter{ValueFormatter::Plugins::hitachi()};

void set_up() {}

void tear_down() {
    symtab.reset();
}

// clang-format off
void test_char_constant() {
    E8("\"a\"",   0x61);
    E8("\"a\"+5", 0x66);
    E8("5+\"a\"", 0x66);
    X8("\"\"",    MISSING_CLOSING_DQUOTE, "\"\"", "\"\"");
    X8("\"a",     MISSING_CLOSING_DQUOTE, "\"a",  "\"a");
    X8("\"a+5",   MISSING_CLOSING_DQUOTE, "\"a+5","\"a+5");
    X8("5+\"a",   MISSING_CLOSING_DQUOTE, "\"a",  "\"a");
    X8("\" ",     MISSING_CLOSING_DQUOTE, "\" ",  "\" ");
    E8("\"\"\"\"", 0x22);

    // Multi-char constants
    E16("\"AB\"",   0x4142);
    E32("\"ABC\"",  0x00414243);
    E32("\"ABCD\"", 0x41424344);
    X32("\"ABCDE\"", OVERFLOW_RANGE, "\"ABCDE\"", "\"ABCDE\"");

    // Doubled-quote escape inside multi-char constant; exercises the
    // doubled-delimiter handling that defineString relies on for .sdata.
    E16("\"\"\"a\"",  0x2261);
    E32("\"\"\"ab\"", 0x00226162);

    E16("\"a\"", 0x61);
    E32("\"a\"", 0x61);
}

void test_dec_constant() {
    E32("2147483647",    0x7fffffff);
    E32("d'2147483647",  0x7fffffff);
    E32("D'2147483647",  0x7fffffff);
    E32("d'2147483647'", 0x7fffffff);
    E32("D'2147483647'", 0x7fffffff);
}

void test_hex_constant() {
    E8("H'0",   0x00);
    E8("H'7f",  0x7f);
    E8("H'80",  0x80);
    E8("H'FF'", 0xff);
    X8("H'100", OVERFLOW_RANGE, "H'100", "");

    E16("H'0",     0x0000);
    E16("H'7fff",  0x7fff);
    E16("H'8000",  0x8000);
    E16("H'ffff",  0xffff);
    X16("H'10000", OVERFLOW_RANGE, "H'10000", "");

    E32("H'0",         0x00000000);
    E32("H'7FFFFFFF",  0x7fffffff);
    E32("h'7FFFFFFF",  0x7fffffff);
    E32("H'7FFFFFFF'", 0x7fffffff);
    E32("h'7FFFFFFF'", 0x7fffffff);
    E32("H'80000000",  0x80000000);
    E32("H'FFFFffff",  0xffffffff);
    X32("H'100000000", OVERFLOW_RANGE, "H'100000000", "");

    E32("0x0",         0x00000000);
    E32("0x7FFFFFFF",  0x7fffffff);
    E32("0x80000000",  0x80000000);
    E32("0xFFFFffff",  0xffffffff);
    X32("0x100000000", OVERFLOW_RANGE, "0x100000000", "");
}

void test_oct_constant() {
    E8("Q'0",   0x00);
    E8("Q'177", 0x7f);
    E8("Q'200", 0x80);
    E8("Q'377", 0xff);
    X8("Q'400", OVERFLOW_RANGE, "Q'400", "");

    E16("Q'0",      0x0000);
    E16("Q'077777", 0x7fff);
    E16("Q'100000", 0x8000);
    E16("Q'177777", 0xffff);
    X16("Q'200000", OVERFLOW_RANGE, "Q'200000", "");

    E32("Q'0",            0x00000000);
    E32("Q'17777777777",  0x7fffffff);
    E32("q'17777777777",  0x7fffffff);
    E32("Q'17777777777'", 0x7fffffff);
    E32("q'17777777777'", 0x7fffffff);
    E32("Q'37777777777",  0xffffffff);
    X32("Q'40000000000",  OVERFLOW_RANGE, "Q'40000000000", "");

    E32("00",           0x00000000);
    E32("017777777777", 0x7fffffff);
    E32("020000000000", 0x80000000);
    E32("037777777777", 0xffffffff);
    X32("040000000000", OVERFLOW_RANGE, "040000000000", "");
}

void test_bin_constant() {
    E8("B'0",         0x00);
    E8("B'01111111",  0x7f);
    E8("B'10000000",  0x80);
    E8("B'11111111",  0xff);
    X8("B'100000000", OVERFLOW_RANGE, "B'100000000", "");

    E16("B'0",                 0x0000);
    E16("B'0111111111111111",  0x7fff);
    E16("B'1000000000000000",  0x8000);
    E16("B'1111111111111111",  0xffff);
    X16("B'10000000000000000", OVERFLOW_RANGE, "B'10000000000000000", "");

    E32("B'0",                                 0x00000000);
    E32("B'01111111111111111111111111111111",  0x7fffffff);
    E32("b'01111111111111111111111111111111",  0x7fffffff);
    E32("B'01111111111111111111111111111111'", 0x7fffffff);
    E32("b'01111111111111111111111111111111'", 0x7fffffff);
    E32("B'10000000000000000000000000000000",  0x80000000);
    E32("B'11111111111111111111111111111111",  0xffffffff);
    X32("B'100000000000000000000000000000000", OVERFLOW_RANGE, "B'100000000000000000000000000000000", "");

    E32("0b0",                                 0x00000000);
    E32("0b01111111111111111111111111111111",  0x7fffffff);
    E32("0b10000000000000000000000000000000",  0x80000000);
    E32("0b11111111111111111111111111111111",  0xffffffff);
    X32("0b100000000000000000000000000000000", OVERFLOW_RANGE, "0b100000000000000000000000000000000", "");
}

void test_unary_operator() {
    E8("~0",   0xFF);
    E8("~-1",  0x00);

    E16("~H'0",    0xFFFF);
    E16("~H'FF",   0xFF00);
    E16("~-1",     0x0000);

    E32("~H'0",    0xFFFFFFFF);
    E32("~H'FF",   0xFFFFFF00);
    E32("~H'FFFF", 0xFFFF0000);
    E32("~-1",     0x00000000);

    E16("HIGH H'1234", 0x12);
    E16("LOW  H'1234", 0x34);

    E16("HIGH H'1234+1",   0x13);  // (HIGH H'1234) + 1
    E16("LOW  H'1234+1",   0x35);  // (LOW H'1234) + 1

    E16("~HIGH H'1234",    0xFFED); // ~(HIGH H'1234)
    E16("HIGH (~H'1234)",  0xED);   // HIGH (~H'1234)

    E32("HWORD H'12345678", 0x1234);
    E32("LWORD H'12345678", 0x5678);

    E32("HIGH LWORD H'12345678", 0x56); // HIGH (LWORD H'12345678)
    E32("LOW HWORD H'12345678",  0x34); // LOW (HWORD H'12345678)

    E32("high H'1234", 0x12);
    E32("low  H'1234", 0x34);
    E32("hword H'12345678", 0x1234);
    E32("lword H'12345678", 0x5678);
}

void test_binary_operator() {
    E8("(20) * (3)",  60);
    E8("(20) * (-3)", -60);

    E8("(20) / (3)",  6);
    E8("(20) / (-3)", -6);

    E8("(20) % (3)",  2);
    E8("(20) % (-3)", 2);

    E8("H'FF ~ H'0F", 0xF0);  // XOR
    E8("H'0F ~ H'FF", 0xF0);  // XOR
    E8("H'FF ~ H'FF", 0x00);  // XOR

    E16("H'FF ~ H'0F",   0xF0);
    E16("H'FFFF ~ H'FF", 0xFF00);

    E32("H'FFFF ~ H'FF", 0xFF00);

    E8("B'1100 & B'1010",  0x08);
    E8("B'1100 | B'1010",  0x0E);
    E8("B'1100 ^ B'1010",  0x06);  // standard XOR with ^

    E8("1 << 4", 0x10);
    E8("H'80 >> 4", 0x08);

    E8("1+2*3",   7);
    E8("(1+2)*3", 9);
}

void test_current_address() {
    context.currentLocation = 0x1000;
    E16("$",        0x1000);
    E16("$+2",      0x1002);
    E16("$-2",      0x0FFE);
    X16("$+H'F000", OVERFLOW_RANGE, "$+H'F000", "");
    E16("$-H'1001", 0xFFFF);
    E32("$+H'F000", 0x00010000);
    E32("$-H'1001", 0xFFFFFFFF);

    E16("*",        0x1000);
    E16("*+2",      0x1002);
    E16("*-2",      0x0FFE);

    E16(".",        0x1000);
    E16(".+2",      0x1002);
    E16(".-2",      0x0FFE);

    symtab.intern(0x1000, "table");
    context.currentLocation = 0x1100;
    E16("$-table",     0x100);
    E16("($-table)/2", 0x080);
}

void test_scan() {
    SERR('|', "|1+2|",          0,        "|1+2|", NOT_AN_EXPECTED, "|1+2|");
    SCAN('|', "1+2|3+4",        3,        "|3+4");
    SCAN('|', "1+(2|3)+4|5",    8,        "|5");
    SCAN('|', "1+\"|\"+2|3",    '|'+3,    "|3");
    SCAN(',', "1+2|3+4",        7,        "");
    SCAN(',', "\",\"+\"\"\"\",abc", ','+'\"', ",abc");
    SCAN('x', "0x1230xG",       0x1230,   "xG");
    SCAN('b', "0b1010b0",       0b1010,   "b0");
    SCAN('H', "H'1AB0HHX",      0x1AB0,   "HHX");
    SCAN('Q', "Q'1230QQX",      01230,    "QQX");
    SCAN('B', "B'1010BBX",      0b1010,   "BBX");
    SCAN(',', "H'1230,",        0x1230,   ",");
    SCAN(',', "H'1230',",       0x1230,   ",");
}

void test_errors() {
    EXPR("H'bcdefg", 0xBCDEF, "g");
    EXPR("Q'345678", 034567,  "8");
    EXPR("B'101012", 0b10101, "2");
    EXPR("456789a",  456789,  "a");
}

void test_comment() {
    EXPR(" - 1 ", -1, "");
    EXPR(" + 1 ",  1, "");
    EXPR(" ~ 0 ", -1, "");
    EXPR(" 1 + 2 ; + 3 ", 3, "; + 3 ");
    EXPR(" ( 1 + 2 ) * 3 ", 9, "");
}

void test_formatter_8bit() {
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

    BIN(0,    8, "b'00000000");
    BIN(32,   8, "b'00100000");
    BIN(128,  8, "b'10000000");
    BIN(255,  8, "b'11111111");
    BIN(256,  8, "b'00000000");
    BIN(-32,  8, "b'11100000");
    BIN(-128, 8, "b'10000000");
    BIN(-255, 8, "b'00000001");
    BIN(-256, 8, "b'00000000");

    BIN(0,    -8, "b'00000000");
    BIN(32,   -8, "b'00100000");
    BIN(128,  -8, "-b'10000000");
    BIN(255,  -8, "-b'00000001");
    BIN(256,  -8, "b'00000000");
    BIN(-32,  -8, "-b'00100000");
    BIN(-128, -8, "-b'10000000");
    BIN(-255, -8, "b'00000001");
    BIN(-256, -8, "b'00000000");

    UBIN(0,    8, "B'00000000");
    UBIN(32,   8, "B'00100000");
    UBIN(128,  8, "B'10000000");
    UBIN(255,  8, "B'11111111");
    UBIN(256,  8, "B'00000000");
    UBIN(-32,  8, "B'11100000");
    UBIN(-128, 8, "B'10000000");
    UBIN(-255, 8, "B'00000001");
    UBIN(-256, 8, "B'00000000");

    UBIN(0,    -8, "B'00000000");
    UBIN(32,   -8, "B'00100000");
    UBIN(128,  -8, "-B'10000000");
    UBIN(255,  -8, "-B'00000001");
    UBIN(256,  -8, "B'00000000");
    UBIN(-32,  -8, "-B'00100000");
    UBIN(-128, -8, "-B'10000000");
    UBIN(-255, -8, "B'00000001");
    UBIN(-256, -8, "B'00000000");

    OCT(0,    8, "q'000");
    OCT(32,   8, "q'040");
    OCT(128,  8, "q'200");
    OCT(255,  8, "q'377");
    OCT(256,  8, "q'000");
    OCT(-32,  8, "q'340");
    OCT(-128, 8, "q'200");
    OCT(-255, 8, "q'001");
    OCT(-256, 8, "q'000");

    OCT(0,    -8, "q'000");
    OCT(32,   -8, "q'040");
    OCT(128,  -8, "-q'200");
    OCT(255,  -8, "-q'001");
    OCT(256,  -8, "q'000");
    OCT(-32,  -8, "-q'040");
    OCT(-128, -8, "-q'200");
    OCT(-255, -8, "q'001");
    OCT(-256, -8, "q'000");

    UOCT(0,    8, "Q'000");
    UOCT(32,   8, "Q'040");
    UOCT(128,  8, "Q'200");
    UOCT(255,  8, "Q'377");
    UOCT(256,  8, "Q'000");
    UOCT(-32,  8, "Q'340");
    UOCT(-128, 8, "Q'200");
    UOCT(-255, 8, "Q'001");
    UOCT(-256, 8, "Q'000");

    UOCT(0,    -8, "Q'000");
    UOCT(32,   -8, "Q'040");
    UOCT(128,  -8, "-Q'200");
    UOCT(255,  -8, "-Q'001");
    UOCT(256,  -8, "Q'000");
    UOCT(-32,  -8, "-Q'040");
    UOCT(-128, -8, "-Q'200");
    UOCT(-255, -8, "Q'001");
    UOCT(-256, -8, "Q'000");

    HEX(0,    8, "h'00");
    HEX(32,   8, "h'20");
    HEX(128,  8, "h'80");
    HEX(255,  8, "h'ff");
    HEX(256,  8, "h'00");
    HEX(-32,  8, "h'e0");
    HEX(-128, 8, "h'80");
    HEX(-255, 8, "h'01");
    HEX(-256, 8, "h'00");

    HEX(0,   -8, "h'00");
    HEX(32,  -8, "h'20");
    HEX(128, -8, "-h'80");
    HEX(255, -8, "-h'01");
    HEX(256, -8, "h'00");
    HEX(-32, -8, "-h'20");
    HEX(-128,-8, "-h'80");
    HEX(-255,-8, "h'01");
    HEX(-256,-8, "h'00");

    UHEX(0,    8, "H'00");
    UHEX(32,   8, "H'20");
    UHEX(128,  8, "H'80");
    UHEX(255,  8, "H'FF");
    UHEX(256,  8, "H'00");
    UHEX(-32,  8, "H'E0");
    UHEX(-128, 8, "H'80");
    UHEX(-255, 8, "H'01");
    UHEX(-256, 8, "H'00");

    UHEX(0,   -8, "H'00");
    UHEX(32,  -8, "H'20");
    UHEX(128, -8, "-H'80");
    UHEX(241, -8, "-H'0F");
    UHEX(256, -8, "H'00");
    UHEX(-32, -8, "-H'20");
    UHEX(-128,-8, "-H'80");
    UHEX(-255,-8, "H'01");
    UHEX(-256,-8, "H'00");

    // Relax

    RHEX(0,    8, "0");
    RHEX(32,   8, "32");
    RHEX(33,   8, "h'21");
    RHEX(128,  8, "h'80");
    RHEX(255,  8, "h'ff");
    RHEX(256,  8, "0");
    RHEX(-32,  8, "h'e0");
    RHEX(-33,  8, "h'df");
    RHEX(-128, 8, "h'80");
    RHEX(-255, 8, "1");
    RHEX(-256, 8, "0");

    RHEX(0,   -8, "0");
    RHEX(32,  -8, "32");
    RHEX(33,  -8, "h'21");
    RHEX(128, -8, "-h'80");
    RHEX(255, -8, "-1");
    RHEX(256, -8, "0");
    RHEX(-32, -8, "-32");
    RHEX(-33, -8, "-h'21");
    RHEX(-128,-8, "-h'80");
    RHEX(-255,-8, "1");
    RHEX(-256,-8, "0");

    URHEX(0,    8, "0");
    URHEX(32,   8, "32");
    URHEX(33,   8, "H'21");
    URHEX(128,  8, "H'80");
    URHEX(255,  8, "H'FF");
    URHEX(256,  8, "0");
    URHEX(-32,  8, "H'E0");
    URHEX(-33,  8, "H'DF");
    URHEX(-128, 8, "H'80");
    URHEX(-255, 8, "1");
    URHEX(-256, 8, "0");

    URHEX(0,   -8, "0");
    URHEX(32,  -8, "32");
    URHEX(33,  -8, "H'21");
    URHEX(128, -8, "-H'80");
    URHEX(255, -8, "-1");
    URHEX(256, -8, "0");
    URHEX(-32, -8, "-32");
    URHEX(-47, -8, "-H'2F");
    URHEX(-128,-8, "-H'80");
    URHEX(-255,-8, "1");
    URHEX(-256,-8, "0");
}

void test_formatter_16bit() {
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

    BIN(0,         16, "b'0000000000000000");
    BIN(32,        16, "b'0000000000100000");
    BIN(0x8000,    16, "b'1000000000000000");
    BIN(0xffff,    16, "b'1111111111111111");
    BIN(0x10000,   16, "b'0000000000000000");
    BIN(-32,       16, "b'1111111111100000");
    BIN(-0x8000,   16, "b'1000000000000000");
    BIN(-0xffff,   16, "b'0000000000000001");
    BIN(-0x10000,  16, "b'0000000000000000");

    BIN(0,        -16, "b'0000000000000000");
    BIN(32,       -16, "b'0000000000100000");
    BIN(0x8000,   -16, "-b'1000000000000000");
    BIN(0xffff,   -16, "-b'0000000000000001");
    BIN(0x10000,  -16, "b'0000000000000000");
    BIN(-32,      -16, "-b'0000000000100000");
    BIN(-0x8000,  -16, "-b'1000000000000000");
    BIN(-0xffff,  -16, "b'0000000000000001");
    BIN(-0x10000, -16, "b'0000000000000000");

    OCT(0,         16, "q'000000");
    OCT(32,        16, "q'000040");
    OCT(0x8000,    16, "q'100000");
    OCT(0xffff,    16, "q'177777");
    OCT(0x10000,   16, "q'000000");
    OCT(-32,       16, "q'177740");
    OCT(-0x8000,   16, "q'100000");
    OCT(-0xffff,   16, "q'000001");
    OCT(-0x10000,  16, "q'000000");

    OCT(0,        -16, "q'000000");
    OCT(32,       -16, "q'000040");
    OCT(0x8000,   -16, "-q'100000");
    OCT(0xffff,   -16, "-q'000001");
    OCT(0x10000,  -16, "q'000000");
    OCT(-32,      -16, "-q'000040");
    OCT(-0x8000,  -16, "-q'100000");
    OCT(-0xffff,  -16, "q'000001");
    OCT(-0x10000, -16, "q'000000");

    HEX(0,         16, "h'0000");
    HEX(32,        16, "h'0020");
    HEX(0x8000,    16, "h'8000");
    HEX(0xffff,    16, "h'ffff");
    HEX(0x10000,   16, "h'0000");
    HEX(-32,       16, "h'ffe0");
    HEX(-0x8000,   16, "h'8000");
    HEX(-0xffff,   16, "h'0001");
    HEX(-0x10000,  16, "h'0000");

    HEX(0,        -16, "h'0000");
    HEX(32,       -16, "h'0020");
    HEX(0x8000,   -16, "-h'8000");
    HEX(0xffff,   -16, "-h'0001");
    HEX(0x10000,  -16, "h'0000");
    HEX(-32,      -16, "-h'0020");
    HEX(-0x8000,  -16, "-h'8000");
    HEX(-0xffff,  -16, "h'0001");
    HEX(-0x10000, -16, "h'0000");

    // Relax

    RHEX(0,         16, "0");
    RHEX(32,        16, "32");
    RHEX(33,        16, "h'0021");
    RHEX(0x8000,    16, "h'8000");
    RHEX(0xffff,    16, "h'ffff");
    RHEX(0x10000,   16, "0");
    RHEX(-32,       16, "h'ffe0");
    RHEX(-33,       16, "h'ffdf");
    RHEX(-0x8000,   16, "h'8000");
    RHEX(-0xffff,   16, "1");
    RHEX(-0x10000,  16, "0");

    RHEX(0,        -16, "0");
    RHEX(32,       -16, "32");
    RHEX(33,       -16, "h'0021");
    RHEX(0x8000,   -16, "-h'8000");
    RHEX(0xffff,   -16, "-1");
    RHEX(0x10000,  -16, "0");
    RHEX(-32,      -16, "-32");
    RHEX(-33,      -16, "-h'0021");
    RHEX(-0x8000,  -16, "-h'8000");
    RHEX(-0xffff,  -16, "1");
    RHEX(-0x10000, -16, "0");
}

void test_formatter_32bit() {
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

    BIN(0,           32, "b'00000000000000000000000000000000");
    BIN(32,          32, "b'00000000000000000000000000100000");
    BIN(0x80000000,  32, "b'10000000000000000000000000000000");
    BIN(0xffffffff,  32, "b'11111111111111111111111111111111");
    BIN(-32,         32, "b'11111111111111111111111111100000");
    BIN(-0x80000000, 32, "b'10000000000000000000000000000000");
    BIN(-0xffffffff, 32, "b'00000000000000000000000000000001");

    BIN(0,           -32, "b'00000000000000000000000000000000");
    BIN(32,          -32, "b'00000000000000000000000000100000");
    BIN(0x80000000,  -32, "-b'10000000000000000000000000000000");
    BIN(0xffffffff,  -32, "-b'00000000000000000000000000000001");
    BIN(-32,         -32, "-b'00000000000000000000000000100000");
    BIN(-0x80000000, -32, "-b'10000000000000000000000000000000");
    BIN(-0xffffffff, -32, "b'00000000000000000000000000000001");

    OCT(0,           32, "q'00000000000");
    OCT(32,          32, "q'00000000040");
    OCT(0x80000000,  32, "q'20000000000");
    OCT(0xffffffff,  32, "q'37777777777");
    OCT(-32,         32, "q'37777777740");
    OCT(-0x80000000, 32, "q'20000000000");
    OCT(-0xffffffff, 32, "q'00000000001");

    OCT(0,           -32, "q'00000000000");
    OCT(32,          -32, "q'00000000040");
    OCT(0x80000000,  -32, "-q'20000000000");
    OCT(0xffffffff,  -32, "-q'00000000001");
    OCT(-32,         -32, "-q'00000000040");
    OCT(-0x80000000, -32, "-q'20000000000");
    OCT(-0xffffffff, -32, "q'00000000001");

    HEX(0,           32, "h'00000000");
    HEX(32,          32, "h'00000020");
    HEX(33,          32, "h'00000021");
    HEX(0x80000000,  32, "h'80000000");
    HEX(0xffffffff,  32, "h'ffffffff");
    HEX(-32,         32, "h'ffffffe0");
    HEX(-33,         32, "h'ffffffdf");
    HEX(-0x80000000, 32, "h'80000000");
    HEX(-0xffffffff, 32, "h'00000001");

    HEX(0,           -32, "h'00000000");
    HEX(32,          -32, "h'00000020");
    HEX(33,          -32, "h'00000021");
    HEX(0x80000000,  -32, "-h'80000000");
    HEX(0xffffffff,  -32, "-h'00000001");
    HEX(-32,         -32, "-h'00000020");
    HEX(-33,         -32, "-h'00000021");
    HEX(-0x80000000, -32, "-h'80000000");
    HEX(-0xffffffff, -32, "h'00000001");

    // Uppercase

    UHEX(0,           32, "H'00000000");
    UHEX(32,          32, "H'00000020");
    UHEX(0x80000000,  32, "H'80000000");
    UHEX(0xffffffff,  32, "H'FFFFFFFF");
    UHEX(-32,         32, "H'FFFFFFE0");
    UHEX(-0x80000000, 32, "H'80000000");
    UHEX(-0xffffffff, 32, "H'00000001");

    UHEX(0,           -32, "H'00000000");
    UHEX(32,          -32, "H'00000020");
    UHEX(0x80000000,  -32, "-H'80000000");
    UHEX(0xffffffff,  -32, "-H'00000001");
    UHEX(-32,         -32, "-H'00000020");
    UHEX(-0x80000000, -32, "-H'80000000");
    UHEX(-0xffffffff, -32, "H'00000001");

    // Relax

    RHEX(0,           32, "0");
    RHEX(32,          32, "32");
    RHEX(33,          32, "h'00000021");
    RHEX(0x80000000,  32, "h'80000000");
    RHEX(0xffffffff,  32, "h'ffffffff");
    RHEX(-32,         32, "h'ffffffe0");
    RHEX(-33,         32, "h'ffffffdf");
    RHEX(-0x80000000, 32, "h'80000000");
    RHEX(-0xffffffff, 32, "1");

    RHEX(0,           -32, "0");
    RHEX(32,          -32, "32");
    RHEX(33,          -32, "h'00000021");
    RHEX(0x80000000,  -32, "-h'80000000");
    RHEX(0xffffffff,  -32, "-1");
    RHEX(-32,         -32, "-32");
    RHEX(-33,         -32, "-h'00000021");
    RHEX(-0x80000000, -32, "-h'80000000");
    RHEX(-0xffffffff, -32, "1");

    // Uppercase and Relax

    URHEX(0,           32, "0");
    URHEX(32,          32, "32");
    URHEX(33,          32, "H'00000021");
    URHEX(0X80000000,  32, "H'80000000");
    URHEX(0xffffffff,  32, "H'FFFFFFFF");
    URHEX(-32,         32, "H'FFFFFFE0");
    URHEX(-33,         32, "H'FFFFFFDF");
    URHEX(-0X80000000, 32, "H'80000000");
    URHEX(-0xffffffff, 32, "1");

    URHEX(0,           -32, "0");
    URHEX(32,          -32, "32");
    URHEX(33,          -32, "H'00000021");
    URHEX(0x80000000,  -32, "-H'80000000");
    URHEX(0xffffffff,  -32, "-1");
    URHEX(-32,         -32, "-32");
    URHEX(-33,         -32, "-H'00000021");
    URHEX(-0x80000000, -32, "-H'80000000");
    URHEX(-0xffffffff, -32, "1");
}
// clang-format on

void run_tests() {
    RUN_TEST(test_char_constant);
    RUN_TEST(test_dec_constant);
    RUN_TEST(test_hex_constant);
    RUN_TEST(test_oct_constant);
    RUN_TEST(test_bin_constant);
    RUN_TEST(test_unary_operator);
    RUN_TEST(test_binary_operator);
    RUN_TEST(test_current_address);
    RUN_TEST(test_scan);
    RUN_TEST(test_errors);
    RUN_TEST(test_comment);
    RUN_TEST(test_formatter_8bit);
    RUN_TEST(test_formatter_16bit);
    RUN_TEST(test_formatter_32bit);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
