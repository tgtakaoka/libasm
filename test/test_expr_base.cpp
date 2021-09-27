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

ValueParser parser;
ValueFormatter formatter;

static void set_up() {}

static void tear_down() {
    symtab.reset();
}

// clang-format off
static void test_char_constant() {
    E8("'a'",    0x61, OK);
    E8("'\\''",  0x27, OK);
    E8("'\\\"'", 0x22, OK);
    E8("'\\?'",  0x3F, OK);
    E8("'\\\\'", 0x5C, OK);
    E8("'\\b'",  0x08, OK);
    E8("'\\t'",  0x09, OK);
    E8("'\\n'",  0x0A, OK);
    E8("'\\r'",  0x0D, OK);
    E8("'\\X0'", 0x00, OK);
    E8("'\\xfF'", 0xFF, OK);
    E8("'\\0'",  0x00, OK);
    E8("'\\377'", 0xFF, OK);

    E8("'\\x100'", 0,  OVERFLOW_RANGE);
    E8("'\\400'", 0,   OVERFLOW_RANGE);
    E8("''",     0,    MISSING_CLOSING_QUOTE);
    E8("'a",     0,    MISSING_CLOSING_QUOTE);
    E8("'\\z'",  0,    UNKNOWN_ESCAPE_SEQUENCE);

    E16("'a'", 0x61, OK);
    E32("'a'", 0x61, OK);
}

static void test_dec_constant() {
    E8("0",   0x00, OK);
    E8("127", 0x7f, OK);
    E8("128", 0x80, OK);
    E8("255", 0xff, OK);
    E8("256", 0,    OVERFLOW_RANGE);

    E16("0",     0x0000, OK);
    E16("32767", 0x7fff, OK);
    E16("32768", 0x8000, OK);
    E16("65535", 0xffff, OK);
    E16("65536", 0,      OVERFLOW_RANGE);

    E32("0",          0x00000000, OK);
    E32("2147483647", 0x7fffffff, OK);
    E32("2147483648", 0x80000000, OK);
    E32("4294967295", 0xffffffff, OK);
    E32("4294967296", 0,          OVERFLOW_RANGE);
}

static void test_hex_constant() {
    E8("0x0",   0x00, OK);
    E8("0x7f",  0x7f, OK);
    E8("0x80",  0x80, OK);
    E8("0xFF",  0xff, OK);
    E8("0x100", 0,    OVERFLOW_RANGE);

    E16("0x0",     0x0000, OK);
    E16("0x7fff",  0x7fff, OK);
    E16("0x8000",  0x8000, OK);
    E16("0xffff",  0xffff, OK);
    E16("0x10000", 0,      OVERFLOW_RANGE);

    E32("0x0",         0x00000000, OK);
    E32("0x7FFFFFFF",  0x7fffffff, OK);
    E32("0x80000000",  0x80000000, OK);
    E32("0xFFFFffff",  0xffffffff, OK);
    E32("0x100000000", 0,          OVERFLOW_RANGE);
}

static void test_oct_constant() {
    E8("0177", 0x7f, OK);
    E8("0200", 0x80, OK);
    E8("0377", 0xff, OK);
    E8("0400", 0,    OVERFLOW_RANGE);

    E16("00",      0x0000, OK);
    E16("0077777", 0x7fff, OK);
    E16("0100000", 0x8000, OK);
    E16("0177777", 0xffff, OK);
    E16("0200000", 0,      OVERFLOW_RANGE);

    E32("00",           0x00000000, OK);
    E32("017777777777", 0x7fffffff, OK);
    E32("020000000000", 0x80000000, OK);
    E32("037777777777", 0xffffffff, OK);
    E32("040000000000", 0,          OVERFLOW_RANGE);
}

static void test_bin_constant() {
    E8("0b0",         0x00, OK);
    E8("0b01111111",  0x7f, OK);
    E8("0b10000000",  0x80, OK);
    E8("0b11111111",  0xff, OK);
    E8("0b100000000", 0,    OVERFLOW_RANGE);

    E16("0b0",                 0x0000, OK);
    E16("0b0111111111111111",  0x7fff, OK);
    E16("0b1000000000000000",  0x8000, OK);
    E16("0b1111111111111111",  0xffff, OK);
    E16("0b10000000000000000", 0,      OVERFLOW_RANGE);

    E32("0b0",                                 0x00000000, OK);
    E32("0b01111111111111111111111111111111",  0x7fffffff, OK);
    E32("0b10000000000000000000000000000000",  0x80000000, OK);
    E32("0b11111111111111111111111111111111",  0xffffffff, OK);
    E32("0b100000000000000000000000000000000", 0,          OVERFLOW_RANGE);
}

static void test_unary_operator() {
    E8("-1",   0xff, OK);
    E8("-128", 0x80, OK);
    E8("-129", 0,    OVERFLOW_RANGE);

    E16("-1",     0xffff, OK);
    E16("-32768", 0x8000, OK);
    E16("-32769", 0,      OVERFLOW_RANGE);

    E32("-1",          0xffffffff, OK);
    E32("-2147483648", 0x80000000, OK);
    E32("-2147483649", 0,          OVERFLOW_RANGE);

    E8("+128", 0x80, OK);
    E8("+129", 0x81, OK);
    E8("+256", 0,    OVERFLOW_RANGE);

    E16("+32768", 0x8000, OK);
    E16("+32769", 0x8001, OK);
    E16("+65536", 0,      OVERFLOW_RANGE);

    E32("+2147483648", 0x80000000, OK);
    E32("+2147483649", 0x80000001, OK);
    E32("+4294967296", 0,          OVERFLOW_RANGE);

    E8("~+0",    0xFF, OK);
    E8("~(1|8)", 0xF6, OK);
    E8("~-1",    0x00, OK);

    E16("~0x0",     0xFFFF, OK);
    E16("~(1|010)", 0xFFF6, OK);
    E16("~-0b1",    0x0000, OK);

    E32("~+0",    0xFFFFFFFF, OK);
    E32("~(1|8)", 0xFFFFFFF6, OK);
    E32("~-1",    0x00000000, OK);
}

static void test_binary_operator() {
    E8("2+3",   5, OK);
    E8("-128+383", 255, OK);
    E8("2-3",  -1, OK);
    E8("255-383", -128, OK);
    E8("2*3",   6, OK);
    E8("20/3",  6, OK);
    E8("22/(-5)", -4, OK);
    E8("-22/5", -4, OK);
    E8("-22/(-5)", 4, OK);
    E8("20%3",  2, OK);
    E8("22%(-5)", 2, OK);
    E8("-22%5", -2, OK);
    E8("-22%(-5)", -2, OK);

    E8("1<<0", 1, OK);
    E8("1<<7", 128, OK);
    E8("1<<8", 0, OVERFLOW_RANGE);
    E8("0<<8", 0, OK);
    E8("-1>>8", -1, OK);
    E8("-1<<8", 0, OVERFLOW_RANGE);
    E8("-128>>1", -64, OK);
    E8("-128>>7", -1, OK);
    E8("-128>>8", -1, OK);
    E8("0x80>>7", 0x01, OK);
    E8("0x80>>8", 0, OK);
    E8("0xFF>>4", 0x0F, OK);
    E8("(0xFF<<4)&0xFF", 0xF0, OK);

    E8("0b0001|0b0100", 0x05, OK);
    E8("0b1011&0b0110", 0x02, OK);
    E8("0b0110^0b0011", 0x05, OK);

    E32("2+3",   5, OK);
    E32("2-3",  -1, OK);
    E32("2*3",   6, OK);
    E32("20/3",  6, OK);
    E32("20%3",  2, OK);

    E32("0x00000001<<31", 0x80000000, OK);
    E32("0x00000001<<32", 0, OK);
    E32("0x80000000>>31", 1, OK);
    E32("0x80000000>>32", 0, OK);
    E32("1>>(-1)", 0, OK);
    E32("1<<(-1)", 0, OK);
    E32("-1<<31", -0x80000000, OK);
    E32("-1<<32", 0, OK);
    E32("-1>>31", -1, OK);
    E32("-1>>32", -1, OK);

    E32("0b0001|0b0100", 0x05, OK);
    E32("0b1011&0b0110", 0x02, OK);
    E32("0b0110^-1",    0xfffffff9, OK);
}

static void test_overflow() {
    E8("0x1000-0x1080", -128, OK);
    E8("0x1000-0x1081", 0, OVERFLOW_RANGE);
    E8("0x10ff-0x1000", 255, OK);
    E8("0x1100-0x1000", 0, OVERFLOW_RANGE);
    E8("0xE000-0xE080", -128, OK);
    E8("0xE000-0xE081", 0, OVERFLOW_RANGE);
    E8("0xE0ff-0xE000", 255, OK);
    E8("0xE100-0xE000", 0, OVERFLOW_RANGE);

    E16("0xE000-0x6000", 0x8000, OK);
    E16("0xE000-0x5fff", 0x8001, OK);
    E16("0xE000-0xE001", -1,     OK);
    E16("0x2000-0xA000", -32768, OK);
    E16("0x2000-0xA001", 0,      OVERFLOW_RANGE);
    E16("0x3000-0x3001", -1   ,  OK);

    E16("0xE000+(-0x2000)", 0xC000, OK);
    E16("(-0x2000)+0xE000", 0xC000, OK);
}

static void test_precedence() {
    E16("1+2-3+4",    4, OK);
    E16("1+2*3+4",   11, OK);
    E16("1+2-7/3",    1, OK);
    E16("1-8%3*3",   -5, OK);
    E16("(1+2)*3+4", 13, OK);
    E16("1+(2-7)/3",  0, OK);
    E16("(1-8)%3*3", -3, OK);

    E8("1<<2+3", 0x20, OK);
    E8("(1<<2)+3" , 7, OK);
    E8("1<<2*3", 0x40, OK);
    E8("(1<<2)*3", 12, OK);
    E8("0x20>>2+3",    1, OK);
    E8("(0x20>>2)+3", 11, OK);
    E8("0x40>>2*3",    1, OK);
    E8("(0x40>>2)*3", 48, OK);

    E8("1<<4>>1",   8, OK);
    E8("1<<(4>>1)", 4, OK);
    E8("4>>1<<1",   4, OK);
    E8("4>>(1<<1)", 1, OK);

    E8("1|2&6^8", 11, OK);
    E8("1|2^4&12", 7, OK);
    E8("1&3|4^8", 13, OK);
    E8("1^2|4&12", 7, OK);
    E8("1&3^4|8", 13, OK);
    E8("1^2&6|8", 11, OK);

    E8("(1|2)&6^8", 10, OK);
    E8("(1|2)^4&12", 7, OK);
    E8("1&(3|4)^8",  9, OK);
    E8("1^(2|4)&12", 5, OK);
    E8("1&3^(4|8)", 13, OK);
    E8("1^2&(6|8)",  3, OK);

    E8("1|2&(6^8)",  3, OK);
    E8("1|(2^4)&12", 5, OK);
    E8("1&3|(4^8)", 13, OK);
    E8("(1^2)|4&12", 7, OK);
    E8("1&(3^4)|8",  9, OK);
    E8("(1^2)&6|8", 10, OK);

    E8("1|2<<3",   17, OK);
    E8("(1|2)<<3", 24, OK);
    E8("3&7<<1",    2, OK);
    E8("(3&7)<<1",  6, OK);
    E8("1^3<<3",   25, OK);
    E8("(1^3)<<3", 16, OK);
}

static void test_current_address() {
    parser.setCurrentOrigin(0x1000);
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

static void test_errors() {
    E32("undef",   0, UNDEFINED_SYMBOL);
    E32("+undef",  0, UNDEFINED_SYMBOL);
    E32("-undef",  0, UNDEFINED_SYMBOL);
    E32("~undef",  0, UNDEFINED_SYMBOL);
    E32("(undef)", 0, UNDEFINED_SYMBOL);
    E32("1-undef", 0, UNDEFINED_SYMBOL);
    E32("undef+1", 0, UNDEFINED_SYMBOL);
    E32("undef",   0, UNDEFINED_SYMBOL);
    E32("0xcdefg", 0, ILLEGAL_CONSTANT);
    E32("0345678", 0, ILLEGAL_CONSTANT);
    E32("0b10102", 0, ILLEGAL_CONSTANT);
    E32("456789A", 0, ILLEGAL_CONSTANT);
    E32("2*(1+3",  0, MISSING_CLOSING_PAREN);
    E32("2*(1+3]", 0, MISSING_CLOSING_PAREN);
    E32("(((0)))",            0, OK);
    E32("1+(2+(3))",          6, OK);
    E32("1+(2+(3+(4)))",     10, OK);
    E32("1+(2+(3+(4+(5))))", 15, TOO_COMPLEX_EXPRESSION);
    E32("'a",      0, MISSING_CLOSING_QUOTE);
    E32("'\\'",    0, MISSING_CLOSING_QUOTE);
    E32("'\\x20",  0, MISSING_CLOSING_QUOTE);
    E32("'\\a'",   0, UNKNOWN_ESCAPE_SEQUENCE);
    E32("--1",     0, UNKNOWN_EXPR_OPERATOR);
    E32("-+1",     0, UNKNOWN_EXPR_OPERATOR);
    E32("+-1",     0, UNKNOWN_EXPR_OPERATOR);
    E32("++1",     0, UNKNOWN_EXPR_OPERATOR);
    E32("1<2",     0, UNKNOWN_EXPR_OPERATOR);
    E32("1>2",     0, UNKNOWN_EXPR_OPERATOR);
    E32("100/0",   0, DIVIDE_BY_ZERO);
    E32("100%0",   0, DIVIDE_BY_ZERO);
}

static void test_spaces() {
    E32(" - 1 ", -1, OK);
    E32(" + 1 ",  1, OK);
    E32(" ~ 0 ", -1, OK);
    E32(" 1 + 2 ; + 3 ", 3, OK);
    E32(" ( 1 + 2 ) * 3 ", 9, OK);
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
    HEX(33,   8, true, "0x21");
    HEX(128,  8, true, "0x80");
    HEX(255,  8, true, "0xff");
    HEX(256,  8, true, "0");
    HEX(-32,  8, true, "0xe0");
    HEX(-33,  8, true, "0xdf");
    HEX(-128, 8, true, "0x80");
    HEX(-255, 8, true, "1");
    HEX(-256, 8, true, "0");

    HEX(0,    -8, true, "0");
    HEX(32,   -8, true, "32");
    HEX(33,   -8, true, "0x21");
    HEX(128,  -8, true, "-0x80");
    HEX(255,  -8, true, "-1");
    HEX(256,  -8, true, "0");
    HEX(-32,  -8, true, "-32");
    HEX(-33,  -8, true, "-0x21");
    HEX(-128, -8, true, "-0x80");
    HEX(-255, -8, true, "1");
    HEX(-256, -8, true, "0");

    HEX(0,    8, false, "0x00");
    HEX(32,   8, false, "0x20");
    HEX(128,  8, false, "0x80");
    HEX(255,  8, false, "0xff");
    HEX(256,  8, false, "0x00");
    HEX(-32,  8, false, "0xe0");
    HEX(-128, 8, false, "0x80");
    HEX(-255, 8, false, "0x01");
    HEX(-256, 8, false, "0x00");

    HEX(0,    -8, false, "0x00");
    HEX(32,   -8, false, "0x20");
    HEX(128,  -8, false, "-0x80");
    HEX(255,  -8, false, "-0x01");
    HEX(256,  -8, false, "0x00");
    HEX(-32,  -8, false, "-0x20");
    HEX(-128, -8, false, "-0x80");
    HEX(-255, -8, false, "0x01");
    HEX(-256, -8, false, "0x00");
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
    HEX(33,        16, true, "0x0021");
    HEX(0x8000,    16, true, "0x8000");
    HEX(0xffff,    16, true, "0xffff");
    HEX(0x10000,   16, true, "0");
    HEX(-32,       16, true, "0xffe0");
    HEX(-33,       16, true, "0xffdf");
    HEX(-0x8000,   16, true, "0x8000");
    HEX(-0xffff,   16, true, "1");
    HEX(-0x10000,  16, true, "0");

    HEX(0,        -16, true, "0");
    HEX(32,       -16, true, "32");
    HEX(33,       -16, true, "0x0021");
    HEX(0x8000,   -16, true, "-0x8000");
    HEX(0xffff,   -16, true, "-1");
    HEX(0x10000,  -16, true, "0");
    HEX(-32,      -16, true, "-32");
    HEX(-33,      -16, true, "-0x0021");
    HEX(-0x8000,  -16, true, "-0x8000");
    HEX(-0xffff,  -16, true, "1");
    HEX(-0x10000, -16, true, "0");

    HEX(0,         16, false, "0x0000");
    HEX(32,        16, false, "0x0020");
    HEX(0x8000,    16, false, "0x8000");
    HEX(0xffff,    16, false, "0xffff");
    HEX(0x10000,   16, false, "0x0000");
    HEX(-32,       16, false, "0xffe0");
    HEX(-0x8000,   16, false, "0x8000");
    HEX(-0xffff,   16, false, "0x0001");
    HEX(-0x10000,  16, false, "0x0000");

    HEX(0,        -16, false, "0x0000");
    HEX(32,       -16, false, "0x0020");
    HEX(0x8000,   -16, false, "-0x8000");
    HEX(0xffff,   -16, false, "-0x0001");
    HEX(0x10000,  -16, false, "0x0000");
    HEX(-32,      -16, false, "-0x0020");
    HEX(-0x8000,  -16, false, "-0x8000");
    HEX(-0xffff,  -16, false, "0x0001");
    HEX(-0x10000, -16, false, "0x0000");
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
    HEX(33,          24, true, "0x000021");
    HEX(0x800000,    24, true, "0x800000");
    HEX(0xffffff,    24, true, "0xffffff");
    HEX(0x1000000,   24, true, "0");
    HEX(-32,         24, true, "0xffffe0");
    HEX(-33,         24, true, "0xffffdf");
    HEX(-0x800000,   24, true, "0x800000");
    HEX(-0xffffff,   24, true, "1");
    HEX(-0x1000000,  24, true, "0");

    HEX(0,          -24, true, "0");
    HEX(32,         -24, true, "32");
    HEX(33,         -24, true, "0x000021");
    HEX(0x800000,   -24, true, "-0x800000");
    HEX(0xffffff,   -24, true, "-1");
    HEX(0x1000000,  -24, true, "0");
    HEX(-32,        -24, true, "-32");
    HEX(-33,        -24, true, "-0x000021");
    HEX(-0x800000,  -24, true, "-0x800000");
    HEX(-0xffffff,  -24, true, "1");
    HEX(-0x1000000, -24, true, "0");

    HEX(0,           24, false, "0x000000");
    HEX(32,          24, false, "0x000020");
    HEX(0x800000,    24, false, "0x800000");
    HEX(0xffffff,    24, false, "0xffffff");
    HEX(0x1000000,   24, false, "0x000000");
    HEX(-32,         24, false, "0xffffe0");
    HEX(-0x800000,   24, false, "0x800000");
    HEX(-0xffffff,   24, false, "0x000001");
    HEX(-0x1000000,  24, false, "0x000000");

    HEX(0,          -24, false, "0x000000");
    HEX(32,         -24, false, "0x000020");
    HEX(0x800000,   -24, false, "-0x800000");
    HEX(0xffffff,   -24, false, "-0x000001");
    HEX(0x1000000,  -24, false, "0x000000");
    HEX(-32,        -24, false, "-0x000020");
    HEX(-0x800000,  -24, false, "-0x800000");
    HEX(-0xffffff,  -24, false, "0x000001");
    HEX(-0x1000000, -24, false, "0x000000");
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
    HEX(33,          32, true, "0x00000021");
    HEX(0x80000000,  32, true, "0x80000000");
    HEX(0xffffffff,  32, true, "0xffffffff");
    HEX(-32,         32, true, "0xffffffe0");
    HEX(-33,         32, true, "0xffffffdf");
    HEX(-0x80000000, 32, true, "0x80000000");
    HEX(-0xffffffff, 32, true, "1");

    HEX(0,           -32, true, "0");
    HEX(32,          -32, true, "32");
    HEX(33,          -32, true, "0x00000021");
    HEX(0x80000000,  -32, true, "-0x80000000");
    HEX(0xffffffff,  -32, true, "-1");
    HEX(-32,         -32, true, "-32");
    HEX(-33,         -32, true, "-0x00000021");
    HEX(-0x80000000, -32, true, "-0x80000000");
    HEX(-0xffffffff, -32, true, "1");

    HEX(0,           32, false, "0x00000000");
    HEX(32,          32, false, "0x00000020");
    HEX(0x80000000,  32, false, "0x80000000");
    HEX(0xffffffff,  32, false, "0xffffffff");
    HEX(-32,         32, false, "0xffffffe0");
    HEX(-0x80000000, 32, false, "0x80000000");
    HEX(-0xffffffff, 32, false, "0x00000001");

    HEX(0,           -32, false, "0x00000000");
    HEX(32,          -32, false, "0x00000020");
    HEX(0x80000000,  -32, false, "-0x80000000");
    HEX(0xffffffff,  -32, false, "-0x00000001");
    HEX(-32,         -32, false, "-0x00000020");
    HEX(-0x80000000, -32, false, "-0x80000000");
    HEX(-0xffffffff, -32, false, "0x00000001");
}

static void test_formatter_9bit() {
    HEX(-128 *2, -9, false, "-0x100");
    HEX(   0 *2, -9, false, "0x000");
    HEX(+127 *2, -9, false, "0x0fe");
}

static void test_formatter_13bit() {
    HEX(-2048 *2, -13, false, "-0x1000");
    HEX(    0 *2, -13, false, "0x0000");
    HEX(+2047 *2, -13, false, "0x0ffe");
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
    RUN_TEST(test_overflow);
    RUN_TEST(test_precedence);
    RUN_TEST(test_current_address);
    RUN_TEST(test_errors);
    RUN_TEST(test_spaces);
    RUN_TEST(test_formatter_8bit);
    RUN_TEST(test_formatter_16bit);
    RUN_TEST(test_formatter_24bit);
    RUN_TEST(test_formatter_32bit);
    RUN_TEST(test_formatter_9bit);
    RUN_TEST(test_formatter_13bit);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
