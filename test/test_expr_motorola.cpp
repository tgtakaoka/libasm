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

MotorolaValueParser parser;
MotorolaValueFormatter formatter;

static void set_up() {
    formatter.setCStyle(false);
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
static void test_char_constant() {
    E8("'a'",     0x61, OK);
    E8("'a'+5",   0x66, OK);
    E8("5+'a'",   0x66, OK);
    E8("'a",      0x61, OK);
    E8("'a+5",    0x66, OK);
    E8("5+'a",    0x66, OK);
    E8("' ",      0x20, OK);
    E8("''",      0x27, OK);
    E8("'\\''",   0x27, OK);
    E8("'\\\"'",  0x22, OK);
    E8("'\\?'",   0x3F, OK);
    E8("'\\\\'",  0x5C, OK);
    E8("'\\b'",   0x08, OK);
    E8("'\\t'",   0x09, OK);
    E8("'\\n'",   0x0A, OK);
    E8("'\\r'",   0x0D, OK);
    E8("'\\X0'",  0x00, OK);
    E8("'\\xfF'", 0xFF, OK);
    E8("'\\0'",   0x00, OK);
    E8("'\\377'", 0xFF, OK);

    E8("'\\x100'", 0, OVERFLOW_RANGE);
    E8("'\\400'",  0, OVERFLOW_RANGE);
    E8("'\\z'",    0, UNKNOWN_ESCAPE_SEQUENCE);

    E16("'a'", 0x61, OK);
    E32("'a'", 0x61, OK);
}

static void test_char_closing() {
    MotorolaValueParser parser(/*closingQuote*/ true);

    E8("'a",     0, MISSING_CLOSING_QUOTE);
    E8("'a+5",   0, MISSING_CLOSING_QUOTE);
    E8("5+'a",   0, MISSING_CLOSING_QUOTE);
    E8("' ",     0, MISSING_CLOSING_QUOTE);
    E8("''",     0, MISSING_CLOSING_QUOTE);
}

static void test_dec_constant() {
    E8("0",   0x00, OK);
    E8("127", 0x7f, OK);
    E8("128", 0x80, OK);
    E8("255", 0xff, OK);
    E8("256", 0,    OVERFLOW_RANGE);

    E16("&0",     0x0000, OK);
    E16("&32767", 0x7fff, OK);
    E16("&32768", 0x8000, OK);
    E16("&65535", 0xffff, OK);
    E16("&65536", 0,      OVERFLOW_RANGE);

    E32("&0",          0x00000000, OK);
    E32("&2147483647", 0x7fffffff, OK);
    E32("2147483648",  0x80000000, OK);
    E32("&4294967295", 0xffffffff, OK);
    E32("4294967296",  0,          OVERFLOW_RANGE);
    E32("&9999999999", 0,          OVERFLOW_RANGE);
}

static void test_hex_constant() {
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
    E16("*",       0x1000, OK);
    E16("*+2",     0x1002, OK);
    E16("*-2",     0x0FFE, OK);
    E16("*+$F000", 0,      OVERFLOW_RANGE);
    E16("*-$1001", 0xFFFF, OK);
    E32("*+$F000", 0x00010000, OK);
    E32("*-$1001", 0xFFFFFFFF, OK);
    E16(".",       0x1000, OK);
    E16(".+2",     0x1002, OK);
    E16(".-2",     0x0FFE, OK);
    E16(".+0xF000", 0,      OVERFLOW_RANGE);
    E16(".-0x1001", 0xFFFF, OK);
    E32(".+0xF000", 0x00010000, OK);
    E32(".-0x1001", 0xFFFFFFFF, OK);

    symtab.intern(0x1000, "table");
    parser.setCurrentOrigin(0x1100);
    E16("*-table",     0x100, OK);
    E16("(*-table)/2", 0x80,  OK);
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
    SCAN('$', "$1230$X230",   "$1230");
    SCAN('@', "@1230@8230",   "@1230");
    SCAN('%', "%1010%2010",   "%1010");
}

static void test_errors() {
    E32("$bcdefg", 0, ILLEGAL_CONSTANT);
    E32("@345678", 0, ILLEGAL_CONSTANT);
    E32("%101012", 0, ILLEGAL_CONSTANT);
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
    HEX(33,   8, true, "$21");
    HEX(128,  8, true, "$80");
    HEX(255,  8, true, "$ff");
    HEX(256,  8, true, "0");
    HEX(-32,  8, true, "$e0");
    HEX(-33,  8, true, "$df");
    HEX(-128, 8, true, "$80");
    HEX(-255, 8, true, "1");
    HEX(-256, 8, true, "0");

    HEX(0,   -8, true, "0");
    HEX(32,  -8, true, "32");
    HEX(33,  -8, true, "$21");
    HEX(128, -8, true, "-$80");
    HEX(255, -8, true, "-1");
    HEX(256, -8, true, "0");
    HEX(-32, -8, true, "-32");
    HEX(-33, -8, true, "-$21");
    HEX(-128,-8, true, "-$80");
    HEX(-255,-8, true, "1");
    HEX(-256,-8, true, "0");

    HEX(0,    8, false, "$00");
    HEX(32,   8, false, "$20");
    HEX(128,  8, false, "$80");
    HEX(255,  8, false, "$ff");
    HEX(256,  8, false, "$00");
    HEX(-32,  8, false, "$e0");
    HEX(-128, 8, false, "$80");
    HEX(-255, 8, false, "$01");
    HEX(-256, 8, false, "$00");

    HEX(0,   -8, false, "$00");
    HEX(32,  -8, false, "$20");
    HEX(128, -8, false, "-$80");
    HEX(255, -8, false, "-$01");
    HEX(256, -8, false, "$00");
    HEX(-32, -8, false, "-$20");
    HEX(-128,-8, false, "-$80");
    HEX(-255,-8, false, "$01");
    HEX(-256,-8, false, "$00");
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
    HEX(33,        16, true, "$0021");
    HEX(0x8000,    16, true, "$8000");
    HEX(0xffff,    16, true, "$ffff");
    HEX(0x10000,   16, true, "0");
    HEX(-32,       16, true, "$ffe0");
    HEX(-33,       16, true, "$ffdf");
    HEX(-0x8000,   16, true, "$8000");
    HEX(-0xffff,   16, true, "1");
    HEX(-0x10000,  16, true, "0");

    HEX(0,        -16, true, "0");
    HEX(32,       -16, true, "32");
    HEX(33,       -16, true, "$0021");
    HEX(0x8000,   -16, true, "-$8000");
    HEX(0xffff,   -16, true, "-1");
    HEX(0x10000,  -16, true, "0");
    HEX(-32,      -16, true, "-32");
    HEX(-33,      -16, true, "-$0021");
    HEX(-0x8000,  -16, true, "-$8000");
    HEX(-0xffff,  -16, true, "1");
    HEX(-0x10000, -16, true, "0");

    HEX(0,         16, false, "$0000");
    HEX(32,        16, false, "$0020");
    HEX(0x8000,    16, false, "$8000");
    HEX(0xffff,    16, false, "$ffff");
    HEX(0x10000,   16, false, "$0000");
    HEX(-32,       16, false, "$ffe0");
    HEX(-0x8000,   16, false, "$8000");
    HEX(-0xffff,   16, false, "$0001");
    HEX(-0x10000,  16, false, "$0000");

    HEX(0,        -16, false, "$0000");
    HEX(32,       -16, false, "$0020");
    HEX(0x8000,   -16, false, "-$8000");
    HEX(0xffff,   -16, false, "-$0001");
    HEX(0x10000,  -16, false, "$0000");
    HEX(-32,      -16, false, "-$0020");
    HEX(-0x8000,  -16, false, "-$8000");
    HEX(-0xffff,  -16, false, "$0001");
    HEX(-0x10000, -16, false, "$0000");
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
    HEX(33,          24, true, "$000021");
    HEX(0x800000,    24, true, "$800000");
    HEX(0xffffff,    24, true, "$ffffff");
    HEX(0x1000000,   24, true, "0");
    HEX(-32,         24, true, "$ffffe0");
    HEX(-33,         24, true, "$ffffdf");
    HEX(-0x800000,   24, true, "$800000");
    HEX(-0xffffff,   24, true, "1");
    HEX(-0x1000000,  24, true, "0");

    HEX(0,          -24, true, "0");
    HEX(32,         -24, true, "32");
    HEX(33,         -24, true, "$000021");
    HEX(0x800000,   -24, true, "-$800000");
    HEX(0xffffff,   -24, true, "-1");
    HEX(0x1000000,  -24, true, "0");
    HEX(-32,        -24, true, "-32");
    HEX(-33,        -24, true, "-$000021");
    HEX(-0x800000,  -24, true, "-$800000");
    HEX(-0xffffff,  -24, true, "1");
    HEX(-0x1000000, -24, true, "0");

    HEX(0,           24, false, "$000000");
    HEX(32,          24, false, "$000020");
    HEX(0x800000,    24, false, "$800000");
    HEX(0xffffff,    24, false, "$ffffff");
    HEX(0x1000000,   24, false, "$000000");
    HEX(-32,         24, false, "$ffffe0");
    HEX(-0x800000,   24, false, "$800000");
    HEX(-0xffffff,   24, false, "$000001");
    HEX(-0x1000000,  24, false, "$000000");

    HEX(0,          -24, false, "$000000");
    HEX(32,         -24, false, "$000020");
    HEX(0x800000,   -24, false, "-$800000");
    HEX(0xffffff,   -24, false, "-$000001");
    HEX(0x1000000,  -24, false, "$000000");
    HEX(-32,        -24, false, "-$000020");
    HEX(-0x800000,  -24, false, "-$800000");
    HEX(-0xffffff,  -24, false, "$000001");
    HEX(-0x1000000, -24, false, "$000000");
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
    HEX(33,          32, true, "$00000021");
    HEX(0x80000000,  32, true, "$80000000");
    HEX(0xffffffff,  32, true, "$ffffffff");
    HEX(-32,         32, true, "$ffffffe0");
    HEX(-33,         32, true, "$ffffffdf");
    HEX(-0x80000000, 32, true, "$80000000");
    HEX(-0xffffffff, 32, true, "1");

    HEX(0,           -32, true, "0");
    HEX(32,          -32, true, "32");
    HEX(33,          -32, true, "$00000021");
    HEX(0x80000000,  -32, true, "-$80000000");
    HEX(0xffffffff,  -32, true, "-1");
    HEX(-32,         -32, true, "-32");
    HEX(-33,         -32, true, "-$00000021");
    HEX(-0x80000000, -32, true, "-$80000000");
    HEX(-0xffffffff, -32, true, "1");

    HEX(0,           32, false, "$00000000");
    HEX(32,          32, false, "$00000020");
    HEX(0x80000000,  32, false, "$80000000");
    HEX(0xffffffff,  32, false, "$ffffffff");
    HEX(-32,         32, false, "$ffffffe0");
    HEX(-0x80000000, 32, false, "$80000000");
    HEX(-0xffffffff, 32, false, "$00000001");

    HEX(0,           -32, false, "$00000000");
    HEX(32,          -32, false, "$00000020");
    HEX(0x80000000,  -32, false, "-$80000000");
    HEX(0xffffffff,  -32, false, "-$00000001");
    HEX(-32,         -32, false, "-$00000020");
    HEX(-0x80000000, -32, false, "-$80000000");
    HEX(-0xffffffff, -32, false, "$00000001");
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
    RUN_TEST(test_char_closing);
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
