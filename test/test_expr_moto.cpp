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

MotoValueParser parser;
MotoValueFormatter formatter;

static void set_up() {
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
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
    symtab.setCurrentOrigin(0x1000);
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
    symtab.setCurrentOrigin(0x1100);
    E16("*-table",     0x100, OK);
    E16("(*-table)/2", 0x80,  OK);
    E16(".-table",     0x100, OK);
    E16("(.-table)/2", 0x80,  OK);
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
// clang-format on

void run_tests() {
    RUN_TEST(test_hex_constant);
    RUN_TEST(test_oct_constant);
    RUN_TEST(test_bin_constant);
    RUN_TEST(test_current_address);
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
